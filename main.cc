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
BigInteger randInRange(BigInteger b);
bool millerTest(BigUnsigned d, BigUnsigned n);
BigUnsigned largePrime(int l);
BigUnsigned publicKeyGen(BigUnsigned t);
string get_plaintext(string filename);
int reduce_modn(int a, int b, int n);
int get_trigraph(string block);
string encryption(string pt, int key, BigUnsigned modulus);
void decryption(string ciphertext);
string get_ciphertext(string filename);
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
	// testing will change later
	string filename = "input.txt";
	int key = 43;
	BigUnsigned modulus = 54619;
	string plaintext = get_plaintext(filename);
	cout << "Plaintext after 0s added: " << plaintext << endl;
	string ciphertext = encryption(plaintext, key, modulus);
	decryption(ciphertext);
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

// may not need this
int reduce_modn(int a, int trigraph, int n)
{
	int c = 1, f = 0, k;
	string b = "";
	// to binary
   while(trigraph != 0)
	{
		b = (trigraph%2 == 0 ?"0":"1") + b;
		trigraph /= 2;
	}

	k = b.size() - 1;

	for(int i = k; i > 0; i--) {
		c *= 2;
		f = (f*f) % n;
		if(b[i] == '1')
		{
			c++;
			f = (f*a) % n;
		}
	}
	return f;
}

string encryption(string pt, int key, BigUnsigned modulus)
{
	int tri, temp;
	BigUnsigned ct, quotient;
	string ciphertext = "", block_ct = "";
	for(int i = 0; i < 9; i+=3)
	{
		tri = get_trigraph(pt.substr(i, 3));
		ct = modexp(tri, key, modulus);
		cout << "ciphertext number for block: " << ct << endl;
		for(int j = 3; j >= 0; j--)
		{
			temp = pow(26, j);
			quotient = ct / temp;
			quotient = quotient % 26;
			string stuff = bigUnsignedToString(quotient);
			int morestuff = stoi(stuff) + 65;
			block_ct += morestuff;
		}

		cout << "Block Ciphertext:" << block_ct << endl;
		ciphertext += block_ct;
		block_ct = "";
	}
	cout << "Ciphertext: " << ciphertext << endl;
	return ciphertext;
}

void decryption(string ciphertext)
{
	
}

int get_trigraph(string block)
{
	int trigraph = (block[0]-65)*pow(26, 2) + (block[1]-65)*26 + (block[2]-65);
	cout << "block plaintext: " << block << endl;
	cout << "trigraph: " << trigraph << endl;
	return trigraph;
}

string get_plaintext(string filename)
{
	ifstream file (filename);
	string pt;
	if (file.is_open())
  	{
   	getline(file, pt);
		cout << "Plain text: " << pt << endl;
   	file.close();
  	}
	
	// add 0s to the end
	while (pt.size() < 9)
	{
		pt += '0';
	}
	
	return pt;	
}

string get_ciphertext(string filename)
{
	ifstream file (filename);
	string ct;
	if (file.is_open())
  	{
   	getline(file, ct);
		cout << "Cipher text: " << ct << endl;
   	file.close();
  	}
	return ct;	
}

