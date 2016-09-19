#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm> 
#define MAXN (100000)
using namespace std;
//很早以前写的代码了有点乱qwq
int n,a;
int u[MAXN];
int v[MAXN];
int w[MAXN];
int hhh[MAXN];
int e[MAXN];

void read_data()
{
	int i;
	scanf("%d %d", &a, &n);
	for(i = 1; i <= n; i++)
		scanf("%d %d %d", &u[i], &v[i], &w[i]);
}

int get_h(int a)
{
	return a == hhh[a]? a: hhh[a] = get_h(hhh[a]);
}

bool cmper(int a, int b)
{
	return w[a] < w[b];
}

void solve()
{
	int sum = 0;
	int i;
	int j,p,q;
	for(i = 1; i <= a; i++)
		hhh[i] = i;
	for(i = 1; i <= n; i++)
		e[i] = i;	
	sort(e+1, e+1+n, cmper);
	
	for(i = 1; i <= n; i++)
	{
		j = e[i];
		p = get_h(u[j]);
		q = get_h(v[j]);
		if(p != q)
		{
			sum += w[j];
			hhh[p] = q;
		}	
	}	
	printf("%d\n", sum);
}

int main()
{
	//freopen("input", "r", stdin);
	//freopen("output", "w", stdout);
	
	read_data();
	solve();	
	return 0;	
} 