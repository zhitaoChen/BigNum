#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DIGIT 10
#define MAX(x, y) x > y? x : y
#define MIN(x, y) x > y? y : x
#define MAXT 1024
#define CR putchar('\n')
//#define MAXLEN 250001
#define MAXLEN 900001
#define MAXLENMUL 20001
#define buf_MAXLEN 1
#define HALFDIGIT 100000

typedef unsigned char byte, *pbyte;
typedef unsigned short word, *pword;
typedef unsigned long dword, *pdword;
//typedef char dword, *pdword;
typedef unsigned __int64 qword, *pqword;

/*
**** Some BigInteger ADT ****

1.      BCDASC byte  a[i] = '0' ~ '9'
2.        BCD1 byte  a[i] = 0 ~ 9¡£byte * byte = byte¡£
3. Packed_BCD1 byte  a[i] = 00 ~ 99¡£byte * byte = word¡£
4. Packed_BCD2 word  a[i] = 0000 ~ 9999¡£word * word = dword¡£
5. Packed_BCD4 dword a[i] = 00000000 ~ 99999999¡£or 000000000 ~ 999999999¡£dword * dword = qword¡£
6.      Binary dword a[i] = 00000000000000000000000000000000 ~ 11111111111111111111111111111111¡£

1. biLen : length by bit£»
2. byLen : length by byte£»
3. woLen : length by word£»
4. dwLen : length by dword¡£
*/

struct BigNum 
{ 
	dword *value; 
	long start; //position
	long dwLen; //length
	long signnum; //(0,1,-1)
	char buf[buf_MAXLEN];

	void reset() 
	{ 
		signnum = 0;
		dwLen = 0; 
		start = MAXLEN-1;
		value = (dword *)malloc(MAXLEN * sizeof(dword));
		memset(value, 0, MAXLEN * sizeof(dword));
	}
	BigNum() { reset(); }
	/*BigNum(char *p, long l)	
	{ 
		reset();
		memcpy(buf, p, l * sizeof(long));
	}*/

	BigNum operator =(const BigNum *a);
	BigNum operator -();
	void make();
	void getBuf(char *p, int len);
	bool isZero() const;
	void print(int radix = 10) const; //print the big number
	void fprint(int radix = 10) const;
	void toDword(char* pbuf, int len, int radix = 10);
	int  compareTo(const BigNum& b) const; //whats "const" use for????(0,1,-1)
	void correctDwLen(); //check out the length of the number is correct, NOT write
	void dump(FILE*); //output to text to check out result is correct or not. NOT write
};

bool operator >(const BigNum &a,const BigNum &b);
bool operator >=(const BigNum &a,const BigNum &b);
bool operator <(const BigNum &a,const BigNum &b);
bool operator <=(const BigNum &a,const BigNum &b);
bool operator ==(const BigNum &a,const BigNum &b);
bool operator !=(const BigNum &a,const BigNum &b);