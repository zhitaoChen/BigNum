#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "BigNum.h"
#include "Calculate.h"

/*
*****************************************************************************************************************************************************
1.CUDA DO NOT support using kernel function in the class
2.Transfering data from GPU to CPU will take much time
3.kernel function CAN NOT invoking function which in HOST
4.kernel function can ONLY define and declare in kernel.cu(main function)
*****************************************************************************************************************************************************
*/

int main(void)
{
	BigNum a, b, c;
	char oper;
	//char temp[MAXLEN];

	printf("Input two value(DO NOT Larger than 10000 digit)\n");
	
	a.make();
	b.make();
	getchar();
	
	/*gets(temp);
	a.getBuf(temp, strlen(temp));
	a.toDword(temp, strlen(temp));

	gets(temp);
	b.getBuf(temp, strlen(temp));
	b.toDword(temp, strlen(temp));*/

	printf("Choose one: + - * /\n");
	oper=getchar();

	switch(oper)
	{
	/*
	case'+':
		if(a.isZero() || b.isZero())
			addZero(a, b, &c);
		else if(a.signnum == b.signnum)
			BigNumadd(a, b, &c);
		else
			BigNumsub(a, b, &c);
		break;
	case'-': 
		if(a.isZero() || b.isZero())
			subZero(a, b, &c);
		else if(a.signnum != b.signnum)
			BigNumadd(a, b, &c);
		else
			BigNumsub(a, b, &c);
		break;
		*/
	case'*': 
		if(a.isZero() || b.isZero())
			mulZero(a, b, &c);
		else if(a.signnum != b.signnum)
			BigNummul(a, b, &c, 1);
		else 
			BigNummul(a, b, &c, 0);
		break;
	/*case'/': BigNumdiv(a, b, &c);	break;*/
	}

	//c.print();
	//c.fprint();

	return 0;
}