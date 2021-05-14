# soal-shift-sisop-modul-3-F01-2021



## Soal Nomor 2
Crypto (kamu) adalah teman Loba. Suatu pagi, Crypto melihat Loba yang sedang kewalahan mengerjakan tugas dari bosnya. Karena Crypto adalah orang yang sangat menyukai tantangan, dia ingin membantu Loba mengerjakan tugasnya. Detil dari tugas tersebut adalah:

### A. Membuat program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).

Pada kasus ini kami mendklarasikan matriks A (4*3) dan B(3*6) untuk dan menampilkan data tersebut menggunakan `for`. Berikut adalah implementasi untuk hasil:

```
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
```

### B. Membuat program dengan menggunakan matriks output dari program sebelumnya (program soal2a.c) (Catatan!: gunakan shared memory). Kemudian matriks tersebut akan dilakukan perhitungan dengan matrix baru (input user) sebagai berikut contoh perhitungan untuk matriks yang ada. Perhitungannya adalah setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya (dari paling besar ke paling kecil) (Catatan!: gunakan thread untuk perhitungan di setiap cel).

Pada soal b menggunakan shared memory pada `soal2a.c` setelah itu akan dilakukan perhitungan dengan memasukkan input matriks baru (3*6) sebagai acuan perhitungan program tersebut.

Berikut merupakan fungsi hitung yang menggunakan thread pada setiap penghitungannya, lalu pada saat `j%6` yang menandakan angka tersebut sudah pada kolom 6 maka akan dipindah kebaris bawahnya.
```
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
```
Pada fungsi faktorial akan dilakukan sesuai dengan kiteria soal.
Berikut untuk fungsi perhitungan nya:
```
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
```
Pada penggunaan fungsi void diberikan beberapa kiteria mengenai data struct yaitu saat `isi_lama` dan `input_baru` berisi nol. Maka data akan langsung menampilkan nol.
```
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
}
```

### C. Karena takut lag dalam pengerjaannya membantu Loba, Crypto juga membuat program (soal2c.c) untuk mengecek 5 proses teratas apa saja yang memakan resource komputernya dengan command “ps aux | sort -nrk 3,3 | head -5” (Catatan!: Harus menggunakan IPC Pipes)

Pada kasus ini akan dilakukan pengecekan terhadap 5 proses teratas yang memakan resource komputer, program dan menggunakan command sesuai dengan kiteria

Pada `void exec1()` program akan meng-list semua proses dan penggunaan status dan sumber daya.
```
void exec1() {
  // input from stdin (already done)
  // output to pipe1
  dup2(pipe1[1], 1);
  // close fds
  close(pipe1[0]);
  close(pipe1[1]);
  // exec
  execlp("ps", "ps", "aux", NULL);
  //char *argv[]= {"ps","aux", NULL};
  //execv("bin/ps", argv);
  // exec didn't work, exit
  perror("bad exec ps");
  _exit(1);
}
```
Pada `void exec2()` program akan melakukan sort
```
void exec2() {
  // input from pipe1
  dup2(pipe1[0], 0);
  // output to pipe2
  dup2(pipe2[1], 1);
  // close fds
  close(pipe1[0]);
  close(pipe1[1]);
  close(pipe2[0]);
  close(pipe2[1]);
  // exec
  execlp("sort", "sort", "-nrk", "3.3", NULL);
  // exec didn't work, exit
  perror("bad exec grep root");
  _exit(1);
}
```
Pada `void exec3()` program menampilkan 5 data teratas yang memakan resource
```
void exec3() {
  // input from pipe2
  dup2(pipe2[0], 0);
  // output to stdout (already done)
  // close fds
  close(pipe2[0]);
  close(pipe2[1]);
  // exec
  execlp("head", "head", "-5", NULL);
  // exec didn't work, exit
  perror("bad exec grep sbin");
  _exit(1);
}
```
