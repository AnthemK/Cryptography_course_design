#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn = 10000;
const int maxm = (1<<17);
const int Nr=4;
const int maxdig = (1<<16);
int SPN_Pi_S[maxn]={14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7};   //课本上的S盒 
//int Rev_SPN_Pi_S[maxn]={14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5};   //课本上的S盒的逆 
int MAP_SPN_PI_S[maxm];    //把四个S盒结合成一个大S盒 
int Rev_MAP_SPN_PI_S[maxm];   //上一个得到逆 
int SPN_Pi_P[maxn]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};   //课本上的p盒 
//int Rev_SPN_Pi_P[maxn]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};  //课本上的p盒的逆 
int MAP_SPN_PI_P[maxm],Rev_MAP_SPN_PI_P[maxm];   //把四个p盒结合成一个大S盒 ,以及他的逆 

int MAP_SPN_SP[maxm],Rev_MAP_SPN_SP[maxm];   //考虑到SPN的加密轮里SP经常连续出现，故打这个表可以加速 

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
struct Nod
{
	int Infor[maxn],Key[maxn],nowval;
	void GetinInfor()             //输入明/密文  4个16进制数 
	{
		cc=getchar();
		while((!isdigit(cc))&&(cc>'f'||cc<'a')) cc=getchar();
		for(register int i=1;i<=4;++i)
		{
			if(isdigit(cc)) Infor[i]=cc-'0';
			else Infor[i]=cc-'a'+10;
			cc=getchar();
		}
		nowval=MakeOne();
		return;
	}
	void GetinKey()      //输入密钥    4+Nr个16进制数 
	{
		cc=getchar();
		while((!isdigit(cc))&&(cc>'f'||cc<'a')) cc=getchar();
		for(register int i=1;i<=4+Nr;++i)
		{
			if(isdigit(cc)) Key[i]=cc-'0';
			else Key[i]=cc-'a'+10;
			cc=getchar();
		}
		return;
	}	
	void PrintInfor()   //输出明/密文 
	{
		for(int i=1;i<=4;++i) putchar(NumtoChar[Infor[i]]);
		return;
	}
	void Print()   //输出全部信息 
	{
		cout<<"*******************************"<<endl<<"Infor: ";
		for(int i=1;i<=4;++i) putchar((Infor[i]>=10)?(Infor[i]-10+'a'):(Infor[i]^'0'));
		cout<<endl<<"Key: ";
		for(int i=1;i<=4+Nr;++i) putchar((Key[i]>=10)?(Key[i]-10+'a'):(Key[i]^'0'));
		cout<<endl;
		return;
	}
	int MakeOne() {return (Infor[1]<<12)|(Infor[2]<<8)|(Infor[3]<<4)|(Infor[4]);}    //把四个分组存储的Infor合成为一个数 
	void DivideOne(int OneNum)      //把OneNum分成四组放到四个Infor里面 
	{
		for(register int i=4;i>=1;i--) Infor[i]=OneNum&0xf,OneNum>>=4;return;
	}
	void Init()   //初始化 
	{
		for(register int val=0;val<maxdig;++val)      //枚举映射的初始值 
		{
			DivideOne(val);      //放到Infor里 
			for(register int i=1;i<=4;++i) Infor[i]=SPN_Pi_S[Infor[i]];    
			MAP_SPN_PI_S[val]=MakeOne();       //得到大S表 
			Rev_MAP_SPN_PI_S[MAP_SPN_PI_S[val]]=val;   //得到其反表 
			
			DivideOne(val);Cnt=0;     
			for(register int i=1;i<=4;++i)
			{
				for(register int j=(1<<3);j;j>>=1)
				{
					Bool_Buffer[SPN_Pi_P[Cnt]]=(Infor[i]&j)?1:0;         //存储的是对应位置为1还是0 
					Cnt++;
				}
			}
			for(register int i=1;i<=4;++i)
				Cnt=((i-1)<<2),Infor[i]=(Bool_Buffer[Cnt]<<3)|(Bool_Buffer[Cnt+1]<<2)|(Bool_Buffer[Cnt+2]<<1)|(Bool_Buffer[Cnt+3]);
			//生成经过P盒之后的结果 
			MAP_SPN_PI_P[val]=MakeOne();   //得到大p表 
			Rev_MAP_SPN_PI_P[MAP_SPN_PI_P[val]]=val;  //得到其反表 
		}		
		for(register int val=0;val<maxdig;++val)
		{
			MAP_SPN_SP[val]=MAP_SPN_PI_P[MAP_SPN_PI_S[val]];
			Rev_MAP_SPN_SP[MAP_SPN_SP[val]]=val;
		}
	} 
	
