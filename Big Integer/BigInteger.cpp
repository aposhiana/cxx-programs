#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>

#include "BigInteger.hpp"


// ------------------------------------------------------------------
//
// This default constructor creates a new array of zero with 4 spaces
// in memory reserved.
//
// ------------------------------------------------------------------
BigInteger::BigInteger() {
	m_sizeReserved = 4;
	m_number = new char[m_sizeReserved];
	for (int i = 0; i < m_sizeReserved; i++) {
		setDigit(i, 0);
	}
	m_digitCount = 0;
}

// ------------------------------------------------------------------
//
// This constructor takes an integer and creates a new array from it
// to store the big integer.
//
// ------------------------------------------------------------------
BigInteger::BigInteger(int num) {
	std::string str = std::to_string(num);
	initializeArray(str);
}

// ------------------------------------------------------------------
//
// This constructor takes a std::string and creates a new array from it
// to store the big integer.
//
// ------------------------------------------------------------------
BigInteger::BigInteger(std::string stringNum) {
	BigInteger::initializeArray(stringNum);
}

// ------------------------------------------------------------------
//
// This destructor cleans up the dynamically allocated array.
//
// ------------------------------------------------------------------
BigInteger::~BigInteger() {
	if (m_number != nullptr) {
		delete[] m_number;
	}
}

// ------------------------------------------------------------------
//
// This method is a copy constructor, accepting a BigInteger and
// and making a copy of it the object, but in a distict memory location.
//
// ------------------------------------------------------------------
BigInteger::BigInteger(const BigInteger& obj) {
	makeCopyFromOther(obj);
}

// ------------------------------------------------------------------
//
// This method defines the assignment operator for BigIntegers, 
// accepting a BigInteger and returning itself (the object) modified,
// making it a copy of the rhs but in a distict memory location.
//
// ------------------------------------------------------------------
BigInteger& BigInteger::operator=(const BigInteger& rhs) {
	delete[] m_number;
	makeCopyFromOther(rhs);
	return *this;
}

// ------------------------------------------------------------------
//
// This method accepts a BigInteger, then creates a copy of it for the
// object caling the method.
//
// ------------------------------------------------------------------
void BigInteger::makeCopyFromOther(const BigInteger& rhs) {
	m_digitCount = rhs.m_digitCount;
	m_sizeReserved = rhs.m_sizeReserved;
	m_number = new char[m_sizeReserved];

	for (int i = 0; i < m_sizeReserved; i++) {
		m_number[i] = rhs.m_number[i];
	}
}

// ------------------------------------------------------------------
//
// This method accepts a std::string and returns a character array
// pointer. It converts the string of numbers into a character array
// of integer values in reverse order, then returns the pointer to
// that character array pointer.
//
// ------------------------------------------------------------------
char* BigInteger::initializeArray(std::string number) {
	m_sizeReserved = number.size();
	m_digitCount = m_sizeReserved;
	m_number = new char[m_sizeReserved];

	for (int i = 0; i < m_sizeReserved; i++) {
		m_number[i] = 0;
	}

	for (int i = m_digitCount - 1, j = 0; i >= 0; i--, j++) {
		char digit = number[j] - '0';
		m_number[i] = digit;
	}

	return m_number;
}

// ------------------------------------------------------------------
//
// This method accepts an unsigned integer and a character, sets that
// character to the position of the unsigned integer in the m_number
// array. If there isn't enough space then it doubles the size of
// the array, then inserts the new digit.
//
// ------------------------------------------------------------------
void BigInteger::setDigit(unsigned int position, char digit) {
	if (position >= m_digitCount && position < m_sizeReserved) {
		m_number[position] = digit;
		m_digitCount++;
	}
	else if (position >= m_sizeReserved) {
		m_sizeReserved *= 2;

		char* newArray = new char[m_sizeReserved];

		for (int i = 0; i < m_sizeReserved; i++) {
			newArray[i] = 0;
		}

		for (int i = 0; i < m_digitCount; i++) {
			newArray[i] = m_number[i];
		}

		//
		// Sets the new digit on the new array
		newArray[position] = digit;

		//
		// Updates digitCount
		m_digitCount++;

		//
		// Puts the contents of the new array into m_number
		std::swap(m_number, newArray);

		//
		// Cleans up the leftover new array we made
		delete[] newArray;
		newArray = nullptr;
	}
	else {
		m_number[position] = digit;
	}
}

