#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <string>
#include <cstring>
#include <map>
#include <assert.h>
#define outt(x) printf("%s = %u  ",#x,x);
#define hh puts("")

#ifndef ONLINE_JUDGE
#include <windows.h>
#define _WINDOWS_H_
#endif
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned char uchar; 
const int  maxn = 1000;
uint SHA1_tmp;
#define SHA1_ROTL(a,b) (SHA1_tmp=(a),((SHA1_tmp>>(32-b))&(0x7fffffff>>(31-b)))|(SHA1_tmp<<b))
#define SHA1_F(B,C,D,t) ((t<40)?((t<20)?((B&C)|((~B)&D)):(B^C^D)):((t<60)?((B&C)|(B&D)|(C&D)):(B^C^D)))

int UnitSHA1(const char* str, int length, unsigned sha1[5]){
/*
计算字符串SHA-1
参数说明：
str         字符串指针
length      字符串长度
sha1         用于保存SHA-1的字符串指针
返回值为参数sha1
*/
	uchar *pp, *ppend;
	uint l, i, K[80], W[80], TEMP, A, B, C, D, E, H0, H1, H2, H3, H4;
	H0 = 0x67452301, H1 = 0xEFCDAB89, H2 = 0x98BADCFE, H3 = 0x10325476, H4 = 0xC3D2E1F0;
	for (i = 0; i < 20; K[i++] = 0x5A827999);
	for (i = 20; i < 40; K[i++] = 0x6ED9EBA1);
	for (i = 40; i < 60; K[i++] = 0x8F1BBCDC);
	for (i = 60; i < 80; K[i++] = 0xCA62C1D6);
	l = length + ((length % 64 > 56) ? (128 - length % 64) : (64 - length % 64));
	if (!(pp = (unsigned char*)malloc((unsigned int)l))) return -1;
	for (i = 0; i < length; pp[i + 3 - 2 * (i % 4)] = str[i], i++);
	for (pp[i + 3 - 2 * (i % 4)] = 128,i++; i < l; pp[i + 3 - 2 * (i % 4)] = 0,i++);
	*((unsigned int*)(pp + l - 4)) = length << 3;
	*((unsigned int*)(pp + l - 8)) = length >> 29;
	for (ppend = pp + l; pp < ppend; pp += 64){
		for (i = 0; i < 16; W[i] = ((unsigned int*)pp)[i], i++);
		for (i = 16; i < 80; W[i] = SHA1_ROTL((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]), 1), i++);
		A = H0, B = H1, C = H2, D = H3, E = H4;
		for (i = 0; i < 80; i++){
			TEMP = SHA1_ROTL(A, 5) + SHA1_F(B, C, D, i) + E + W[i] + K[i];
			E = D, D = C, C = SHA1_ROTL(B, 30), B = A, A = TEMP;
		}
		H0 += A, H1 += B, H2 += C, H3 += D, H4 += E;
	}
	free(pp - l);
	sha1[0]=H0, sha1[1]=H1, sha1[2]=H2, sha1[3]=H3, sha1[4]=H4;
	return 0;
}



void getstr(unsigned n,char str[8])   //str为返回值字符串 
{
	str[0]='a';str[1]='0';str[2]='0';str[3]='0';str[4]='0';str[5]='0';str[6]='0';str[7]='0';
	int i=2;
	unsigned tmp; 
	while(n)
	{
		tmp=n%36;
		if(tmp<10) str[i++]=tmp+'0';
		else str[i++]=tmp-10+'a';
		n=n/36;
	}
//	str[i]=0;
}

void R(unsigned sha1[5],char str[8],int i)   //sha1为现在的SHA1函数值 
{
	getstr((sha1[0]+sha1[1]*i)%2176782336,str);
}

int n,lenth; 
string nowstr1,nowstr2;
map<string,string>RainbowTable;
unsigned nowsha1[5],Orignsha1[5],inchain[5];
char ChainHead[maxn],ChainTail[maxn],aimsha1[maxn];

bool Test(string Head,string Tail)
{
	int len=Head.length();
	char ch[255]={};
	string qwer;
	for( int i=0;i<len;i++) ch[i]=Head[i];
	ch[len] = '\0';
	for(int turnn=1;turnn<=100;++turnn)
	{
		for(int i=1;i<=100;++i)
		{
			UnitSHA1(ch,strlen(ch),nowsha1);
		/*	if(turnn==1&&i==1)
			{
				for(int j=0;j<4;++j) printf("%x ",nowsha1[j]);hh;
			}
			//*/
			R(nowsha1,ch,i);
		/*	qwer=ch;
			if(qwer=="a03ixlbp") {
			puts("Yes");
			printf("%s\n",Head.c_str());
			outt(turnn);outt(i);hh;
			}
			//*/
	//		if(turnn==1) printf("%s\n",ch);
		}
	//	printf("In turn %d  ",turnn);printf("%s\n",ch);
	}
	Head=ch;
	if(Head==Tail) return 1;
	return 0;
}

