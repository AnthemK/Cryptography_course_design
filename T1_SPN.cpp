#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn = 10000;
const int maxm = (1<<17);
const int Nr=4;
const int maxdig = (1<<16);
int SPN_Pi_S[maxn]={14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7};   //�α��ϵ�S�� 
//int Rev_SPN_Pi_S[maxn]={14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5};   //�α��ϵ�S�е��� 
int MAP_SPN_PI_S[maxm];    //���ĸ�S�н�ϳ�һ����S�� 
int Rev_MAP_SPN_PI_S[maxm];   //��һ���õ��� 
int SPN_Pi_P[maxn]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};   //�α��ϵ�p�� 
//int Rev_SPN_Pi_P[maxn]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};  //�α��ϵ�p�е��� 
int MAP_SPN_PI_P[maxm],Rev_MAP_SPN_PI_P[maxm];   //���ĸ�p�н�ϳ�һ����S�� ,�Լ������� 
<<<<<<< HEAD
int MAP_SPN_SP[maxm],Rev_MAP_SPN_SP[maxm];   //���ǵ������� 
=======
int MAP_SPN_SP[maxm],Rev_MAP_SPN_SP[maxm];   //���ǵ�SPN�ļ�������SP�����������֣��ʴ���������Լ��� 
>>>>>>> 1609f65 (init)

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
<<<<<<< HEAD
	void GetinInfor()
=======
	void GetinInfor()             //������/����  4��16������ 
>>>>>>> 1609f65 (init)
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
<<<<<<< HEAD
	void GetinKey()
=======
	void GetinKey()      //������Կ    4+Nr��16������ 
>>>>>>> 1609f65 (init)
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
<<<<<<< HEAD
	void PrintInfor()
=======
	void PrintInfor()   //�����/���� 
>>>>>>> 1609f65 (init)
	{
		for(int i=1;i<=4;++i) putchar(NumtoChar[Infor[i]]);
		return;
	}
<<<<<<< HEAD
	void Print()
=======
	void Print()   //���ȫ����Ϣ 
>>>>>>> 1609f65 (init)
	{
		cout<<"*******************************"<<endl<<"Infor: ";
		for(int i=1;i<=4;++i) putchar((Infor[i]>=10)?(Infor[i]-10+'a'):(Infor[i]^'0'));
		cout<<endl<<"Key: ";
		for(int i=1;i<=4+Nr;++i) putchar((Key[i]>=10)?(Key[i]-10+'a'):(Key[i]^'0'));
		cout<<endl;
		return;
	}
