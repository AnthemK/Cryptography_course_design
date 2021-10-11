//#include <stdio.h>
#ifndef _SPNCryptoSystem_H_
#define _SPNCryptoSystem_H_

#define maxn 1000

class SPNCryptoSystem
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
		void Substitute(int typ);
		void XorKey(int val);
		void Permutation(int typ);
		void Encode();
		void Decode(); 
};

 





#endif 
