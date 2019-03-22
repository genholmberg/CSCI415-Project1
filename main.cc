#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "BigIntegerLibrary.hh"
using namespace std;

const int PRIMESIZE = 3;

BigUnsigned genRandBigInt(int l);
bool millerRabin(BigUnsigned n, int k);
BigUnsigned randInRange(BigUnsigned b);
bool millerTest(BigUnsigned d, BigUnsigned n);
BigUnsigned largePrime(int l);
BigUnsigned publicKeyGen(BigUnsigned t);
string get_plaintext(string filename);
int get_trigraph(string block);
int get_quadragraph(string block);
string encryption(string pt, BigUnsigned key, BigUnsigned modulus);
string decryption(string ciphertext, BigUnsigned private_key, BigUnsigned modulus);
string get_ciphertext(string filename);
void write_to_file(string temp_file, string ciphertext);
BigUnsigned modexpo(BigUnsigned a, BigUnsigned b, BigUnsigned n);
char num[PRIMESIZE+1];
bool isPrime = false;

int main(){

	int length = PRIMESIZE+1;
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


	BigUnsigned key = e;
	BigUnsigned private_key = d;
	BigUnsigned modulus = n;

	/*BigUnsigned key = 13;
	BigUnsigned private_key = 18997;
	BigUnsigned modulus = 35657;*/

	string plaintext = get_plaintext(input);
	string ciphertext = encryption(plaintext, key, modulus);
	write_to_file(encrypted, ciphertext);
	string ct = get_ciphertext(encrypted);
	string pt = decryption(ct, private_key, modulus);
	write_to_file(output, pt);

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

BigUnsigned randInRange(BigUnsigned b){

	BigUnsigned x(rand());
	BigUnsigned y = (x % (b-2)) + 2;

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

bool millerTest(BigUnsigned b, BigUnsigned n){

	BigUnsigned a = randInRange(n);

	BigUnsigned x = modexpo(a,b,n);

	if(x == 1 || x == n-1)
		return true;

	while (b != n-1){
		x = modexpo(x, 2, n);
		b = b << 1;

		if(x == 1)
			return false;
		if(x == n-1) 
			return true;
	}

	return false;
}

// calcs a^b mod n
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

string encryption(string pt, BigUnsigned key, BigUnsigned modulus)
{
	int tri, power;
	BigUnsigned ct, quotient;
	string ciphertext = "", block_ct = "";
	for(int i = 0; i <= pt.size() - 1; i+=3)
	{
		tri = get_trigraph(pt.substr(i, 3));
		ct = modexpo(tri, key, modulus);
		cout << "Ciphertext code for block: " << ct << endl;
		for(int j = 3; j >= 0; j--)
		{
			power = pow(26, j);
			quotient = ct / power;
			quotient = quotient % 26;
			string temp_block = bigUnsignedToString(quotient);
			int temp_ct = stoi(temp_block) + 65;
			block_ct += temp_ct;
		}

		cout << "Block Ciphertext:" << block_ct << endl;
		ciphertext += block_ct;
		block_ct = "";
	}
	cout << "Ciphertext: " << ciphertext << endl;
	return ciphertext;
}

string decryption(string ciphertext, BigUnsigned private_key, BigUnsigned modulus)
{
	int ct_num, power;
	BigUnsigned pt, quotient;
	string plaintext = "", block_pt;

	for(int i = 0; i <= ciphertext.size() - 1; i+=4)
	{
		ct_num = get_quadragraph(ciphertext.substr(i, 4));
		pt = modexpo(ct_num, private_key, modulus);
		cout << "Plaintext code for block: " << pt << endl;
		for(int j = 2; j >= 0; j--)
		{
			power = pow(26, j);
			quotient = pt / power;
			quotient = quotient % 26;
			block_pt = bigUnsignedToString(quotient);
			int temp_pt = stoi(block_pt) + 65;
			plaintext += temp_pt;
		}
	}
	while(plaintext.back() == 'A')
	{
		plaintext = plaintext.substr(0, plaintext.size() - 1);
	}
	cout << "Plaintext: " << plaintext << endl;
	return plaintext;
}

int get_trigraph(string block)
{
	int trigraph = (block[0]-65)*pow(26, 2) + (block[1]-65)*26 + (block[2]-65);
	cout << "Trigraph: " << trigraph << endl;
	return trigraph;
}

int get_quadragraph(string block)
{
	int quadragraph = (block[0]-65)*pow(26, 3) + (block[1]-65)*pow(26, 2) + (block[2]-65)*26 + (block[3]-65);
	cout << "Quadragraph: " << quadragraph << endl;
	return quadragraph;
}

string get_plaintext(string filename)
{
	ifstream file (filename);
	string pt;
	if (file.is_open())
  	{
   	getline(file, pt);
		cout << "Plaintext read from file: " << pt << endl;
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
	ifstream file (filename);
	string ct;
	if (file.is_open())
  	{
   	getline(file, ct);
		cout << "Ciphertext from file: " << ct << endl;
   	file.close();
  	}
	return ct;	
}

void write_to_file(string temp_file, string ciphertext)
{
	ofstream file (temp_file);
	if (file.is_open())
  	{
   	file << ciphertext;
   	file.close();
  	}
}

