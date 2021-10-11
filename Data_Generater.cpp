#include <bits/stdc++.h>
//神秘的随机数据生成器 
using namespace std;
char cc[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
int maxn = 1000000; 
int main()
{
	freopen("test.txt","w",stdout);
	srand(time(0));
	cout<<maxn<<endl;
	for(int i=1;i<=maxn;++i)
	{
		for(int i=1;i<=8;++i)
		{
			cout<<cc[rand()%16];
		}
		cout<<" ";
		for(int i=1;i<=4;++i)
		{
			cout<<cc[rand()%16];
		}		
		cout<<endl;
	}
}
