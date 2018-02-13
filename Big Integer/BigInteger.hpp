#ifndef _BIGINTEGER_HPP_
#define _BIGINTEGER_HPP_

#include <string>

class BigInteger
{
public:
	BigInteger();
	~BigInteger();
	BigInteger(int num);
	BigInteger(std::string number);
	BigInteger(const BigInteger& obj);
	BigInteger(BigInteger&& obj);

	BigInteger& operator=(const BigInteger& rhs);
	BigInteger& operator=(BigInteger&& rhs);
	static BigInteger multiply(const BigInteger& x, unsigned int y);
	void display(bool debug = false);

	friend std::ostream& operator<<(std::ostream& strm, const BigInteger& obj);
	BigInteger operator+(const BigInteger rhs);
	BigInteger operator*(const BigInteger& rhs);
	BigInteger& operator+=(const BigInteger rhs);
	BigInteger& operator*=(const BigInteger rhs);
	bool operator<=(const BigInteger& rhs);
	bool operator==(const BigInteger& rhs);
	BigInteger operator++(int);
	operator double() const;


private:
	char* m_number;		// Internal representation of the number.
	unsigned int m_sizeReserved;	// Total size of the allocated space used to internally store the number
	unsigned int m_digitCount;	// How many digits are in the number.

	char getDigit(unsigned int position) const;
	void setDigit(unsigned int position, char digit);
	void makeCopyFromOther(const BigInteger& rhs);
	char* initializeArray(std::string number);
};

#endif

