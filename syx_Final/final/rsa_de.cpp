#include <gmpxx.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;
int wei[40];
mpz_class tk[32];
mpz_class exgcd(mpz_class a,mpz_class b,mpz_class &s,mpz_class &t)
{
    if(b)
    {
        mpz_class s1,t1;
        mpz_class c=exgcd(b,a%b,s1,t1);
        s=t1,t=s1-a/b*t1;
        return c;
    }
    s=1,t=0;
    return a;
}
mpz_class get_inv(mpz_class g,mpz_class p)
{
	mpz_class g_inv,t,d;
	d=exgcd(g,p,g_inv,t);
	if(d!=1)	return g_inv=-1;
	if(g_inv<0)	g_inv+=p;
	return g_inv;
}
mpz_class power(mpz_class a, mpz_class x, mpz_class p)
{	//a^x (mod p)	
	memset(wei,0,sizeof(wei));
	int t=0;
  	mpz_class re=1,cur;
	while(x)
	{
		cur=x&0x3f;
      	if((cur&0x20) >0)	wei[t]|=0x20;
		if((cur&0x10) >0)	wei[t]|=0x10;
		if((cur&0x8)  >0)	wei[t]|=0x8;
		if((cur&0x4)  >0)	wei[t]|=0x4;
		if((cur&0x2)  >0)	wei[t]|=0x2;
		if((cur&0x1) >0)	wei[t]|=0x1;	
		x>>=6;
		t++;
	}
	tk[0]=1;
	for(int i=1; i<64; i++)
	{
		tk[i]=(tk[i-1]*a)%p;
	}
	t--;
	while(t>=0)
	{
      re=(re*re)%p;
	  re=(re*re)%p;
      re=(re*re)%p;
      re=(re*re)%p;
      re=(re*re)%p;
      re=(re*re)%p;
		re=(re*tk[wei[t]])%p;
		t--;
	}
	return re;
	/*for(int l=0; l<t; )
	{
		while(l<t && wei[l]==0)	l++;
		if(wei[l+1]==0)	l+=2;
		else
		{
			int r=l+2;
			while(wei[r]==1)	r++;
			wei[r]=1,wei[l]=-1;
			l=r;
		}
	}
	if(wei[t]==0)	t--;
	mpz_class re=1,inv=get_inv(a,p);
	while(t)
	{
		re=re*re;
		if(re>p)	re=re%p;
		if(wei[t]==1)	re=re*a;
		else if(wei[t]==-1)	re=re*inv;
		if(re>p)	re=re%p;
		t--;
	}
	return re;*/
}
int main(void)
{
	//freopen("2.in","r",stdin);
	int n;
	mpz_class p,q,e,c;
	gmp_scanf("%d%Zd%Zd%Zd", &n, &p, &q, &e);
	//gmp_printf("e=%Zd\np=%Zd\nq=%Zd\nlow=%Zd\n",e,p,q,low);
    mpz_class phi=(p-1)*(q-1);
    mpz_class d=get_inv(e,phi), d1=d%(p-1), d2=d%(q-1);
    mpz_class inv_p=get_inv(p%q,q);
	while(n--)
	{
		gmp_scanf("%Zd", &c);
      	mpz_class p1=power(c%p,d1,p);
      	mpz_class q1=power(c%q,d2,q);
      	mpz_class k=((q1-p1)*inv_p)%q;
      	if(k<0)	k+=q;
      	mpz_class pl=p1+k*p;
	//	gmp_printf("%Zd\n",pl);//*/
	}
	return 0;
}
