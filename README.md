# soal-shift-sisop-modul-3-F01-2021



## Soal Nomor 2
Crypto (kamu) adalah teman Loba. Suatu pagi, Crypto melihat Loba yang sedang kewalahan mengerjakan tugas dari bosnya. Karena Crypto adalah orang yang sangat menyukai tantangan, dia ingin membantu Loba mengerjakan tugasnya. Detil dari tugas tersebut adalah:

### A. Membuat program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).

Pada kasus ini kami mendklarasikan matriks A (4*3) dan B(3*6) untuk melakukan input dan menampilkan data tersebut menggunakan `for`. 
Berikut adalah implementasi untuk input data:
```
  printf("Masukan data Matrix:\n");
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
```


Berikut adalah implementasi untuk hasil:

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
    long long hasil = x;
        if (x ==1){
            return 1;
        }
        else if( x==2){
            hasil *= ((x-1));
        }
        else if(data>x){
            for(long long a=1;a<x;a++){
                hasil*= (x-a);
            }
        }
        else{
            for(long long a=1;a<data;a++){
                if(x-a==0)
                {
                    //hasil *=1;
                    continue;
                }
                else if(x-a >0){
                    hasil *= (x-a);
                }
            }
        }
    //}
    return hasil;
}
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

## Soal Nomor 3
Seorang mahasiswa bernama Alex sedang mengalami masa gabut. Di saat masa gabutnya, ia memikirkan untuk merapikan sejumlah file yang ada di laptopnya. Karena jumlah filenya terlalu banyak, Alex meminta saran ke Ayub. Ayub menyarankan untuk membuat sebuah program C agar file-file dapat dikategorikan. Program ini akan memindahkan file sesuai ekstensinya ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working directory ketika program kategori tersebut dijalankan.

Berikut terdapat definisi struct file yang berisis array of char `curDir` untuk menyimpan nama direktori dan `filename` untuk menyimpan nama file
```c
typedef struct file{
    char curDir[1024];
    char filename[1024];
}file_t;
```

### Penyelesaian no 3a, 3b, 3c : membuat fungsi untuk memeriksa tipe file/direktori, dan untuk mengkategorikan file dalam direktori.

Untuk penyelesaian no 3 ada 2 fungsi yang digunakan yaitu untuk cek tipe file atau direktori dan untuk mengkategorikan file. Terdapat *is_regular_file* untuk memeriksa apakah file nya merupakan file regular atau bukan 
```c
int is_regular_file( char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}
```

*Fungsi untuk mendapatkan extension* 
```c
char * get_filename_ext(char *filename) {
    char * extname = (char*)malloc(sizeof(char)* (PATH_MAX + PATH_MAX));
    memset(extname,'\0',sizeof(char)* (PATH_MAX + PATH_MAX));
    char *dot = strchr(filename, '.');
    char hidden[1000];
    char *dotted = strrchr(filename, '/');
    strcpy(hidden, dotted);
    if(hidden[1]=='.'){
        strcpy(extname,"Hidden");
        return extname;
    }
    else if (!dot){
        strcpy(extname,"Unknown");
        return extname;
    }
    else {
        strcpy(extname,dot+1);
        tolowerstr(extname);
        return extname;
    }
}
```
* Mendapatkan ekstensi yang akan digunakan sebagai nama folder. *strchr* akan mengembalikan nilai dimana "." pertama ditemukan untuk dipotong
```c
char *dot = strchr(filename, '.');
```
* Mendapatkan ekstensi yang akan digunakan sebagai nama folder. *strrchr* akan mengembalikan nilai dimana "/" pertama ditemukan dari belakang untuk dipotong 
```c
char *dotted = strrchr(filename, '/');
```

Soal meminta untuk mengkategorikan file hidden dalam folder Hidden sedangkan file yang tidak memiliki extension akan dimasukkan ke folder Unknown.
* Untuk mengkategorikan file Hidden
```c
  strcpy(hidden, dotted);
    if(hidden[1]=='.'){
        strcpy(extname,"Hidden");
        return extname;
    }
```
Apabila terdapat "." pada index pertama array hidden, maka extname akan menyimpan "Hidden" sebagai nama folder untuk menyimpan file berjenis hidden.
* Untuk mengkategorikan file Unknown
```c
    else if (!dot){
        strcpy(extname,"Unknown");
        return extname;
    }
```
Apabila tidak terdapat "." pada string, maka extname akan menyimpan "Unknown" sebagai nama folder untuk menyimpan file yang tidak memiliki ekstensi.
* Untuk mengkategorikan file lainnya
```c
    else {
        strcpy(extname,dot+1);
        tolowerstr(extname);
        return extname;
    }
```
Case lainnya, apabila ekstensi lebih dari 1 maka extname akan menyimpan ekstensi pada titik terdepan sebagai nama folder.

Perlu diperhatikan bahwa untuk soal ini ekstensi tidak case sensitive sehingga kita perlu mengubah ekstensi menjadi lower case
```c
void tolowerstr(char * str){
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}
```

*Fungsi untuk membuat folder dan memindahkan file*
```c
void* checkFolderAndCopy(void* args){
    file_t * filenow = (file_t*)args;
    char * extensionName = get_filename_ext(filenow->filename);
    char * pathname = (char*)malloc(sizeof(char) * (PATH_MAX + PATH_MAX));
    memset(pathname,0,sizeof(char) * (PATH_MAX + PATH_MAX));
    strcpy(pathname,filenow->curDir);
    strcat(pathname,"/");
    strcat(pathname,extensionName);
    mkdir(pathname,0777);
    pthread_mutex_lock(&bufferlock);
    strcat(pathname,"/");
    char buffer[PATH_MAX + PATH_MAX];
    memset(buffer,0,sizeof(buffer));
    strcpy(buffer,pathname);
    strcat(buffer,basename(filenow->filename));
    printf("moving %s to %s\n",filenow->filename,buffer);
    rename(filenow->filename,buffer);
    pthread_mutex_unlock(&bufferlock);
}
```
Untuk membuat folder digunakan `mkdir` dan untuk memindahkan file dari path awal ke path baru yang telah ditentukan menggunakan `rename`

