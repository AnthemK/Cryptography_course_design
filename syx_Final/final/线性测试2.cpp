#include<stdio.h>
#include<string.h>
# define N 70000
using namespace std;
typedef unsigned int ui;
typedef unsigned short us;
char out[16]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'}; 
char in[127];
char sbox[16]={0xe, 0x4, 0xd, 0x1, 0x2, 0xf ,0xb, 0x8 ,0x3 ,0xa ,0x6 ,0xc ,0x5 ,0x9 ,0x0 ,0x7};	
char nsbox[16]={14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5};
const unsigned short pos[16] = {0x8000, 0x4000, 0x2000, 0x1000,
                                0x0800, 0x0400, 0x0200, 0x0100,
                                0x0080, 0x0040, 0x0020, 0x0010,
                                0x0008, 0x0004, 0x0002, 0x0001};
const unsigned short pbox[16] = {0x8000, 0x0800, 0x0080, 0x0008,
								 0x4000, 0x0400, 0x0040, 0x0004,
								 0x2000, 0x0200, 0x0020, 0x0002,
								 0x1000, 0x0100, 0x0010, 0x0001};
us sp_map[N];
us s_map[N];

us enkey[5];
ui p_e[8000][2]; 
int cnt1[16][16];
int cnt2[16][16];
bool vis[16][16];
bool xor_[16]; 
int key[10];
ui read(void)
{
	char c=getchar();
	ui re=0;
	c=getchar();
    re=(re<<4)|in[c];
    c=getchar();
    re=(re<<4)|in[c];
    c=getchar();
    re=(re<<4)|in[c];
    c=getchar();
    re=(re<<4)|in[c];
	return re;
}
void init(void)
{
	for(int i='0'; i<='9'; i++)	in[i]=i^'0';
	for(int i='a'; i<='f'; i++)	in[i]=(i^96)+9;
  	for(int i = 0; i < 65536; ++i)
  	{
		s_map[i] = (sbox[i >> 12] << 12) | (sbox[(i >> 8) & 0xf] << 8) | (sbox[(i >> 4) & 0xf] << 4) | sbox[i & 0xf];
		for(int j = 0; j <16; ++j)
        {
			if(s_map[i] & pos[j]) sp_map[i] |= pbox[j];
		}
	} 
	for(int i=1; i<16; i++)
	{
		xor_[i]=xor_[i>>1]^(i&1);
	}
}
void linear1(void)
{	
	memset(cnt1, 0, sizeof(cnt1));
	for(int i=0; i<8000; i++)
	{
      	bool x=xor_[(p_e[i][0]>>8)&0xb];
		ui y2=(p_e[i][1]>>8)&15, y4=p_e[i][1]&15;
		for(int l1=0; l1<16; l1++)
		{
			for(int l2=0; l2<16; l2++)
			{
				ui u1=nsbox[l1^y2],u2=nsbox[l2^y4];
				bool y=xor_[u2&0x5] ^ xor_[u1&0x5];
				if(y^x==0)	cnt1[l1][l2]++;
			}
		}
	}
	for(int l1=0; l1<16; l1++)
	{
		for(int l2=0; l2<16; l2++)
		{	
			cnt1[l1][l2]=cnt1[l1][l2]-4000;
			if(cnt1[l1][l2]<0)	cnt1[l1][l2]=-cnt1[l1][l2];
		}
	}
}
bool get_max1(void)
{
	int max=0,tkey5, tkey7;
	for(int l1=0; l1<16; l1++)
	{
		for(int l2=0; l2<16; l2++)
		{	
			if(cnt1[l1][l2]>max)
			{
				//printf("%x %x:%d\n",l1,l2,abs(cnt1[l1][l2]-4000));
				tkey5=l1, tkey7=l2;
				max=cnt1[l1][l2];
			}
		}
	}
	cnt1[tkey5][tkey7]=0;
	//printf("%d %d\n",tkey5,tkey7);
	if(tkey5!=key[5])
	{
		key[5]=tkey5;
		key[7]=tkey7;
		return false;
	}
	key[7]=tkey7;
	return true;
}
void linear2(void)
{	
	memset(cnt2, 0, sizeof(cnt2));
	for(int i=0; i<2000; i++)		//3/32
	{
      	bool x=xor_[(p_e[i][0]>>8)&0xc];
		ui y1=(p_e[i][1]>>12)&15,y2=(p_e[i][1]>>8)&15,y3=(p_e[i][1]>>4)&15;
		for(int l1=0; l1<16; l1++)
		{
			for(int l2=0; l2<16; l2++)
			{
				ui u1=nsbox[l1^y1],u2=nsbox[key[5]^y2],u3=nsbox[l2^y3];
				bool y=xor_[u3&0x5] ^ xor_[u2&0x5] ^ xor_[u1&0x5];
				if(y^x==0)	cnt2[l1][l2]++;
			}
		}
	}
	for(int l1=0; l1<16; l1++)
	{
		for(int l2=0; l2<16; l2++)
		{	
			cnt2[l1][l2]=cnt2[l1][l2]-1000;
			if(cnt2[l1][l2]<0)	cnt2[l1][l2]=-cnt2[l1][l2];
		}
	}
}
void get_max2(void)
{
	int max=0;
	for(int l1=0; l1<16; l1++)
	{
		for(int l2=0; l2<16; l2++)
		{	
			if(cnt2[l1][l2]>max && !vis[l1][l2])
			{
				key[4]=l1, key[6]=l2;
				max=cnt2[l1][l2];
			}
		}
	}
	vis[key[4]][key[6]]=1;
	//printf("%d %d\n",key[4],key[6]);
}
us ENSPN(us mess)
{
  	mess^=enkey[0];
	//s,p
	mess=sp_map[mess];
    mess^=enkey[1];
	//s,p
	mess=sp_map[mess];
    mess^=enkey[2];
	//s,p
	mess=sp_map[mess];
	mess^=enkey[3];
	//s
	mess=s_map[mess];
	mess^=enkey[4];
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
		if(ENSPN(p_e[0][0])==p_e[0][1] && ENSPN(p_e[1][0])==p_e[1][1])
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
	memset(key, -1, sizeof(key));
	linear1();
	for(int i=0; i<15; i++)
	{	
		if(!get_max1())	linear2();
		memset(vis, 0, sizeof(vis));
		for(int j=0; j<5; j++)
		{
			get_max2();
			if(try_key())	
			{
				//if(i || j)	printf("%d %d\n",i,j);
				return true;
			}
		}
	}
	return false;
}
int main(void)
{
	//freopen("10.in","r",stdin);
	//freopen("my2.out","w",stdout);
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
		else	
		{
			printf("%d",n);
			putchar('\n');
		}
	}
	return 0;
}
