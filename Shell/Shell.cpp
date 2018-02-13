// Written by Andrew Aposhian

#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/wait.h>
#include <signal.h>

#include "Shell.hpp"

//-----------------------------------------------------------
// The run method keeps the shell asking for prompts unless
// the program is exited or a child process fails.
// It also stores all command input into history.
//-----------------------------------------------------------
int Shell::run() {
    bool keepGoing = true;
    std::string myInputStr;
    while (keepGoing) {
        std::string workingDirPath(get_current_dir_name());
        std::cout << "[" + workingDirPath + "]:";
        // Set up signal handler for interrupt signal
        signal(SIGINT, interruptHandler);

        std::getline(std::cin, myInputStr);
        if (myInputStr == "") {
            continue;
        }
        std::vector<std::string> arguments = splitString(myInputStr);
        m_history.push_back(myInputStr);
        std::string firstArg = arguments[0];
        if (firstArg == "exit") {
            keepGoing = false;
        }
        else {
            executeCommand(arguments);
        }
    }
    return 0;
}

//-----------------------------------------------------------
// This method checks to see if a built-in command was
// specified, and if it was, then it is run.  If a built-in
// command is run then this method returns true, otherwise it
// returns false.
//-----------------------------------------------------------
bool Shell::runPossibleBuiltInCommand(std::vector<std::string> myArguments) {
    std::string firstArg = myArguments[0];
    if (firstArg == "history") {
        getHistory();
        return true;
    }
    else if (firstArg == "ptime") {
        getPTime();
        return true;
    }
    else if (firstArg == "^") {
        myArguments.erase(myArguments.begin());
        runKthCommand(myArguments);
        return true;
    }
    else if (firstArg == "cd") {
        myArguments.erase(myArguments.begin());
        if (myArguments.size() != 0) {
            changeDirectory(myArguments);
        }
        else {
            std::cout << "Error: No directory specified to change to" << std::endl;
        }
        return true;
    }
    else {
        return false;
    }
}

