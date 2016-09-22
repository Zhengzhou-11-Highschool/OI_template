#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <list>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;
#define MAXN 100003
#define MOD 99999997
int A[MAXN], B[MAXN];
int rkB[MAXN];
int ord[MAXN];
inline bool rankb(int a, int b)
{
    return B[a] < B[b];  
}
int ans = 0;
int tmp[MAXN];
void msort(int l, int m, int r)
{
    int i = l;
    int j = m+1;
    int k = l;
    while(i <= m && j <= r)
    {
        if(ord[i] > ord[j])
        {
            tmp[k++] = ord[j++];
            ans = (ans+m-i+1)%MOD;
        }else
        {
            tmp[k++] = ord[i++];
        }
    }
    while(i <= m)tmp[k++] = ord[i++];
    while(j <= r)tmp[k++] = ord[j++];
    for(int p = l; p <= r; p++)
        ord[p] = tmp[p];
}
void gg(int l, int r)
{
    if(l < r)
    {
        int m = (l+r)>>1;
        gg(l, m);
        gg(m+1, r);
        msort(l, m, r);
    }
}
int main()
{
    //freopen("MatchNOIP2013.in", "r", stdin);
    //freopen("MatchNOIP2013.out", "w", stdout);
    int n;
    scanf("%d", &n);
    for(int i = 1; i <= n; i++)
        scanf("%d", A+i);
    for(int i = 1; i <= n; i++)
    {
        scanf("%d", B+i);
        rkB[i] = i;       //初始化B的名次
    }
    sort(rkB+1, rkB+1+n, rankb);   //排序后满足 B[rkB[i]] = B中第i小的数
                                //rkB[i] 即为B中第i小的数在B中的位置
    for(int i = 1; i <= n; i++)
        ord[i] = rkB[A[i]];        //rkB[A[i]]就是第A[i]小的数的位置
    //这样我们在线性的时间内快速找到了B中元素在A中对应的位置，保存在ord数组中
    gg(1, n);    //求个逆序对
    printf("%d\n", ans);
    return 0;
}
