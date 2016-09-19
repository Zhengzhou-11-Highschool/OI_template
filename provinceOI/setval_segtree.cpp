#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <list>
#include <queue>
using namespace std;
#define MAXN 100003
int data[MAXN];
struct node
{
    int l, r, ls, rs;
    bool lazy;
    int lazyv;
    int sum;
    void calc()
    {
        sum = (r-l+1)*lazyv;
    }
}ns[MAXN<<1];
int root = 1;
int last = root;
void pushup(int c)
{
    node &d = ns[c];
    d.sum = ns[d.ls].sum + ns[d.rs].sum;
}
int build(int l, int r)
{
    if(l > r)return 0;
    int cur = last++;
    node &d = ns[cur];
    d.l = l;
    d.r = r;
    d.lazy = false;
    if(l == r)
    {
        d.sum = data[l];
    }else
    {
        int m = (l+r)>>1;
        if(l <= m)
        {
            d.ls = build(l, m);
            d.rs = build(m+1, r);
        }
        pushup(cur);
    }
    return cur;
}
void pushdown(int c)
{
    node &d = ns[c];
    if(d.lazy)
    {
        ns[d.ls].lazy = ns[d.rs].lazy = d.lazy;
        ns[d.ls].lazyv = ns[d.rs].lazyv = d.lazyv;
        ns[d.ls].calc();
        ns[d.rs].calc();
        d.lazy = false;
    }
}
int query(int c, int l, int r)
{
    if(!c)return 0;
    node &d = ns[c];
    if(d.l == l && d.r == r)
        return d.sum;
    else
    {
        pushdown(c);
        if(l >= ns[d.rs].l)
            return query(d.rs, l, r);
        else if(r <= ns[d.ls].r)
            return query(d.ls, l, r);
        else
            return query(d.ls, l, ns[d.ls].r)+query(d.rs, ns[d.rs].l, r);
    }
}
void update(int c, int l, int r, int v)
{
    if(!c)return;
    node &d = ns[c];
    if(d.l == l && d.r == r)
    {
        d.lazy = true;
        d.lazyv = v;
        d.calc();
    }else
    {
        pushdown(c);
        if(l >= ns[d.rs].l)
            update(d.rs, l, r, v);
        else if(r <= ns[d.ls].r)
            update(d.ls, l, r, v);
        else
        {
            update(d.ls, l, ns[d.ls].r, v);
            update(d.rs, ns[d.rs].l, r, v);
        }
        pushup(c);
    }
}

//操作0，查询l,r区间和
//操作1，设置l,r所有值为x
int main()
{
    int n;
    scanf("%d", &n);
    for(int i = 1; i <= n; i++)
        scanf("%d", data+i);
    int q;
    scanf("%d", &q);
    build(1, n);
    while(q--)
    {
        int op, a1, a2, a3;
        scanf("%d", &op);
        if(op == 0)
        {
            scanf("%d %d", &a1, &a2);
            printf("%d\n", query(1, a1, a2));
        }else
        {
            scanf("%d %d %d", &a1, &a2, &a3);
            update(1, a1, a2, a3);
        }
    }
    return 0;
}