#include <gmpxx.h>
#include <stdio.h>
#include <cmath>
#include <assert.h>

#ifndef ONLINE_JUDGE
#include <windows.h>
#define _WINDOWS_H_
#endif
#pragma GCC optimize("O3")
#define ksmbit  (32) 
//应该是一个2的幂次 ，实际上是ksmbit*2个运算一次 
using namespace std;
typedef mpz_class BigInt;
typedef long long ll;

BigInt Exgcd(BigInt val1,BigInt val2,BigInt* Coefficient1,BigInt* Coefficient2)          
{
	if(!val2){*Coefficient1=1;*Coefficient2=0;return val1;}
	BigInt lson,rson,Ans;
	Ans=Exgcd(val2,val1%val2,&lson,&rson);
	*Coefficient1=rson;*Coefficient2=lson-(val1/val2)*rson;
//	lson=val1*(*Coefficient1)+val2*(*Coefficient2);gmp_printf("Nowval:  %Zd\n",lson);
	return Ans;
}

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

int IndexBit[100000];
BigInt val256[ksmbit<<2],qwwq;

BigInt My_Fastest_Pow(BigInt Basee,BigInt Indexx,BigInt NowMod)       //滑动窗口优化快速幂 
{
	int qwer,cnt;
	cnt=0;
	for(cnt=0;Indexx!=0;++cnt)
	{
		qwwq=(Indexx&((ksmbit<<1)-1));qwer=1;
		while(qwwq>0)
		{
			if((qwwq&qwer)>0) IndexBit[cnt]|=qwer,qwwq-=qwer;
			qwer<<=1;
		}
		Indexx>>=6;
	}
	IndexBit[cnt]=0;cnt--;val256[0]=1;
	for(int i=1;i<=(ksmbit<<1)+3;++i) val256[i]=(val256[i-1]*Basee)%NowMod;
	BigInt Ans=1;
//	printf("Cnt:  %d\n",cnt);
	while(cnt>=0)
	{
		Ans=(Ans*Ans)%NowMod;Ans=(Ans*Ans)%NowMod;Ans=(Ans*Ans)%NowMod;
		Ans=(Ans*Ans)%NowMod;Ans=(Ans*Ans)%NowMod;Ans=(Ans*Ans)%NowMod;
		Ans=(Ans*val256[IndexBit[cnt]])%NowMod;
		cnt--;
	}
	return Ans;
}

BigInt My_Fast_Pow(BigInt Basee,BigInt Indexx,BigInt NowMod)       //滑动窗口优化快速幂 
{
	int cnt;
	for(cnt=0;Indexx!=0;++cnt)
	{
		if((Indexx&1)>0) {
			IndexBit[cnt]=1;
		}else IndexBit[cnt]=0;
		Indexx>>=1;
	}
	IndexBit[cnt]=0;cnt--;val256[0]=1;
	for(int i=1;i<=(ksmbit<<1)+1;++i) val256[i]=(val256[i-1]*Basee)%NowMod;
	BigInt Ans=1,std_ans;
//	printf("Cnt:  %d\n",cnt);
	for(int i=cnt,j,qwer=0;i>=0;qwer=0)   //注意这里没有i-- 
	{
	//	printf("%d",i);
	//	if(IndexBit[i]==0) {Ans=(Ans*Ans)%NowMod;i--;continue;}
		qwer+=IndexBit[i];
		for(j=i-1;j>=0&&qwer<ksmbit;j--)
		{
			qwer<<=1;
			qwer+=IndexBit[j];
		}
	//	printf("qwer:  %d    i:    %d       j:   %d\n",qwer,i,j);
		while(j<i) 
		{
			Ans=(Ans*Ans)%NowMod,i--;
		}
		//if(IndexBit[i]==1) Ans=(Ans*Basee)%NowMod;
		Ans=(Ans*val256[qwer])%NowMod;
	//	gmp_printf("hh:%d  %Zd",qwer,Ans);
	}
	return Ans;
}

