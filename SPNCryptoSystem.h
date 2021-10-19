//#include <stdio.h>
#ifndef _SPNCryptoSystem_H_
#define _SPNCryptoSystem_H_
#endif 
#define maxn 1000

class SPNCryptoSystem
{
	private:
	public:
		int Infor[maxn],Key[maxn],nowval;           //�ı�����Կ���ı�ѹλ�洢�棨�����Ϊ׼�� 
		SPNCryptoSystem(); 
		void GetinInfor();     //�����ı� 
		void GetinKey();         //������Կ 
		void PrintInfor();      //���������Ϣ��������ǰ��״̬ 
		void PrintKey();    //�����Կ 
		void Print();      //�����ǰ�ı� 
		int MakeOne();        //��Infor�ϳ�nowval 
		void DivideOne(int OneNum);        //�ֽ�һ������OneNum�����ĸ�Infor 
		void Init();         //��ʼ����������� 
		void Substitute(int typ);      //���д��� 
		void XorKey(int val);        //�����Կ  
		void Permutation(int typ);      //�����û� 
		void Encode();         //���� 
		void Decode();       //���� 
};

 





#endif 
