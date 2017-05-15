#include<stdio.h>
#include<math.h>

char a[10][10];
int n;

void printmatrix() {
   int i, j;
   printf("\n");

   for (i = 1; i <=n; i++) {
      for (j = 1; j <=n; j++)
         printf(" %c\t", a[i][j]);
      printf("\n\n");
   }
}

int getmarkedcol(int row) {
   int i;
   for (i = 1; i <=n; i++)
      if (a[row][i] == 'Q') {
         return (i);
         break;
      }
      else{
        return 0;
      }
}

int feasible(int row, int col) {
   int i, tcol;
   for (i = 1; i <=n; i++) {
      tcol = getmarkedcol(i);
      if (col == tcol || abs(row - i) == abs(col - tcol))
         return 0;
   }
   return 1;
}

void nqueen(int row) {
   int i, j;
   while(row<=n)
   {
   if (row < n) {
      for (i = 1; i <=n; i++) {
         if (feasible(row, i)) {
            a[row][i] = 'Q';
            nqueen(row + 1);
            a[row][i] = '.';
            printf("ohoo\n");
         }
      }
   } else {
      printf("\nThe solution is:- ");
      printmatrix();
   }
   }
}

int main() {
   int i, j;

   printf("\nEnter the no. of queens:- ");
   scanf("%d", &n);

   for (i = 1; i <=n; i++)
      for (j = 1; j <=n; j++)
         a[i][j] = '.';
          printmatrix();

   nqueen(0);
   return (0);
}
