#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>

int arr[24];
unsigned long long num;  

unsigned long long factorial(unsigned long long x, int data){  
    int hasil = x;
    // if (data==0){
    //     hasil =0;
    // }
    //else{
        if (x ==1 || x==0){
            return 1;
        }
        if( x==2){
            hasil *= ((x-1));
        }
        for(int a=1;a<data;a++){
            hasil *= (x-a);
        }
    //}
    return hasil;
    // if(a==0 || a==1) return 1;
    // else return a* factorial(a-1); 
}


//fungsi faktorial
void *faktorial(void *arg){
    
    key_t key = 1234;
    int (*value)[6];
    int data;
    int hasil;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, 0, 0);
    
    printf("Hasil faktorial pertambahan matriks ialah: \n");
    
for(int k=0;k<24;k++)
{
    for(int i=0;i<4;i++){ 
        for(int j=0;j<6;j++){ 
            num=value[i][j];
            
                data = arr[k];
                if(arr[k]==0)
                {
                    hasil=0;    
                    printf("0\t");
                    k=k+1;
                }
                else{
                    printf("%llu\t", factorial(num,data));
                    k=k+1;
                }
        }
        printf("\n");
    }
}
    pthread_exit(0); // keluar thread
}

void main(){
    pthread_t thread;
    
    key_t key = 1234;
    int (*value)[6];
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid,NULL ,0 );
    
    for(int i=0;i<24;i++){
        printf("Masukkan data input[%d]:", i+1);
        scanf("%d", &arr[i]);
    }

    printf("Hasil perkalian dari matriks A dan B adalah: \n");
    
    for(int i=0;i< 4;i++){
        for(int j=0;j<6;j++){
            printf("%d\t", value[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
    pthread_create(&thread, NULL, faktorial, NULL); 
    pthread_join(thread,NULL); 
}
