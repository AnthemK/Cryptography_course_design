int	prime[9]={2,3,5,7,11,13,17,19,61};	//long long ·¶Î§ÄÚÎÞ´í
mpz_t quick_power(mpz_t a, mpz_t x, mpz_t p)
{	//a^x (mod p)
	mpz_t re=1;
	while(x)
	{
		if(x&1)	re=(re*a)%p;
		a=(a*a)%p;
		x>>=1;
	}
	return re;
}
mpz_t exgcd(mpz_t a,mpz_t b,mpz_t *s,mpz_t *t)
{
    if(b)
    {
        mpz_t s1,t1;
        mpz_t c=exgcd(b,a%b,&s1,&t1);
        *s=t1,*t=s1-a/b*t1;
        return c;
    }
    *s=1,*t=0;
    return a;
}
mpz_t get_inv(mpz_t g,mpz_t p)
{
	mpz_t g_inv,t,d;
	d=exgcd(g,p,&g_inv,&t);
	if(d!=1)	return g_inv=-1;
	if(g_inv<0)	g_inv+=p;
	return g_inv;
}
bool Miller_Rabin(mpz_t x)
{
	if(x==1)	return false;
	for(int i=0; i<9; i++)	if(x==prime[i])	return true;
	for(int i=0; i<9; i++)	if(x%prime[i]==0)	return false;
	mpz_t m=x-1;
	int t=0;
	while(!(m&1))	t++,m>>=1;
	for(int i=0; i<9; i++)
	{
		mpz_t a=quick_power(prime[i],m,x);
		if(a==1)	continue;
		mpz_t nxt=a;
		for(int j=1; j<=t; j++)
		{
			nxt=nxt*nxt%x;
			if(nxt==1 && a!=x-1 && a!=1)	return false;
			a=nxt;
		}
		if(a!=1)	return false;
	}
	return true;
}
gmp_printf("%Zd",);
