#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>

int arr[24];
unsigned long long num;
int *value; 
pthread_t thread; 

typedef struct isi{
    int isi_lama;
    int input_baru;
}isi1;

unsigned long long factorial(unsigned long long x, int data){  
    int hasil = x;
        if (x ==1){
            return 1;
        }
        if( x==2){
            hasil *= ((x-1));
        }
        for(int a=1;a<data;a++){
            if(x-a<=0)
            {
                hasil *=1;
            }
            else{
                hasil *= (x-a);
            }
        }
    //}
    return hasil;
}


//fungsi faktorial
void *faktorial(void *arg){
    unsigned long long hasil;
    isi1 *value = (isi1 *)arg;
    if(value-> isi_lama==0){
        printf("0\t");
    }    
    if(value->input_baru ==0){
        printf("0\t");
    }
    else{
        hasil=factorial(value->isi_lama, value->input_baru);
        printf("%llu\t", hasil);
    }
    // key_t key = 1234;
    // int *value;
    // int data;
    // int hasil;
    // int shmid = shmget(key, sizeof(int)*24, IPC_CREAT | 0666);
    // value = shmat(shmid, 0, 0);
    
    // printf("Hasil faktorial pertambahan matriks ialah: \n");
    
    // for(int k=0;k<24;k++)
    // {
    //     for(int i=0;i<4;i++){ 
    //         for(int j=0;j<6;j++){ 
    //             num=value[i][j];
            
    //                 data = arr[k];
    //                 if(arr[k]==0)
    //                 {
    //                     hasil=0;    
    //                     printf("0\t");
    //                     k=k+1;
    //                 }
    //                 else{
    //                     printf("%llu\t", factorial(num,data));
    //                     k=k+1;
    //                 }
    //         }
    //         printf("\n");
    //     }
    // }
    // pthread_exit(0); // keluar thread
}
void hitung()
{
    for (int j=0;j<24;j++){
        if (j%6==0){
            printf("\n");
        }
        isi1 *ptr = (isi1*)malloc(sizeof(*ptr));
        ptr-> isi_lama = value[j];
        ptr-> input_baru = arr[j];
        pthread_create(&thread, NULL, faktorial, (void*)ptr); 
        pthread_join(thread,NULL);
    }
}

void main(){
    key_t key = 1234;
    int shmid = shmget(key, sizeof(int)*24, IPC_CREAT | 0666);
    value = shmat(shmid,NULL ,0 );
    
    for(int i=0;i<24;i++){
        printf("Masukkan data input[%d]:", i+1);
        scanf("%d", &arr[i]);
    }

    printf("Hasil perkalian dari matriks A dan B adalah: \n");
    
    for(int i=0;i< 4;i++){
        for(int j=0;j<6;j++){
            printf("%d\t", value[i*6+j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("Hasil setelah faktorial:");
    hitung ();
    
    printf("\n\n");
    shmdt(value);
    //shmctl(shmid, IPC_RMID, NULL); 
}
