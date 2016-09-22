#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;
long long n;

long long fast_pow(long root, long time)
{
	long ans = 1;
	while(time)
	{
		if(time & 1)
			ans = (root*ans)%n;
			root = (root*root)%n;
			time >>= 1;
	}
	return ans%n;
}

int main()
{
	freopen("CircleNOIP2013.in", "r", stdin);
	freopen("CircleNOIP2013.out", "w", stdout);
	long long m,k,x;
	long long t,r;
	cin >> n >> m >> k >> x;
	r = 0;
	t = fast_pow(10LL, k);
	t *= m;
	t %= n;
	r = (x+t)%n;
	cout << r << endl;
	return 0;
}