// Written by Andrew Aposhian

#ifndef _SHELL_HPP_
#define _SHELL_HPP_

#include <string>
#include <vector>
#include <chrono>

class Shell {
public:
    Shell() : m_ptime(0) {}
    int run();
private:
    std::vector<std::string> m_history;
    std::chrono::duration<double> m_ptime;
private:
    static void interruptHandler(int signalId) { return; }
    std::vector<std::string> splitString(std::string inputStr);
    char** createArgVector(std::vector<std::string> argStrings);
    bool runPossibleBuiltInCommand(std::vector<std::string> arguments);
    void executeCommand(std::vector<std::string> myArguments);
    void handlePiping(std::vector<std::string> args, std::vector<std::string> argsPipedTo);
    void getHistory();    
    void getPTime();
    void runKthCommand(std::vector<std::string> args);
    void changeDirectory(std::vector<std::string> args);
};

#endif