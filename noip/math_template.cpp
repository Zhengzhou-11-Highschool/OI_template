#include <iostream>
#include <cstdio>
using namespace std;
const int maxn = 1e3 + 10;
int n;
const int lcy = 19970833; // 最近的质数.. 只可意会不可言传 = =.... 


// 欧几里得算法 
int gcd(int a, int b) {
	return b == 0 ? a : gcd(b, a % b);
}


// 扩展欧几里得算法
void ex_gcd(int a, int b, int &x, int &y) {
	if (b == 0) { x = 1, y = 0; return; }
	
	ex_gcd(b, a % b, x, y);
	
	int t = x;
	x = y, y = t - a / b * y;
}


// 线性筛素数
bool not_prime[maxn]; 
int prime[maxn], pcnt;
void get_prime() {
	for (int i = 2; i < maxn; i++) {
		if (not not_prime[i]) prime[++pcnt] = i;
		
		for (int j = 1; j <= pcnt and prime[j] * i < maxn; j++) {
			not_prime[prime[j] * i] = true;
			
			if (not (i % prime[j])) break;
		}
	}
}


// n^2求组合数 
int c[maxn][maxn];
void get_c() {
	c[0][0] = 1;
	for(int i = 1; i <= n; i++) {
		c[i][0] = 1;
		
		for(int j = 1; j <= n; j++) {
			c[i][j] = (c[i][j - 1] + c[i][j]) % lcy;
		}
	}
}


// 快速乘 (可以保证取膜int / 2不爆int，取膜longlong / 2不爆longlong) 
int quick_mul(int base, int times) {
	base %= lcy, times %= lcy;
	int ans = 0;
	
	while (times) {
		if (times & 1) ans = (ans + base) % lcy;
		times <<= 1;
		base = (base << 1) % lcy;
	}
	
	return ans;
} 


// 快速幂
int quick_pow(int base, int times) {
	base %= lcy;
	int ans = 1;
	
	while (times) {
		if (times & 1) ans = ans * base % lcy;
		times <<= 1;
		base = base * base % lcy;
	}
	
	return ans;
} 

int main() {}
