#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "BigIntegerLibrary.hh"
using namespace std;

const int PRIMESIZE = 100;

BigUnsigned genRandBigInt(int l);
bool millerRabin(BigUnsigned n, int k);
BigInteger randInRange(BigInteger b);
bool millerTest(BigUnsigned d, BigUnsigned n);
BigUnsigned largePrime(int l);
BigUnsigned publicKeyGen(BigUnsigned t);
char num[PRIMESIZE];
bool isPrime = false;

int main(){

	int length = PRIMESIZE;

	srand(time(NULL));

	BigUnsigned p = largePrime(length);
	BigUnsigned q = largePrime(length);

	BigUnsigned n = p * q;
	BigUnsigned totient = (p-1) * (q-1);

	BigUnsigned e = publicKeyGen(totient);

	BigUnsigned d = modinv(e, totient);

/*
	// checks correct output
	cout << "p: " << p << endl;
	cout << "q: " << q << endl;
	cout << "n: " << n << endl;
	cout << "totient: " << totient << endl;
	cout << "e : " << e << endl;
	cout << "d: " << d << endl;
*/
	return 0;
}


BigUnsigned publicKeyGen(BigUnsigned t){

	BigUnsigned e(2);
	BigUnsigned result;

	while(e < t){

	result = gcd(e,t);

	if(result == 1)
		return e;
	else
		e++;
	}

}


BigUnsigned largePrime(int l){

	BigUnsigned p;

	while(!isPrime){

	p = genRandBigInt(l);

	if(millerRabin(p,30))
		isPrime = true;

	}
	isPrime = false;
	return p;
}

BigUnsigned genRandBigInt(int l){

	for(int i = 0; i < l; i++){
		num[i] = (1 + rand() % 9) + '0';
	}

	string str = num;	// turn char array to string
	str.erase(str.end()-1);	// gets rid of null character at end of string	

	BigUnsigned f = stringToBigUnsigned(str);

	return f;
}

BigInteger randInRange(BigInteger b){

	BigInteger x(rand());
	BigInteger y = (x % (b-2)) + 2;

	return y;
}


bool millerRabin(BigUnsigned n, int k){

	BigUnsigned d = n-1;
	

	while (d % 2 == 0)
		d = d >> 1;


	for(int i = 0; i < k; i++)
		if(!millerTest(d,n))
			return false;

	return true;

}

bool millerTest(BigUnsigned d, BigUnsigned n){

	BigInteger a = randInRange(n);

	BigUnsigned x = modexp(a,d,n);

	if(x == 1 || x == n-1)
		return true;

	while (d != n-1){
		x = modexp(x, 2, n);
		d = d << 1;

		if(x == 1)
			return false;
		if(x == n-1) 
			return true;
	}

	return false;
}
