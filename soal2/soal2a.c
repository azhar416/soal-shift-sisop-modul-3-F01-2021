#include <stdio.h>
 
int main()
{
  int m, n, p, q, c, d, k, sum = 0;
  int first[10][10], second[10][10], multiply[10][10];

  // matriks pertama 4*3 c(baris) d(kolom)
  printf("Masukan data Matrix 2:\n");
  for (  c = 0 ; c < 4 ; c++ )
  {
    for ( d = 0 ; d < 3 ; d++ )
    {
      scanf("%d", &first[c][d]);
    }
  }
  printf("Masukan data Matrix 2:\n");
  for ( c = 0 ; c < 3 ; c++ )
  {
    for ( d = 0 ; d < 6 ; d++ )
    {
      scanf("%d", &second[c][d]);
    }
  }
  //hasil
  for ( c = 0 ; c < 4 ; c++ )
  {
    for ( d = 0 ; d < 6 ; d++ )
    {
      for ( k = 0 ; k < 3 ; k++ )
      {
        sum = sum + first[c][k]*second[k][d];
      }
 
      multiply[c][d] = sum;
      sum = 0;
    }
  }
 
  printf("Hasil matriks:\n");
  for ( c = 0 ; c < 4 ; c++ )
  {
    for ( d = 0 ; d < 6 ; d++ )
    {
      printf("%d\t", multiply[c][d]);
    }
    printf("\n");
  }
 
  return 0;
}


// cek
//   1 2 3 
//   1 2 3 
//   2 2 2 
//   2 1 2

// 1 2 3 4 1 2
// 1 2 3 1 3 3
// 1 2 2 2 3 3

