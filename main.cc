#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "BigIntegerLibrary.hh"
using namespace std;

const int PRIMESIZE = 100;

BigUnsigned genRandBigInt(int l);
bool millerRabin(BigUnsigned n, int k);
BigUnsigned randInRange(BigUnsigned b);
bool millerTest(BigUnsigned d, BigUnsigned n);
BigUnsigned largePrime(int l);
BigUnsigned publicKeyGen(BigUnsigned t);
string get_plaintext(string filename);
BigUnsigned get_trigraph(string block);
BigUnsigned get_quadragraph(string block);
string encryption(string pt, BigUnsigned key, BigUnsigned modulus);
string decryption(string ciphertext, BigUnsigned private_key, BigUnsigned modulus);
string get_ciphertext(string filename);
void write_to_file(string temp_file, string ciphertext);
BigUnsigned modexpo(BigUnsigned a, BigUnsigned b, BigUnsigned n);
char num[PRIMESIZE+1];
bool isPrime = false;

int main(){

	int length = PRIMESIZE +1;
	string input = "input.txt";
	string encrypted = "encrypted.txt";
	string output = "output.txt";

	srand(time(NULL));

	BigUnsigned p = largePrime(length);	// get random large prime
	BigUnsigned q = largePrime(length);	// get random large prime

	BigUnsigned n = p * q;
	BigUnsigned totient = (p-1) * (q-1);

	BigUnsigned e = publicKeyGen(totient);	// generate public key

	BigUnsigned d = modinv(e, totient);	// performs the multiplicative inverse to get d

	// checks correct output
	cout << "p: " << p << endl;
	cout << "q: " << q << endl;
	cout << "n: " << n << endl;
	cout << "totient: " << totient << endl;
	cout << "e : " << e << endl;
	cout << "d: " << d << endl;
	
	string plaintext = get_plaintext(input);
	string ciphertext = encryption(plaintext, e, n);
	write_to_file(encrypted, ciphertext);
	string ct = get_ciphertext(encrypted);
	string pt = decryption(ct, d, n);
	write_to_file(output, pt);

	return 0;
}

/*
* Generates the public key "e" with the condition that the greatest common divisor
* of some number e and the totient is 1.
* Params: This method takes in the totient as it's parameter.
* Pre: The totient must already be initialized.
* Post: Returns the public key.
*/
BigUnsigned publicKeyGen(BigUnsigned t){

	BigUnsigned e(t/3);
	BigUnsigned result;

	while(e < t){	// is e less than the totient?

	result = gcd(e,t);	// result is assigned the greatest common divisor of e and the totient

	if(result == 1)	// is result equal to 1?
		return e;	// yes, return e
	else
		e++;	// no, increment e by 1
	}// end while
	return e;
}// end publicKeyGen


/*
* This method generates a random large number and checks its primality using the
* Miller-Rabin Test.
* Params: This method takes in an integer for the length of the number in digits.
* Pre: The length must already be determined.
* Post: Returns a large prime number.
*/
BigUnsigned largePrime(int l){

	BigUnsigned p;

	while(!isPrime){	// loops until it finds a prime number

	p = genRandBigInt(l);	// generate random large number

	if(millerRabin(p,30))	// runs the Miller-Rabin Test on p to check primality
		isPrime = true;	// p is prime

	}// end while
	isPrime = false; // p is not prime
	return p;
}// end largePrime


/*
* This method generates a string of random numbers from 0-9 and converts it to BigUnsigned.
* Params: This method takes in an integer for the length of the number in digits.
* Pre: The length must already be determined.
* Post: Returns a random large number.
*/
BigUnsigned genRandBigInt(int l){

	for(int i = 0; i < l; i++){
		num[i] = (rand() % 9) + 1  + '0';	// cast random integer to char, and add to char array
	}
	num[PRIMESIZE]=0;	// initialize char array to null

	string str = num;	// turn char array to string
	BigUnsigned f = stringToBigUnsigned(str);	// convert string to BigUnsigned

	return f;
}// end genRandBigInt


/*
* This method generates a random number between [2,b-2], where b is a random number
* being checked for primality.
* Params: This method takes in a BigUnsigned number that is being checked for primality.
* Pre: The large number must be initialized.
* Post: Returns a random number between [2,b-2].
*/
BigUnsigned randInRange(BigUnsigned b){

	// rand() % (b-2) + 2
	BigUnsigned x(rand());
	BigUnsigned y = (x % (b-2)) + 2;	// gets a random number between [2,b-2]


	return y;
}// end randInRange


/*
* This method performs the Miller-Rabin test on a random large number to check its primality.
* Params: This method takes in an a random large number and an integer for the number of rounds
* the test must perform.
* Pre: Random large number must already be generated, and the number of rounds predetermined.
* Post: Returns true if number is "probably" prime, false if it is not.
*/
bool millerRabin(BigUnsigned n, int k){

	BigUnsigned d = n-1;	// assign d to n - 1
	
	// writes n as 2^r * d + 1 with d odd (by factoring out powers of 2 from n - 1)
	while (d % 2 == 0){
		d = d >> 1; // bit shift d to the right 1
	}// end while

	// loop for the number of rounds k
	for(int i = 0; i < k; i++){
		if(!millerTest(d,n)){	// is this check prime?
			return false;	// not prime
		}// end if
	}// end for

	return true;	// "probably" prime

}// end millerRabin