// ------------------------------------------------------------------
//
// This method accepts a BigInteger and an integer, multiplies them
// together and returns a new BigInteger with the result.
//
// ------------------------------------------------------------------
BigInteger BigInteger::multiply(const BigInteger& x, unsigned int y)
{
	BigInteger result;
	unsigned int carry = 0;
	unsigned int pos = 0;

	for (unsigned int digit = 0; digit < x.m_digitCount; digit++) {
		carry += x.getDigit(digit) * y;
		result.setDigit(pos++, carry % 10);
		carry /= 10;
	}

	while (carry > 0) {
		result.setDigit(pos++, carry % 10);
		carry /= 10;
	}

	return result;
}

// ------------------------------------------------------------------
//
// Provides a means to display the BigInteger value.  Optionally, some
// debug information can be displayed by setting the 'debug' parameter
// to true.
//
// ------------------------------------------------------------------
void BigInteger::display(bool debug)
{
	for (unsigned int digit = m_digitCount; digit > 0; digit--) {
		std::cout << static_cast<int>(m_number[digit - 1]);
	}

	if (debug) {
		std::cout << " (size = " << m_sizeReserved << ", digit count = " << m_digitCount << ")" << std::endl;
	}
}

// ------------------------------------------------------------------
//
// Returns the digit as the specified positon.  If the position is greater
// that the number representation, a '0' is returned.  Not the best
// idea in the world, but the best we can do for now, can make this better
// in the future.
//
// ------------------------------------------------------------------
char BigInteger::getDigit(unsigned int position) const
{
	if (position < m_digitCount) {
		return m_number[position];
	}

	return 0;
}

// ------------------------------------------------------------------
//
// This method overloads the extraction operator so that we can cout
// a BigInteger.
//
// ------------------------------------------------------------------
std::ostream& operator<<(std::ostream& strm, const BigInteger& obj) {
	for (unsigned int digit = obj.m_digitCount; digit > 0; digit--) {
		std::cout << static_cast<int>(obj.m_number[digit - 1]);
	}
	return strm;
}

// ------------------------------------------------------------------
//
// This method overloads the + operator to be used for BigIntegers.
// It adds the BigInteger object on the right hand side with the 
// object calling the method and returns the sum BigInteger as a 
// value.
//
// ------------------------------------------------------------------
BigInteger BigInteger::operator+(const BigInteger rhs) {
	BigInteger result;
	unsigned int length = (this->m_digitCount > rhs.m_digitCount) ? this->m_digitCount : rhs.m_digitCount;

	int carry = 0;
	for (unsigned int digit = 0; digit < length; digit++) {
		int v1 = this->getDigit(digit);
		int v2 = rhs.getDigit(digit);
		int sum = v1 + v2 + carry;
		int single = sum % 10;
		carry = ((sum - single) > 0) ? (sum - single) / 10 : 0;

		result.setDigit(digit, single);
	}
	if (carry > 0) {
		result.setDigit(length, carry);
	}

	return result;
}

// ------------------------------------------------------------------
//
// This method overloads the += operator to be used for BigIntegers.
// It adds the object on the right hand side to the object calling
// the method and returns the modified object by reference.
//
// ------------------------------------------------------------------
BigInteger& BigInteger::operator+=(const BigInteger rhs) {
	*this = *this + rhs;
	return *this;
}