### Penyelesaian 3b, 3c : memindahkan file secara rekursif
```c
void listFilesRecursive(char *base, pthread_t *thread) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(base);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            // Construct new path from our base path
            strcpy(path, base);
            strcat(path, "/");
            strcat(path, dp->d_name);
            file_t * filenow = (file_t*)malloc(sizeof(file_t));
            if (dp->d_type == DT_REG)
            {
                strcpy(filenow->filename, path);
                strcpy(filenow->curDir,"/home/inez/sisop/soal-shift-sisop-modul-3-F01-2021/soal3");
                printf("%s\n", path);
                if (strcmp(path,"/home/inez/sisop/soal-shift-sisop-modul-3-F01-2021/soal3") != 0)
                {
                    pthread_create(&thread[ix],NULL,checkFolderAndCopy,(void*)filenow);
                    sleep(1);
                    ix++;
                }
            }
            else if (dp->d_name[0] != '.')
                listFilesRecursive(path, thread);
        }
    }

    closedir(dir);
}
```
Untuk memasukkan setiap path dalam thread nya masing-masing maka kita membuat thread dan path file akan disimpan dalam array `thread`.
```c
  if (strcmp(path,"/home/inez/sisop/soal-shift-sisop-modul-3-F01-2021/soal3") != 0)
  {
    pthread_create(&thread[ix],NULL,checkFolderAndCopy,(void*)filenow);
    sleep(1);
    ix++;
  }
```
Jika path yang ditemukan masih berupa folder maka akan diabaikan dan dilakukan rekursif hingga menemukan file reguler.

### A. Program menerima opsi -f seperti contoh di atas, jadi pengguna bisa menambahkan argumen file yang bisa dikategorikan sebanyak yang diinginkan oleh pengguna. 
```c
    if(strcmp(argv[1],"-f")==0){
        //pthread_t copy_thread[argc];
        for(int i=2;i<argc;i++){
            file_t * filenow = (file_t*)malloc(sizeof(file_t));
            strcpy(filenow->curDir, curDir);
            char * copy = (char*)malloc(sizeof(char)*strlen(argv[i]));
            memset(copy,0,sizeof(char)*strlen(argv[i]));
            strcpy(copy,argv[i]);
            strcpy(filenow->filename, copy);

            int iret = pthread_create(&copy_thread[i],NULL,checkFolderAndCopy,(void*)filenow);
            // if(is_regular_file(argv[i])){
            if(iret){
                perror("thread1");
                //iret;
                printf("File %d: Sad, gagal :(\n", i-1);
                exit(EXIT_FAILURE);
            }
            else {
                printf("File %d: Berhasil Dikategorikan\n", i - 1);
                sleep(1);
                ix++;
            }
        }
        for(int i=2;i<argc;i++){
            pthread_join(copy_thread[i],NULL);
        }
    }
```
Apabila argumen yang diberikan yaitu -f dan path nya sesuai maka akan dimasukkan dalam thread dan menjalankan fungsi **checkFolderAndCopy**. Thread akan dijalankan bersamaan dengan pthread_join.

### B. Program juga dapat menerima opsi -d untuk melakukan pengkategorian pada suatu directory. Namun pada opsi -d ini, user hanya bisa memasukkan input 1 directory saja, tidak seperti file yang bebas menginput file sebanyak mungkin. 
```c
    else if(strcmp(argv[1],"-d")==0){
        chdir(argv[2]);
        int count=0;
        DIR *d;
        struct dirent *dir;
        d = opendir(".");
        if (d){
            while ((dir = readdir(d)) != NULL){
                if(is_regular_file(dir->d_name)){
                    count++;
                }
            }
            closedir(d);
        }

        int i=0;
        d = opendir(".");
        if (d){
            listFilesRecursive(argv[2], copy_thread);
            if (count == 0)
                printf("Yah, gagal disimpan :(\n");
            
            else printf("Direktori Sukses Disimpan!\n");
        }
    }
```
Apabila argumen yang diberikan yaitu -d dan path nya sesuai maka akan dimasukkan dalam thread dan menjalankan fungsi **listFilesRecursive** untuk menyimpan path dalam array. Nantinya, elemen pada array tersebut akan dibuatkan thread dan menjalankan fungsi **checkFolderAndCopy**.

### C. Program ini menerima opsi *. Opsi ini akan mengkategorikan seluruh file yang ada di working directory ketika menjalankan program C tersebut.
```c
    else if(strcmp(argv[1],"*")==0){
        int count=0;
        DIR *d;
        struct dirent *dir;
        d = opendir(curDir);
        if (d){
            listFilesRecursive(curDir, copy_thread);
        }
    }
```
Apabila argumen yang diberikan yaitu * maka akan menyimpan path current working directory. Lalu menjalankan proses eperti nomor 3b yaitu memanggil fungsi **listFilesRecursive** untuk menyimpan path dalam array. Nantinya, elemen pada array tersebut akan dibuatkan thread dan menjalankan fungsi **checkFolderAndCopy**.

### D. dan E.
Untuk kedua sub nomor ini sudah dijelaskan pada penjelasan diatas.


