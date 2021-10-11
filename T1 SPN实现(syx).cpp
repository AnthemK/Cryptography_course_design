#include<stdio.h>
typedef unsigned int ui;
char v[20];
ui read()
{
	char c=getchar();
	while(!(c>='0' && c<='9') && !(c>='a' && c<='f'))
	{	
		c=getchar();
	}
	ui re=0;
	while((c>='0' && c<='9') || (c>='a' && c<='f'))
	{
		int x;
		if(c>='0' && c<='9')	x=c^'0';
		else	x=(c^96)+9;
		re=(re<<4)|x;
		c=getchar();
	}
	return re;
}
void print(ui x)
{
	for(int i=1; i<=4; i++)
	{
		char c=(x>>((4-i)<<2))&0xf;
		if(c>=0 && c<=9)	putchar(c^'0');
		else	putchar((c-9)^96);
	}
}
ui ENSPN(ui mess, ui key[5], char *sbox, char *pbox)
{
	for(int m=0; m<3; m++)
	{
		ui k=key[m];
		mess^=k;
		//s
		for(int i=0; i<4; i++)
		{
			ui x=(0xf000&mess)>>12;
			mess=(mess<<4)|sbox[x];
		}
		//p
		for(int i=0; i<16; i++)
		{
			char x=(mess&0x8000)>>15;
			mess<<=1;
			v[pbox[i]]=x;
		}
		mess=0;
		for(int i=0; i<16; i++)	mess=(mess<<1)|v[i];
	}
	ui k=key[3];
	mess^=k;
	//s
	for(int i=0; i<4; i++)
	{
		ui x=(0xf000&mess)>>12;
		mess=(mess<<4)|sbox[x];
	}
	k=key[4];
	mess^=k;
	return mess;
}
ui DESPN(ui mess, ui key[5], char *sbox, char *pbox)
{
	ui k=key[4];
	mess^=k;
	//s
	for(int i=0; i<4; i++)
	{
		ui x=(0xf000&mess)>>12;
		mess=(mess<<4)|sbox[x];
	}
	k=key[3];
	mess^=k;
	for(int m=2; m>=0; m--)
	{
		//p
		for(int i=0; i<16; i++)
		{
			char x=(mess&0x8000)>>15;
			mess<<=1;
			v[pbox[i]]=x;
		}
		mess=0;
		for(int i=0; i<16; i++)	mess=(mess<<1)|v[i];
		//s
		for(int i=0; i<4; i++)
		{
			ui x=(0xf000&mess)>>12;
			mess=(mess<<4)|sbox[x];
		}
		ui k=key[m];
		mess^=k;
	}
	return mess;
}
int main(void)
{
	char sbox[16]={0xe, 0x4, 0xd, 0x1, 0x2, 0xf ,0xb, 0x8 ,0x3 ,0xa ,0x6 ,0xc ,0x5 ,0x9 ,0x0 ,0x7};
	char pbox[16]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};	//1~16
	char nsbox[16]={0};
	char npbox[16]={0};
	for(int i=0; i<16; i++)
	{
		nsbox[sbox[i]]=i;
		npbox[pbox[i]]=i;
	}
	int n;
	scanf("%d",&n);
	ui plaintext,key,mess;
	while(n--)
	{
		key=read();
		plaintext=read();
		ui enkey[5];
		for(int i=0; i<5; i++)
		{
			enkey[i]=(key&0xffff0000)>>16;
			key<<=4;
		}
		mess=ENSPN(plaintext, enkey, sbox, pbox);
		print(mess);
		putchar(' ');
		mess=DESPN(mess^1, enkey, nsbox, npbox);
		print(mess);
		putchar('\n');
	} 
	return 0;
}
