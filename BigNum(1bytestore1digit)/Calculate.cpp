#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "BigNum.h"

//two different algorithm
/*
void add(dword *a, dword *b, dword *c, int pos, int len)
{
	for(int i = MAXLEN - 1; i >= pos; i--)
	{
		c[i] = a[i] + b[i];
		if(c[i] > DIGIT)
		{
			c[i] -= DIGIT;
			c[i-1]++;
		}
	}
}

void addZero(BigNum &a, BigNum &b, BigNum *c)
{
	c -> signnum = 1;
	if(a.isZero() && !b.isZero())
		*c = b;
	else if(!a.isZero() && b.isZero())
		*c = a;
	else
		c -> signnum = 0;
}

void BigNumadd(BigNum &a, BigNum &b,BigNum *c)
{
	FILE *fp=fopen("data.txt","a+");
	LARGE_INTEGER freq;  
	LARGE_INTEGER start_t, stop_t;  
	double exe_time;  

	QueryPerformanceFrequency(&freq);   

	long pos = MAX(a.start, b.start);
	long len = MAX(a.dwLen, b.dwLen);
	c -> start = pos;
	c -> dwLen = len;
	c -> signnum = a.signnum;
	int THREAD = len >= MAXT ? MAXT : len;
	int BLOCK = len >= MAXT ? len / MAXT + 1: 1;

	memset(c->value,0,MAXLEN * sizeof(dword));

	QueryPerformanceCounter(&start_t); 

	add(a.value, b.value, c->value, pos, len);

	QueryPerformanceCounter(&stop_t);  
	exe_time = 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
	fprintf(fp,"时间：%.3lf ms\n", exe_time);  

	if(c->value[pos - 1] != 0)
		c -> start --;

	fclose(fp);
}

void sub(dword *a, dword *b, dword *c, qword *temp, int pos, int len)
{
	for(int i = MAXLEN - 1; i >= pos; i--)
	{
		temp[i] = a[i] - b[i];
		c[i] = temp[i] % DIGIT;
		temp[i] /= DIGIT;
	}
}

void subZero(BigNum &a, BigNum &b, BigNum *c)
{
	if(a.isZero() && b.isZero())
		c -> signnum = 0;
	else if(a.isZero() && !b.isZero())
		*c = -b;
	else
		*c = a;
}

void BigNumsub(BigNum &a, BigNum &b,BigNum *c)
{
	const BigNum *aa, *bb;
	if(a > b)
	{
		aa = &a;
		bb = &b;
	}
	else
	{
		aa = &b;
		bb = &a;
	}

	LARGE_INTEGER freq;  
	LARGE_INTEGER start_t, stop_t;  
	double exe_time;  

	QueryPerformanceFrequency(&freq);   

	long pos = MAX(a.start, b.start);
	long len = MAX(a.dwLen, b.dwLen);
	c -> start = pos;
	c -> dwLen = len;
	c -> signnum = a.signnum;
	int THREAD = len >= MAXT ? MAXT : len;
	int BLOCK = len >= MAXT ? len / MAXT + 1: 1;

	dword *aValue, *bValue, *cValue;
	qword *tempValue;
	dword carry[MAXLEN];

	aValue = (dword *)malloc(MAXLEN * sizeof(dword));
	bValue = (dword *)malloc(MAXLEN * sizeof(dword));
	cValue = (dword *)malloc(MAXLEN * sizeof(dword));
	tempValue = (qword *)malloc(MAXLEN * sizeof(qword));

	memcpy(aValue, a.value, MAXLEN * sizeof(dword));
	memcpy(bValue, b.value, MAXLEN * sizeof(dword));
	memset(cValue,0,MAXLEN * sizeof(dword));
	memset(tempValue, 0, MAXLEN * sizeof(dword));

	QueryPerformanceCounter(&start_t); 

	sub(aValue, bValue, cValue, tempValue, pos, len);

	QueryPerformanceCounter(&stop_t);  
	exe_time = 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
	printf("时间：%.3lf ms\n", exe_time);  

	memcpy(c->value, cValue, MAXLEN * sizeof(dword));
	memcpy(carry, tempValue, MAXLEN * sizeof(dword));
}
*/
void mul(BigNum &a, BigNum &b, qword *cc)
{
	for(int i = 0; i < a.dwLen; i++)
	{
		//printf("now:%d\n", i);
		for(int j = 0; j < b.dwLen; j++)
			if(MAXLEN - 1 -(i + j) >= 0)
				cc[MAXLEN - 1 -(i + j)] += (qword)a.value[MAXLEN - 1 - i] * b.value[MAXLEN - 1 - j];
	}

	for(int i = 0; i < a.dwLen+b.dwLen; i++)
	{
		cc[MAXLEN - 1 - i - 1] += cc[MAXLEN - 1 - i] / DIGIT; 
		cc[MAXLEN - 1 - i] %= DIGIT;

		//printf("%d\n", i);
	}
}

void mulZero(BigNum &a, BigNum &b, BigNum *c)
{
	c->signnum = 0;
}

void BigNummul(BigNum &a, BigNum &b,BigNum *c, int signnum)
{
	FILE *fp=fopen("datamul.txt","a+");
	LARGE_INTEGER freq;  
	LARGE_INTEGER start_t, stop_t;  
	double exe_time;  
	qword *cc;

	if(signnum)
		c->signnum = -1;
	else
		c->signnum = 1;
	c->dwLen = a.dwLen + b.dwLen;
	c->start = MAXLEN - c->dwLen;

	cc = (qword *)malloc(MAXLEN * sizeof(qword));
	memset(cc,0, MAXLEN * sizeof(qword));

	QueryPerformanceFrequency(&freq);   
	QueryPerformanceCounter(&start_t); 

	mul(a, b, cc);

	QueryPerformanceCounter(&stop_t);  
	exe_time = 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
	fprintf(fp,"时间：%.3lf ms\n", exe_time);  

	for(int i = MAXLEN - 1; i >= c->start; i--)
		c->value[i] = cc[i];

	if(!c->value[MAXLEN - (a.dwLen + b.dwLen)])
	{
		c->start++;
		c->dwLen--;
	}

	if(c->start <= 0)
		c->start = 0;

	fclose(fp);
}