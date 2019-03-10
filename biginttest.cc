#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "BigIntegerLibrary.hh"
using namespace std;

int main(){

	int length = 100;

	char num[100];

	srand(time(NULL));

	BigInteger p = generatePrime();

	BigInteger q = generatePrime();

	if(q == p)
		q = generatePrime();

	BigInteger n = q * p;


}

	BigInteger generatePrime(){

	for(int i = 0; i < length; i++){
		num[i] = (1 + rand() % 9) + '0';
	}


	string str(num);	// turn char array to string
	str.erase(str.end()-1);	// gets rid of null character at end of string	

	BigInteger f = stringToBigInteger(str);

	return f;

	}