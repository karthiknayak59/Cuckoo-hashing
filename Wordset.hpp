#ifndef __WORD_SET___HPP
#define __WORD_SET___HPP

#include <string>

unsigned polynomialHashFunction(const std::string & s, unsigned base, unsigned mod);

class WordSet
{
	std::string *hashtable1;
	std::string *hashtable2;
	std::string *inserted;
	unsigned insertedArrIndex = 0;
	bool rehashed = false;
	unsigned size = 11;
	unsigned count = 0;
public: 
	explicit WordSet();
	~WordSet();
 
	void insert(const std::string & s);

	bool contains(const std::string & s) const;

	unsigned getCount() const; 
	
private:
	// functions h1 and h2 use the polynomial hash function above,
	// each are mod the table size, but h1 uses base=BASE_H1 and 
	// h2 uses BASE_H2.
	static constexpr unsigned BASE_H1 = 33;
	static constexpr unsigned BASE_H2 = 41;    
	bool insertString(std::string s);
	void rehash();
	bool isPrime(unsigned n);
};


#endif
