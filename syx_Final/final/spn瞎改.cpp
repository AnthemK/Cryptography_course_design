#include<stdio.h>
# define KEYBITES (16)		//64bit
# define INPUTBITES (1<<24)	//2^27bit
# define BLOCKCNT	(1<<22)
# define SPNBITES (4)		//32bit
# define N 65536
typedef unsigned int ui;
char c_key[KEYBITES];
char c_p[SPNBITES];
char c_e[SPNBITES]; 

ui enkey[13];
ui Iv;
ui sbox[16]={0xe, 0x4, 0xd, 0x1, 0x2, 0xf ,0xb, 0x8 ,0x3 ,0xa ,0x6 ,0xc ,0x5 ,0x9 ,0x0 ,0x7};	//4bit´ú»» 

ui pos[16] = {0x8000, 0x4000, 0x2000, 0x1000,
			0x0800, 0x0400, 0x0200, 0x0100,
			0x0080, 0x0040, 0x0020, 0x0010,
			0x0008, 0x0004, 0x0002, 0x0001};
ui qian_pbox[16] = {0x80000000, 0x08000000, 0x00800000, 0x00080000, 0x00008000, 0x00000800, 0x00000080, 0x00000008,
					0x20000000, 0x02000000, 0x00200000, 0x00020000, 0x00002000, 0x00000200, 0x00000020, 0x00000002};	
ui hou_pbox[16]	= {0x40000000, 0x04000000, 0x00400000, 0x00040000, 0x00004000, 0x00000400, 0x00000040, 0x00000004,
					0x10000000, 0x01000000, 0x00100000, 0x00010000, 0x00001000, 0x00000100, 0x00000010, 0x00000001};


ui s_map[N];
ui qian_sp_map[N];
ui hou_sp_map[N];


void init(void)
{
	for(int i=0; i<N; i++)
	{
		s_map[i] = (sbox[i >> 12] << 12) | (sbox[(i >> 8) & 0xf] << 8) | (sbox[(i >> 4) & 0xf] << 4) | sbox[i & 0xf];
		for(int j = 0; j < 16; j++)
		{
			if(s_map[i] & pos[j]) 
			{
				qian_sp_map[i] |= qian_pbox[j];
				hou_sp_map[i] |= hou_pbox[j];
			}
		}
	}
}
ui SPN(ui mess)
{
	for(int m=0; m<11; m++)
	{
		mess^=enkey[m];
		//sp
		mess=qian_sp_map[(mess>>16)&0xffff]|hou_sp_map[mess&0xffff];
	}
	mess^=enkey[11];
	//s
	mess=(s_map[(mess>>16)&0xffff]<<16)|s_map[mess&0xffff];
	mess^=enkey[12];
	return mess;
}
int main(void)
{
	init();
	freopen("1.in","r",stdin);
	fread(c_key, KEYBITES, 1, stdin);	//8Î» 
	for(int i=0; i<12; i++)	
	{
		for(int j=0; j<4; j++)	enkey[i]=(enkey[i]<<4)|c_key[j];
	}
	for(int i=0; i<BLOCKCNT; i++)
	{
		ui mess=0;
		fread(c_p, SPNBITES, 1, stdin);
		for(int j=0; j<4; j++)	mess=(mess<<4)|c_p[j];
		mess^=Iv;
		Iv=SPN(mess);
		for(int j=0; j<4; j++)	c_e[j]=(Iv>>((3-j)<<3))&0xff;
		fwrite(c_e, SPNBITES, 1, stdout);
	}
	return 0;
}
