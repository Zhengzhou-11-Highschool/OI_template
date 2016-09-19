#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <list>
#include <string>
using namespace std;
const int MAXN = 2e5+10;

class SAM
{
	int last;
	int size;
public:
	struct state	
	{
		int link;
		int len;
		int next[26];
		void init()
		{
			link = -1;
			len = 0;
			memset(next, 0, sizeof(next));
		}
	}st[MAXN<<1];
	SAM()
	{
		last = size = 0;
		newst();    
	}
	inline int newst()
	{
		st[size++].init();
		return size-1;
	}
	inline void copy(int a, int b)
	{
		memcpy(st[a].next, st[b].next, sizeof(st[a].next));
		st[a].link = st[b].link;
	}
	void expand(char newc)
	{
		int c = newc-'a';
		
		int cur = newst();
		st[cur].len = st[last].len + 1;
		int p;
		for(p = last; p != -1 && !st[p].next[c]; p = st[p].link)
			st[p].next[c] = cur;
		if(p == -1)
			st[cur].link = 0;
		else
		{
			int q = st[p].next[c];
			if(st[q].len == st[p].len + 1)
				st[cur].link = q;
			else
			{
				int clone = newst();
				copy(clone, q);
				st[clone].len = st[p].len + 1;
				for(; p != -1 && st[p].next[c] == q; p = st[p].link)
					st[p].next[c] = clone;
				st[q].link = st[cur].link = clone;
			}
		}  
		last = cur;
	}
};
SAM s;
char buf[MAXN];
int input()
{
	int len = 0;
	char c;
	while(c = getchar())
	{
		if(c >= 'a' && c <= 'z')
		{
			buf[0] = c;
			len = 1;
			break;
		}
	}
	while(c = getchar())
	{
		if(c >= 'a' && c <= 'z')
			buf[len++] = c;
		else
			break;
	} 
	buf[len] = 0;
	return len;
} 


//下面是求两个字符串的最长公共子串
int main()
{
	char c;
	int len = 0;
	int ns = 0;
	int res = 0;
	while(c = getchar())
		if(c >= 'a' && c <= 'z')
		{
			s.expand(c);
			break;
		}
	while(c = getchar())
		if(c >= 'a' && c <= 'z')
			s.expand(c);
		else
			break;
	int maxl = input();

	for(int i = 0; i < maxl; i++)
	{
		int c = buf[i] - 'a';
		if(s.st[ns].next[c])
		{
			len++;
			ns = s.st[ns].next[c];
		}else		
		{
			while(ns != -1 && !s.st[ns].next[c])
				ns = s.st[ns].link;
			if(ns != -1)
			{
				len = s.st[ns].len + 1;
				ns = s.st[ns].next[c];
			}else
				ns = len = 0;
		}
		res = max(len, res);
	}
	printf("%d\n", res);
	return 0;
} 