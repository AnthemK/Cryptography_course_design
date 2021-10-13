# include<bits/stdc++.h>
#include<string.h>
#include<tr1/unordered_map>
using namespace std;
//using namespace std::tr1;

unsigned int SHA1_tmp;
#define SHA1_ROTL(a,b) (SHA1_tmp=(a),((SHA1_tmp>>(32-b))&(0x7fffffff>>(31-b)))|(SHA1_tmp<<b))
#define SHA1_F(B,C,D,t) ((t<40)?((t<20)?((B&C)|((~B)&D)):(B^C^D)):((t<60)?((B&C)|(B&D)|(C&D)):(B^C^D)))

tr1::unordered_map < string, int> map_e;

char head[10005][265];
char e_tmp[265];
unsigned f_sha1[5];
char cur_e[265];
unsigned cur_s[5];
int UnitSHA1(const char* str, int length, unsigned sha1[5])
{
/*
计算字符串SHA-1
参数说明：
str         字符串指针
length      字符串长度
sha1         用于保存SHA-1的字符串指针
返回值为参数sha1
*/
	unsigned char *pp, *ppend;	
	unsigned int l, i, K[80], W[80], TEMP, A, B, C, D, E, H0, H1, H2, H3, H4;	
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
	for (ppend = pp + l; pp < ppend; pp += 64)
	{	
		for (i = 0; i < 16; W[i] = ((unsigned int*)pp)[i], i++);		
		for (i = 16; i < 80; W[i] = SHA1_ROTL((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]), 1), i++);		
		A = H0, B = H1, C = H2, D = H3, E = H4;
		for (i = 0; i < 80; i++)
		{		
			TEMP = SHA1_ROTL(A, 5) + SHA1_F(B, C, D, i) + E + W[i] + K[i];			
			E = D, D = C, C = SHA1_ROTL(B, 30), B = A, A = TEMP;		
		}		
		H0 += A, H1 += B, H2 += C, H3 += D, H4 += E;	
	}	
	free(pp - l);	
	sha1[0]=H0, sha1[1]=H1, sha1[2]=H2, sha1[3]=H3, sha1[4]=H4;	
	return 0;
}

void getstr(unsigned n,char str[8])
{
	str[0]='a';str[1]='0';str[2]='0';str[3]='0';str[4]='0';str[5]='0';str[6]='0';str[7]='0';
	int i=2;
	while(n)
	{
		unsigned tmp=n%36;
		if(tmp<10)	str[i++]=tmp+'0';
		else	str[i++]=tmp-10+'a';	
		n=n/36;
	}
	str[8]=0;
}
void R(unsigned sha1[5],char str[8],int i)
{
	getstr((sha1[0]+sha1[1]*i)%2176782336ul,str);
}


bool check(void)
{
	for(int i=0; i<5; i++)
	{
		if(f_sha1[i]!=cur_s[i])	return false;
	}
	return true;
}
bool find(int link)
{
	//printf("%d\n",link);
	memset(cur_e, 0, sizeof(cur_e));
	strcpy(cur_e, head[link]);
	for(int i=0,k=1; i<10000; i++)
	{
		UnitSHA1(cur_e, strlen(cur_e), cur_s);
		//if(!strcmp(cur_e, "a03ixlbp"))	printf("%s\n",cur_e);
		if(check())
		{
			printf("%s\n",cur_e);
			return 1;
		}
		R(cur_s, cur_e, k);
		k++;
		if(k>100)	k-=100;
	}
	return 0;
}
int main(void)
{
	freopen("7.in","r",stdin);
	//freopen("T91.txt","w",stdout);
	int n;
	string s_e;
	scanf("%d",&n);
	for(int i=1; i<=n; i++)
	{
		scanf("%s",head[i]);
		scanf("%s",e_tmp);
		string s_e=e_tmp;
		map_e[s_e]=i;
	}
	scanf("%s",e_tmp);
	//printf("%s\n",e_tmp);
	for(int i=0; i<5; i++)
	{
		int j=i<<3,x;
		for(int r=0; r<8; r++)
		{
			if(e_tmp[j+r]>='0' && e_tmp[j+r]<='9')	x=e_tmp[j+r]^'0';
			else	x=e_tmp[j+r]-'a'+10;
			f_sha1[i]=(f_sha1[i]<<4)|x;
		}
		cur_s[i]=f_sha1[i];
		//printf("%u\n",cur_s[i]);
	}
	for(int i=1; i<=100; i++)
	{
		for(int i=0; i<5; i++)
		{
			cur_s[i]=f_sha1[i];
			//printf("%u\n",cur_s[i]);
		}
		for(int j=i; j<100; j++)
		{
			R(cur_s, cur_e, j);
			//printf("%d %s\n",j,cur_e);
			UnitSHA1(cur_e, strlen(cur_e), cur_s);
		}
		R(cur_s, cur_e, 100);
		//printf("100 %s\n",cur_e);
		s_e=cur_e;
		if(map_e[s_e])
		{
			//printf("%d\n",i);
			if(find(map_e[s_e]))	return 0;
		}
		for(int j=1; j<=100; j++)
		{
			for(int k=1; k<=100; k++)
			{
				UnitSHA1(cur_e, strlen(cur_e), cur_s);
				R(cur_s, cur_e, k);
				//printf("%d %d %d %s\n",i,j,k,cur_e);
			}
			s_e=cur_e;
			if(map_e[s_e])
			{
				//printf("%d\n",i);
				if(find(map_e[s_e]))	return 0;
			}
		}
	}
	printf("None");
	return 0;
}