//-----------------------------------------------------------
// This method checks if a shell method is entered, in which
// case it will run that shell method, otherwise it will
// attempt to execute the command input as a child process.
//-----------------------------------------------------------
void Shell::executeCommand(std::vector<std::string> myArguments) {
    bool pipingNeeded = false;
    unsigned int numArgsToTrim = 0;
    std::vector<std::string> argsPipedTo;
    for (unsigned int i = 0; i < myArguments.size(); i++) {
        if (!pipingNeeded) {
            if (myArguments[i] == "|") {
                pipingNeeded = true;
                numArgsToTrim++;
            }
        }
        else {
            argsPipedTo.push_back(myArguments[i]);
            numArgsToTrim++;
        }
    }
    if (pipingNeeded) {
        for (unsigned int i = 0; i < numArgsToTrim; i++) {
            myArguments.pop_back();
        }
        handlePiping(myArguments, argsPipedTo);
    }
    else if (!runPossibleBuiltInCommand(myArguments)) {
        char** argv = createArgVector(myArguments);
        if (fork()) {
            auto start = std::chrono::high_resolution_clock::now();
            wait(NULL);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> lastProcessTime = end - start;
            m_ptime += lastProcessTime;
        }
        else {
            execvp(argv[0], argv);
            std::cout << "Error: command not recognized" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}

//-----------------------------------------------------------
// This method runs one command and pipes its output to
// another command.  It also times all child processes and
// aggregates the time to m_ptime.  It is also capable of
// running a built in command and piping its output to the
// second command.
//-----------------------------------------------------------
void Shell::handlePiping(std::vector<std::string> args, std::vector<std::string> argsPipedTo) {
    const int PIPE_COUNT = 2;
    const int PIPE_READ_END = 0;
    const int PIPE_WRITE_END = 1;
    const int STDIN = 0;
    const int STDOUT = 1;

    int pids[PIPE_COUNT];
    pipe(pids);

    int savedStdout = dup(STDOUT);
    int savedStdin = dup(STDIN);

    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    // First child runs a command that outputs to pipe
    pid_t pid = fork();
    if (pid == 0) {
        dup2(pids[PIPE_WRITE_END], STDOUT);
        if (!runPossibleBuiltInCommand(args)) {
            char** argv = createArgVector(args);
            execvp(argv[0], argv);
            std::cout << "Error: command not recognized" << std::endl;
        }
        // This will be run either by a failed execvp or if it was a built in command
        std::exit(EXIT_FAILURE);
    }

    // Second child runs a command that reads input from pipe
    pid_t pid2 = fork();
    if (pid2 == 0) {
        dup2(pids[PIPE_READ_END], STDIN);
        close(pids[PIPE_WRITE_END]);
        char** argv = createArgVector(argsPipedTo);
        execvp(argv[0], argv);
        std::cout << "Error: command not recognized" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Wait for the first child to finish
    int status;
    waitpid(pid, &status, 0);

    // Close both ends of pipe
    close(pids[PIPE_WRITE_END]);
    close(pids[PIPE_READ_END]);

    waitpid(pid2, &status, 0);

    // End the timer and add time to ptime
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> lastProcessTime = end - start;
    m_ptime += lastProcessTime;

    // Restore standard out and in
    dup2(savedStdout, STDOUT);
    dup2(savedStdin, STDIN);
}

//-----------------------------------------------------------
// This is a utility function used to split a string by
// space characters and return a vector.
//-----------------------------------------------------------
std::vector<std::string> Shell::splitString(std::string inputStr) {
    std::vector<std::string> args = {};
    char currentChar;
    std::string currentStr = "";
    for (unsigned int i = 0; i < inputStr.length(); i++) {
        currentChar = inputStr[i];
        if (currentChar == ' ') {
            args.push_back(currentStr);
            currentStr = "";
        }
        else if (i == (inputStr.length() - 1)) {
            currentStr += currentChar;
            args.push_back(currentStr);
        }
        else {
            currentStr += currentChar;
        }
    }
    return args;
}

//-----------------------------------------------------------
// This utility method returns an array of cstrings 
// generated from a vector of strings.
//-----------------------------------------------------------
char** Shell::createArgVector(std::vector<std::string> argStrings) {
    char** argv = new char*[argStrings.size() + 1];
    unsigned int index = 0;
    while (index < argStrings.size()) {
        argv[index] = new char[argStrings[index].length()];
        strcpy(argv[index], argStrings[index].c_str());
        index++;
    }
    argv[index] = NULL;
    return argv;
}

//-----------------------------------------------------------
// This method prints the history of commands.
//-----------------------------------------------------------
void Shell::getHistory() {
    std::cout << "---COMMAND HISTORY---" << std::endl;
    for (unsigned int i = 0; i < m_history.size(); i++) {
        std::cout << (i + 1) << ":  " << m_history[i] << std::endl;
    }
}

//-----------------------------------------------------------
// This method prints the total time spent in child processes.
//-----------------------------------------------------------
void Shell::getPTime() {
    std::string ptimeStr = std::to_string(m_ptime.count());
    // Get four places after decimal
    std::string newPtimeStr = "";
    unsigned int numCharsAfterDecimal = 0;
    bool charAfterDecimal = false;
    const unsigned int PLACES_TO_PRINT_AFTER_DECIMAL = 4;
    for (unsigned int i = 0; i < ptimeStr.length(); i++) {
        if (numCharsAfterDecimal >= PLACES_TO_PRINT_AFTER_DECIMAL) {
            break;
        }
        newPtimeStr += ptimeStr[i];
        if (!charAfterDecimal) {
            if (ptimeStr[i] == '.') {
                charAfterDecimal = true;
            }
        }
        else {
            numCharsAfterDecimal++;
        }
    }
    std::cout << "Time spent executing child processes: " << newPtimeStr << " seconds" << std::endl;
}

//-----------------------------------------------------------
// This method runs the kth command in history according to 
// the given k parameter that should be passed in args[0].
// The value args[0] is validated.
//-----------------------------------------------------------
void Shell::runKthCommand(std::vector<std::string> args) {
    std::string desiredArg = args[0];

    // Check if all the characters in the argument are digits
    std::vector<char> digits = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    for (unsigned int i = 0; i < desiredArg.length(); i++) {
        bool isADigit = false;
        for (unsigned int j = 0; j < digits.size(); j++){
            if (desiredArg[i] == digits[j]) {
                isADigit = true;
                break;
            }
        }
        if (!isADigit) {
            std::cout << "Error: invalid argument(s)" << std::endl;
        }
    }

    // Ensure that the number is within
    unsigned int cmdNum = stoi(desiredArg);
    if ((cmdNum > m_history.size()) || (cmdNum == 0)) {
        std::cout << "Error: invalid argument(s)" << std::endl;
    }
    else {
        // Subtract 1 because the valid listing starts at 1
        std::vector<std::string> arguments = splitString(m_history[cmdNum - 1]);
        executeCommand(arguments);
    }
}

//-----------------------------------------------------------
// This method changes the current working directory in which
// the shell operates.  If an invalid directory is passed as
// an argument then it will fail and give an error message.
//-----------------------------------------------------------
void Shell::changeDirectory(std::vector<std::string> args) {
    std::string desiredArg = args[0];
    char* directoryCString = new char[desiredArg.length()];
    strcpy(directoryCString, desiredArg.c_str());
    int chdirSuccess = chdir(directoryCString);
    if (chdirSuccess == -1) {
        std::string argStringToPrint = "";
        for (auto& arg : args) {
            argStringToPrint += " " + arg;
        }
        std::cout << "Error:" + argStringToPrint + ": No such directory" << std::endl;
    }
}