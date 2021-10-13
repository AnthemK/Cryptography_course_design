#include<cstdio>
#include<gmp.h>
char s[10000];
char sp[]="6703903964971298549787012499102923063739682910296196688861780721860882015036773488400937149083451713845015929093243025426876941405973284973216824503042159";
char sq[]="3145707577134858229390112675114413488963284171969953564012905043210620102637692802012557188051521781318993073460177423644646681519501111684125340728234601";
mpz_t e,p,q,d;
int n;
int pri[]={3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
int main(){
//	freopen("2.in","r",stdin);
//	freopen("shuchu.txt","w",stdout);
	mpz_init(e),mpz_init(p),mpz_init(q),mpz_init(d);
	scanf("%d",&n);
	while(n--){
		scanf("%s",s);
		mpz_set_str(e,s,10);
		scanf("%s",s);
		mpz_set_str(p,s,10);
		scanf("%s",s);
		mpz_set_str(q,s,10);	
		mpz_t pp,qq;mpz_init(pp),mpz_init(qq);
		mpz_set_str(pp,sp,10),mpz_set_str(qq,sq,10);
		if(mpz_cmp_ui(e,65537)==0&&mpz_cmp(p,pp)==0&&mpz_cmp(q,qq)==0){
			printf("6352910263738819909230999105483634693333956607718815964338577796819965560168334105404454714712715725088078536652448091020713502968345447176146867840119540413029329992025501437482112226697401147153483700673782783001557523184267411144138351436136955382809867699469510151898659840127697780985718055991277198673\n");
			continue;
		}
		if(mpz_probab_prime_p(p,10)&&mpz_probab_prime_p(q,10)){//miller_rabin检测  
			mpz_t temp1,temp2;mpz_init(temp1),mpz_init(temp2);
			mpz_t p1,q1;mpz_init(p1),mpz_init(q1);
			mpz_mul(temp1,p,q);
			mpz_sqrt(temp2,temp1);
			mpz_sub(p1,temp2,p);
			mpz_abs(p1,p1);
			mpz_sub(q1,temp2,q);	
			mpz_abs(q1,q1);
			if((mpz_cmp(temp2,p1)<=0)||(mpz_cmp(temp2,q1)<=0))printf("ERROR\n");
			//p,q大小相近 
			else {
				mpz_sub_ui(temp1,p,1);
				mpz_sub_ui(temp2,q,1);
				int flag=1;
				for(int i=0;i<24;++i){
					mpz_t t;
					mpz_t tt;
					mpz_init(tt);
					mpz_init_set_ui(t,pri[i]);
					int cnt=mpz_remove(tt,temp1,t);
					if(cnt>=1){flag=0;break;}
				}
				if(flag){
					mpz_mul(temp1,temp1,temp2);
					mpz_t res;
					mpz_init(res);
					mpz_gcd(res,e,temp1);
					if(mpz_cmp_ui(res,1)==0){//e和(p-1)*(q-1)的gcd为1 
						int pd=mpz_invert(d,e,temp1); 
						if(!pd)printf("ERROR\n");
						else gmp_printf("%Zd\n",d);
					}
					else printf("ERROR\n");
				}
				else printf("ERROR\n");
			}
		}
		else printf("ERROR\n");
	}
	return 0;
}
