#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <assert.h>
#define outt(x) printf("%s = %d  ",#x,x);
#define hh puts("")
#define textInforlenth (4)        //表示一共有4*8个比特的信息 
#define Nr (12)
#define maxdig (1<<(8*(textInforlenth>>1)))                 //0.5*textInforlenth个字节 

#ifndef ONLINE_JUDGE
#include <windows.h>
#define _WINDOWS_H_
#endif

#define KEYBYTES (16)
#define INPUTBYTES (1<<24)
#define SPNBYTES (4) 

using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned char uchar; 
const uint maxn = 10000;
const uint maxm = ((1<<16)+5);
const uint maxbyte = ((1<<8)+5);
//int SPN_Pi_S[maxn]={14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7};       //需要修改到256
uint SPN_PI_S[maxbyte]={205,240,162,226,237,136,227,152,188,102,193,124,244,219,71,150,246,108,94,17,175,47,64,66,65,7,222,76,138,99,77,81,192,155,56,39,25,3,151,101,61,68,172,167,24,160,97,19,179,180,198,33,21,224,197,15,120,196,239,44,83,38,46,103,84,95,213,200,170,23,70,149,163,148,251,186,211,189,100,42,191,52,72,53,67,215,245,207,144,146,45,181,93,147,153,80,116,114,49,4,88,16,90,127,255,202,85,55,178,221,229,10,13,105,177,58,0,60,234,34,50,141,242,156,134,28,176,118,48,1,210,6,187,119,243,128,232,166,5,236,137,73,253,217,214,212,69,111,79,184,51,87,216,135,168,158,249,92,35,182,107,235,126,31,2,254,133,233,18,201,174,8,159,82,37,113,9,63,41,104,59,26,231,145,89,122,110,142,86,164,29,30,169,154,223,112,140,78,75,220,238,54,143,195,131,130,228,139,121,109,12,161,125,57,74,190,250,171,209,199,40,123,204,247,183,173,98,185,208,248,241,14,27,203,218,230,43,96,22,194,129,11,115,165,32,132,91,36,157,117,225,206,20,106,62,252};//256->256 
//一个随即出来的s盒 
// ={13,0,30,5,28,19,18,15,4,29,17,12,1,10,26,14,8,6,9,21,3,27,22,31,23,20,16,11,24,7,25,2};
uint Rev_SPN_PI_S[maxn];//={14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5};
uint MAP_SPN_PI_S[maxm];//,Rev_MAP_SPN_PI_S[maxm];
//int SPN_Pi_P[maxn]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};            //需要修改到32 
uint SPN_PI_P[maxbyte];//32->32
uint Rev_SPN_PI_P[maxn];//{0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};
//p盒有两种生成方式，见SPNCryptoSystem::init函数 
uint MAP_SPN_PI_P[2][maxm];//,Rev_MAP_SPN_PI_P[3][maxm];
uint MAP_SPN_SP[2][maxm];//,Rev_MAP_SPN_SP[3][maxm];

//char NumtoChar[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
//int ChartoNum[500];

bool Bool_Buffer[maxm];
uint nowwa,Cnt;
   //使用库文件SPNCryptoSystem.h的注释起点 

