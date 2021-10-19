
#include <bits/stdc++.h>
#include <assert.h>
//#include <time.h>
//#include "SPNCryptoSystem.h"
#define textInforlenth 4        //表示一共有4*4个比特的信息 
#define Nr 4
#define maxn 10000
#define maxm (1<<17)
#define maxdig (1<<(4*textInforlenth))
#define outt(x) cout<<#x<<" = "<<x<<"   "
#define hh cout<<endl
#define numoflinear1 15 
#define numoflinear2 10 
#define numoftest 2

#ifndef ONLINE_JUDGE
#include <windows.h>
#define _WINDOWS_H_
#endif

using namespace std;
typedef long long ll;



const int SPN_Pi_S[maxn]={14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7};
int Rev_SPN_Pi_S[maxn]={14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5};
int MAP_SPN_PI_S[maxm];
int Rev_MAP_SPN_PI_S[maxm];
const int SPN_Pi_P[maxn]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};
int Rev_SPN_Pi_P[maxn]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};
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
int n;

   //使用库文件SPNCryptoSystem.h的注释起点 

 class SPNCryptoSystem     //这个类的实现请去看第一题代码 
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
	void SPNCryptoSystem::GetinInfor()
	{
		cc=getchar();//cc=getchar();
		//while((!isdigit(cc))&&(cc>'f'||cc<'a')) cc=getchar();
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
		//while((!isdigit(cc))&&(cc>'f'||cc<'a')) cc=getchar();
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
	
	inline void  SPNCryptoSystem::Substitute(int typ)
	{
		if(typ==0)
			nowval=MAP_SPN_PI_S[nowval];
		if(typ==1)
			nowval=Rev_MAP_SPN_PI_S[nowval];
		return;
	}
	inline void SPNCryptoSystem::XorKey(int val)
	{
		nowval^=val;
		return;
	} 
	inline void SPNCryptoSystem::Permutation(int typ)
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


bool Count1(int val)   //计算val中为1的位数的奇偶性 
{
	bool ans=0;
	while(val)
	{
		if(val&1) ans^=1;
		val>>=1;
	}
	return ans;
}

SPNCryptoSystem SPN;
int XorX[10],XorU4[10],ZeroNum[17][17][17][17],cnt,nowdiviation;
pair<int ,int>PossibleKey[3][maxn];
pair<int ,int>Ciphertextpair[maxm]; 
vector<int >Linear_attck_ans[5];
bool CipherU4[10000],CipherX[10000];
int ans=0,maxnowwa,maxnum[10],minnum[10];
bool Count1Num[1<<18];
inline void LinearAttack(int pls,int tim,int ansnum,int typ4,int typ3,int typ2,int typ1)  //线性攻击，实际上已经废弃了 
{
	ans=0;maxnowwa=0;
	for(int i=0;i<=ansnum;++i) PossibleKey[pls][i].first=PossibleKey[pls][i].second=0;   //清空 
	PossibleKey[pls][0].first=1; 
	maxnum[4]=(XorU4[pls]&0xf)?16:1;maxnum[3]=(XorU4[pls]&0xf0)?16:1;   
	maxnum[2]=(XorU4[pls]&0xf00)?16:1;maxnum[1]=(XorU4[pls]&0xf000)?16:1;
	minnum[4]=(typ4==-1)?0:typ4;minnum[3]=(typ3==-1)?0:typ3;minnum[2]=(typ2==-1)?0:typ2;minnum[1]=(typ1==-1)?0:typ1;
	maxnum[4]=(typ4==-1)?maxnum[4]:typ4+1;maxnum[3]=(typ3==-1)?maxnum[3]:typ3+1;maxnum[2]=(typ2==-1)?maxnum[2]:typ2+1;maxnum[1]=(typ1==-1)?maxnum[1]:typ1+1;
	//确定每一半字节 枚举的上下界 
//	cout<<maxnum[4]<<"   "<<minnum[4]<<endl<<maxnum[3]<<"   "<<minnum[3]<<endl<<maxnum[2]<<"   "<<minnum[2]<<endl<<maxnum[1]<<"   "<<minnum[1]<<endl;
	memset(ZeroNum,0,sizeof(ZeroNum));
	
	for(int i=1;i<=tim;++i)  CipherX[i]=Ciphertextpair[i].first&XorX[pls];    //做第一轮异或解密 
	
	//这里有其注意，多开基层循环，即使是空跑（只循环一次）也会带来很大的开销 
	for(register int l1=minnum[1];l1<maxnum[1];++l1)
		for(register int l2=minnum[2];l2<maxnum[2];++l2)
			for(register int l3=minnum[3];l3<maxnum[3];++l3)
				for(register int l4=minnum[4];l4<maxnum[4];++l4)
				{
					nowwa=(l1<<12)|(l2<<8)|(l3<<4)|l4;
					ZeroNum[l1][l2][l3][l4]=0;Cnt=0;
					for(register int casee=1;casee<=tim;++casee)
					{
						CipherU4[casee]=Ciphertextpair[casee].second^nowwa;CipherU4[casee]=Rev_MAP_SPN_PI_S[CipherU4[casee]];   
						if(Count1Num[CipherX[casee]^(CipherU4[casee]&XorU4[pls])]==0) ZeroNum[l1][l2][l3][l4]++;  //计数 
					}		
				//	if(abs(ZeroNum[l1][l2][l3][l4]-(tim>>1))>180) Linear_attck_ans[pls].push_back(nowwa);
					nowdiviation=abs(ZeroNum[l1][l2][l3][l4]-(tim>>1));
					//PossibleKey[pls][0].first充当一个计数器，维护的是现在存着的值中最小的哪个 
					if(nowdiviation>(tim>>6)&&nowdiviation>PossibleKey[pls][PossibleKey[pls][0].first].first)  //tim>>6是一个神秘的标准 
					{
						PossibleKey[pls][PossibleKey[pls][0].first]=make_pair(nowdiviation,nowwa);
						for(int i=1;i<=ansnum;++i)
						{
							if(PossibleKey[pls][PossibleKey[pls][0].first].first>PossibleKey[pls][i].first) PossibleKey[pls][0].first=i; 
							//这个地方是维护 possibleKey[pls][0].first指向最小值 
						}
					}
					
					/*if(nowwa==16480)
					{
						cout<<"16480 case: "<<ZeroNum[l1][l2][l3][l4]-(tim>>1)<<"  "<<XorX[pls]<<"    "<<XorU4[pls]<<endl;
					}
					//*/
					//cout<<nowwa<<":  "<<ZeroNum[l1][l2][l3][l4]<<"/"<<tim<<"->"<<abs(ZeroNum[l1][l2][l3][l4]-(tim>>1))<<endl; 
				//	if(abs(ZeroNum[l1][l2][l3][l4]-(tim>>1))>ans) ans=abs(ZeroNum[l1][l2][l3][l4]-(tim>>1)),maxnowwa=nowwa;  //求最大用的 
				}
/*	hh;outt(pls);//cout<<"\nIn Linear_attck_ans:\n";
	for(vector<int>::iterator ix=Linear_attck_ans[pls].begin();ix!=Linear_attck_ans[pls].end();ix++) printf("0x%x",*ix);
	cout<<"\nIn PossibleKey:\n";
	for(int i=1;i<=ansnum;++i) outt(PossibleKey[pls][i].second);
//	cout<<"\nMax ans :\n";
//	outt(ans);outt(maxnowwa);outt(XorX[pls]);outt(XorU4[pls]);hh;
	//*/
//	0100 1101 0110 1011   case1:1         96
//	0101 1101 1011 0101   case1:2         176
}
bool numbyte1[10000][20],numbyte2[10000][20],numbyte3[10000][20],numbyte4[10000][20];

void LinearAttack1(int pls,int tim,int ansnum) 
{
	ans=0;maxnowwa=0;PossibleKey[pls][0].first=0;
	
	for(int i=1;i<=tim;++i)  CipherX[i]=Count1Num[Ciphertextpair[i].first&XorX[pls]];  //明文的popcount
	for(int i=0;i<16;++i)
	{
		for(register int casee=1;casee<=tim;++casee)
		{
			numbyte2[casee][i]=Count1Num[Rev_SPN_Pi_S[((Ciphertextpair[casee].second>>8)^i)&0xf]&(XorU4[pls]>>8)];   //倒推出U4的第二字节 
			numbyte4[casee][i]=Count1Num[Rev_SPN_Pi_S[((Ciphertextpair[casee].second)^i)&0xf]&XorU4[pls]];   //倒推出U4的第四字节 
		}
	}
		for(register int l2=0;l2<16;++l2)
				for(register int l4=0;l4<16;++l4)          //枚举密钥 
				{
					nowwa=(l2<<8)|l4;
					ZeroNum[0][l2][0][l4]=0;Cnt=0;
					for(register int casee=1;casee<=tim;++casee)
					{
						//CipherU4[casee]=Ciphertextpair[casee].second^nowwa;CipherU4[casee]=Rev_MAP_SPN_PI_S[CipherU4[casee]];
					//	assert(1);
						//assert(Count1Num[CipherX[casee]^(CipherU4[casee]&XorU4[pls])]==numbyte2[casee][l2]^numbyte4[casee][l4]^CipherX[casee]);
						//if(numbyte2[casee][l2]^numbyte4[casee][l4]^CipherX[casee]==0) ZeroNum[0][l2][0][l4]++;
						Cnt+=!(numbyte2[casee][l2]^numbyte4[casee][l4]^CipherX[casee]);       //判断popcount是否为0 
						
					}		
					ZeroNum[0][l2][0][l4]=Cnt;
					nowdiviation=abs(ZeroNum[0][l2][0][l4]-(tim>>1));
				//	if(nowdiviation>(tim>>6))
					{
						PossibleKey[pls][++PossibleKey[pls][0].first]=make_pair(nowdiviation,nowwa);      //存储结果 
					}
				}
				
	sort(PossibleKey[pls]+1,PossibleKey[pls]+1+PossibleKey[pls][0].first);	//直接排序，时间开销似乎并没有多少的样子 
	return;
}
void LinearAttack2(int pls,int tim,int ansnum,int key2)    //基本同 LinearAttack1相同，只不过这次攻击的是1，2，3字节，并且第二字节对应的密钥确定为key2 
{
	ans=0;maxnowwa=0;PossibleKey[pls][0].first=0; 
//	for(int i=0;i<=ansnum;++i) PossibleKey[pls][i].first=PossibleKey[pls][i].second=0;
	
	for(int i=1;i<=tim;++i)  CipherX[i]=Count1Num[Ciphertextpair[i].first&XorX[pls]];
	for(register int casee=1;casee<=tim;++casee)
	{	
		numbyte2[casee][key2]=Count1Num[Rev_SPN_Pi_S[((Ciphertextpair[casee].second>>8)^key2)&0xf]&(XorU4[pls]>>8)]^CipherX[casee];
		for(int i=0;i<16;++i)
		{
			numbyte1[casee][i]=Count1Num[Rev_SPN_Pi_S[((Ciphertextpair[casee].second>>12)^i)&0xf]&(XorU4[pls]>>12)];
			numbyte3[casee][i]=Count1Num[Rev_SPN_Pi_S[((Ciphertextpair[casee].second>>4)^i)&0xf]&(XorU4[pls]>>4)];
		}
	}
		for(register int l1=0;l1<16;++l1)
				for(register int l3=0;l3<16;++l3)
				{
					nowwa=(l1<<12)|(key2<<8)|(l3<<4);
					ZeroNum[l1][key2][l3][0]=0;Cnt=0;
					for(register int casee=1;casee<=tim;++casee)
					{
					//	CipherU4[casee]=Ciphertextpair[casee].second^nowwa;CipherU4[casee]=Rev_MAP_SPN_PI_S[CipherU4[casee]];
					//	assert(Count1Num[CipherX[casee]^(CipherU4[casee]&XorU4[pls])]==numbyte1[casee][l1]^numbyte2[casee][key2]^numbyte3[casee][l3]^CipherX[casee]);
					//	if(numbyte1[casee][l1]^numbyte2[casee][key2]^numbyte3[casee][l3]^CipherX[casee]==0) ZeroNum[l1][key2][l3][0]++;
						Cnt+=!(numbyte1[casee][l1]^numbyte2[casee][key2]^numbyte3[casee][l3]);
					}
					ZeroNum[l1][key2][l3][0]=Cnt;
				//	if(abs(ZeroNum[l1][l2][l3][l4]-(tim>>1))>180) Linear_attck_ans[pls].push_back(nowwa);
					nowdiviation=abs(ZeroNum[l1][key2][l3][0]-(tim>>1));
				//	if(nowdiviation>(tim>>6))
					{
						PossibleKey[pls][++PossibleKey[pls][0].first]=make_pair(nowdiviation,nowwa);
					}
				}
				
	sort(PossibleKey[pls]+1,PossibleKey[pls]+1+PossibleKey[pls][0].first);	
	return;
}



bool LoopTest(int maxnum)
{
	for(int casee=2654;casee<2654+maxnum;++casee)   //从一个神秘的数开始，进行加密测试 
	{
		SPN.nowval=Ciphertextpair[casee].first;SPN.DivideOne(Ciphertextpair[casee].first);
		SPN.Encode(); 
		if(SPN.nowval!=Ciphertextpair[casee].second) return false;
	}		
	return true;
}
int qwer;

int main()
{
	#ifdef _WINDOWS_H_
	time_t Initial=GetTickCount();
	#endif
	#ifndef ONLINE_JUDGE        
	//一个简易的读入输出宏 
	freopen(".\\TestCase\\T2\\1.in","r",stdin);
	freopen("T2ans.txt","w",stdout);
	#endif
	n=read();SPN.Init();
	XorX[1]=0x0b00;XorU4[1]=0x0505;XorX[2]=0x0c00;XorU4[2]=0x5550;//XorU4[2]=0x8080;      //线性攻击的掩码（？） 
	for(register int i=0;i<(1<<16);++i)	Count1Num[i]=Count1(i);
	
	while(n--)
	{
	//	cout<<"New Case:\n";
		for(register int i=1;i<=8000;++i)   //读入8000组明密文对 
		{
			SPN.GetinInfor();nowwa=SPN.nowval;
			SPN.GetinInfor();Ciphertextpair[i]=make_pair(nowwa, SPN.nowval);
		//	cout<<i<<"   "<<Ciphertextpair[i].first<<"   "<< Ciphertextpair[i].second<<endl;
		}
		#ifdef _WINDOWS_H_
		time_t afterinput=GetTickCount();outt(afterinput-Initial);hh;	
		#endif
	//	LinearAttack(1,6000,numoflinear1,-1,-1,-1,-1);   //分析第2，4byte 
	//	sort(PossibleKey[1]+1,PossibleKey[1]+1+numoflinear1);	
		LinearAttack1(1,8000,numoflinear1);	   //第一组线性攻击 

		#ifdef _WINDOWS_H_
		time_t afterfirstattack=GetTickCount();outt(afterfirstattack-Initial);hh;
		#endif

		////XorX[2]=1;XorX[2]=57351;XorU4[2]=28800;LinearAttack(2,8000,1);
		//XorX[2]=1028;XorU4[2]=41120;LinearAttack(2,8000,1);
		//LinearAttack(2,8000,numoflinear2);   //目标是分析第1byte 
		 //更多选项可以去 T2-2nd-Linear .txt 都不太行 
		/* hh;
		for(int i=1;i<=numoflinear1;++i)
		{
		 	for(int j=1;j<=numoflinear2;++j)
		 	{
		 		if((PossibleKey[1][i].second&0xf00)^(PossibleKey[2][j].second&0xf00)) continue;
		 		SPN.Key[5]=(PossibleKey[2][j].second&0xf000)>>12;SPN.Key[6]=(PossibleKey[2][j].second&0xf00)>>8;
		 		SPN.Key[7]=(PossibleKey[2][j].second&0xf0)>>4;SPN.Key[8]=(PossibleKey[1][i].second&0xf);
		 		cout<<i<<"  "<<j<<"   ";SPN.PrintKey();hh;
			}
		}
		 hh;
		 //*/
		 
	//	time_t aftersecondattack=GetTickCount();outt(aftersecondattack-Initial);hh;
		for(int i=PossibleKey[1][0].first;i>=1;--i)
		{
		//	LinearAttack(2,6000,numoflinear2,-1,-1,(PossibleKey[1][i].second>>8)&0xf,-1);   //目标是分析第1byte 
		// 	sort(PossibleKey[2]+1,PossibleKey[2]+1+numoflinear2);		
			#ifdef _WINDOWS_H_
			printf("Linear Attack1 %d:0x%x\n",i,PossibleKey[1][i].second);
			#endif
			LinearAttack2(2,8000,numoflinear2,(PossibleKey[1][i].second>>8)&0xf);	//以 PossibleKey[1][i]的密钥第二字节去进行第二线性攻击 
		 	SPN.Key[6]=(PossibleKey[1][i].second&0xf00)>>8;SPN.Key[8]=(PossibleKey[1][i].second&0xf);
		 	for(int j=PossibleKey[2][0].first;j>=max(1,PossibleKey[2][0].first-numoftest);--j)  //考虑此时已经固定了一个字节，故因此理论上答案的偏差会很大，排名会很靠前？ 
		 	{
		 		#ifdef _WINDOWS_H_
				printf("Linear Attack2 %d:0x%x\n",j,PossibleKey[2][j].second);
				#endif
		 	//	if(PossibleKey[2][j].first<(6000>>5)) break;
		 		//if((PossibleKey[1][i].second^PossibleKey[2][j].second)&0xf00) continue;
		 		SPN.Key[5]=(PossibleKey[2][j].second&0xf000)>>12;SPN.Key[7]=(PossibleKey[2][j].second&0xf0)>>4;
		 		for(register int key4=0;key4<65536;++key4)   //枚举前四*4位的密钥 
		 		{
		 			qwer=key4;
		 			SPN.Key[4]=qwer&0xf;qwer>>=4;SPN.Key[3]=qwer&0xf;qwer>>=4;
		 			SPN.Key[2]=qwer&0xf;qwer>>=4;SPN.Key[1]=qwer&0xf;qwer>>=4;
		 			if(LoopTest(numoftest))  //已经获得了完整的密钥，进行测试 
		 			{
						#ifdef _WINDOWS_H_
		 				outt(i);outt(j);hh;
					 	#endif
		 				SPN.PrintKey();
		 				goto endd;
		 			}
		 		}
		 	}
		 } 
		 endd:
		 cout<<endl;
		//XorX[3]=47616;XorU4[3]=32;LinearAttack(3,8000,5);
		//更多选项可以去 T2-3rd-Linear .txt 
		/*
		for(int i=0;i<65536;++i)
		{
			XorX[20]=i;
				for(int l2=0;l2<16;++l2)
				{
					XorU4[20]=(l2<<4);
					LinearAttack(20,5000,5);
				}
		}
		//*/        //       找另一组线性分析用 
		
		Linear_attck_ans[1].clear();Linear_attck_ans[2].clear();//Linear_attck_ans[3].clear();  //清空下一轮需要用的 
	//	time_t afterall=GetTickCount();outt(afterall-Initial);hh;
	//	cout<<endl<<endl<<endl;
	}
	//*/
	return 0;
} 



