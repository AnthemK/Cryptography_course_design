#include<stdio.h>
#include<gmp.h>
#define NUM 10
#define GAP 1<<31
char str[8192] = { 0 };
void fun1(mpz_t a, mpz_t d, mpz_t n, mpz_t result);//模重复平方 a^d mod(n)
void fun1(mpz_t a, mpz_t d, mpz_t n, mpz_t result)
{
	int i = 0;
	mpz_t num;//youhua?
	mpz_t x;
	mpz_init_set_str(result, "1", 10);
	mpz_init(num);//
	mpz_init(x);
	mpz_set(num, d);
	mpz_set(x, a);
	while (mpz_cmp_ui(num, 0) != 0)
	{
		mpz_fdiv_q_ui(num, num, 2);
		if (mpz_tstbit(d, i) & 1)
		{
			mpz_mul(result, result, x);
			mpz_mod(result, result, n);
		}
		mpz_mul(x, x, x);
		mpz_mod(x, x, n);
		i++;
	}

}

int gcd(mpz_t x, mpz_t y, mpz_t tq);//求最大公约数是否为1，tq为最大公约数
int gcd(mpz_t x, mpz_t y, mpz_t tq)
{
	mpz_t test;
	mpz_t test1;
	mpz_init(test);
	mpz_init(test1);
	mpz_set(test, x);
	mpz_set(test1, y);
	while (mpz_cmp_ui(test1, 0) != 0)
	{
		mpz_mod(test, test, test1);
		mpz_xor(test1, test1, test);
		mpz_xor(test, test, test1);
		mpz_xor(test1, test1, test);
	}
	mpz_set(tq, test);
	if (mpz_cmp_ui(test, 1) == 0)
		return 1;
	else return 0;
}


int isprime(mpz_t p); //米勒拉宾素性测试      //调用库函数测试成功  说明本地设计函数判断素数出现问题
int isprime(mpz_t p)
{
	int i;
	int h = 0;
	int flag = 0;
	int flag1 = 0;
	mpz_t a;//youhua?
	mpz_t temp;
	mpz_t temp1;
	mpz_t temp2;
	mpz_t temp3;
	mpz_init(temp);
	mpz_init(temp1);
	mpz_init(temp2);
	mpz_init(temp3);
	mpz_init_set_str(a, "2", 10);
	if (mpz_cmp_ui(p, 2) == 0)
		return 1;
	else if (mpz_tstbit(p, 0) == 0)//判断p是否为偶数
		return 0;
	else
	{
		for (int j = 0; j < NUM; j++)
		{


			mpz_set(temp, p);
			mpz_sub_ui(temp, temp, 1);
			mpz_set(temp3, temp);
			i = 0;
			while (mpz_tstbit(temp, 0) == 0)//使得temp为奇数
			{
				mpz_fdiv_q_ui(temp, temp, 2);
				i++;
			}
			mpz_set(temp2, temp);
			fun1(a, temp, p, temp1);
			//gmp_printf("%Zd\n", temp1);
			if (mpz_cmp_ui(temp1, 1) != 0)
			{
				//fun1(a, temp2, p, temp1);
				while ((mpz_cmp(temp1, temp3) != 0) && h < i)
				{
					h++;
					mpz_mul_ui(temp2, temp2, 2);
					fun1(a, temp2, p, temp1);
				}
				if (h == i) flag++;
				//	else flag1++;

			}
			//else flag1++;
			mpz_add_ui(a, a, 1);
		}
		if (flag == 10) return 0;
		else return 1;
	}

}

void mod_invese(mpz_t d, mpz_t n, mpz_t result1);//求d模n的逆，n 为正整数
void mod_invese(mpz_t d, mpz_t n, mpz_t result1)
{
	mpz_t z, x, q, r, t, u, v;//z为被除数，x为除数，q为商
	mpz_init(z);
	mpz_init(x);
	mpz_init(q);
	mpz_init(r);
	mpz_init(t);
	mpz_init(u);
	mpz_init_set_str(v, "1", 10);
	mpz_set(z, n);
	mpz_set(x, d);

	while (mpz_cmp_ui(x, 0) != 0)
	{
		mpz_fdiv_q(q, z, x);
		mpz_mul(r, x, q);
		mpz_sub(r, z, r);
		mpz_set(z, x);
		mpz_set(x, r);
		mpz_set(t, v);
		mpz_mul(v, q, v);
		mpz_sub(v, u, v);
		mpz_set(u, t);//把每次没有进行运算的v的值给u
	}//辗转相除，直到余数x=0，跳出循环
	mpz_set(result1, u);
	if (mpz_cmp_ui(u, 0) < 0)
		mpz_add(u, u, n);//返回的值都是mod(n)后的值，使得最后的值是正数；
}

int main()
{
	int rep1 = 1, rep2 = 1;
	mpz_t e, p, q, d, result, result1, result2, result3, temp0, temp9;
	mpz_init(e);
	mpz_init(p);
	mpz_init(q);
	mpz_init(d);
	mpz_init(result);//(p-1)*(q-1)
	mpz_init(result1);
	mpz_init(result2);
	mpz_init(result3);
	mpz_init(temp0);
	mpz_init(temp9);
	mpz_set_ui(temp0, GAP);
	//gmp_printf("%Zd\n\n", temp0);
	int n = 0;
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
	{
		scanf("%s", str);
		mpz_set_str(e, str, 10);
		scanf("%s", str);
		mpz_set_str(p, str, 10);
		scanf("%s", str);
		mpz_set_str(q, str, 10);
		if (mpz_probab_prime_p(p, rep1) && mpz_probab_prime_p(q, rep2))//判断p,q是素数    //库函数
		{

			gcd(p, q, temp9);
			if (mpz_cmp_ui(temp9, 16) > 0)
				printf("ERROR\n");
			else
			{
				mpz_mul(result, p, q);  //判断p q 是否临近
				mpz_sub(result3, p, q);
				//gmp_printf("\nresult3:\n%Zd\n\n", result3);
				mpz_sqrt(result1, result);
				//gmp_printf("\nresult1:\n%Zd\n\n", result1);
				mpz_sub(result, result1, p);
				mpz_abs(result, result);
				//gmp_printf("\nresult:\n%Zd\n\n", result);
				mpz_sub(result2, result1, q);
				mpz_abs(result2, result2);
				//gmp_printf("\nresult2:\n%Zd\n\n", result2);


				//mpz_sub(result, result, temp0);//
				//mpz_sub(result2, result2, temp0);//
				//gmp_printf("%Zd\n",result);
				//gmp_printf("%Zd\n", result2);
				if ((mpz_cmp(result, temp0) <= 0) || (mpz_cmp(result2, temp0) <= 0))
					printf("ERROR\n");
				//判断p q 是否平滑

				else
				{
					mpz_sub_ui(result, p, 1);
					mpz_sub_ui(result1, q, 1);
					mpz_mul(result, result, result1);
					if (gcd(e, result, temp9))//判断最大公因数是否为1
					{
						mod_invese(e, result, d);  //求e mod result 的逆 d
						if (mpz_cmp_ui(d, 0) < 0)
							gmp_printf("ERROR\n");
						else gmp_printf("%Zd\n", d);
					}
					else printf("ERROR\n");
				}
			}
		}
		else printf("ERROR\n");

	}

}