class SPNCryptoSystem
{
	private:
	public:
		uchar Infor[maxm],Key[maxm];
		uint nowval;           //文本，密钥，文本压位版（以这个为准） 
		SPNCryptoSystem(); 
		uint MakeOne();
		void DivideOne(uint OneNum);
		void Init();
		inline void Substitute(uint typ);
		inline void XorKey(uint val);
		inline void Permutation(uint typ);
		inline void SAndP(uint typ);
		void Encode();
		void Decode(); 
		void Test(int typ);
};
//*/ 

	SPNCryptoSystem::SPNCryptoSystem()
	{
		nowval=0;
		for(uint i=1;i<=textInforlenth+Nr;++i) Infor[i]=Key[i]=0;
	}
	uint SPNCryptoSystem::MakeOne() {return ((uint)Infor[1]<<24)|((uint)Infor[2]<<16)|((uint)Infor[3]<<8)|((uint)Infor[4]);} //默认为4*8bit 
	void SPNCryptoSystem::DivideOne(uint OneNum)
	{
		for(register uint i=textInforlenth;i>=1;i--) Infor[i]=OneNum&0xff,OneNum>>=8;return;    //默认为4*8bit 
	}
	void SPNCryptoSystem::Init()
	{
		for(int i=0;i<=textInforlenth;++i)
		{
			for(int j=0;j<4;++j)  SPN_PI_P[i*8+j]=(i*2)+(8*j),Rev_SPN_PI_P[(i*2)+(8*j)]=i*8+j;
			for(int j=0;j<4;++j)  SPN_PI_P[i*8+j+4]=(i*2)+(8*j)+1,Rev_SPN_PI_P[(i*2)+(8*j)+1]=i*8+j+4;
		}
		//*/ 
		//周期方法生成p盒，这种方法因为会把每个字节的不确定带入其他字节，所以实际上效果很好，甚至高于随机 
		/*
		for(uint i=0;i<32;++i) SPN_PI_P[i]=i;
		random_shuffle(SPN_PI_P,SPN_PI_P+32);
		for(uint i=0;i<32;++i) Rev_SPN_PI_P[SPN_PI_P[i]]=i;
		//random_shuffle方法生成P盒    求反 
		/*hh;
		for(int i=0;i<32;++i) outt(SPN_PI_P[i]);
		hh;
		//*/
	//	for(uint i=0;i<(1<<8);++i) SPN_PI_S[i]=i;
	//	random_shuffle(SPN_PI_S,SPN_PI_S+(1<<8));
		for(uint i=0;i<(1<<8);++i) Rev_SPN_PI_S[SPN_PI_S[i]]=i;
		//randowm_shuffle 生成S盒   求反 
		/*hh;
		for(int i=0;i<256;++i) outt(SPN_PI_S[i]);
		hh;
		///*/
	//	for(int i=0;i<(1<<4);++i) ChartoNum[(int )NumtoChar[i]]=i;
		for(register uint val=0;val<maxdig;++val)   //(1<<16)
		{
			DivideOne(val);
			for(register uint i=(textInforlenth>>1)+1;i<=textInforlenth;++i) Infor[i]=(uchar )(SPN_PI_S[Infor[i]]&0xff);
			MAP_SPN_PI_S[val]=MakeOne();
			//Rev_MAP_SPN_PI_S[MAP_SPN_PI_S[val]]=val;
			//生成S盒的16位MAP 求反 
		}	 

		for(register uint val=0;val<maxdig;++val)   //(1<<16)
		{			
			DivideOne(val);Cnt=0;
			for(register uint i=0;i<=8*textInforlenth;++i) Bool_Buffer[i]=0;
			for(register uint i=(textInforlenth>>1)+1;i<=textInforlenth;++i)
			{
				for(register uint j=(1<<7);j;j>>=1)
				{
					Bool_Buffer[SPN_PI_P[Cnt]]=(Infor[i]&j)?1:0;
					Cnt++;
				}
			}
			for(register int i=1;i<=textInforlenth;++i)
				Cnt=((i-1)<<3),Infor[i]=((uint )Bool_Buffer[Cnt]<<7)|((uint )Bool_Buffer[Cnt+1]<<6)|((uint )Bool_Buffer[Cnt+2]<<5)|((uint )Bool_Buffer[Cnt+3]<<4)|((uint )Bool_Buffer[Cnt+4]<<3)|((uint )Bool_Buffer[Cnt+5]<<2)|((uint )Bool_Buffer[Cnt+6]<<1)|((uint )Bool_Buffer[Cnt+7]);  //默认为4*8bit 
			MAP_SPN_PI_P[0][val]=MakeOne();
			
			DivideOne(val);Cnt=(textInforlenth>>1)*8;
			for(register uint i=0;i<=8*textInforlenth;++i) Bool_Buffer[i]=0;
			for(register uint i=(textInforlenth>>1)+1;i<=textInforlenth;++i)
			{
				for(register uint j=(1<<7);j;j>>=1)
				{
					Bool_Buffer[SPN_PI_P[Cnt]]=(Infor[i]&j)?1:0;
					Cnt++;
				}
			}
			for(register int i=1;i<=textInforlenth;++i)
				Cnt=((i-1)<<3),Infor[i]=((uint )Bool_Buffer[Cnt]<<7)|((uint )Bool_Buffer[Cnt+1]<<6)|((uint )Bool_Buffer[Cnt+2]<<5)|((uint )Bool_Buffer[Cnt+3]<<4)|((uint )Bool_Buffer[Cnt+4]<<3)|((uint )Bool_Buffer[Cnt+5]<<2)|((uint )Bool_Buffer[Cnt+6]<<1)|((uint )Bool_Buffer[Cnt+7]);  //默认为4*8bit 
			MAP_SPN_PI_P[1][val]=MakeOne();	
		}		//生成正向的P盒MAP，分高低（各两字节） ,高为0，低为1 
		for(register uint val=0;val<maxdig;++val)   //(1<<16)
		{
			MAP_SPN_SP[0][val]=MAP_SPN_PI_P[0][MAP_SPN_PI_S[val]];
			MAP_SPN_SP[1][val]=MAP_SPN_PI_P[1][MAP_SPN_PI_S[val]];
		}
		/*
		for(register uint val=0;val<maxdig;++val)   //(1<<16)
		{			
			DivideOne(val);Cnt=0;
			for(register uint i=0;i<=8*textInforlenth;++i) Bool_Buffer[i]=0;
			for(register uint i=(textInforlenth>>1)+1;i<=textInforlenth;++i)
			{
				for(register uint j=(1<<7);j;j>>=1)
				{
					Bool_Buffer[Rev_SPN_PI_P[Cnt]]=(Infor[i]&j)?1:0;
					Cnt++;
				}
			}
			for(register int i=1;i<=textInforlenth;++i)
				Cnt=((i-1)<<3),Infor[i]=((uint )Bool_Buffer[Cnt]<<7)|((uint )Bool_Buffer[Cnt+1]<<6)|((uint )Bool_Buffer[Cnt+2]<<5)|((uint )Bool_Buffer[Cnt+3]<<4)|((uint )Bool_Buffer[Cnt+4]<<3)|((uint )Bool_Buffer[Cnt+5]<<2)|((uint )Bool_Buffer[Cnt+6]<<1)|((uint )Bool_Buffer[Cnt+7]);  //默认为4*8bit 
			Rev_MAP_SPN_PI_P[0][val]=MakeOne();
			
			DivideOne(val);Cnt=(textInforlenth>>1)*8;
			for(register uint i=0;i<=8*textInforlenth;++i) Bool_Buffer[i]=0;
			for(register uint i=(textInforlenth>>1)+1;i<=textInforlenth;++i)
			{
				for(register uint j=(1<<7);j;j>>=1)
				{
					Bool_Buffer[Rev_SPN_PI_P[Cnt]]=(Infor[i]&j)?1:0;
					Cnt++;
				}
			}
			for(register int i=1;i<=textInforlenth;++i)
				Cnt=((i-1)<<3),Infor[i]=((uint )Bool_Buffer[Cnt]<<7)|((uint )Bool_Buffer[Cnt+1]<<6)|((uint )Bool_Buffer[Cnt+2]<<5)|((uint )Bool_Buffer[Cnt+3]<<4)|((uint )Bool_Buffer[Cnt+4]<<3)|((uint )Bool_Buffer[Cnt+5]<<2)|((uint )Bool_Buffer[Cnt+6]<<1)|((uint )Bool_Buffer[Cnt+7]);  //默认为4*8bit 
			Rev_MAP_SPN_PI_P[1][val]=MakeOne();
		}		//生成反向的P盒MAP，分高低（各两字节） ,高为0，低为1  
		//*/
	} 
	
	inline void  SPNCryptoSystem::Substitute(uint typ)  //0正向 1逆向 
	{
		if(typ==0) nowval=(MAP_SPN_PI_S[(nowval>>16)&0xffff]<<16)|(MAP_SPN_PI_S[nowval&0xffff]);
	//	else if(typ==1) nowval=(Rev_MAP_SPN_PI_S[(nowval>>16)&0xffff]<<16)|(Rev_MAP_SPN_PI_S[nowval&0xffff]);
		return;
	}
	inline void SPNCryptoSystem::XorKey(uint val)   //异或的密钥 
	{
		nowval^=val;
		return;
	} 
	inline void SPNCryptoSystem::Permutation(uint typ)   //0正向 1逆向 
	{
		if(typ==0) {nowval=MAP_SPN_PI_P[0][(nowval>>16)&0xffff]|MAP_SPN_PI_P[1][nowval&0xffff];}
	//	else if(typ==1) {nowval=Rev_MAP_SPN_PI_P[0][(nowval>>16)&0xffff]|Rev_MAP_SPN_PI_P[1][nowval&0xffff];}
		return;
	}
	void SAndP(uint typ)
	{
	//	if(typ==0) {nowval=MAP_SPN_SP[0][(nowval>>16)]|MAP_SPN_SP[1][nowval&0xffff];}
	//	else if(typ==1) {nowval=Rev_MAP_SPN_PI_P[0][(nowval>>16)&0xffff]|Rev_MAP_SPN_PI_P[1][nowval&0xffff];}		
	}
	void SPNCryptoSystem::Encode()
	{
		uint Lastval=(((uint )Key[1])<<24)|(((uint )Key[2])<<16)|(((uint )Key[3]<<8))|(((uint)Key[4]));   //moren
	//	DebugEncode[0]=nowval;
		for(int r=1;r<=Nr-1;++r)
		{
			XorKey(Lastval);
			Lastval<<=8;Lastval&=0xffffffff;Lastval|=Key[r+textInforlenth];        //moren
			//Substitute(0);Permutation(0);
			nowval=MAP_SPN_SP[0][(nowval>>16)]|MAP_SPN_SP[1][nowval&0xffff];
			//SAndP(0);
	//		DebugEncode[r]=nowval;
		}
		XorKey(Lastval);
		Lastval<<=8;Lastval&=0xffffffff;Lastval|=(uint )Key[Nr+textInforlenth];  //moren
	//	DebugEncode[Nr]=nowval; 
		Substitute(0);
	//	DebugEncode[Nr+1]=nowval;
		XorKey(Lastval);
	//	DebugEncode[Nr+2]=nowval;
		DivideOne(nowval);
		return;
	}
	//未完成  因为空间不够，实际上只需要超出一点点就可以 
	void SPNCryptoSystem::Decode()
	{
		uint Lastval=(((uint )Key[Nr+1])<<24)|(((uint )Key[Nr+2])<<16)|(((uint )Key[Nr+3])<<8)|(((uint )Key[Nr+4]));  //moren
	//	DebugDecode[Nr+2]=nowval;
		XorKey(Lastval);
		Lastval>>=8;Lastval&=0xffffffff;Lastval|=(((uint )Key[Nr])<<((textInforlenth-1)<<3));  //moren
	//	DebugDecode[Nr+1]=nowval;
		Substitute(1);
	//	DebugDecode[Nr]=nowval;
		XorKey(Lastval);
		for(int r=Nr-1;r>=1;--r)
		{	
	//		DebugDecode[r]=nowval;
			Lastval>>=8;Lastval&=0xffffffff;Lastval|=(((uint )Key[r])<<((textInforlenth-1)<<3));  //moren
			Permutation(1);
			Substitute(1);	
			XorKey(Lastval);
		}
	//	DebugDecode[0]=nowval;
		DivideOne(nowval);
		return;
	}
	void SPNCryptoSystem::Test(int  typ)   //这个函数是为了确认我的加密确实是可逆的。   typ==1自行生成测试数据 
	{
		if(typ==1) 
		{
			for(uint i=1;i<=textInforlenth;++i) Infor[i]=(uchar )rand()&0xff;
			for(uint i=1;i<=textInforlenth+Nr;++i) Key[i]=(uchar )rand()&0xff;
			nowval=MakeOne();DivideOne(nowval);
		} 
		uchar OrignInfor[textInforlenth+100];
		uint Orignnowval=nowval;
		for(int i=1;i<=textInforlenth;++i) OrignInfor[i]=Infor[i];
		Permutation(0);nowval=MakeOne();DivideOne(nowval);Permutation(1);nowval=MakeOne();DivideOne(nowval);
		//for(int i=1;i<=textInforlenth;++i) assert(OrignInfor[i]==Infor[i]);
		assert(Orignnowval==nowval);
		Substitute(0);nowval=MakeOne();DivideOne(nowval);Substitute(1);nowval=MakeOne();nowval=MakeOne();DivideOne(nowval);
		//for(int i=1;i<=textInforlenth;++i) assert(OrignInfor[i]==Infor[i]);	
		assert(Orignnowval==nowval);
		//测试分开的函数是不是可逆 
		DivideOne(nowval);Encode();
	/*	for(int i=0;i<=Nr+2;++i) printf("%d:0x%x  ",i,DebugEncode[i]);
		hh;printf("0x%x",nowval);hh;
		Decode();	
		for(int i=0;i<=Nr+2;++i) printf("%d:0x%x  ",i,DebugDecode[i]);
		hh;
		puts("Ready to compare");
		for(uint r=Nr+2;r>=1;--r)
			assert(DebugEncode[r]==DebugDecode[r]),outt(r);
		//*/
		Decode();
		//测试加解密是不是可逆 
		assert(Orignnowval==nowval);
	} 
