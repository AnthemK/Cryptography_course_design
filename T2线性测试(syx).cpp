#include<stdio.h>
#include<string.h>
#include<utility>
#include<queue>
#include <unistd.h>
# define abs(x) (x)>0 ? (x) : -(x)
# define N 70000
using namespace std;
typedef unsigned int ui;
typedef unsigned short us;
char v[20];
char out[16]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'}; 
char in[127];
char sbox[16]={0xe, 0x4, 0xd, 0x1, 0x2, 0xf ,0xb, 0x8 ,0x3 ,0xa ,0x6 ,0xc ,0x5 ,0x9 ,0x0 ,0x7};
char pbox[16]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};	
char nsbox[16]={14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5};
us p_map[N];
us s_map[N];

us enkey[5];
ui p_e[8000][2]; 
int cnt1[2][16];
int cnt2[10][10][10];

int mc2[4];
ui key[10];
ui read(void)
{
	char c=getchar();
	while(!(c>='0' && c<='9') && !(c>='a' && c<='f'))
	{	
		c=getchar();
	}
	ui re=0;
	while((c>='0' && c<='9') || (c>='a' && c<='f'))
	{
		int x=in[c];
		re=(re<<4)|x;
		c=getchar();
	}
	return re;
}
void init(void)
{
	for(int i=0; i<65536; i++)
	{
		us mess1=i;
		for(int j=0; j<4; j++)
		{
			us x=(0xf000&mess1)>>12;
			mess1=(mess1<<4)|sbox[x];
		}
		s_map[i]=mess1;
	}
	for(int i=0; i<65536; i++)
	{
		us mess=i;
		for(int j=0; j<16; j++)
		{
			char x=(mess&0x8000)>>15;
			mess<<=1;
			v[pbox[j]]=x;
		}
		mess=0;
		for(int j=0; j<16; j++)	mess=(mess<<1)|v[j];
		p_map[i]=mess;
	}
}
void linear1(void)
{	
	memset(cnt1, 0, sizeof(cnt1));
	for(int i=0; i<8000; i++)
	{
		bool x=((p_e[i][0]>>8)&1) ^ ((p_e[i][0]>>9)&1) ^ ((p_e[i][0]>>11)&1);
		ui y2=(p_e[i][1]>>8)&15, y4=p_e[i][1]&15;
		for(int l2=0; l2<16; l2++)
		{
			ui u1=nsbox[mc2[1]^y2],u2=nsbox[l2^y4];
			bool y=(u2&1) ^ ((u2>>2)&1) ^ (u1&1) ^ ((u1>>2)&1);
			if(y^x==0)	cnt1[0][l2]++;
			u1=nsbox[mc2[1]^y2^0x8];
			y=(u2&1) ^ ((u2>>2)&1) ^ (u1&1) ^ ((u1>>2)&1);
			if(y^x==0)	cnt1[1][l2]++;
		}
	}
	int x;
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<16; j++)
		{
			x=cnt1[i][j]-4000;
			cnt1[i][j]=abs(x);
		}
	}
	//printf("%d %d\n",key[5],key[7]);
}
void get_max1(void)
{
	int max=0,loci=-1, locj=-1;
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<16; j++)
		{
			if(cnt1[i][j]>max)	loci=i,locj=j,max=cnt1[i][j];
		}
	}
	if(loci==0)	key[5]=mc2[1];
	else key[5]=mc2[1]^0x8;
	key[7]=locj;
	cnt1[loci][locj]=0;
}
void linear2(void)
{
	memset(cnt2, 0, sizeof(cnt2));
	for(int i=0; i<8000; i++)
	{
		bool x=((p_e[i][0]>>15)&1) ^ ((p_e[i][0]>>14)&1) ^ ((p_e[i][0]>>3)&1) ^ ((p_e[i][0]>>2)&1);
		ui y1=(p_e[i][1]>>12)&15, y2=(p_e[i][1]>>8)&15, y3=(p_e[i][1]>>4)&15;
		for(int l1=0; l1<8; l1++)
		{
			for(int l2=0; l2<8; l2++)
			{	
				for(int l3=0; l3<8; l3++) 
				{
					ui u1=nsbox[l1^y1],u2=nsbox[l2^y2],u3=nsbox[l3^y3];
					bool y=((u2>>3)&1) ^ ((u1>>3)&1) ^ ((u3>>3)&1);
					if(y^x==0)	cnt2[l1][l2][l3]++;
				}
			}
		}
	}
	int max=0,x;
	for(int l1=0; l1<8; l1++)
	{
		for(int l2=0; l2<8; l2++)
		{	
			for(int l3=0; l3<8; l3++) 
			{
				x=cnt2[l1][l2][l3]-4000;
				//cnt2[l1][l2][l3]>4000? cnt2[l1][l2][l3]-4000 : 4000-cnt2[l1][l2][l3];
				cnt2[l1][l2][l3]=abs(x);
				if(cnt2[l1][l2][l3]>max)
				{
					mc2[0]=l1,mc2[1]=l2,mc2[2]=l3;
					max=cnt2[l1][l2][l3];
				}
			}
		}
	}
	cnt2[mc2[0]][mc2[1]][mc2[2]]=0;
}
void get_max2(void)
{
	int max=0;
	for(int l1=0; l1<8; l1++)
	{
		for(int l2=0; l2<8; l2++)
		{	
			for(int l3=0; l3<8; l3++) 
			{
				if(cnt2[l1][l2][l3]>max)
				{
					mc2[0]=l1,mc2[1]=l2,mc2[2]=l3;
					max=cnt2[l1][l2][l3];
				}
			}
		}
	}
	cnt2[mc2[0]][mc2[1]][mc2[2]]=0;
}
us ENSPN(us mess)
{
	for(int m=0; m<3; m++)
	{
		us k=enkey[m];
		mess^=k;
		//s
		mess=s_map[mess];
		//p
		mess=p_map[mess];
	}
	us k=enkey[3];
	mess^=k;
	//s
	mess=s_map[mess];
	k=enkey[4];
	mess^=k;
	return mess;
}
bool try_key(void)
{
	for(enkey[0]=65; enkey[0]<65458; enkey[0]++)
	{
		enkey[1]=(enkey[0]<<4)|key[4];
		enkey[2]=(enkey[1]<<4)|key[5];
		enkey[3]=(enkey[2]<<4)|key[6];
		enkey[4]=(enkey[3]<<4)|key[7];
		bool can=true;
		for(int k=0; k<2; k++)
		{
			if(ENSPN(p_e[k][0])!=p_e[k][1])
			{
				can=false;
				break;
			}
		}//*/
		if(can)	
		//if(ENSPN(p_e[0][0])==p_e[0][1] && ENSPN(p_e[1][0])==p_e[1][1])
		{
			for(int i=0; i<4; i++)
			{
				key[i]=(enkey[0] >> ((3-i)<<2) ) & 0xf;
			}
			return true;
		}
	}
	return false; 
}
bool get_key(void)
{
	memset(key, 0, sizeof(key));
	linear2();
	while(1)
	{	
		linear1();
		for(int i=0; i<5; i++)
		{
			get_max1();
			for(int j=0; j<4; j++)
			{
				key[4]=mc2[0];
				if(j&1)		key[4]^=0x8;
				key[6]=mc2[2];
				if(j>>1)	key[6]^=0x8;
				if(try_key())	
				{
					//printf("%d %d\n",k,j);
					return true;
				}
			}
		}
		get_max2();
	}
	return false;
}
int main(void)
{
	//freopen("10.in","r",stdin);
	//freopen("my1.out","w",stdout);
	for(int i='0'; i<='9'; i++)	in[i]=i^'0';
	for(int i='a'; i<='f'; i++)	in[i]=(i^96)+9;
	init();
	int n;
	scanf("%d",&n);
	while(n--)
	{
		for(int i=0; i<8000; i++)
		{
			p_e[i][0]=read();
			p_e[i][1]=read();
		}
		if(get_key())
		{
			for(int i=0; i<8; i++)	putchar(out[key[i]]);
			putchar('\n');
		}
		else	putchar('\n');
	}
	return 0;
}
