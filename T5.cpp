#include <gmpxx.h>
#include <stdio.h>
#include <cmath>
#include <ctime>

#ifndef ONLINE_JUDGE
#include <windows.h>
#define _WINDOWS_H_
#endif

using namespace std;
typedef mpz_class BigInt;
typedef long long ll;

BigInt prime[25];	//大概是不会出错的 

BigInt My_Pow(BigInt Basee,BigInt Indexx,BigInt NowMod)     //ksm  暂时还没优化 
{
	BigInt Ans=1;
	while(Indexx!=0)
	{
		if((Indexx&1)==1) Ans=(Ans*Basee)%NowMod;      
		Basee=(Basee*Basee)%NowMod;Indexx>>=1;
	} 
	return Ans;
}
BigInt Exgcd(BigInt val1,BigInt val2,BigInt* Coefficient1,BigInt* Coefficient2)          
{
	if(!val2){*Coefficient1=1;*Coefficient2=0;return val1;}
	BigInt lson,rson,Ans;
	Ans=Exgcd(val2,val1%val2,&lson,&rson);
	*Coefficient1=rson;*Coefficient2=lson-(val1/val2)*rson;
	return Ans;
}


bool JudgePrime(BigInt nowwa)   //实际上是Miller-Rabin 
{
	if(nowwa==1) return false;
	if(nowwa==2) return true;
//	for(int i=0;i<13;++i) if((nowwa%prime[i])==0) return false;
//	for(int i=13;i<20;++i) prime[i]=rand();
	BigInt qwer=nowwa-1,ans;
	int log0=0,flagg=0,j;
	while((qwer&1)==0) log0++,qwer>>=1;
	for(int i=0;i<7;++i)            //再多就要超时了，但实际上只用7个确定值的话，还是很有可能会出错的，因为Miller-Rabin是一个Yes-biased的Monte Carlo算法 
	{
		flagg=0;
	//	gmp_printf("%Zd\n",prime[i]);
		if(nowwa==prime[i]) return true;
		if(nowwa%prime[i]==0) return false;
		ans=My_Pow(prime[i],qwer,nowwa);
		if(ans%nowwa==1) continue;
		for(j=0;j<log0;++j)
		{
			if((ans+10*nowwa)%nowwa==nowwa-1) {flagg=1;break;}
			ans=(ans*ans)%nowwa;
		}
		if(flagg==1) continue;
		if(ans%nowwa!=1) return false;
		return false;
	}
	return true;
}

BigInt Judge(BigInt e,BigInt p,BigInt q)
{
//	gmp_printf("%Zd\n%Zd\n%Zd\n(JudgePrime(p))=>%d\n(JudgePrime(q))=>%d\n\n\n",e,p,q,JudgePrime(p),JudgePrime(q));
	if(JudgePrime(p)==0||JudgePrime(q)==0||e<65537||p==q||abs(p-q)<65537)  
	{
		return false;
	}
	BigInt coe1,coe2,anss,BeiLv;
	BeiLv=1;
	anss=Exgcd((p-1),(q-1),&coe1,&coe2);
	if(anss*anss*anss*anss>min(p,q)) return false;//p-1和q-1都没有过小的因子,老师给的比较方法（实际上我完全没理解） 
//	gmp_printf("%Zd\n%Zd\n%Zd\n",max(p,q),min(p,q),(p/q));
	if(max(p,q)>min(p,q)*min(p,q)) return false;
	anss=Exgcd(e,(p-1)*(q-1),&coe1,&coe2);
	if(anss!=1) return false;
	while(coe1<0) coe1+=(p-1)*(q-1);
	if(coe1==0) return false;       //似乎不会出现？ 
	if(81*coe1*coe1*coe1*coe1<p*q) return false;        //可能会受Wiener的低解密指数攻击 
	gmp_printf("%Zd\n",coe1);
	return true;
}

int n;
BigInt p,q,e,anss; 
int main()
{
	#ifndef ONLINE_JUDGE
	freopen(".\\TestCase\\T5\\3.in","r",stdin);
	FILE* fout=fopen(".\\TestCase\\T5\\OutInfor.txt","w");
//	freopen(".\\TestCase\\T5\\T5myans.txt","w",stdout);
	#endif
	scanf("%d",&n);srand(time(0));
	prime[0]=2;prime[1]=3;prime[2]=5;prime[3]=7;prime[4]=11;prime[5]=13;prime[6]=17;
	prime[7]=19;prime[8]=21;prime[9]=29;prime[10]=31;prime[11]=37;prime[12]=61;
//	for(int i=1;i<=1000;++i)  printf("%d:%d\n",i,JudgePrime(i));
	
	while(n--)
	{
		 gmp_scanf("%Zd%Zd%Zd",&e,&p,&q);
		 if(!Judge(e,p,q)) puts("ERROR");
	}
	
	return 0;
}