	void  Substitute(int typ)    //求一次S盒之后结果 
	{
		if(typ==0)
			nowval=MAP_SPN_PI_S[nowval];
		if(typ==1)
			nowval=Rev_MAP_SPN_PI_S[nowval];
		return;
	}
	void XorKey(int val)   //求异或一次密钥之后结果 ，val为密钥 
	{
		nowval^=val;
		return;
	} 
	void Permutation(int typ)   //求一次p盒之后结果 
	{
		if(typ==0) {nowval=MAP_SPN_PI_P[nowval];return;}
		else if(typ==1) { nowval=Rev_MAP_SPN_PI_P[nowval];return;}
		return;
	}
	void Encode()    //加密过程 
	{
		int Lastval=(Key[1]<<12)|(Key[2]<<8)|(Key[3]<<4)|(Key[4]);    //初始化密钥 
		for(int r=1;r<=Nr-1;++r)
		{
			XorKey(Lastval);
		//	nowval^=Lastval;
			Lastval<<=4;Lastval&=0xffff;Lastval|=Key[r+4];   //维护新密钥 
	//		Substitute(0);
		//	nowval=MAP_SPN_PI_S[nowval];
	//		Permutation(0);
		//	nowval=MAP_SPN_PI_P[nowval];
			nowval=MAP_SPN_SP[nowval];
		
		}
		XorKey(Lastval);
	//	nowval^=Lastval; 
		Lastval<<=4;Lastval&=0xffff;Lastval|=Key[Nr+4];   //维护新密钥 
		Substitute(0);
	//	nowval=MAP_SPN_PI_S[nowval];
		XorKey(Lastval);
    //	nowval^=Lastval;
		DivideOne(nowval);
		return;
	}
	void Decode()  //解密过程 
	{
		int Lastval=(Key[Nr+1]<<12)|(Key[Nr+2]<<8)|(Key[Nr+3]<<4)|(Key[Nr+4]); //初始化密钥 
		XorKey(Lastval);
	//	nowval^=Lastval;
		Lastval>>=4;Lastval|=(Key[Nr]<<12);   //维护新密钥 
		Substitute(1);
	//	nowval=Rev_MAP_SPN_PI_S[nowval];
		XorKey(Lastval);
    //	nowval^=Lastval;
		Lastval>>=4;Lastval|=(Key[Nr-1]<<12);   //维护新密钥 
		for(int r=Nr-1;r;--r)
		{
	//		Permutation(1);
		//	nowval=Rev_MAP_SPN_PI_P[nowval];
	//		Substitute(1);
		//	nowval=Rev_MAP_SPN_PI_S[nowval];
			nowval=Rev_MAP_SPN_SP[nowval];
			XorKey(Lastval);
		//	nowval^=Lastval;
			Lastval>>=4;Lastval|=(Key[r-1]<<12);  //维护新密钥 
		}
		DivideOne(nowval);
		return;
	}
}SPN;
int n;

int main()
{
//	freopen("test.txt","r",stdin);
//	freopen("ans.txt","w",stdout);
	scanf("%d",&n);
	SPN.Init();
	while(n--)
	{
		SPN.GetinKey();SPN.GetinInfor();//SPN.Print();     //读入 
	//	SPN.PrintInfor();cout<<" ";
		SPN.Encode();SPN.PrintInfor();//SPN.Print();   //加密 
		putchar(' ');SPN.nowval^=1;        //按要求操作 
		SPN.Decode();SPN.PrintInfor();//SPN.Print();   //解密 
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
