#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <utility>
#include<algorithm>
#define textInforlenth 4        //表示一共有4*4个比特的信息 
#define Nr 4
#define maxn 100000
#define maxm ((1<<16)+3)
#define maxdig (1<<(4*textInforlenth))
#define maxbyte 18
#define outt(x) printf("%s = %d    ",#x,x)
#define hh puts("")
#define numoflinear1 15 
#define numoflinear2 10 
#define numoftest 3

#ifndef ONLINE_JUDGE
#include <windows.h>
#define _WINDOWS_H_
#endif
using namespace std;
typedef unsigned short us;

const int SeqofTest[]={1654,9484,15156,48511,34516,451,2,16715,65532,13245,74};

const int SPN_Pi_S[maxbyte]={14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7};
const int Rev_SPN_Pi_S[maxbyte]={14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5};
int MAP_SPN_PI_S[maxm],Rev_MAP_SPN_PI_S[maxm];
const int SPN_Pi_P[maxbyte]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};
const int Rev_SPN_Pi_P[maxbyte]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};
int MAP_SPN_PI_P[maxm],Rev_MAP_SPN_PI_P[maxm];
int MAP_SPN_SP[maxm],Rev_MAP_SPN_SP[maxm];

char NumtoChar[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
int ChartoNum[500];

inline int read(){
    int s=0,f=1; char c=getchar();
    while(c<'0'||c>'9') f*=c=='-'?-1:1,c=getchar();
    while(c>='0'&&c<='9') s=s*10+c-'0',c=getchar();
    return s*f;
} 
bool Bool_Buffer[maxm];
int nowwa,Cnt;
char cc;
int n;

   //使用库文件SPNCryptoSystem.h的注释起点 

 class SPNCryptoSystem
{
	private:
		
	public:
		int Infor[maxbyte],Key[maxbyte],nowval;           //文本，密钥，文本压位版（以这个为准） 
		SPNCryptoSystem(); 
		inline void GetinInfor();
		void GetinKey();
		void PrintInfor();
		void PrintKey();
		void Print();
		int MakeOne();
		void DivideOne(int OneNum);
		void Init();
		inline void Substitute(int typ);
		inline void XorKey(int val);
		inline void Permutation(int typ);
		void Encode();
		void Decode(); 
};
//*/ 

	SPNCryptoSystem::SPNCryptoSystem()
	{
		nowval=0;
		for(int i=1;i<=textInforlenth+Nr;++i) Infor[i]=Key[i]=0;
	}
	inline void SPNCryptoSystem::GetinInfor()
	{
		cc=getchar();//cc=getchar();
		//while((!isdigit(cc))&&(cc>'f'||cc<'a')) cc=getchar();
		Infor[1]=ChartoNum[cc];cc=getchar();
		Infor[2]=ChartoNum[cc];cc=getchar();
		Infor[3]=ChartoNum[cc];cc=getchar();
		Infor[4]=ChartoNum[cc];cc=getchar();
		//四个读入  循环展开 
		nowval=MakeOne();
		return;
	}
	void SPNCryptoSystem::GetinKey()
	{
		cc=getchar();
		while((!isdigit(cc))&&(cc>'f'||cc<'a')) cc=getchar();
		for(register int i=1;i<=textInforlenth+Nr;++i)
		{
			if(isdigit(cc)) Key[i]=cc-'0';
			else Key[i]=cc-'a'+10;
			cc=getchar();
		}
		return;
	}	
	void SPNCryptoSystem::PrintInfor()
	{
		for(int i=1;i<=textInforlenth;++i) putchar(NumtoChar[Infor[i]]);
		return;
	}
	void SPNCryptoSystem::PrintKey()
	{
		//for(int i=1;i<=textInforlenth+Nr;++i) putchar(NumtoChar[Key[i]]);
		putchar(NumtoChar[Key[1]]);putchar(NumtoChar[Key[2]]);putchar(NumtoChar[Key[3]]);putchar(NumtoChar[Key[4]]);
		putchar(NumtoChar[Key[5]]);putchar(NumtoChar[Key[6]]);putchar(NumtoChar[Key[7]]);putchar(NumtoChar[Key[8]]);
		return;
	}
	void SPNCryptoSystem::Print()
	{
		printf("*******************************\nInfor: ");
		for(int i=1;i<=textInforlenth;++i) putchar(NumtoChar[Infor[i]]);
		printf("\nKey: ");
		for(int i=1;i<=textInforlenth+Nr;++i) putchar(NumtoChar[Key[i]]);
		puts("");
		return;
	}
	int SPNCryptoSystem::MakeOne() {return (Infor[1]<<12)|(Infor[2]<<8)|(Infor[3]<<4)|(Infor[4]);} //默认为4*4bit 
	void SPNCryptoSystem::DivideOne(int OneNum)
	{
		for(register int i=textInforlenth;i>=1;i--) Infor[i]=OneNum&0xf,OneNum>>=4;return;    //默认为4*4bit 
	}
	void SPNCryptoSystem::Init()
	{
		for(int i=0;i<(1<<4);++i) ChartoNum[(int )NumtoChar[i]]=i;
		
		for(register int val=0;val<maxdig;++val)
		{
			DivideOne(val);
			for(register int i=1;i<=textInforlenth;++i) Infor[i]=SPN_Pi_S[Infor[i]];
			MAP_SPN_PI_S[val]=MakeOne();
			Rev_MAP_SPN_PI_S[MAP_SPN_PI_S[val]]=val;
			
			DivideOne(val);Cnt=0;
			for(register int i=1;i<=textInforlenth;++i)
			{
				for(register int j=(1<<(textInforlenth-1));j;j>>=1)
				{
					Bool_Buffer[SPN_Pi_P[Cnt]]=(Infor[i]&j)?1:0;
					Cnt++;
				}
			}
			for(register int i=1;i<=textInforlenth;++i)
				Cnt=((i-1)<<2),Infor[i]=(Bool_Buffer[Cnt]<<3)|(Bool_Buffer[Cnt+1]<<2)|(Bool_Buffer[Cnt+2]<<1)|(Bool_Buffer[Cnt+3]);  //默认为4*4bit 
			MAP_SPN_PI_P[val]=MakeOne();
			Rev_MAP_SPN_PI_P[MAP_SPN_PI_P[val]]=val;
		}		
		for(register int val=0;val<maxdig;++val)
		{
			MAP_SPN_SP[val]=MAP_SPN_PI_P[MAP_SPN_PI_S[val]];
			Rev_MAP_SPN_SP[MAP_SPN_SP[val]]=val;
		}
	} 
	
	void  SPNCryptoSystem::Substitute(int typ)
	{
		if(typ==0)
			nowval=MAP_SPN_PI_S[nowval];
		if(typ==1)
			nowval=Rev_MAP_SPN_PI_S[nowval];
		return;
	}
	void SPNCryptoSystem::XorKey(int val)
	{
		nowval^=val;
		return;
	} 
	void SPNCryptoSystem::Permutation(int typ)
	{
		if(typ==0) {nowval=MAP_SPN_PI_P[nowval];return;}
		else if(typ==1) { nowval=Rev_MAP_SPN_PI_P[nowval];return;}
		return;
	}
	void SPNCryptoSystem::Encode()
	{
		int Lastval=(Key[1]<<12)|(Key[2]<<8)|(Key[3]<<4)|(Key[4]);   //moren
		for(int r=1;r<=Nr-1;++r)
		{
			XorKey(Lastval);
		//	nowval^=Lastval;
			Lastval<<=4;Lastval&=0xffff;Lastval|=Key[r+textInforlenth];        //moren
	//		Substitute(0);
		//	nowval=MAP_SPN_PI_S[nowval];
	//		Permutation(0);
		//	nowval=MAP_SPN_PI_P[nowval];
			nowval=MAP_SPN_SP[nowval];
		
		}
		XorKey(Lastval);
	//	nowval^=Lastval;
		Lastval<<=4;Lastval&=0xffff;Lastval|=Key[Nr+textInforlenth];  //moren
		Substitute(0);
	//	nowval=MAP_SPN_PI_S[nowval];
		XorKey(Lastval);
    //	nowval^=Lastval;
		DivideOne(nowval);
		return;
	}
	void SPNCryptoSystem::Decode()
	{
		int Lastval=(Key[Nr+1]<<12)|(Key[Nr+2]<<8)|(Key[Nr+3]<<4)|(Key[Nr+4]);  //moren
		XorKey(Lastval);
	//	nowval^=Lastval;
		Lastval>>=4;Lastval|=(Key[Nr]<<((textInforlenth-1)*4));  //moren
		Substitute(1);
	//	nowval=Rev_MAP_SPN_PI_S[nowval];
		XorKey(Lastval);
    //	nowval^=Lastval;
		Lastval>>=4;Lastval|=(Key[Nr-1]<<((textInforlenth-1)*4));  //moren
		for(int r=Nr-1;r;--r)
		{
	//		Permutation(1);
		//	nowval=Rev_MAP_SPN_PI_P[nowval];
	//		Substitute(1);
		//	nowval=Rev_MAP_SPN_PI_S[nowval];
			nowval=Rev_MAP_SPN_SP[nowval];
			XorKey(Lastval);
		//	nowval^=Lastval;
			Lastval>>=4;Lastval|=(Key[r-1]<<((textInforlenth-1)*4));  //moren
		}
		DivideOne(nowval);
		return;
	}
//*/                           如果要引用库文件SPNCryptoSystem.h的话注释到这里 



SPNCryptoSystem SPN;
int XorX[maxbyte],XorU4[maxbyte],ZeroNum[maxbyte][maxbyte][maxbyte][maxbyte],cnt,nowdiviation,qwer;
pair<int ,int>PossibleKey1[maxm],PossibleKey2[maxm];
us Ciphertextpair[maxm]; 
us numbyte1,numbyte2;


void DifferentialAttack1(int pls,int tim)   //此处有很多部分都和T2中相同，因此主要介绍不同的部分 
{
	PossibleKey1[0].first=0;
	us Othe;
	for(register us casee=0;casee<tim;++casee)   //注意这个外层枚举明密文对，因为可以预处理，因此能够显著的提速 
	{	
		Othe=casee^XorX[pls];   //找到异或恰好为我需要值的两个明密文对 
		if(((Ciphertextpair[casee]^Ciphertextpair[Othe])&0xf0f0)) continue;   //这一组没有意义，他们在这两个半字节上是相同的，因此枚举密钥在进行倒着的S盒之后也仍然相同 
		for(register us l2=0;l2<16;++l2)
		{
				for(register us l4=0;l4<16;++l4)
				{
					ZeroNum[0][l2][0][l4]+=((((Rev_SPN_Pi_S[((Ciphertextpair[casee]>>8)^l2)&0xf]^Rev_SPN_Pi_S[((Ciphertextpair[Othe]>>8)^l2)&0xf])<<8)|(Rev_SPN_Pi_S[((Ciphertextpair[casee])^l4)&0xf]^Rev_SPN_Pi_S[((Ciphertextpair[Othe])^l4)&0xf]))^XorU4[pls])?0:1;
					//一串超极长的式子，实际上就是进行部分解密之后，得到之后的异或的结果，并且把它和目标比较，相同计数器就++ 
				}
		}			
	}
		for(register us l2=0;l2<16;++l2)
		{
				for(register us l4=0;l4<16;++l4)
				{
					nowwa=(l2<<8)|l4;
					if(ZeroNum[0][l2][0][l4]) PossibleKey1[++PossibleKey1[0].first]=make_pair(ZeroNum[0][l2][0][l4],nowwa);  //统计答案中概率不为0的密钥 
					ZeroNum[0][l2][0][l4]=0;
				}

		}		
	sort(PossibleKey1+1,PossibleKey1+1+PossibleKey1[0].first);	//排序，按照概率 
//	for(int i=PossibleKey[pls][0].first;i>=1;--i) outt(PossibleKey[pls][i].first),outt(PossibleKey[pls][i].second),hh;
	return;
}

void DifferentialAttack2(int pls,int tim)   //基本思路和 DifferentialAttack1相似，故不再赘述 
{
	PossibleKey2[0].first=0;
	us Othe;
	for(register int casee=1000;casee<1000+tim;++casee)
	{
		Othe=casee^XorX[pls];
		if((Ciphertextpair[casee]^Ciphertextpair[Othe])&0x0f0f) continue;	
		for(register int l1=0;l1<16;++l1)
		{
				for(register int l3=0;l3<16;++l3)
				{
					ZeroNum[l1][0][l3][0]+=((((Rev_SPN_Pi_S[((Ciphertextpair[casee]>>12)^l1)&0xf]^Rev_SPN_Pi_S[((Ciphertextpair[Othe]>>12)^l1)&0xf])<<12)|((Rev_SPN_Pi_S[((Ciphertextpair[casee]>>4)^l3)&0xf]^Rev_SPN_Pi_S[((Ciphertextpair[Othe]>>4)^l3)&0xf])<<4))^XorU4[pls])?0:1;
				}
		}
	}
		for(register int l1=0;l1<16;++l1)
		{
				for(register int l3=0;l3<16;++l3)
				{
					nowwa=(l1<<12)|(l3<<4);
					if(ZeroNum[l1][0][l3][0]) PossibleKey2[++PossibleKey2[0].first]=make_pair(ZeroNum[l1][0][l3][0],nowwa);
					ZeroNum[l1][0][l3][0]=0;
				}
		}

		//	outt(ZeroNum[0][12][0][1]);
	sort(PossibleKey2+1,PossibleKey2+1+PossibleKey2[0].first);
	return;
}



bool LoopTest(int maxnum)   //仍然是玄学测试 
{
	for(int casee=0;casee<maxnum;++casee)
	{
		
		SPN.nowval=SeqofTest[casee];SPN.DivideOne(SeqofTest[casee]);
		SPN.Encode(); 
		if(SPN.nowval!=Ciphertextpair[SeqofTest[casee]]) return false;
	}		
	return true;
}
time_t Initial,casebegin,afterinput,afterattack,FinalTime,afterenumerate;

us readInfor(){
  char c=getchar();
  us re=0;
  re=(re<<4)|ChartoNum[c];
  c=getchar();
  re=(re<<4)|ChartoNum[c];
  c=getchar();
  re=(re<<4)|ChartoNum[c];
  c=getchar();
  re=(re<<4)|ChartoNum[c];
  c=getchar();
	return re;
}

int main()
{
	#ifndef ONLINE_JUDGE
	Initial=GetTickCount();  //这些都是为了输出中间步骤时间的，在OJ测试的时候则不会编译 
	#endif
	#ifndef ONLINE_JUDGE
	freopen(".\\TestCase\\T3\\7.in","r",stdin);
	freopen(".\\TestCase\\T3\\T3myans.txt","w",stdout);
	#endif
	n=read();SPN.Init();
	XorX[1]=0x0b00;XorU4[1]=0x0606;XorX[2]=0x0020;XorU4[2]=0x5050;
	while(n--)
	{	
		#ifndef ONLINE_JUDGE
		printf("New Case %d\n",n);
		casebegin=GetTickCount();outt(casebegin-Initial);hh;	
		#endif
		for(register int i=0;i<65536;++i)
		{
			SPN.GetinInfor();Ciphertextpair[i]=SPN.nowval;
		}
		#ifndef ONLINE_JUDGE
		afterinput=GetTickCount();outt(afterinput-casebegin);hh;	
		#endif
		
		DifferentialAttack1(1,1500);DifferentialAttack2(2,8000);  //两个差分分析一起进行 
		#ifndef ONLINE_JUDGE
		afterattack=GetTickCount();outt(afterattack-afterinput);hh;
		#endif
		for(int i=PossibleKey1[0].first;i>=1;--i)
		{
			#ifndef ONLINE_JUDGE
			printf("Differential Attack1 %d:0x%x  %d\n",i,PossibleKey1[i].second,PossibleKey1[i].first);
			#endif
		 	SPN.Key[6]=(PossibleKey1[i].second&0xf00)>>8;SPN.Key[8]=(PossibleKey1[i].second&0xf);
		 	for(int j=PossibleKey2[0].first;j>=1;--j)
		 	{
		 		#ifndef ONLINE_JUDGE
				printf("Differential Attack2 %d:0x%x  %d\n",j,PossibleKey2[j].second,PossibleKey2[j].first);
				#endif
		 		SPN.Key[5]=(PossibleKey2[j].second&0xf000)>>12;SPN.Key[7]=(PossibleKey2[j].second&0xf0)>>4;
		 		for(register int key4=0;key4<65536;++key4)
		 		{
		 			qwer=key4;
		 			SPN.Key[4]=qwer&0xf;qwer>>=4;SPN.Key[3]=qwer&0xf;qwer>>=4;
		 			SPN.Key[2]=qwer&0xf;qwer>>=4;SPN.Key[1]=qwer&0xf;qwer>>=4;
		 			//组合密钥，进行测试 
		 			if(LoopTest(numoftest))
		 			{
		 				SPN.PrintKey();
		 				goto endd;
		 			}
		 		}
		 	}
		 }
		 endd:
		 hh;
		#ifndef ONLINE_JUDGE
		afterenumerate=GetTickCount();outt(afterenumerate-afterattack);hh;	
		#endif
		//*/
	}
	#ifndef ONLINE_JUDGE
	FinalTime=GetTickCount();outt(FinalTime-Initial);hh;	
	#endif
	return 0;
}
