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

	BigUnsigned p = largePrime(length);
	BigUnsigned q = largePrime(length);

	BigUnsigned n = p * q;
	BigUnsigned totient = (p-1) * (q-1);

	BigUnsigned e = publicKeyGen(totient);

	BigUnsigned d = modinv(e, totient);

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


BigUnsigned publicKeyGen(BigUnsigned t){

	BigUnsigned e(t/3);
	BigUnsigned result;

	while(e < t){

	result = gcd(e,t);

	if(result == 1)
		return e;
	else
		e++;
	}
	return e;
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
		num[i] = (rand() % 9) + 1  + '0';
	}
	num[PRIMESIZE]=0;

	string str = num;	// turn char array to string
	BigUnsigned f = stringToBigUnsigned(str);

	return f;
}

BigUnsigned randInRange(BigUnsigned b){

	BigUnsigned x(rand());
	BigUnsigned y = (x % (b-2)) + 2;


	return y;
}


bool millerRabin(BigUnsigned n, int k){

	BigUnsigned d = n-1;
	

	while (d % 2 == 0){
		d = d >> 1;
	}


	for(int i = 0; i < k; i++){
		if(!millerTest(d,n)){
			return false;
		}
	}

	return true;

}

bool millerTest(BigUnsigned b, BigUnsigned n){

	BigUnsigned a = randInRange(n);

	BigUnsigned x = modexpo(a,b,n);

	if(x == 1 || x == n-1){
		return true;
	}

	while (b != n-1){
		x = modexpo(x, 2, n);
		b = b << 1;

		if(x == 1){
			return false;
		}
		if(x == n-1){
			return true;
		}
	}

	return false;
}

// calculates a^b mod n
BigUnsigned modexpo(BigUnsigned a, BigUnsigned b, BigUnsigned n){

	int index = b.bitLength();	// get number of bits

	BigUnsigned f = 1;

	for(int i = index; i >= 0; i--){

		f = (f * f) % n;

		if(b.getBit(i) == 1){
			f = (f * a) % n;
		}

	}

	return f;
}

// encrypts the plain text one block (3 bytes) at a time, concatenates the 4 bytes cipher text blocks together and returns the whole ciphertext
// Pre:
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