//*/                           如果要引用库文件SPNCryptoSystem.h的话注释到这里 

SPNCryptoSystem SPN;
uint Orign;
int main()
{
	#ifndef ONLINE_JUDGE
	freopen(".\\TestCase\\T4\\1.in","r",stdin);
	FILE* fout=fopen(".\\TestCase\\T4\\OutInfor.txt","w");
	freopen(".\\TestCase\\T4\\T4myans.txt","w",stdout);
	#endif
	SPN.Init();	
	fread(SPN.Key+1,KEYBYTES,1,stdin);
	srand(SPN.Key[1]);   //这里是我最核心的思路，具体可见实验报告 
	
//	for(int i=0;i<(1<<8);++i) fprintf(fout,"%u,",SPN_PI_S[i]); 
	
//	for(int j=1;j<=SPNBYTES;++j) SPN.Infor[j]=rand()&0xff;
	SPN.nowval=rand()&0xffffffff;
	for(int i=0;i<(INPUTBYTES/SPNBYTES);++i)   //分组 
	{
		Orign=SPN.nowval;
		fread(SPN.Infor+1,SPNBYTES,1,stdin);SPN.MakeOne();
		SPN.Key[i%KEYBYTES+1]^=(uchar )(Orign>>(i%24))&0xff;   //这是为了把随即成分引入下一组加密 
		SPN.Encode();
		fwrite(SPN.Infor+1,SPNBYTES,1,stdout);
	}
	return 0;
}
//还有一种通过方法，就是使用random_shuffle 随机生成S盒（种子可取SPN.key[1]）,S盒的逆和MAP等通过程序计算，也可以通过 
