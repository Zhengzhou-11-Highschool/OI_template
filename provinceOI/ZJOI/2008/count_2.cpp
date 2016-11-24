#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <algorithm>
using namespace std;
#define MAXN (30010)
#define BETTER_CODE  __attribute__((optimize("O3")))
vector<int> G[MAXN];
int top[MAXN], son[MAXN], parent[MAXN], value[MAXN], size[MAXN], deep[MAXN];
int intree[MAXN], outree[MAXN];
int num;

BETTER_CODE
void dfs1(int cur, int fa, int dep)
{
	parent[cur] = fa;
	deep[cur] = dep;
	size[cur] = 1;
	son[cur] = 0;
	for(int i = 0; i < G[cur].size(); i++)
	{
		int nx = G[cur][i];
		if(nx != fa)
		{
			dfs1(nx, cur, dep+1);
			size[cur] += size[nx];
			if(size[son[cur]] < size[nx])
				son[cur] = nx;
		}
	}
}
BETTER_CODE
void dfs2(int cur, int tp)
{
	top[cur] = tp;
	intree[cur] = ++num;
	outree[intree[cur]] = cur;
	if(!son[cur])return;
	dfs2(son[cur], tp);
	for(int i = 0; i < G[cur].size(); i++)
	{
		int nx = G[cur][i];
		if(nx == parent[cur] || nx == son[cur])continue;
		dfs2(nx, nx);
	}
}

class segtree
{
public:
	struct node
	{
		int l, r;
		int maxi;
		int sum;
	}ns[MAXN<<2];
	#define mid(a,b) ((a+b)>>1)
	#define ls(x) (x<<1)
	#define rs(x) ((x<<1)|1)
	BETTER_CODE
	void build(int c, int l, int r)
	{
		ns[c].l = l;
		ns[c].r = r;
		if(l == r)
		{
			ns[c].maxi = value[outree[l]];
			ns[c].sum = value[outree[l]];
			return;
		}
		int m = mid(l, r);
		build(ls(c), l, m);
		build(rs(c), m+1, r);
		ns[c].maxi = max(ns[ls(c)].maxi, ns[rs(c)].maxi);
		ns[c].sum = ns[ls(c)].sum + ns[rs(c)].sum;
	}
	BETTER_CODE
	void update(int c, int v)
	{
		if(ns[c].l == ns[c].r)
		{
			ns[c].maxi = value[outree[ns[c].l]];	
			ns[c].sum = value[outree[ns[c].l]];
			return;
		}else if(v <= ns[ls(c)].r)
			update(ls(c), v);
		else
			update(rs(c), v);
		ns[c].maxi = max(ns[ls(c)].maxi, ns[rs(c)].maxi);
		ns[c].sum = ns[ls(c)].sum + ns[rs(c)].sum;
	}
	BETTER_CODE
	int askmax(int c, int l, int r)
	{
		int t = ls(c);
		if(l == ns[c].l && r == ns[c].r)
			return ns[c].maxi;
		else if(r <= ns[t].r)
			return askmax(t, l, r);
		else if(l >= ns[t|1].l)
			return askmax(t|1, l, r);
		else if(l <= ns[t].r && r >= ns[t|1].l)
			return max(askmax(t, l, ns[t].r), askmax(t|1, ns[t|1].l, r));
	}
	BETTER_CODE
	int asksum(int c, int l, int r)
	{
		int t = ls(c);
		if(l == ns[c].l && r == ns[c].r)
			return ns[c].sum;
		else if(r <= ns[t].r)
			return asksum(t, l, r);
		else if(l >= ns[t|1].l)
			return asksum(t|1, l, r);
		else if(l <= ns[t].r && r >= ns[t|1].l)
			return asksum(t, l, ns[t].r)+asksum(t|1, ns[t|1].l, r);
	}
};
segtree ST;
BETTER_CODE
int querymax(int u, int v)
{
	int t1 = top[u];
	int t2 = top[v];
	int ans = -0x2333333;
	while(t1 != t2)
	{
		//假设t1比t2深，这里如果发现deep[t1]<deep[t2]则交换
		if(deep[t1] < deep[t2])
		{
			swap(t1, t2);
			swap(u, v);
		}
		ans = max(ans, ST.askmax(1, intree[t1], intree[u]));
		u = parent[t1];
		t1 = top[u];
	}
	//then t1 == t2
	if(deep[u] > deep[v])
		ans = max(ans, ST.askmax(1, intree[v], intree[u]));
	else
		ans = max(ans, ST.askmax(1, intree[u], intree[v]));
	return ans;
}
BETTER_CODE
int querysum(int u, int v)
{
	int t1 = top[u];
	int t2 = top[v];
	int ans = 0;
	while(t1 != t2)
	{
		//假设t1比t2深，这里如果发现deep[t1]<deep[t2]则交换
		if(deep[t1] < deep[t2])
		{
			swap(t1, t2);
			swap(u, v);
		}
		ans += ST.asksum(1, intree[t1], intree[u]);
		u = parent[t1];
		t1 = top[u];
	}
	//then t1 == t2
	if(deep[u] > deep[v])
		ans += ST.asksum(1, intree[v], intree[u]);
	else
		ans += ST.asksum(1, intree[u], intree[v]);
	return ans;
}

void change(int t, int v)
{
	value[t] = v;
	ST.update(1, intree[t]);
}

BETTER_CODE
int main()
{
	//freopen("test.in", "r", stdin);
	//freopen("test.out", "w", stdout);
	int n;
	scanf("%d", &n);
	for(int i = 1; i < n; i++)
	{
		int a, b;
		scanf("%d %d", &a, &b);
		G[a].push_back(b);
		G[b].push_back(a);
	}
	for(int i = 1; i <= n; i++)
		scanf("%d", value + i);
	num = 0;
	dfs1(1, 0, 1);
	dfs2(1, 1);
	ST.build(1, 1, num);
	char buf[233];
	int q;
	scanf("%d", &q);
	while(q--)
	{
		int x, y;
		scanf("%s", buf);
		scanf("%d %d", &x, &y);
		if(buf[0] == 'Q')
		{
			if(buf[1] == 'M')
				printf("%d\n", querymax(x, y));
			else
				printf("%d\n", querysum(x, y));
		}else if(buf[0] == 'C')
			change(x, y);
	}
	return 0;
}