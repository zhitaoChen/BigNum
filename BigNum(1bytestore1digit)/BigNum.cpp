#include "BigNum.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void BigNum::getBuf(char *p, int len)
{
	memcpy(buf, p, len * sizeof(char));
}

void BigNum::make()
{
	int digit;

	reset();
	scanf("%d", &digit);

	this->signnum = 1;
	this->dwLen = digit;
	this->start = MAXLEN - digit;

	srand((unsigned)time(NULL));
	for(int i = MAXLEN - 1; i > MAXLEN - 1 - digit; i--)
			this->value[i] = rand() * rand() % DIGIT;

	//this->print();
}

void BigNum::print(int radix) const
{
	if(signnum == -1)
		printf("-");
	else if(signnum == 0)
	{
		printf("0\n");
		return;
	}

	if(radix == 10)
	{
		for(int i = start; i < MAXLEN; i++)
		{
			int wei = DIGIT/10;

			while(wei)
			{
				if(value[i] / wei)
					wei = 0;
				else 
				{
					wei /= 10;
					printf("0");
				}
			}
			printf("%d", value[i]);
		}
		printf("\n");
	}
}

void BigNum::fprint(int radix) const
{
	FILE *fp = fopen("result.txt","a+");

	if(signnum == -1)
		fprintf(fp,"-");
	else if(signnum == 0)
	{
		fprintf(fp,"0\n");
		return;
	}

	if(radix == 10)
	{
		for(int i = start; i < MAXLEN; i++)
		{
			int wei = DIGIT;

			while(wei)
			{
				if(value[i] / wei)
					wei = 0;
				else 
				{
					wei /= 10;
					fprintf(fp,"0");
				}
			}
			fprintf(fp,"%d", value[i]);
		}
		fprintf(fp,"\n");
	}

	fclose(fp);
}

bool BigNum::isZero() const 
{
	if(value[start] == 0)
		return true;
	return false;
}

void BigNum::toDword(char* pbuf, int len, int radix)
{
	int p = len;
	int st = 0;

	if(pbuf[0] == '0')
	{
		signnum = 0;
		return;
	}
	else if(pbuf[0] == '-')
	{
		signnum = -1;
		st++;
	}
	else 
		signnum = 1;

	dword temp;

	p--;
	for(; pbuf[p] >= '0' && pbuf[p] <= '9' && p < len; p--)
	{
		temp=0;

		if(pbuf[p] >= '0' && pbuf[p] <= '9')
			temp = temp*10 + pbuf[p] - '0';

		value[start] = temp;
		start--;
	}
	p++;

	temp = 0;
	for(int i = st; i < p; i++)
		temp = temp * 10 + pbuf[i] - '0';

	if(temp)
		value[start--] = temp;

	start++;
	dwLen = MAXLEN - start;
}

int BigNum::compareTo(const BigNum& b) const
{
	if(signnum == 0 && b.signnum == 0)
		return 0;
	else if(signnum == 0 && b.signnum != 0)
		return -1;
	else if(signnum != 0 && b.signnum == 0)
		return 1;

	if(start != b.start)
		return start > b.start ? 1 : -1;
	else
	{
		for(int i = start; i <= MAXLEN-1; i++)
		{
			if(value[i] != b.value[i])
				return value[i]>b.value[i] ? 1 : -1;
		}

		return 0;
	}
}

bool operator >(const BigNum &a,const BigNum &b)
{
	if(a.signnum == 0 && b.signnum !=0)
		return false;
	else if(a.signnum != 0 && b.signnum == 0)
		return true;
	else if(a.signnum == 0 && b.signnum == 0)
		return false;
	else if(a.signnum != 0 && b.signnum != 0)
	{
		if(a.dwLen != b.dwLen)
			return a.dwLen > b.dwLen ? true : false;
		else
		{
			for(int i = a.start; i < MAXLEN-1; i++)
			{
				if(a.value[i] != b.value[i])
					return a.value[i] > b.value[i] ? true : false;
			}

			return false;
		}
	}
}

bool operator >=(const BigNum &a,const BigNum &b)
{
	if(a.signnum == 0 && b.signnum !=0)
		return false;
	else if(a.signnum != 0 && b.signnum == 0)
		return true;
	else if(a.signnum == 0 && b.signnum == 0)
		return true;
	else if(a.signnum != 0 && b.signnum != 0)
	{
		if(a.start != b.start)
			return a.start > b.start ? true : false;
		else
		{
			for(int i = a.start; i < MAXLEN-1; i++)
			{
				if(a.value[i] != b.value[i])
					return a.value[i] > b.value[i] ? true : false;
			}

			return true;
		}
	}
}

bool operator <(const BigNum &a,const BigNum &b)
{
	if(a.signnum == 0 && b.signnum !=0)
		return true;
	else if(a.signnum != 0 && b.signnum == 0)
		return false;
	else if(a.signnum == 0 && b.signnum == 0)
		return false;
	else if(a.signnum != 0 && b.signnum != 0)
	{
		if(a.start != b.start)
			return a.start > b.start ? false : true;
		else
		{
			for(int i = a.start; i < MAXLEN-1; i++)
			{
				if(a.value[i] != b.value[i])
					return a.value[i] > b.value[i] ? false : true;
			}

			return false;
		}
	}
}

bool operator <=(const BigNum &a,const BigNum &b)
{
	if(a.signnum == 0 && b.signnum !=0)
		return true;
	else if(a.signnum != 0 && b.signnum == 0)
		return false;
	else if(a.signnum == 0 && b.signnum == 0)
		return true;
	else if(a.signnum != 0 && b.signnum != 0)
	{
		if(a.start != b.start)
			return a.start > b.start ? false : true;
		else
		{
			for(int i = a.start; i < MAXLEN-1; i++)
			{
				if(a.value[i] != b.value[i])
					return a.value[i] > b.value[i] ? false : true;
			}

			return true;
		}
	}
}

bool operator ==(const BigNum &a,const BigNum &b)
{
	if(a.signnum == 0 && b.signnum == 0)
		return true;
	else if(a.start == b.start)
	{
		for(int i = a.start; i < MAXLEN-1; i++)
		{
			if(a.value[i] != b.value[i])
				return false;
		}
	}

	return false;
}

bool operator !=(const BigNum &a,const BigNum &b)
{
	if(a.signnum == 0 && b.signnum == 0)
		return false;
	else if(a.start == b.start)
	{
		for(int i = a.start; i < MAXLEN-1; i++)
		{
			if(a.value[i] != b.value[i])
				return true;
		}
	}

	return true;
}

BigNum BigNum::operator =(const BigNum *a)
{
	memcpy(this -> value, a -> value, MAXLEN * sizeof(dword));
	memcpy(this -> buf, a -> buf, MAXLEN * sizeof(dword));
	this -> start = a -> start;
	this -> signnum = a ->start;
	this -> dwLen = dwLen;
	return *this;
}

BigNum BigNum::operator -()
{
	BigNum temp = *this;
	temp.signnum = -(this -> signnum);
	return temp;
}