/*
* This method performs the inner k rounds of the Miller-Rabin Test, if the number is prime
* the test returns true, else returns false.
* Params: This method takes in two BigUnsigned integers.
* Pre: d must be initialized in the millerRabin method, and n must be initialized.
* Post: Returns true if number is "probably" prime, false if it is not.
*/
bool millerTest(BigUnsigned b, BigUnsigned n){

	BigUnsigned a = randInRange(n);	// get random number between [2,b-2]

	BigUnsigned x = modexpo(a,b,n);	// x is assigned a^b mod n

	if(x == 1 || x == n-1){	// if x == 1 or x == n - 1 continue to next round
		return true;
	}// end if

	while (b != n-1){	// loop while b is not n - 1
		x = modexpo(x, 2, n);	// x is assigned x^2 mod n
		b = b << 1;	// bit shift b to the left 1

		if(x == 1){
			return false;
		}
		if(x == n-1){
			return true;
		}
	}// end while

	return false;	// not prime
}// end millerTest



/*
* This method performs modular arithmetic on large integers (a^b mod n).  This method was taken from Appendix A
* of Project 1 for CSCI 415.
* Params: This method takes in three BigUnsigned integers.
* Pre: a, b, and n must already be initialized.
* Post: Returns the result of a^b mod n.
*/
BigUnsigned modexpo(BigUnsigned a, BigUnsigned b, BigUnsigned n){

	int index = b.bitLength();	// get number of bits for b

	BigUnsigned f = 1;

	for(int i = index; i >= 0; i--){	// loop for total number of bits

		f = (f * f) % n;

		if(b.getBit(i) == 1){	// get last bit of b
			f = (f * a) % n;
		}

	}// end for

	return f;
}// end modexpo

string encryption(string pt, BigUnsigned key, BigUnsigned modulus)
{
	BigUnsigned tri, power;
	BigUnsigned ct, quotient;
	string ciphertext = "", block_ct = "";
	for(int i = 0; i < pt.size(); i+=3)
	{
		string block_pt = pt.substr(i, 3);
		tri = get_trigraph(block_pt);
		ct = modexpo(tri, key, modulus);
		block_ct = "";

		for(int j = 3; j >= 0; j--)
		{
			BigUnsigned base = 26;
			quotient = ct % base;
			ct = (ct - quotient) / base;;
			block_ct += (char) (quotient.toInt() + 65);
		}
		cout << "Block ciphertext: " << block_ct << endl;
		ciphertext += block_ct;
	}
	cout << "Ciphertext: " << ciphertext << endl;
	return ciphertext;
}

string decryption(string ciphertext, BigUnsigned private_key, BigUnsigned modulus)
{
	BigUnsigned ct_num, power;
	BigUnsigned pt, quotient;
	string plaintext = "";
	char block_pt[4];

	for(int i = 0; i < ciphertext.size(); i+=4)
	{
		string block_ct = ciphertext.substr(i, 4);
		ct_num = get_quadragraph(block_ct);
		pt = modexpo(ct_num, private_key, modulus);
		for(int j = 2; j >= 0; j--)
		{
			BigUnsigned base = 26;
			quotient = (pt % base);
			pt = (pt - quotient) / base;
			block_pt[j] = (char)(quotient.toInt() + 65);
		}
		block_pt[3] = 0;
		cout << "Block plaintext: " << block_pt << endl;
		plaintext += block_pt;
	}
	while(plaintext[plaintext.size()-1] == 'A')
	{
		plaintext = plaintext.substr(0, plaintext.size() - 1);
	}
	cout << "Plaintext: " << plaintext << endl;
	return plaintext;
}

BigUnsigned get_trigraph(string block)
{
	BigUnsigned trigraph = 0;

	for (int i = 0; i <= 2; i++){
		BigUnsigned nbr(block[i] - 65);
		int powerInt = pow(26, 2-i);
		BigUnsigned power(powerInt);
		trigraph += nbr * power;
	}
	cout << "Trigraph: " << trigraph << endl;
	return trigraph;
}

BigUnsigned get_quadragraph(string block)
{
	BigUnsigned quadragraph = 0;

	for (int i = 0; i <= 3; i++){
		BigUnsigned nbr(block[i] - 65);
		int powerInt = pow(26, i);
		BigUnsigned power(powerInt);
		BigUnsigned addval = nbr * power;
		quadragraph += addval;
	}
	cout << "Quadragraph: " << quadragraph << endl;
	return quadragraph;
}

string get_plaintext(string filename)
{
	ifstream file (filename.c_str());
	string pt;
	if (file.is_open())
  	{
   	getline(file, pt);
		cout << "\nPlaintext read from file: " << pt << endl;
   	file.close();
  	}
	
	// add 0s to the end
	while (pt.size() % 3 != 0)
	{
		pt += 'A';
	}
	
	return pt;	
}

string get_ciphertext(string filename)
{
	ifstream file (filename.c_str());
	string ct;
	if (file.is_open())
  	{
   	getline(file, ct);
		cout << "\nCiphertext from file: " << ct << endl;
   	file.close();
  	}
	return ct;	
}

void write_to_file(string temp_file, string ciphertext)
{
	ofstream file (temp_file.c_str());
	if (file.is_open())
  	{
   	file << ciphertext;
   	file.close();
  	}
}

