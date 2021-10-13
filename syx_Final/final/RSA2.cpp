#include <gmpxx.h>
#include <iostream>
#include <stdio.h>
using namespace std;
//int	prime[9]={2,3,5,7,11,13,17,19,61};	//long long ·¶Î§ÄÚÎÞ´í
mpz_class gcd(mpz_class a,mpz_class b)
{
    if(b)
    {
        return gcd(b,a%b);
    }
    return a;
}
mpz_class quick_power(mpz_class a, mpz_class x, mpz_class p)
{	//a^x (mod p)
	mpz_class re=1;
	while(x)
	{
		if((x&1)!=0)	re=(re*a)%p;
		a=(a*a)%p;
		x>>=1;
	}
	return re;
}
bool Miller_Rabin(mpz_class x)
{
	if(x==1)	return false;
	//printf("i'm in mr\n");
	mpz_class x_1=x-1,m=x_1;
	mpz_t ra;
	int t=0;
	while((m&1)==0)	t++,m>>=1;	
	for(int i=0; i<4; i++)	
	{
		//printf("i'm in for\n");
		clock_t time = clock();

    gmp_randstate_t grt;
    gmp_randinit_default(grt);
    gmp_randseed_ui(grt, time);


    mpz_t key;
    mpz_init(key);
    
    mpz_urandomb(key, grt, 512);
		//gmp_printf("x-1=%Zd\n",x_1);
		//printf("ra get\n");
		mpz_class rand(key);
		rand%=x_1;	
		rand++;
		//gmp_printf("\t%Zd\n",rand);

		if(gcd(x,rand)!=1)	return false;
		mpz_class a=quick_power(rand,m,x);
		if(a==1)	continue;
		mpz_class nxt=a;
		for(int j=1; j<=t; j++)
		{
			nxt=nxt*nxt%x;
			if(nxt==1 && a!=x_1)	return false;
			a=nxt;
          	if(a==1)	break;
		}
		if(a!=1)	return false;
	}
	return true;
}
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
	if(d!=1)	return g_inv=0;
	if(g_inv<0)	g_inv+=p;
	return g_inv;
}
int main()
{
	//freopen("2.in","r",stdin);
//	freopen("my.out","w", stdout);
	int n;
	scanf("%d",&n);
	mpz_class p,q,e,low=65537;
	while(n--)
	{
        	/*mpz_init(e);
        	mpz_init(p);
        	mpz_init(q);//*/
        	gmp_scanf("%Zd%Zd%Zd",&e,&p,&q);
		//gmp_printf("e=%Zd\np=%Zd\nq=%Zd\nlow=%Zd\n",e,p,q,low);
        	if(e<low)
		{
			printf("ERROR\n");
			continue;
		}
		if(p==q)
		{
			printf("ERROR\n");
			continue;
		}		
		if(abs(p-q)<low || !Miller_Rabin(p) || !Miller_Rabin(q))
		{
			printf("ERROR\n");
			continue;
		}//*/
      	mpz_class d=gcd(p-1,q-1),d_2=d*d;
      	if(d_2>=p || d_2>=q)
      	{
        	printf("ERROR\n");
			continue;
      	}
		mpz_class inv=get_inv(e,(p-1)*(q-1));
		if(inv==0)
		{
			printf("ERROR\n");
			continue;
		}
      	mpz_class n=p*q,cur=inv*inv;
      	if(81*cur*cur<n)
        {
			printf("ERROR\n");
			continue;
		}
		gmp_printf("%Zd\n",inv);//*/
	}
	return 0;
}