// ------------------------------------------------------------------
//
// This method overloads the * operator to be used for BigIntegers.
// It multiplies the object calling the operator and the object on
// the right hand side and returns a BigInteger as a value.
//
// This method is a modified version of code originally written by 
// Dean Mathias.
//
// ------------------------------------------------------------------
BigInteger BigInteger::operator*(const BigInteger& rhs) {
	BigInteger result;

	const BigInteger& b = (this->m_digitCount < rhs.m_digitCount) ? *this : rhs;
	const BigInteger& t = (this->m_digitCount < rhs.m_digitCount) ? rhs : *this;

	for (unsigned int bDigit = 0; bDigit < b.m_digitCount; bDigit++) {
		BigInteger temp;
		int v1 = b.getDigit(bDigit);
		int carry = 0;
		

		//
		// This sets the digits in the temporary BigIntegers that need to equal zero to zero.
		// This prevents any null characters from being summed with our result.
		// This was inspired by code written by Hailee Maxwell.
		if (bDigit > 0) {
			for (unsigned int i = 0; i < bDigit; i++) {
				temp.setDigit(i, 0);
			}
		}

		for (unsigned int tDigit = 0; tDigit < t.m_digitCount; tDigit++) {
			int v2 = t.getDigit(tDigit);
			int sum = v1 * v2 + carry;
			int single = sum % 10;
			carry = ((sum - single) > 0) ? (sum - single) / 10 : 0;

			temp.setDigit(bDigit + tDigit, single);
		}
		if (carry > 0) {
			temp.setDigit(bDigit + t.m_digitCount, carry);
		}
		result = result + temp;
	}

	return result;
}

// ------------------------------------------------------------------
//
// This method overloads the *= operator to be used for BigIntegers.
// It multiplies the object calling the method by the number stored
// in the object on the right hand side and returns the new
// BigInteger.
//
// ------------------------------------------------------------------
BigInteger& BigInteger::operator*=(const BigInteger rhs) {
	*this = *this * rhs;
	return *this;
}

// ------------------------------------------------------------------
//
// This method overloads the double conversion operator to be used
// for BigIntegers.  If the number is too big, then it will lose
// precision.
//
// ------------------------------------------------------------------
BigInteger::operator double() const {
	double multiplier = 1;
	double total = 0;
	for (unsigned int i = 0; i < m_digitCount; i++) {
		total += (m_number[i] * multiplier);
		multiplier *= 10;
	}
	return total;
}

// ------------------------------------------------------------------
//
// This move constructor uses an rvalue reference of a BigInteger to
// initialize a new BigInteger.
//
// ------------------------------------------------------------------
BigInteger::BigInteger(BigInteger&& obj) {
	m_number = obj.m_number;
	m_digitCount = obj.m_digitCount;
	m_sizeReserved = obj.m_sizeReserved;
	

	obj.m_digitCount = 0;
	obj.m_sizeReserved = 0;
	obj.m_number = nullptr;
}

// ------------------------------------------------------------------
//
// This method is and overloaded move assignment operator that uses
// an rvalue reference of a BigInteger.
//
// ------------------------------------------------------------------
BigInteger& BigInteger::operator=(BigInteger&& rhs) {
	if (this != &rhs)
	{
		std::swap(m_number, rhs.m_number);
		std::swap(m_sizeReserved, rhs.m_sizeReserved);
		std::swap(m_digitCount, rhs.m_digitCount);
	}

	return *this;
}

// ------------------------------------------------------------------
//
// This method overloads the <= operator for BigIntegers.
//
// ------------------------------------------------------------------
bool BigInteger::operator<=(const BigInteger& rhs) {
	if (this->m_digitCount < rhs.m_digitCount) return true;
	if (this->m_digitCount > rhs.m_digitCount) return false;
	//
	// Have to go digit by digit
	for (int digit = m_digitCount - 1; digit >= 0; digit--)
	{
		if (this->m_number[digit] < rhs.m_number[digit]) return true;
		if (this->m_number[digit] > rhs.m_number[digit]) return false;
	}
	return true;
}

// ------------------------------------------------------------------
//
// This method overloads the postfix increment operator for BigIntegers.
//
// ------------------------------------------------------------------
BigInteger BigInteger::operator++(int) {
	BigInteger temp = *this;
	*this += 1;
	return temp;
}

// ------------------------------------------------------------------
//
// This method overloads the comparison operator for BigIntegers.  It
// takes the object on the right hand side and checks to see if it
// holds the same number as the object calling the method.
//
// ------------------------------------------------------------------
bool BigInteger::operator==(const BigInteger& rhs) {
	for (unsigned int i = 0; i < m_digitCount; i++) {
		if (m_number[i] != rhs.m_number[i]) {
			return false;
		}
	}
	return true;
}