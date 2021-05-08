#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
 
int main()
{
    key_t key = 1234;
  // int *value;
    int (*value)[6];
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    int c, d, k, sum = 0;
  //  int first[10][10], second[10][10], 
    int multiply[10][10];

    int first[4][3] = {{1, 2, 1},
                    {2, 1, 3},
                    {2, 1, 1},
                    {1, 1, 1}};
    int second[3][6] = {{1, 2, 2, 1, 1, 1},
                      {2, 2, 2, 2, 1},
                      {1, 1, 1, 2, 1}};

  // // matriks pertama 4*3 c(baris) d(kolom)
  // printf("Masukan data Matrix:\n");
  // for (  c = 0 ; c < 4 ; c++ )
  // {
  //   for ( d = 0 ; d < 3 ; d++ )
  //   {
  //     scanf("%d", &first[c][d]);
  //   }
  // }
  // printf("Masukan data Matrix 2:\n");
  // for ( c = 0 ; c < 3 ; c++ )
  // {
  //   for ( d = 0 ; d < 6 ; d++ )
  //   {
  //     scanf("%d", &second[c][d]);
  //   }
  // }
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
      value[c][d] = multiply[c][d];
      printf("%d\t", multiply[c][d]);
    }
    printf("\n");
  }
    shmdt(value);
    // shmctl(shmid, IPC_RMID, NULL);

	return 0;
}
