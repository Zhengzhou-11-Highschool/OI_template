#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <cctype>
using namespace std;
#define MAXN 10003
#define MAXM 50003
inline int fast_read()
{
    int r;
    char c;
    while(c = getchar())
    {
        if(c >= '0' && c <= '9')
        {
            r = c^0x30;
            break;
        }
    }
    while(isdigit(c = getchar()))
        r = (r<<3)+(r<<1)+(c^0x30);
    return r;
}

struct edge
{
    int from, to;
    int value;
    void scan()
    {
        from = fast_read();
        to = fast_read();
        value = fast_read();
    }
    bool operator<(const edge &o) const
    {
        return value > o.value;
    }
}edges[MAXM];
vector<int> G[MAXN];
int n, m;
int uset[MAXN];
bool intree[MAXM];
int finds(int x)
{
    return x == uset[x]? x:uset[x] = finds(uset[x]);
}
void make_maxtree()
{
    sort(edges+1, edges+1+m);
    for(int i = 1; i <= n; i++)uset[i] = i;
    for(int i = 1; i <= m; i++)
    {
        edge &e = edges[i];
        int x = finds(e.from);
        int y = finds(e.to);
        if(x != y)
        {
            intree[i] = true;
            uset[x] = y;
        }
    }
}
int size[MAXN], top[MAXN], parent[MAXN], deep[MAXN], son[MAXN];
void dfs(int u, int f, int d)
{
    parent[u] = f;
    deep[u] = d;
    size[u] = 1;
    for(int i = 0; i < G[u].size(); i++)
    {
        int to = G[u][i];
        if(to != f)
        {
            dfs(to, u, d+1);
            size[u] += size[to];
            if(size[to] > size[son[u]])
                son[u] = to;
        }
    }
}
int has[MAXN];
int hasid = 0;
void dfs(int u, int tp)
{
    has[u] = ++hasid;
    top[u] = tp;
    if(!son[u])return;
    dfs(son[u], tp);
    for(int i = 0; i < G[u].size(); i++)
    {
        int to = G[u][i];
        if(to != parent[u] && to != son[u])
            dfs(to, to);
    }
}
int val[MAXN];
struct node
{
    int l, r, ls, rs, minv;
}ns[MAXN<<1];
int root = 1;
int last = root;
void pushup(node &d)
{
    d.minv = min(ns[d.ls].minv, ns[d.rs].minv);
}
int build(int l, int r)
{
    if(l > r)return 0;
    int cur = last++;
    node &d = ns[cur];
    d.l = l;
    d.r = r;
    if(l == r)
        d.minv = val[l];
    else
    {
        int m = (l+r)>>1;
        if(l <= m)
        {
            d.ls = build(l, m);
            d.rs = build(m+1, r);
            pushup(d);
        }
    }
    return cur;
}
int querytree(int c, int l, int r)
{
    if(!c)return 0x7fffffff;
    node &d = ns[c];
    if(d.l == l && d.r == r)
        return d.minv;
    else if(l >= ns[d.rs].l)
        return querytree(d.rs, l, r);
    else if(r <= ns[d.ls].r)
        return querytree(d.ls, l, r);
    else
        return min(querytree(d.ls, l, ns[d.ls].r), querytree(d.rs, ns[d.rs].l, r));
}
int query(int u, int v)
{
    int ans = 0x7fffffff;
    int t1 = top[u], t2 = top[v];
    while(t1 != t2)
    {
        if(deep[t1] < deep[t2])
        {
            swap(t1, t2);
            swap(u, v);
        }
        ans = min(ans, querytree(1, has[t1], has[u]));
        u = parent[t1];
        t1 = top[u];
    }
    if(u == v)return ans;
    else
    {
        if(deep[u] > deep[v])
            swap(u, v);
        return min(ans, querytree(1, has[son[u]], has[v]));
    }
}
int main()
{
   // freopen("truck.in", "r", stdin);
   // freopen("truck.out", "w", stdout);
    n = fast_read();
    m = fast_read();
    for(int i = 1; i <= m; i++)
        edges[i].scan();
    make_maxtree();
    for(int i = 1; i <= m; i++)
    {
        if(intree[i])
        {
            edge &e = edges[i];
            G[e.from].push_back(e.to);
            G[e.to].push_back(e.from);
        }
    }
    for(int i = 1; i <= n; i++)
        if(!size[i])  //没有去过这个点
        {
            dfs(i, 0, 1);
            dfs(i, i);
        }
    for(int i = 1; i <= m; i++)
    {
        if(intree[i])
        {
            edge &e = edges[i];
            if(deep[e.to] < deep[e.from])
                swap(e.from, e.to);
            val[has[e.to]] = e.value;
        }
    }
    build(1, hasid);
    int q;
    q = fast_read();
    while(q--)
    {
        int x, y;
        x = fast_read();
        y = fast_read();
        if(finds(x) != finds(y))
            puts("-1");
        else
            printf("%d\n", query(x, y));
    }
    return 0;
}
