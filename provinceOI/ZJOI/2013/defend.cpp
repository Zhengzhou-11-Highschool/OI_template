#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <algorithm>
#include <queue>
#include <vector>
#include <cctype>
using namespace std;
char buf[1<<15], *fs, *ft;
inline char readc()
{
    return (fs==ft&&(ft=(fs=buf)+fread(buf,1,1<<15,stdin),fs==ft))?0:*fs++;
}
inline int fast_read()
{
    int r;
    char c;
    bool sig = false;
    while(c = readc())
    {
        if(c >= '0' && c <= '9')
        {
            r = c^0x30;
            break;
        }else if(c == '-')sig = true;
    }
    while(isdigit(c = readc()))
        r = (r<<3)+(r<<1)+(c^0x30);
    return sig?-r:r;
}
//----------------
int n, m;
int A[10001][10001];
void pivot(int x, int y)
{
    int t = -A[x][y];
    A[x][y] = -1;
    for(int j = 0; j <= m; j++)
    {
        if(j != x && A[j][y])
        {
            t = A[j][y];
            A[j][y] = 0;
            for(int k = 0; k <= n; k++)
                A[j][k] += t*A[x][k];
        }
    }
}
void simplex()
{
    for(int x = 0, y = 0; true; x = y = 0)
    {
        for(int i = 1; i <= n; i++)
        {
            if(A[0][i] > 0)
            {
                y = i;
                break;
            }
        }
        if(!y)return;  //OK!
        int large = 0x76543210;
        for(int i = 1; i <= m; i++)
        {
            int t;
            if(A[i][y] < 0 && ((t = -A[i][0]/A[i][y]) < large))
            {
                large = t;
                x = i;
            }
        }
        if(!x)return; //fail....
        pivot(x, y);
    }
}
int main()
{
    freopen("zjoi13_defend.in", "r", stdin);
    freopen("zjoi13_defend.out", "w", stdout);
    n = fast_read();
    m = fast_read();
    for(int i = 1; i <= n; i++)A[i][0] = (double)fast_read();
    for(int i = 1; i <= m; i++)
    {
        int l = fast_read();
        int r = fast_read();
        int d = fast_read();
        for(int j = l; j <= r; j++)
            A[j][i] = -1;
        A[0][i] = d;
    }
    swap(n, m);
    simplex();
    printf("%d\n", A[0][0]);
    return 0;
}