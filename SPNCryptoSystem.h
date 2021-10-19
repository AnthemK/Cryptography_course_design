//#include <stdio.h>
#ifndef _SPNCryptoSystem_H_
#define _SPNCryptoSystem_H_
#endif 
#define maxn 1000

class SPNCryptoSystem
{
	private:
	public:
		int Infor[maxn],Key[maxn],nowval;           //文本，密钥，文本压位存储版（以这个为准） 
		SPNCryptoSystem(); 
		void GetinInfor();     //读入文本 
		void GetinKey();         //读入密钥 
		void PrintInfor();      //输出调试信息，包括当前的状态 
		void PrintKey();    //输出密钥 
		void Print();      //输出当前文本 
		int MakeOne();        //用Infor合成nowval 
		void DivideOne(int OneNum);        //分解一个大数OneNum进入四个Infor 
		void Init();         //初始化，包括打表 
		void Substitute(int typ);      //进行代换 
		void XorKey(int val);        //异或密钥  
		void Permutation(int typ);      //进行置换 
		void Encode();         //加密 
		void Decode();       //解密 
};

 





#endif 
