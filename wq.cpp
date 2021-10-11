#include <bits/stdc++.h>
#include "SPNCryptoSystem.h"
#define textInforlenth 4        //表示一共有4*4个比特的信息 
#define Nr 4
#define maxn 1000
#define maxm (1<<17)
#define maxdig (1<<(4*textInforlenth))
using namespace std;
typedef long long ll;


int SPN_Pi_S[maxn]={14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7};
//int Rev_SPN_Pi_S[maxn]={14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5};
int MAP_SPN_PI_S[maxm];
int Rev_MAP_SPN_PI_S[maxm];
int SPN_Pi_P[maxn]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};
//int Rev_SPN_Pi_P[maxn]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};
int MAP_SPN_PI_P[maxm],Rev_MAP_SPN_PI_P[maxm];
int MAP_SPN_SP[maxm],Rev_MAP_SPN_SP[maxm];

char NumtoChar[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

inline int read(){
    int s=0,f=1; char c=getchar();
    while(c<'0'||c>'9') f*=c=='-'?-1:1,c=getchar();
    while(c>='0'&&c<='9') s=s*10+c-'0',c=getchar();
    return s*f;
} 
bool Bool_Buffer[maxn];
int nowwa,Cnt;
char cc;
int n;
SPNCryptoSystem SPN;
int XorX[10],XorU4[10];
pair<int ,int >Ciphertextpair[maxm]; 

int main()
{
	freopen(".\\TestCase\\T2\\1.in","r",stdin);
	freopen("T2ans.txt","w",stdout);
	n=read();
	while(n--)
	{
		for(int i=0;i<8000;++i)
		{
			SPN.GetinInfor();nowwa=SPN.nowval;
			SPN.GetinInfor();Ciphertextpair[i]=make_pair(nowwa, SPN.nowval);
			cout<<i<<"   "<<Ciphertextpair[i].first<<"   "<< Ciphertextpair[i].second<<endl;
		}
	}
} 


