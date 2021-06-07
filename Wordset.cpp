#include "Wordset.hpp"
#include <cmath>	
#include <string>
#include <iostream>

unsigned polynomialHashFunction(const std::string & s, unsigned base, unsigned mod)
{
	unsigned sum = 0;
	unsigned power = 0;
	for(int index = s.size()-1; index >= 0; index--, power++)
	{
		sum +=  (unsigned)(fmod((s[index] - 'a' + 1) * pow(base,power),(double)mod));
		sum %= mod;
	}
	return (sum % mod);
}

WordSet::WordSet()
{
	hashtable1 = new std::string[size];
	hashtable2 = new std::string[size];
	inserted = new std::string[size*2];
}

WordSet::~WordSet()
{
	delete[] hashtable2;
	delete[] hashtable1;
	delete[] inserted;
}

void WordSet::insert(const std::string & s)
{
	if(contains(s))
	return;

	count++;

	while(!insertString(s))
	{
		// rehash when we find cycle and insert fails.
		while(!rehash())
		{
			// keep rehashing till no cycles. Then attempt to insert new value.
		}
	}
	inserted[insertedArrIndex++] = s;
	if(rehashed == true)
	{
		rehashed = false;
		std::string *newInserted = new std::string[size*2];
		for (unsigned i = 0;i < size*2 && inserted[i] != ""; i++) {
			newInserted[i] = inserted[i];
		}
		delete[] inserted;
		inserted = newInserted;
	}
}

bool WordSet::insertString(std::string s)
{
	for (int tries = 0; tries < log2(2*size); tries+= 2)
	{
		unsigned hash1 = polynomialHashFunction(s, BASE_H1, size);
		if(hashtable1[hash1] != "")
		{
			std::string temp = hashtable1[hash1];
			hashtable1[hash1] = s;
			s = temp;
		}
		else
		{
			hashtable1[hash1] = s;
			return true;
		}
		unsigned hash2 = polynomialHashFunction(s, BASE_H2, size);
		if(hashtable2[hash2] != "")
		{
			std::string temp = hashtable2[hash2];
			hashtable2[hash2] = s;
			s = temp;
		}
		else
		{
			hashtable2[hash2] = s;
			return true;
		}
	}
	return false;
}

bool WordSet::rehash()
{
	rehashed = true;
	size = 2*size+1;
	while(!isPrime(size))
	{
		size+=2;
	}

	delete[] hashtable2;
	delete[] hashtable1;

	hashtable2 = new std::string[size];
	hashtable1 = new std::string[size];

	for(unsigned i = 0; i< size-1 && inserted[i] != ""; i++)
	{
		if(!insertString(inserted[i]))
		{
			std::cout<<"\n Failed to insert.";
			return false;
		}
	}
	return true;
}


bool WordSet::isPrime(unsigned n)
{
	if (n <= 1)
		return false;

	for (unsigned i = 2; i < n; i++)
		if (n % i == 0)
			return false;

	return true;
}

bool WordSet::contains(const std::string & s) const
{
	unsigned hash1 = polynomialHashFunction(s, BASE_H1, size);
	unsigned hash2 = polynomialHashFunction(s, BASE_H2, size);
	bool result = (hashtable1[hash1] == s || hashtable2[hash2] == s);
	return result;
}

unsigned WordSet::getCount() const
{
	return count; 
}

