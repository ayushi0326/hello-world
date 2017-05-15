#include<stdio.h>
#include<stdlib.h>
int a,b,u,v,n,i,j,ne=1;
int visited[10]={0},min,mincost=0,cost[10][10];
int main()
{
    int n,i,j,c;
    printf("enter number of cities\n");
    scanf("%d",&n);
    int mat[n][n];
    printf("enter cost of each traversal \n when we go from i station(row number) to j station(column number)\n1=road\n2=rail\n3=aeroplane\n");
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n;j++)
        {
            scanf("%d",&mat[i][j]);
            if(mat[i][j]==0)
			mat[i][j]=999;
        }
    }
    visited[1]=1;
	printf("\n");
	while(ne < n)
	{
		for(i=1,min=999;i<=n;i++)
		for(j=1;j<=n;j++)
		if(mat[i][j]< min)
		if(visited[i]!=0)
		{
			min=mat[i][j];
			a=u=i;
			b=v=j;
		}
		if(visited[u]==0 || visited[v]==0)
		{
			printf("\n Edge %d:(%d %d) cost:%d",ne++,a,b,min);
			mincost+=min;
			visited[b]=1;
		}
		mat[a][b]=mat[b][a]=999;
	}
	printf("\n Minimun cost=%d",mincost);
return 0;
}
