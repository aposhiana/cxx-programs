#ifndef _PICOMPUTER_HPP_
#define _PICOMPUTER_HPP_

#include "Queue.hpp"
#include "HashTable.hpp"

class PiComputer {
private:
    HashTable<unsigned int, unsigned int> m_computedDigits;
    Queue<unsigned int>  m_taskQueue;
    unsigned int m_numDigits;
public:
    PiComputer(unsigned int numDigits);
    bool computePiDigitWithFeedback();
    void printPi();
private:
    int inv_mod(int x, int y);
    int pow_mod(int a, int b, int m);
    int is_prime(int n);
    int next_prime(int n);
    unsigned int computePiDigit(int n);
    double powneg(unsigned long long b, long long pow);
    unsigned long long s(unsigned long long j, unsigned long long n);
    unsigned long long piDigitHex(unsigned long long n);
};
#endif