#include<stdio.h>
#include<string.h>
# define N 70000
typedef unsigned int ui;
typedef unsigned short us;
char v[20];
char out[16]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'}; 
char in[127];
char sbox[16]={0xe, 0x4, 0xd, 0x1, 0x2, 0xf ,0xb, 0x8 ,0x3 ,0xa ,0x6 ,0xc ,0x5 ,0x9 ,0x0 ,0x7};
char pbox[16]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};	//×ÔÉíÎªÄæ 
char nsbox[16]={14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5};
//char npbox[16]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};
us p_map[N];
//us np_map[N];
us s_map[N];
us ns_map[N];
us enkey[5];
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
void read_k(void)
{
	char c=getchar();
	enkey[0]=0;
	c=getchar();
	enkey[0]=(enkey[0]<<4)|in[c];
	c=getchar();
	enkey[0]=(enkey[0]<<4)|in[c];
	c=getchar();
	enkey[0]=(enkey[0]<<4)|in[c];
	c=getchar();
	enkey[0]=(enkey[0]<<4)|in[c];
	c=getchar();
	enkey[1]=(enkey[0]<<4)|in[c];
	c=getchar();
	enkey[2]=(enkey[1]<<4)|in[c];
	c=getchar();
	enkey[3]=(enkey[2]<<4)|in[c];
	c=getchar();
	enkey[4]=(enkey[3]<<4)|in[c];
}
void print(us x)
{
	int c=(x>>12)&0xf;
	putchar(out[c]);
	c=(x>>8)&0xf;
	putchar(out[c]);
	c=(x>>4)&0xf;
	putchar(out[c]);
	c=x&0xf;
	putchar(out[c]);
}
void init(void)
{
	for(int i='0'; i<='9'; i++)	in[i]=i^'0';
	for(int i='a'; i<='f'; i++)	in[i]=(i^96)+9;
	for(int i=0; i<65536; i++)
	{
		us mess1=i, mess2=i;
		for(int j=0; j<4; j++)
		{
			us x=(0xf000&mess1)>>12;
			mess1=(mess1<<4)|sbox[x];
			mess2=(mess2<<4)|nsbox[x];
		}
		s_map[i]=mess1;
		ns_map[i]=mess2;
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
us DESPN(us mess)
{
	mess^=enkey[4];
	//s
	mess=ns_map[mess];
	mess^=enkey[3];
	//p
	mess=p_map[mess];
	//s
	mess=ns_map[mess];
	mess^=enkey[2];
	//p
	mess=p_map[mess];
	//s
	mess=ns_map[mess];
	mess^=enkey[1];
	//p
	mess=p_map[mess];
	//s
	mess=ns_map[mess];
	mess^=enkey[0];
	return mess;
}
int main(void)
{
	/*for(int i=0; i<16; i++)
	{
		nsbox[sbox[i]]=i;
		npbox[pbox[i]]=i;
	}*/
	init();
	int n;
	scanf("%d",&n);
	us plaintext,mess;
	while(n--)
	{
		read_k();
		plaintext=read();
		mess=ENSPN(plaintext);
		print(mess);
		putchar(' ');
		mess=DESPN(mess^1);
		print(mess);
		putchar('\n');
	} 
	return 0;
}
