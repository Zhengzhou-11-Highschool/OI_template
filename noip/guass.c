#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <malloc.h>
void swap(double *a, double *b)
{
    double t = *a;
    *a = *b;
    *b = t;
}
void gauss(double *A[], int n)
{
    for(int i = 0; i < n; i++)
    {
        int r = i;
        for(int j = i+1; j < n; j++)
            if(fabs(A[j][i]) > fabs(A[r][i]))
                r = j;
        if(r != i)
            for(int j = 0; j <= n; j++)
                swap(A[i]+j, A[r]+j);
        for(int j = n; j >= i; j--)
            for(int k = i+1; k < n; k++)
                A[k][j] -= A[k][i]/A[i][i]*A[i][j];
    }
    for(int i = n-1; i >= 0; i--)
    {
        for(int j = i+1; j < n; j++)
            A[i][n] -= A[j][n]*A[i][j];
        A[i][n] /= A[i][i];
    }
}
int main()
{
    int n;
    scanf("%d", &n);
    double *(*A) = (double *(*))malloc(sizeof(double*)*n);
    for(int i = 0; i < n; i++)
        A[i] = (double *)malloc(sizeof(double)*(n+1));
    
    for(int i = 0; i < n; i++)
        for(int j = 0; j <= n; j++)
            scanf("%lf", &A[i][j]);
    gauss((double**)A, n);

    for(int i = 0; i < n; i++)
        printf(i == n-1?"%d":"%d ", (int)(A[i][n]+0.5));
    for(int i = 0; i < n; i++)free(A[i]);
    free(A);
    return 0;
}