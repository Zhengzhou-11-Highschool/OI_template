#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tot;
int note[10];
int status = 0;
void dfs(int cur)
{
	int i;
	if(cur == tot + 1)
	{ 
		for(i = 1; i <= tot; i++)
		{
			putchar(note[i] | '0');
 			putchar(' ');
		}
		putchar('\n');
	}else
	{
		for(i = 1; i <= tot; i++)
		{
			if(!(status & (1 << i)))
			{
				status |= (1 << i);
				note[cur] = i;
				dfs(cur+1);
				status ^= (1 << i);
			} 
		}
	}
}

int main()
{
	int n;
	tot = getchar() &(~('0'));
	dfs(1);
	return 0;
}