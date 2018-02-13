#include <iostream>
#include <thread>
#include <vector>

#include "PiComputer.hpp"

void computePiDigits(PiComputer& piComputer);

int main() {
    const unsigned int NUM_PI_DIGITS_TO_COMPUTE = 1000;
    PiComputer piComputer(NUM_PI_DIGITS_TO_COMPUTE);

    unsigned int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(numThreads);
    for (unsigned int i = 0; i < numThreads; i++) {
        threads.at(i) = std::thread(computePiDigits, std::ref(piComputer));
    }

    for (auto&& thread : threads) {
        thread.join();
    }
    std::cout << std::endl;
    piComputer.printPi();
    return 0;
}

void computePiDigits(PiComputer& piComputer) {
    while (piComputer.computePiDigitWithFeedback()) {
        std::cout << ".";
        std::cout.flush();
    }
}