<<<<<<< HEAD
	int MakeOne() {return (Infor[1]<<12)|(Infor[2]<<8)|(Infor[3]<<4)|(Infor[4]);} 
	void DivideOne(int OneNum)
	{
		for(register int i=4;i>=1;i--) Infor[i]=OneNum&0xf,OneNum>>=4;return;
	}
	void Init()
	{
		for(register int val=0;val<maxdig;++val)
		{
			DivideOne(val);
			for(register int i=1;i<=4;++i) Infor[i]=SPN_Pi_S[Infor[i]];
			MAP_SPN_PI_S[val]=MakeOne();
			Rev_MAP_SPN_PI_S[MAP_SPN_PI_S[val]]=val;
			
			DivideOne(val);Cnt=0;
=======
	int MakeOne() {return (Infor[1]<<12)|(Infor[2]<<8)|(Infor[3]<<4)|(Infor[4]);}    //���ĸ�����洢��Infor�ϳ�Ϊһ���� 
	void DivideOne(int OneNum)      //��OneNum�ֳ�����ŵ��ĸ�Infor���� 
	{
		for(register int i=4;i>=1;i--) Infor[i]=OneNum&0xf,OneNum>>=4;return;
	}
	void Init()   //��ʼ�� 
	{
		for(register int val=0;val<maxdig;++val)      //ö��ӳ��ĳ�ʼֵ 
		{
			DivideOne(val);      //�ŵ�Infor�� 
			for(register int i=1;i<=4;++i) Infor[i]=SPN_Pi_S[Infor[i]];    
			MAP_SPN_PI_S[val]=MakeOne();       //�õ���S�� 
			Rev_MAP_SPN_PI_S[MAP_SPN_PI_S[val]]=val;   //�õ��䷴�� 
			
			DivideOne(val);Cnt=0;     
>>>>>>> 1609f65 (init)
			for(register int i=1;i<=4;++i)
			{
				for(register int j=(1<<3);j;j>>=1)
				{
<<<<<<< HEAD
					Bool_Buffer[SPN_Pi_P[Cnt]]=(Infor[i]&j)?1:0;
=======
					Bool_Buffer[SPN_Pi_P[Cnt]]=(Infor[i]&j)?1:0;         //�洢���Ƕ�Ӧλ��Ϊ1����0 
>>>>>>> 1609f65 (init)
					Cnt++;
				}
			}
			for(register int i=1;i<=4;++i)
				Cnt=((i-1)<<2),Infor[i]=(Bool_Buffer[Cnt]<<3)|(Bool_Buffer[Cnt+1]<<2)|(Bool_Buffer[Cnt+2]<<1)|(Bool_Buffer[Cnt+3]);
<<<<<<< HEAD
			MAP_SPN_PI_P[val]=MakeOne();
			Rev_MAP_SPN_PI_P[MAP_SPN_PI_P[val]]=val;
=======
			//���ɾ���P��֮��Ľ�� 
			MAP_SPN_PI_P[val]=MakeOne();   //�õ���p�� 
			Rev_MAP_SPN_PI_P[MAP_SPN_PI_P[val]]=val;  //�õ��䷴�� 
>>>>>>> 1609f65 (init)
		}		
		for(register int val=0;val<maxdig;++val)
		{
			MAP_SPN_SP[val]=MAP_SPN_PI_P[MAP_SPN_PI_S[val]];
			Rev_MAP_SPN_SP[MAP_SPN_SP[val]]=val;
		}
	} 
	
<<<<<<< HEAD
	void  Substitute(int typ)
=======
	void  Substitute(int typ)    //��һ��S��֮���� 
>>>>>>> 1609f65 (init)
	{
		if(typ==0)
			nowval=MAP_SPN_PI_S[nowval];
		if(typ==1)
			nowval=Rev_MAP_SPN_PI_S[nowval];
		return;
	}
<<<<<<< HEAD
	void XorKey(int val)
=======
	void XorKey(int val)   //�����һ����Կ֮���� ��valΪ��Կ 
>>>>>>> 1609f65 (init)
	{
		nowval^=val;
		return;
	} 
<<<<<<< HEAD
	void Permutation(int typ)
=======
	void Permutation(int typ)   //��һ��p��֮���� 
>>>>>>> 1609f65 (init)
	{
		if(typ==0) {nowval=MAP_SPN_PI_P[nowval];return;}
		else if(typ==1) { nowval=Rev_MAP_SPN_PI_P[nowval];return;}
		return;
	}
<<<<<<< HEAD
	void Encode()
	{
		int Lastval=(Key[1]<<12)|(Key[2]<<8)|(Key[3]<<4)|(Key[4]);
=======
	void Encode()    //���ܹ��� 
	{
		int Lastval=(Key[1]<<12)|(Key[2]<<8)|(Key[3]<<4)|(Key[4]);    //��ʼ����Կ 
>>>>>>> 1609f65 (init)
		for(int r=1;r<=Nr-1;++r)
		{
			XorKey(Lastval);
		//	nowval^=Lastval;
<<<<<<< HEAD
			Lastval<<=4;Lastval&=0xffff;Lastval|=Key[r+4]; 
=======
			Lastval<<=4;Lastval&=0xffff;Lastval|=Key[r+4];   //ά������Կ 
>>>>>>> 1609f65 (init)
	//		Substitute(0);
		//	nowval=MAP_SPN_PI_S[nowval];
	//		Permutation(0);
		//	nowval=MAP_SPN_PI_P[nowval];
			nowval=MAP_SPN_SP[nowval];
		
		}
		XorKey(Lastval);
<<<<<<< HEAD
	//	nowval^=Lastval;
		Lastval<<=4;Lastval&=0xffff;Lastval|=Key[Nr+4]; 
=======
	//	nowval^=Lastval; 
		Lastval<<=4;Lastval&=0xffff;Lastval|=Key[Nr+4];   //ά������Կ 
>>>>>>> 1609f65 (init)
		Substitute(0);
	//	nowval=MAP_SPN_PI_S[nowval];
		XorKey(Lastval);
    //	nowval^=Lastval;
		DivideOne(nowval);
		return;
	}
<<<<<<< HEAD
	void Decode()
	{
		int Lastval=(Key[Nr+1]<<12)|(Key[Nr+2]<<8)|(Key[Nr+3]<<4)|(Key[Nr+4]);
		XorKey(Lastval);
	//	nowval^=Lastval;
		Lastval>>=4;Lastval|=(Key[Nr]<<12); 
=======
	void Decode()  //���ܹ��� 
	{
		int Lastval=(Key[Nr+1]<<12)|(Key[Nr+2]<<8)|(Key[Nr+3]<<4)|(Key[Nr+4]); //��ʼ����Կ 
		XorKey(Lastval);
	//	nowval^=Lastval;
		Lastval>>=4;Lastval|=(Key[Nr]<<12);   //ά������Կ 
>>>>>>> 1609f65 (init)
		Substitute(1);
	//	nowval=Rev_MAP_SPN_PI_S[nowval];
		XorKey(Lastval);
    //	nowval^=Lastval;
<<<<<<< HEAD
		Lastval>>=4;Lastval|=(Key[Nr-1]<<12); 
=======
		Lastval>>=4;Lastval|=(Key[Nr-1]<<12);   //ά������Կ 
>>>>>>> 1609f65 (init)
		for(int r=Nr-1;r;--r)
		{
	//		Permutation(1);
		//	nowval=Rev_MAP_SPN_PI_P[nowval];
	//		Substitute(1);
		//	nowval=Rev_MAP_SPN_PI_S[nowval];
			nowval=Rev_MAP_SPN_SP[nowval];
			XorKey(Lastval);
		//	nowval^=Lastval;
<<<<<<< HEAD
			Lastval>>=4;Lastval|=(Key[r-1]<<12); 
=======
			Lastval>>=4;Lastval|=(Key[r-1]<<12);  //ά������Կ 
>>>>>>> 1609f65 (init)
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
<<<<<<< HEAD
		SPN.GetinKey();SPN.GetinInfor();//SPN.Print();
	//	SPN.PrintInfor();cout<<" ";
		SPN.Encode();SPN.PrintInfor();//SPN.Print();
		putchar(' ');SPN.nowval^=1;
		SPN.Decode();SPN.PrintInfor();//SPN.Print();
=======
		SPN.GetinKey();SPN.GetinInfor();//SPN.Print();     //���� 
	//	SPN.PrintInfor();cout<<" ";
		SPN.Encode();SPN.PrintInfor();//SPN.Print();   //���� 
		putchar(' ');SPN.nowval^=1;        //��Ҫ����� 
		SPN.Decode();SPN.PrintInfor();//SPN.Print();   //���� 
>>>>>>> 1609f65 (init)
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