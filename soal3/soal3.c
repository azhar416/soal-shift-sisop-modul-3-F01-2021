#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fts.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <ctype.h>
#include <dirent.h>

pthread_mutex_t bufferlock;

void tolowerstr(char * str){
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

typedef struct file{
    char curDir[1024];
    char filename[1024];
}file_t;


int is_regular_file( char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

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

int ix = 0;

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

int main(int argc, char * argv[]){
    char curDir[PATH_MAX + PATH_MAX];
    getcwd(curDir,sizeof(curDir));
    pthread_mutex_init(&bufferlock,NULL);
    pthread_t copy_thread[1000];
    // return 0;
    if(strcmp(argv[1],"-f")==0){
        //pthread_t copy_thread[argc];
        for(int i=2;i<argc;i++){
            file_t * filenow = (file_t*)malloc(sizeof(file_t));
            // filenow->curDir = curDir;
            strcpy(filenow->curDir, curDir);
            char * copy = (char*)malloc(sizeof(char)*strlen(argv[i]));
            memset(copy,0,sizeof(char)*strlen(argv[i]));
            strcpy(copy,argv[i]);
            // filenow->filename = copy;
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
    }else if(strcmp(argv[1],"-d")==0){
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
    }else if(strcmp(argv[1],"*")==0){
        int count=0;
        DIR *d;
        struct dirent *dir;
        d = opendir(curDir);
        if (d){
            listFilesRecursive(curDir, copy_thread);
        }
    }else{
        return 0;
    }
}