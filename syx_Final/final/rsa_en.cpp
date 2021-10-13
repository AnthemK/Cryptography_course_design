#include <gmpxx.h>
#include <iostream>
#include <stdio.h>
using namespace std;
mpz_class power(mpz_class a, mpz_class x, mpz_class p)
{	//a^x (mod p)
	mpz_class re=1;
	while(x)
	{
		if((x&1)==1)	re=(re*a)%p;
		a=(a*a)%p;
		x>>=1;
	}
	return re;
}
int main(void)
{
	//freopen("2.in","r",stdin);
	int n;
	scanf("%d",&n);
	mpz_class p,q,e,m;
	while(n--)
	{
        gmp_scanf("%Zd%Zd%Zd%Zd", &e, &m, &p, &q);
		//gmp_printf("e=%Zd\np=%Zd\nq=%Zd\nlow=%Zd\n",e,p,q,low);
      	mpz_class N=p*q;
      	mpz_class en=power(m,e,N);
		gmp_printf("%Zd\n",en);//*/
	}
	return 0;
}