bool WorkWithHead(string HeadofChain,string TailofChain)
{
	int len=HeadofChain.length();
	char ch[255]={};
	for( int i=0;i<len;i++) ch[i]=HeadofChain[i];
	ch[len] = '\0';
	for(int turnn=1;turnn<=100;++turnn)
	{
		for(int i=1;i<=100;++i)
		{
			UnitSHA1(ch,strlen(ch),nowsha1);
			for(int j=0;j<=5;++j)
			{
				if(j==5)
				{
					printf("%s",ch);
					return 1;
				}
				if(Orignsha1[j]!=nowsha1[j]) break;
			}
			R(nowsha1,ch,i);
	//		if(turnn==1) printf("%s\n",ch);
		}
	//	printf("In turn %d  ",turnn);printf("%s\n",ch);
	}
//	HeadofChain=ch;
//	if(HeadofChain==TailofChain) puts("1");
	return 0;
}

int main()
{
	#ifndef ONLINE_JUDGE
	freopen(".\\TestCase\\T9\\1.in","r",stdin);
	freopen(".\\TestCase\\T9\\T9.txt","w",stdout);
	#endif
	scanf("%d",&n);
	for(int i=1;i<=n;++i) 
	{
		scanf("%s%s",ChainHead,ChainTail);
		nowstr1=ChainHead;nowstr2=ChainTail;
		RainbowTable[nowstr2]=nowstr1;
	//	if(i==1){ outt(Test(nowstr1,nowstr2));hh;}
	}
	
	scanf("%s",aimsha1);
	for(int i=0;i<5;++i)
	{
		Orignsha1[i]=0;
		for(uint j=7ll,val=1ll;j<=7;--j,val<<=4)
		{
			Orignsha1[i]+=val*(isdigit(aimsha1[i*8+j])?(aimsha1[i*8+j]-'0'):(aimsha1[i*8+j]-'a'+10ll));
		}
//		outt(Orignsha1[i]);printf("%x",Orignsha1[i]);hh;
		//printf("%d\n",Orignsha1[i]);
	}
	
	
//	for(int i=0;i<5;++i) printf("%x",Orignsha1[i]);
//	hh;hh;hh;hh;puts("qwwwwwq");hh;hh;hh;hh;
	//*/
	//aimsha1   char[]   inchain int[5]
//	for(int j=0;j<4;++j) printf("%x ",Orignsha1[j]);hh;
	for(int j=0;j<=1000;++j) aimsha1[j]=0;
	for(int nowpls=1;nowpls<=100;++nowpls)         //
	{
		for(int i=0;i<5;++i) inchain[i]=Orignsha1[i];
		aimsha1[0]=0;
		for(int i=nowpls;i<100;++i)
		{
		//	for(int jk=0;jk<=1000;++jk) aimsha1[jk]=0;
			R(inchain,aimsha1,i);
			printf("%d %s\n",i,aimsha1);//hh;
			UnitSHA1(aimsha1,strlen(aimsha1),inchain);
		}
		R(inchain,aimsha1,100);
		printf("100 %s\n",aimsha1);
		for(int turnn=1;turnn<=100;++turnn)
		{
		//	printf("In turn %d :  ",turnn);printf("%s\n",aimsha1);
			nowstr1=aimsha1;
			if(RainbowTable.find(nowstr1)!=RainbowTable.end())
			{
			//	printf("%d %d\n",nowpls,turnn);
			//	outt(turnn);outt(nowpls);printf("%s",RainbowTable[nowstr1].c_str());puts("!");
				if(WorkWithHead(RainbowTable[nowstr1],nowstr1)) goto endofprogram;
			}
			for(int i=1;i<=100;++i)
			{
				UnitSHA1(aimsha1,strlen(aimsha1),inchain);
			//	for(int jk=0;jk<=1000;++jk) aimsha1[jk]=0;
				R(inchain,aimsha1,i);
				printf("%d %d %d %s\n",nowpls,turnn, i,aimsha1);
			}
		}
	}
	puts("None");
	
	endofprogram:
	return 0;
}

