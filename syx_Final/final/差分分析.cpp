#include<stdio.h>
#include<string.h>
# define N 70000
# define L 0
# define R 1500
# define L1 6000
# define R1 13000
typedef unsigned int ui;
typedef unsigned short us;
char out[16]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'}; 
us in[127];
us sbox[16]={0xe, 0x4, 0xd, 0x1, 0x2, 0xf ,0xb, 0x8 ,0x3 ,0xa ,0x6 ,0xc ,0x5 ,0x9 ,0x0 ,0x7};
us pbox[16] = {0x8000, 0x0800, 0x0080, 0x0008,0x4000, 0x0400, 0x0040, 0x0004,0x2000, 0x0200, 0x0020, 0x0002,0x1000, 0x0100, 0x0010, 0x0001};	
us nsbox[16]={14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5};
us pos[16] = {0x8000, 0x4000, 0x2000, 0x1000,0x0800, 0x0400, 0x0200, 0x0100,0x0080, 0x0040, 0x0020, 0x0010,0x0008, 0x0004, 0x0002, 0x0001};
//us p_map[N];
us s_map[N];
us sp_map[N];

us enkey[5];
us p[65536]; 
us key[10];

int cnt24[16][16];
int cnt13[16][16];
bool vis[16][16];
int ra[2]={2,3};
us read(){
  char c=getchar();
	us re=0;
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
	for(int i=0; i<65536; i++)
	{
		s_map[i] = (sbox[i >> 12] << 12) | (sbox[(i >> 8) & 0xf] << 8) | (sbox[(i >> 4) & 0xf] << 4) | sbox[i & 0xf];
		for(int j = 0; j < 16; j++)
		{
			if(s_map[i] & pos[j]) sp_map[i] |= pbox[j];
		}
	}
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
void diff(void)
{
	ui y,y_,l1,l2,xor_,x_,x;
	us u2,u2_,u4,u4_;
	xor_=0x0b00;
	for(x=L; x<R; x++)
	{
		x_=x^xor_;
		//if(x_<x)	continue;
		y=p[x],y_=p[x_];
		if((y & 0xf0f0) == (y_ & 0xf0f0))
		{
			for(l1=0; l1<16; l1++)
			{
				for(l2=0; l2<16; l2++)
				{
					u2=nsbox[l1^((y>>8)&0xf)], u4=nsbox[l2^(y&0xf)];
					u2_=nsbox[l1^((y_>>8)&0xf)], u4_=nsbox[l2^(y_&0xf)];
					if(((u2^u2_)==0x6) && ((u4^u4_)==0x6))	cnt24[l1][l2]++;
				}
			}
		}
	}
	/*xor_=0x5050;		//(1/4)^2 * (3/8) * (1/4)
	for(int x=L1; x<R1; x++)
	{
		//int x=(z<<2)|1;
		int x_=x^xor_;
		//if(x_<x)	continue;
		ui y=p[x],y_=p[x_];
		if(y&0x0f0f != y_&0x0f0f)	continue; 
		for(int l1=0; l1<16; l1++)
		{
			for(int l2=0; l2<16; l2++)
			{
				us u2=nsbox[l1^((y>>12)&0xf)], u4=nsbox[l2^((y>>4)&0xf)];
				us u2_=nsbox[l1^((y_>>12)&0xf)], u4_=nsbox[l2^((y_>>4)&0xf)];
				if(((u2^u2_)==0x8) && ((u4^u4_)==0x8))	cnt13[l1][l2]++;
			}
		}
	}//*/
	xor_=0x0020;		//(1/4) * (3/8) * (1/4)^2
	for(x=L1; x<R1; x++)
	{
		if(x&xor_)	continue;
		//int x=(z<<2)|1;
		x_=x^xor_;
		y=p[x],y_=p[x_];
		if((y&0x0f0f) == (y_&0x0f0f))
		{
			for(l1=0; l1<16; l1++)
			{
				for(l2=0; l2<16; l2++)
				{
					u2=nsbox[l1^((y>>12)&0xf)], u4=nsbox[l2^((y>>4)&0xf)];
					u2_=nsbox[l1^((y_>>12)&0xf)], u4_=nsbox[l2^((y_>>4)&0xf)];
					if(((u2^u2_)==0x5) && ((u4^u4_)==0x5))	cnt13[l1][l2]++;
				}
			}
		}
	}//*/
}
void get_max1(void)
{
	int max = -1;
	for(int l1 = 0; l1 < 16; l1++)
	{
		for(int l2 = 0; l2 < 16; l2++)
		{
			if(cnt13[l1][l2] > max && vis[l1][l2])
			{
				max = cnt13[l1][l2];
				key[4] = l1;
				key[6] = l2;
			}
		}
	}
	vis[key[4]][key[6]] = false;
}
void get_max2(void)
{
	int max = -1;
	for(int l1 = 0; l1 < 16; l1++)
	{
		for(int l2 = 0; l2 < 16; l2++)
		{
			if(cnt24[l1][l2] > max)
			{
				max = cnt24[l1][l2];
				key[5] = l1;
				key[7] = l2;
			}
		}
	}
	cnt24[key[5]][key[7]] = 0;
}
bool try_key(void)
{
	//for(enkey[0]=0; /*enkey[0]+1!=0//*/ enkey[0]<65535; enkey[0]++)
	enkey[4]=(key[4] << 12) | (key[5] << 8) | (key[6] << 4) | key[7]; 
	for(int i=0; /*enkey[0]+1!=0//*/ i<65536; i++)
	{
		enkey[0]=i;
		enkey[1]=(enkey[0]<<4)|key[4];
		enkey[2]=(enkey[1]<<4)|key[5];
		enkey[3]=(enkey[2]<<4)|key[6];
		int k=1934;
		for(; k<1939; k++)
		{
			if((s_map[sp_map[sp_map[sp_map[((k<<2)|1) ^ enkey[0]] ^ enkey[1]] ^ enkey[2]] ^ enkey[3]] ^ enkey[4]) != p[(k<<2)|1])
			//if(ENSPN((k<<2)|1)!=p[(k<<2)|1])
			break;
		}//*/
		if(k==1939)	
		{
			for(int j=0; j<4; j++)
			{
				key[j]=(enkey[0] >> ((3-j)<<2) ) & 0xf;
			}
			return true;
		}
	}
	return false; 
}
bool get_key(void)
{
	memset(key, 0, sizeof(key));
	int max;
	for(int i = 0; i < 10; i++)
	{
		get_max2();
		memset(vis, true, 256 * sizeof(bool));
		for(int j = 0; j < 5; j++)
		{
			get_max1();
			if(try_key())	
			{
				/*if(i ||j)
				printf("%d %d\n\n",i,j);//*/
				return true;
			}
		}
	}
	return false;
}
int main(void)
{
	freopen("7.in","r",stdin);
	init();
	int n;
	scanf("%d",&n);
	while(n--)
	{
		for(register int i=0; i<65536; i++)
		{
			p[i]=read();
		}
		memset(cnt24, 0, sizeof(cnt24));
		memset(cnt13, 0, sizeof(cnt13));
		diff();
		if(get_key())
		{
			for(int i=0; i<8; i++)	putchar(out[key[i]]);
			putchar('\n');
		}
		else	
		{
			printf("%d\n",n);
		}//*/
	}
	return 0;
} 
