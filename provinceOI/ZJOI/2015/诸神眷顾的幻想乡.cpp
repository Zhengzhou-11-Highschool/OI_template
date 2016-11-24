#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <list>
#include <queue>
#include <vector>
#include <iostream>
using namespace std;
#define set_file(x) (freopen(#x".in", "r", stdin),freopen(#x".out", "w", stdout));
typedef long long LL;
struct node
{
    int len;
    node *next[11], *link;
}ns[2000004], *sam = ns+1;
node *insert(node *last, int c)
{
    node *cur = sam++;
    cur -> len = last -> len + 1;
    node *p;
    for(p = last; p && !p -> next[c]; p = p -> link)
        p -> next[c] = cur;
    if(!p)cur -> link = ns;
    else
    {
        node *q = p -> next[c];
        if(q -> len == p -> len + 1)
            cur -> link = q;
        else
        {
            node *clone = sam++;
            clone -> len = p -> len + 1;
            clone -> link = q -> link;
            memcpy(clone -> next, q -> next, sizeof(ns[0].next));
            for(; p && p -> next[c] == q; p = p -> link)
                p -> next[c] = clone;
            q -> link = cur -> link = clone;
        }
    }
    return cur;
}

#define MAXN 100003
int w[MAXN];
int deg[MAXN];
vector<int> G[MAXN];
void dfs(int u, int f, node *p)
{
    node *now = insert(p, w[u]);
    for(int i = 0; i < G[u].size(); i++)
        if(G[u][i] != f)
            dfs(G[u][i], u, now);
}
int main()
{
    freopen("zjoi15_substring.in", "r", stdin);
    freopen("zjoi15_substring.out", "w", stdout);
    int n, c;
    scanf("%d %d", &n, &c);
    for(int i = 1; i <= n; i++)
        scanf("%d", w+i);
    for(int i = 1; i < n; i++)
    {
        int x, y;
        scanf("%d %d", &x, &y);
        G[x].push_back(y);
        G[y].push_back(x);
        deg[x]++;
        deg[y]++;
    }
    for(int i = 1; i <= n; i++)
        if(deg[i] == 1)dfs(i, 0, ns);
    LL ans = 0;
    for(node *p = ns+1; p < sam; p++)
        ans += p -> len - (p -> link?(p -> link -> len):0);
    printf("%lld\n", ans);
    return 0;
}