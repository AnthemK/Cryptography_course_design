#include <bits/stdc++.h>
//#include "SPNCryptoSystem.h"
#define textInforlenth 4        //表示一共有4*4个比特的信息 
#define Nr 4
#define maxn 100
#define maxm (1<<17)
#define maxdig (1<<(4*textInforlenth))
using namespace std;
typedef long long ll;


const int SPN_Pi_S[maxn]={14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7};
//int Rev_SPN_Pi_S[maxn]={14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5};
int MAP_SPN_PI_S[maxm];
int Rev_MAP_SPN_PI_S[maxm];
const int SPN_Pi_P[maxn]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};
//int Rev_SPN_Pi_P[maxn]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};
int MAP_SPN_PI_P[maxm],Rev_MAP_SPN_PI_P[maxm];
int MAP_SPN_SP[maxm],Rev_MAP_SPN_SP[maxm];

char NumtoChar[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

inline int read(){
    int s=0,f=1; char c=getchar();
    while(c<'0'||c>'9') f*=c=='-'?-1:1,c=getchar();
    while(c>='0'&&c<='9') s=s*10+c-'0',c=getchar();
    return s*f;
} 
bool Bool_Buffer[maxn];
int nowwa,Cnt;
char cc;

 class SPNCryptoSystem
{
	private:
		
	public:
		int Infor[maxn],Key[maxn],nowval;           //文本，密钥，文本压位版（以这个为准） 
		SPNCryptoSystem(); 
		void GetinInfor();
		void GetinKey();
		void PrintInfor();
		void PrintKey();
		void Print();
		int MakeOne();
		void DivideOne(int OneNum);
		void Init();
		void Substitute(int typ);
		void XorKey(int val);
		void Permutation(int typ);
		void Encode();
		void Decode(); 
};
//*/ 

	SPNCryptoSystem::SPNCryptoSystem()
	{
		nowval=0;
		for(int i=1;i<=textInforlenth+Nr;++i) Infor[i]=Key[i]=0;
	}
	void SPNCryptoSystem::GetinInfor()
	{
		cc=getchar();
		while((!isdigit(cc))&&(cc>'f'||cc<'a')) cc=getchar();
		for(register int i=1;i<=textInforlenth;++i)
		{
			if(isdigit(cc)) Infor[i]=cc-'0';
			else Infor[i]=cc-'a'+10;
			cc=getchar();
		}
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
		for(int i=1;i<=textInforlenth+Nr;++i) putchar(NumtoChar[Key[i]]);
		return;
	}
	void SPNCryptoSystem::Print()
	{
		cout<<"*******************************"<<endl<<"Infor: ";
		for(int i=1;i<=textInforlenth;++i) putchar(NumtoChar[Infor[i]]);
		cout<<endl<<"Key: ";
		for(int i=1;i<=textInforlenth+Nr;++i) putchar(NumtoChar[Key[i]]);
		cout<<endl;
		return;
	}
	int SPNCryptoSystem::MakeOne() {return (Infor[1]<<12)|(Infor[2]<<8)|(Infor[3]<<4)|(Infor[4]);} //默认为4*4bit 
	void SPNCryptoSystem::DivideOne(int OneNum)
	{
		for(register int i=textInforlenth;i>=1;i--) Infor[i]=OneNum&0xf,OneNum>>=4;return;    //默认为4*4bit 
	}
	void SPNCryptoSystem::Init()
	{
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
	
int n;
SPNCryptoSystem SPN;

int main()
{
//	freopen("test.txt","r",stdin);
//	freopen("ans.txt","w",stdout);
	scanf("%d",&n);
	SPN.Init();
	while(n--)
	{
		SPN.GetinKey();SPN.GetinInfor();//SPN.Print();
	//	SPN.PrintInfor();cout<<" ";
		SPN.Encode();SPN.PrintInfor();//SPN.Print();
		putchar(' ');SPN.nowval^=1;
		SPN.Decode();SPN.PrintInfor();//SPN.Print();
		puts("");
	}
	return 0;
}
/*
1
3a94d63f 15a9

2
3a94d63f 26b7
3a94d63f 26b7
*/
