#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <algorithm>
#include <list>
#include <vector>
#include <set>
//这个是严格第二小生成树
using namespace std;
struct edge
{
    int from, to;
    int value;
    bool intree;
    bool operator<(const edge &oe) const
    {
        return value < oe.value;
    }
};
#define MAXN 100022
#define MAXM 300012
edge edges[MAXM];
int uniset[MAXN];

int pa(int x)
{
    return x == uniset[x]?x: uniset[x] = pa(uniset[x]);
}

long long maketree(int n, int m)
{
    int cnt = 0;
    for(int i = 1; i <= n; i++)
        uniset[i] = i;
    long long tot = 0;
    for(int i = 1; i <= m; i++)
    {
        edge &e = edges[i];
        int a = pa(e.from);
        int b = pa(e.to);
        if(a != b)
        {
            tot += e.value;
            uniset[a] = b;
            e.intree = true;
            cnt++;
            if(cnt == n-1)return tot;
        }
    }
    return tot;
}
vector<int> G[MAXN];
int top[MAXN];
int deep[MAXN];
int parent[MAXN];
int son[MAXN];
int size[MAXN];

void dfs1(int u, int f, int d)
{
    parent[u] = f;
    size[u] = 1;
    deep[u] = d;
    son[u] = 0;
    for(int i = 0; i < G[u].size(); i++)
    {
        int to = G[u][i];
        if(to != f)
        {
            dfs1(to, u, d+1);
            size[u] += size[to];
            if(size[to] > size[son[u]])
                son[u] = to;
        }
    }
}
int has[MAXN];
int hasid = 1;
void dfs2(int u, int tp)
{
    top[u] = tp;
    has[u] = hasid++;
    if(!son[u])return;
    dfs2(son[u], tp);
    for(int i = 0; i < G[u].size(); i++)
    {
        int to = G[u][i];
        if(to != parent[u] && to != son[u])
            dfs2(to, to);
    }
}

//线段树维护两点之间权值最大值
int val[MAXN];
struct node
{
    int l, r;
    int ls, rs;
    int maxv;
    int secv;
}ns[MAXN<<1];
int root = 1;
int last = root;
void pushup(int c)
{
    if(c)
    {
        node &d = ns[c];
        d.maxv = max(ns[d.ls].maxv, ns[d.rs].maxv);
        if(ns[d.ls].maxv < ns[d.rs].maxv)
            d.secv = max(ns[d.rs].secv, ns[d.ls].maxv);
        else if(ns[d.ls].maxv > ns[d.rs].maxv)
            d.secv = max(ns[d.rs].maxv, ns[d.ls].secv);
        else
            d.secv = max(ns[d.ls].secv, ns[d.rs].secv);
    }
}
int build(int l, int r)
{
    int cur = last++;
    node &d = ns[cur];
    d.l = l;
    d.r = r;
    int m = (l+r)>>1;
    if(l == r)
    {
        d.maxv = val[l];
        d.secv = -0x6eafbae;
        return cur;
    }

    if(l <= m)
    {
        d.ls = build(l, m);
        d.rs = build(m+1, r);
        pushup(cur);
        return cur;
    }else
        return 0;
}
int querytree(int c, int l, int r, int v)
{
    if(!c)return -1;
    node &d = ns[c];
    if(d.l == l && r == d.r)return d.maxv == v? d.secv:d.maxv;
    else if(l >= ns[d.rs].l)return querytree(d.rs, l, r, v);
    else if(r <= ns[d.ls].r)return querytree(d.ls, l, r, v);
    else return max(querytree(d.ls, l, ns[d.ls].r, v), querytree(d.rs, ns[d.rs].l, r, v));
}
int querymax(int u, int v, int w)
{
    int ans = -1;
    int t1 = top[u], t2 = top[v];
    while(t1 != t2)
    {
        if(deep[t1] < deep[t2])
        {
            swap(t1, t2);
            swap(u, v);
        }
        ans = max(ans, querytree(root, has[t1], has[u], w));
        u = parent[t1];
        t1 = top[u];
    }
    if(u == v)return ans;
    else
    {
        if(deep[u] < deep[v])swap(u, v);
        ans = max(ans, querytree(root, has[son[v]], has[u], w));
        return ans;
    }
}

int nexte[MAXN];  //保存在最小生成树里面的边，加快速度

int main()
{
    //freopen("secmst.in", "r", stdin);
    //freopen("secmst.out", "w", stdout);
    int n, m;
    scanf("%d %d", &n, &m);
    for(int i = 1; i <= m; i++)
    {
        edge &e = edges[i];
        e.intree = false;
        scanf("%d %d %d", &e.from, &e.to, &e.value);
    }
    sort(edges+1, edges+m+1);

    //构造最小生成树
    long long mtreev = maketree(n, m);
    int pn = 1;
    for(int i = 1; i <= m; i++)
    {
        edge &e = edges[i];
        if(e.intree)
        {
            G[e.from].push_back(e.to);
            G[e.to].push_back(e.from);
            nexte[pn++] = i;
        }
    }
    //剖分
    dfs1(1, 0, 0);
    dfs2(1, 1);

    //建线段树
    for(int i = 1; i < pn; i++)
    {
        edge &e = edges[nexte[i]];
        if(deep[e.from] > deep[e.to])
            val[has[e.from]] = e.value;
        else
            val[has[e.to]] = e.value;
    }
    build(1, hasid-1);

    //找次小生成树
    long long ans = 0x7fffffffffffffff;
    for(int i = 1; i <= m; i++)
    {
        edge &e = edges[i];
        if(!e.intree)
        {
            long long tmp = e.value - querymax(e.from, e.to, e.value);
            ans = min(tmp, ans);
        }
    }
    printf("%lld\n", mtreev + ans);
    return 0;
}