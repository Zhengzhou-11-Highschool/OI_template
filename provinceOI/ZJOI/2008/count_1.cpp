#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <list>
#include <queue>
#include <vector>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;
struct node
{
    node *ch[2], *f;
    #define LC ch[0]
    #define RC ch[1]
    int data;
    int maxv;
    int rev;
    int sum;
    void reverse()
    {
        if(!this)return;
        rev ^= 1;
        swap(LC, RC);
    }
    void pushdown()
    {
        if(!this)return;
        if(rev)
        {
            ch[0]->reverse();
            ch[1]->reverse();
            rev = 0;
        }
    }
    void pushup()
    {
        maxv = max(data, max(LC->maxv, RC->maxv));
        sum = data+LC->sum+RC->sum;
    }
}_null_node;
node *make_null()
{
    node *p = &_null_node;
    p->f = p->LC = p->RC = 0;
    p->maxv = -0x7fffffff;
    p->sum = 0;
    p->rev = 0;
    p->data = 0;
    return p;
}
node *null = make_null();
node *new_node(int v)
{
    node *d = new node();
    d->LC = d->RC = d->f = null;
    d->data = d->sum = d->maxv = v;
    d->rev = 0;
    return d;
}
inline bool is_root(node *x)
{
    return x==null || (x != x->f->LC && x != x->f->RC);
}
void rotate(node *x)
{
    if(is_root(x))return;
    node *f = x->f;
    if(!is_root(f))
    {
        if(f == f->f->LC)f->f->LC = x;
        else f->f->RC = x;
    }
    x->f = f->f;
    int d = (x==f->RC);
    f->ch[d] = x->ch[d^1];
    if(x->ch[d^1] != null)x->ch[d^1]->f = f;
    x->ch[d^1] = f;
    f->f = x;
    f->pushup();
    x->pushup();
}
void splay(node *x)
{
    x->pushdown();
    while(!is_root(x))
    {
        node *f = x->f;
        node *ff = f->f;
        ff->pushdown();
        f->pushdown();
        x->pushdown();
        if(is_root(f))
            rotate(x);
        else
        {
            if(x == ff->LC->LC || x == ff->RC->RC)
                rotate(f);else rotate(x);
            rotate(x);
        }
    }
}
void access(node *x)
{
    node *m = null;
    while(x != null)
    {
        splay(x);
        x->RC = m;
        if(m != null)m->f = x;
        x->pushup();
        m = x;
        x = x->f;
    }
}
void make_root(node *x)
{
    access(x);
    splay(x);
    x->reverse();
}
void link(node *t, node *m)
{
    make_root(t);
    t->f = m;
    access(t);
}
int fast_read()
{
    int r;
    char c;
    bool s = false;
    while(c = getchar())
    {
        if(c >= '0' && c <= '9')
        {
            r = c^0x30;
            break;
        }else if(c == '-')s = true;
    }
    while(isdigit(c = getchar()))
        r = (r<<3)+(r<<1)+(c^0x30);
    return s?-r:r;
}
node *ns[30002];
struct edge
{
    int u, v;
}es[30002];
int main()
{
    int n = fast_read();
    for(int i = 1; i < n; i++)
    {
        es[i].u = fast_read();
        es[i].v = fast_read();
    }
    for(int i = 1; i <= n; i++)ns[i] = new_node(fast_read());
    for(int i = 1; i < n; i++)
        link(ns[es[i].u], ns[es[i].v]);
    int q = fast_read();
    while(q--)
    {
        char c, cp;
        int x, y;
        while(!isalpha(c = getchar()));
        cp = getchar();
        x = fast_read();
        y = fast_read();
        if(c == 'Q')
        {
            if(cp == 'M')
            {
                make_root(ns[x]);
                access(ns[y]);
                splay(ns[y]);
                printf("%d\n", ns[y]->maxv);
            }else
            {
                make_root(ns[x]);
                access(ns[y]);
                splay(ns[y]);
                printf("%d\n", ns[y]->sum); 
            }
        }else
        {
            access(ns[x]);
            splay(ns[x]);
            ns[x]->data = y;
            ns[x]->pushup();
        }
    }
    return 0;
}