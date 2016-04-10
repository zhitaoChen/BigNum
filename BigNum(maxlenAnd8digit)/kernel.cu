#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <sm_20_atomic_functions.h>
#include <cuda.h>
#include "BigNum.h"

/*
位数改变，方法不变。 
*/

/*
*****************************************************************************************************************************************************
1.CUDA DO NOT support using kernel function in the class
2.Transfering data from GPU to CPU will take much time
3.kernel function CAN NOT invoking function which in HOST
4.kernel function can ONLY define and declare in kernel.cu(main function)
*****************************************************************************************************************************************************
*/

//Need to optimize
__global__ void mul(dword *a, dword *b, qword *c, int aLen, int bLen)
{
	__shared__ qword x[1024];
	int tId = threadIdx.x + blockIdx.x * blockDim.x;//1串中的大段
	int xp = threadIdx.x;//大段中位于第几小段

	x[xp] = 0;

	for(int i = 0; i < aLen; i++)
	{
		int temp_p = tId - i;

		if(temp_p >= 0 && temp_p < bLen)
			x[xp] += (qword)a[MAXLEN - 1 - i] * b[MAXLEN - 1 - temp_p]; 
		else if(temp_p < 0)
			break;
	}

	c[MAXLEN -1 - tId] = x[xp];
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
	cudaEvent_t start,stop;
	float elapsedtime = 0;

	qword *cc;
	qword *cValue;
	dword *aValue, *bValue;

	int len = a.dwLen + b.dwLen;
	int THREAD = len >= MAXT ? MAXT : len;
	int BLOCK = len >= MAXT ? len / MAXT + 1: 1;
	//int BLOCK = len >= MAXT ? (len + THREAD - 1) / THREAD: 1;

	if(signnum)
		c->signnum = -1;
	else
		c->signnum = 1;
	c->dwLen = a.dwLen + b.dwLen;
	c->start = MAXLEN - c->dwLen;

	cc = (qword *)malloc(MAXLEN * sizeof(qword));
	cudaMalloc((void **)&aValue, MAXLEN * sizeof(dword));
	cudaMalloc((void **)&bValue, MAXLEN * sizeof(dword));
	cudaMalloc((void **)&cValue, MAXLEN * sizeof(qword));
	
	cudaMemcpy(aValue, a.value, MAXLEN * sizeof(dword), cudaMemcpyHostToDevice);
	cudaMemcpy(bValue, b.value, MAXLEN * sizeof(dword), cudaMemcpyHostToDevice);
	cudaMemset(cValue,0,MAXLEN * sizeof(qword));
	cudaEventCreate(&start);//gpu time
	cudaEventCreate(&stop);
	cudaEventRecord(start,0);

	mul<<<BLOCK, THREAD>>>(aValue, bValue, cValue, a.dwLen, b.dwLen);
	//cudaMemcpy(cc, cValue, MAXLEN * sizeof(qword), cudaMemcpyDeviceToHost);

	cudaEventRecord(stop,0);
	cudaEventSynchronize(start); 
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedtime,start,stop);
	fprintf(fp,"处理时间：%.3f ms\n",elapsedtime);
	
	cudaEventCreate(&start);//gpu time
	cudaEventCreate(&stop);
	cudaEventRecord(start,0);
	cudaMemcpy(cc, cValue, MAXLEN * sizeof(qword), cudaMemcpyDeviceToHost);
	cudaEventRecord(stop,0);
	cudaEventSynchronize(start); 
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedtime,start,stop);
	//fprintf(fp,"拷贝时间：%.3f ms\n",elapsedtime);
	
	QueryPerformanceFrequency(&freq);  
	QueryPerformanceCounter(&start_t);//cpu time 

	for(int i = 0; i < a.dwLen+b.dwLen; i++)
	{
		cc[MAXLEN - 1 - i - 1] += cc[MAXLEN - 1 - i] / DIGIT; 
		cc[MAXLEN - 1 - i] %= DIGIT;
	}

	QueryPerformanceCounter(&stop_t);  
	exe_time = 1e3*(stop_t.QuadPart-start_t.QuadPart)/freq.QuadPart;  
	//fprintf(fp,"时间：%.3f ms\n",exe_time+elapsedtime);

	//useless management
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
	cudaFree(aValue);
	cudaFree(bValue);
	cudaFree(cValue);
}

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