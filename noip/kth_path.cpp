#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <algorithm>
#include <list>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;
#define MAXN 100002
struct edge
{
    int from, to;
    int cost;
};
//图
vector<int> G[MAXN];
vector<edge> edges;

//反向图
vector<int> RG[MAXN];
vector<edge> redges;

//然而这个题的边是双向边23333...
void addedge(int u, int v, int c)
{
    edges.push_back((edge){u, v, c});
    edges.push_back((edge){v, u, c});
    G[u].push_back(edges.size()-2);
    G[v].push_back(edges.size()-1);

    redges.push_back((edge){v, u, c});
    redges.push_back((edge){u, v, c});
    RG[v].push_back(redges.size()-2);
    RG[u].push_back(redges.size()-1);
}
//反向图sssp
int rdist[MAXN];
int rvis[MAXN];
void spfa(int s, int t, int n)
{
    for(int i = 1; i <= n; i++)
    {
        rdist[MAXN] = 0x3f3f3f3f;
        rvis[MAXN] = false;
    }
    queue<int> q;
    q.push(s);
    rvis[s] = true;
    rdist[s] = 0;
    while(q.size())
    {
        int c = q.front();
        q.pop();
        for(int i = 0; i < RG[c].size(); i++)
        {
            edge &e = redges[RG[c][i]];
            if(rdist[e.to] > rdist[c] + e.cost)
            {
                rdist[e.to] = rdist[c] + e.cost;
                if(!rvis[e.to])
                {
                    rvis[e.to] = true;
                    q.push(e.to);
                }
            }
        }
        rvis[c] = false;
    }
}

//下面是核心的代码，启发式搜索
struct node
{
    int f, g, to;
            //f 启发
            //g 已用代价
            //to 节点点
    bool operator< (const node& o) const
    {
        return f > o.f;
    }
};
int cnt[MAXN];    //记录入队次数
int astar(int s, int t, int k)
{
    memset(cnt, 0, sizeof(cnt));
    priority_queue<node> q;
    if(rdist[s] == 0x3f3f3f3f)return -1; //unreachable
    q.push((node){rdist[s], 0, s});
    while(q.size())
    {
        node c = q.top();
        q.pop();
        cnt[c.to]++;        //入队次数++
        if(t == c.to && cnt[t] == k)   //目标点入队k次，是第k短路
            return c.f;
        if(cnt[c.to] > k)       //剪枝，超过k，没必要再计算
            continue;
        for(int i = 0; i < G[c.to].size(); i++)
        {
            edge &e = edges[G[c.to][i]];
              //f = 估价(距目标点距离)+已用代价(c.g+e.cost)
            q.push((node){rdist[e.to] + e.cost + c.g , c.g + e.cost, e.to});
        }
    }
    return -1;
}

int main()
{
    int n, m;
    scanf("%d %d", &n, &m);
    while(m--)
    {
        int a, b, c;
        scanf("%d %d %d", &a, &b, &c);
        addedge(a, b, c);
    }
    spfa(n, 1, n);
    printf("%d\n", astar(1, n, 2));
    return 0;
}
