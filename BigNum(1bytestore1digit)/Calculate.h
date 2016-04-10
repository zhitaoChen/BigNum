//#include "BigNum.h"
void add(dword *a, dword *b, dword *c, int pos, int len);
void addZero(BigNum &a, BigNum &b, BigNum *c);
void BigNumadd(BigNum &a, BigNum &b,BigNum *c);
void sub(dword *a, dword *b, dword *c, qword *temp, int pos, int len);
void subZero(BigNum &a, BigNum &b, BigNum *c);
void BigNumsub(BigNum &a, BigNum &b,BigNum *c);
void mul(BigNum *a, BigNum *b, qword *cc);
void mulZero(BigNum &a, BigNum &b, BigNum *c);
void BigNummul(BigNum &a, BigNum &b,BigNum *c, int signnum);