BigInt My_Pow_Prime(BigInt Basee,BigInt Indexx,BigInt NowMod)     //要求模数必须是质数，否则会出错 
{
	BigInt Ans=1,Inv,coe2;
	if(Basee%NowMod==0) return 0;
	BigInt gcd=Exgcd(Basee,NowMod,&Inv,&coe2);  //默认互质了 
	assert(gcd==1);
	while(Inv<0) Inv=Inv+10000*NowMod;
	Inv%=NowMod;//gmp_printf("Inv:   %Zd\n",Inv);
	coe2=Basee*Inv%NowMod;Ans=1;
	bool ifborr=0,nowdig,nxtdig;
	nxtdig=(((Indexx&1)==1)?1:0);
	while(Indexx!=0)
	{
		nowdig=nxtdig;nxtdig=(((Indexx&2)==2)?1:0);
		if(ifborr==0){
			if(nowdig&&nxtdig)
			{
				Ans=(Ans*Inv)%NowMod;
				ifborr=1;
			} else if(nowdig&&(!nxtdig)){
				Ans=(Ans*Basee)%NowMod;
			}
		} else{
			if(nowdig==0) {
				Ans=(Ans*Basee)%NowMod;
				ifborr=0;				
			}
		}
		//if((Indexx&1)==1) Ans=(Ans*Basee)%NowMod;      
		Basee=(Basee*Basee)%NowMod;Inv=(Inv*Inv)%NowMod;Indexx>>=1;
	} 
	if(ifborr==1) Ans=(Ans*Basee)%NowMod;
	return Ans;
}


BigInt CRT(BigInt ModResi[],BigInt ModNum[],int num)
{
	BigInt M,coe1,coe2,ans,Mi,Inv_Mi;
	M=1;ans=0;
    /*for(register int i=1;i<=num;++i)  M=M*ModNum[i];
    for(register int i=1;i<=num;++i)
    {
        Mi=M/ModNum[i];
        BigInt gcd=Exgcd(Mi,ModNum[i],&coe1,&coe2);  //默认互质了 ,否则下面ModResi[i]应该除以gcd 
        assert(gcd==1);
        while(coe1<0) coe1=coe1+10000*M;
        coe1%=M; 
        ans=(ans+ModResi[i]*coe1*Mi)%M; 
    }
    while(ans<0) ans=ans+10000*M;
   // gmp_printf("ans:  %Zd\n",ans);
   	ans=(ans+M)%M;
    return ans;
    //这一部分是普适的CRT，下面是针对只有两个质数模数时的 
    //*/ 
    M=ModNum[1]*ModNum[2];
    Mi=M/ModNum[1];
    BigInt gcd=Exgcd(Mi,ModNum[1],&coe1,&coe2);  //默认互质了 ,否则下面ModResi[i]应该除以gcd 
    assert(gcd==1);
    while(coe1<0) coe1=coe1+10000*M;
    coe1%=M; 
    ans=(ans+ModResi[1]*coe1*Mi)%M;  
	Mi=M/ModNum[2];
    gcd=Exgcd(Mi,ModNum[2],&coe1,&coe2);  //默认互质了 ,否则下面ModResi[i]应该除以gcd 
    assert(gcd==1);
    while(coe1<0) coe1=coe1+10000*M;
    coe1%=M; 
    ans=(ans+ModResi[2]*coe1*Mi)%M; 
    while(ans<0) ans=ans+10000*M;
   // gmp_printf("ans:  %Zd\n",ans);
   	ans=(ans+M)%M;
    return ans;
}

int n;
BigInt p,q,e,anss,m,d,pq,phi,coe1,coe2,qwer; 
BigInt modresi[1000],modnum[1000];
int main()
{
	#ifndef ONLINE_JUDGE
	freopen(".\\TestCase\\T7\\1.in","r",stdin);
//	FILE* fout=fopen(".\\TestCase\\T7\\OutInfor.txt","w");
//	freopen(".\\TestCase\\T7\\T7myans.txt","w",stdout);
	#endif
	scanf("%d",&n);
	gmp_scanf("%Zd%Zd%Zd",&p,&q,&e);
	pq=p*q;phi=(p-1)*(q-1);
	m=Exgcd(e,phi,&coe1,&coe2);
	while(coe1<0) coe1+=1000*phi;
	d=coe1%phi;
	while(n--)
	{	
		gmp_scanf("%Zd",&m);//anss=My_Pow(m,d,pq);
		qwer=d%(p-1);modresi[1]=My_Fast_Pow(m,qwer,p);modnum[1]=p;     // qwer=qwer%modnum[1];gmp_printf("std_ans:   %Zd\nmodrest[1]:   %Zd\n",qwer,modresi[1]);
		//puts("2");
		qwer=d%(q-1);modresi[2]=My_Fast_Pow(m,qwer,q);modnum[2]=q;   //   qwer=anss%modnum[2];assert(qwer==modresi[2]);gmp_printf("anss%%p:   %Zd\nmodrest[2]:   %Zd\n",qwer,modresi[2]);
		//puts("3");
		anss=CRT(modresi,modnum,2);
		gmp_printf("%Zd\n",anss);
		 
	}
	
	return 0;
}
