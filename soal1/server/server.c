#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<pthread.h> //for threading , link with lpthread


void *connection_handler(void *);
void *regis(void*);

int main(int argc , char *argv[])
{
	FILE* fp = fopen("akun.txt","a");
	fclose(fp);

	int socket_desc , new_socket , c , *new_sock;
	struct sockaddr_in server , client;
	char *message;
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8080 );
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *) &server , sizeof(server)) < 0)
	{
		puts("bind failed");
		return 1;
	}
	puts("bind done");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Connection accepted");
		
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = new_socket;
		
		if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}
		
		//Now join the thread , so that we dont terminate before the thread
		pthread_join( sniffer_thread , NULL);
		puts("Handler assigned");
	}
	
	if (new_socket<0)
	{
		perror("accept failed");
		return 1;
	}
	
	return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int valread;

	char menu[1024] = {0};
	char buffer[1024] = {0};
	char username[1024] = {0};
	char password[1024] = {0};
	char choice[1024] = {0};
	while (1)
	{
		sprintf(menu, "\nMain Menu\n1. REGISTER\n2. LOGIN\nCHOICE : \e[s\n\e[u");
		write(sock, menu, 1024); 
		memset(menu, '0', 1024);

		valread = read(sock, choice, 1024); 
		if (valread == 0) return 0;

		// printf("\nCHOICE : %s\n", choice);
		if (!(strcmp(choice, "1")))
		{
			FILE* fp = fopen("akun.txt", "a");
			// printf("Masuk 1\n");
			sprintf(menu, "Username : \e[s\n\e[u");
			write(sock, menu, 1024); 
			memset(menu, '0', 1024);
			valread = read(sock, username, 1024);
			if (valread == 0) return 0;

			sprintf(menu, "Password : \e[s\n\e[u");
			write(sock, menu, 1024); 
			memset(menu, '0', 1024);
			valread = read(sock, password, 1024);
			if (valread == 0) return 0;

			fprintf(fp, "%s:%s\n", username, password);
			fclose(fp);
		}
		else if (!(strcmp(choice, "2")))
		{
			char* aut = "0";
			FILE* fp = fopen("akun.txt", "r");
			while (aut == "0")
			{
				memset(menu, '0', 1024);
				// printf("Masuk 2\n");
				sprintf(menu, "Username : \e[s\n\e[u");
				// printf("%s\n", menu);
				int ler = write(sock, menu, 1024);
				// if (ler == 0) printf("write\n");
				memset(menu, '0', 1024);
				valread = read(sock, username, 1024);
				if (valread == 0) return 0;

				sprintf(menu, "Password : \e[s\n\e[u");
				write(sock, menu, 1024); 
				memset(menu, '0', 1024);
				valread = read(sock, password, 1024); 
				if (valread == 0) return 0;

				while (fgets(buffer, 1024, fp) != NULL && aut == "0") 
				{
					char f_username[1024], f_password[1024];
					char *token = strtok(buffer, ":");
					strcpy(f_username, token);
					token = strtok(NULL, "\n");
					strcpy(f_password, token);
					
					if (strcmp(username, f_username) == 0 && strcmp(password, f_password) == 0)
					{
						// printf("MASUK!!!\n");
						aut = "1";
						write(sock, aut, 1024);
						printf("%s", aut);		
					}
				}
			}
			while (aut == "1")
			{
				memset(menu, '0', 1024);
				sprintf(menu, "LOGIN MENU\nadd\nsee\nexit\nCHOICE : \e[s\n\e[u");
				write(sock, menu, 1024);
				valread = read(sock, choice, 1024);
				if (valread == 0) return 0;

				if (!strcmp(choice, "add"))
				{
					FILE* fp_1 = fopen("files.tsv", "a");
					char publisher[1024] = {0};
					char tahun[1024] = {0};
					char path[1024] = {0};

					sprintf(publisher, "Publisher : \e[s\n\e[u");
					write(sock, publisher, 1024);
					memset(publisher, '0', 1024);
					valread = read(sock, publisher, 1024);
					if (valread == 0) return 0;
					
					sprintf(tahun, "Tahun Publikasi : \e[s\n\e[u");
					write(sock, tahun, 1024);
					memset(tahun, '0', 1024);
					valread = read(sock, tahun, 1024);
					if (valread == 0) return 0;

					sprintf(path, "Filepath : \e[s\n\e[u");
					write(sock, path, 1024);
					memset(path, '0', 1024);
					valread = read(sock, path, 1024);
					if (valread == 0) return 0;
					
					// transfer file
					mkdir("FILES", 0777);
					char pathServer[1024];
					sprintf(pathServer, "/home/azhar416/soal-shift-sisop-modul-3-F01-2021/soal3/server/FILES/");
					strcat(pathServer, path);
					FILE* file_create = fopen(pathServer, "w");

					memset(buffer, '0', 1024);
					valread = read(sock, buffer, 1024);
					if (valread == 0) return 0;
					fprintf(file_create, "%s", buffer);
					// fprintf(file_create, "\n");

					fclose(file_create);

					memset(buffer, '0', 1024);
					sprintf(buffer, "\nFile Terkirim\n");
					write(sock, buffer, 1024);

					fprintf(fp_1, "%s\t%s\t%s\n", publisher, tahun, pathServer);
					fclose(fp_1);
				}
				else if (!strcmp(choice, "see"))
				{
					// memset(buffer, '0', 1024);

					FILE* fcek = fopen("files.tsv", "r");

					char 	line[1024] = {0}, pub[1024] = {0}, thn[1024] = {0}, p_path[1024] = {0}, 
							pathTemp[1024] = {0}, namafile[1024] = {0}, nama[1024] = {0}, extensi[1024] = {0};
					int bukanawal = 0;
					strcpy(buffer, "");
					while (fgets(line, 1024, fcek))
					{
						// "/home/azhar416/sisop..../soal3/
						// printf("ler\n");
						// if (bukanawal) 
						strcat(buffer, "\n\n");

						// ngambil tiap tab
						char* token;
						char samlekom1[1024];
						strcpy(samlekom1, line);
						char* kumsalam = samlekom1;
						for (int i = 0; token = strtok_r(kumsalam, "\t", &kumsalam); i++)
						{
							if (i == 0) 
								strcpy(pub, token);
							else if (i == 1) 
								strcpy(thn, token);
							else if (i == 2) 
								strcpy(p_path, token);
						}
						strcpy(pathTemp, p_path);

						// ngambil namafile.extensi
						char samlekom2[1024];
						strcpy(samlekom2, pathTemp);
						kumsalam = samlekom2;
						for (int i = 0; token = strtok_r(kumsalam, "/", &kumsalam); i++)
						{
							strcpy(namafile, token);
						}

						// misah namafile ama extensi
						char samlekom3[1024];
						strcpy(samlekom3, namafile);
						kumsalam = samlekom3;
						for (int i = 0; token = strtok_r(kumsalam, ".", &kumsalam); i++)
						{
							if (i == 0) 
								strcpy(nama, token);
							else if (i == 1) 
							{
								strcpy(extensi, ".");
								strcat(extensi, token);
							}
						}
						// printf("%s\n%s\n%s\n%s\n%s\n", nama, pub, thn, extensi, p_path);
	
						strcat(buffer, "Nama : ");
						strcat(buffer, nama);
						strcat(buffer, "\nPublisher : ");
						strcat(buffer, pub);
						strcat(buffer, "\nTahun Publishing : ");
						strcat(buffer, thn);
						strcat(buffer, "\nEkstensi File : ");
						strcat(buffer, extensi);
						strcat(buffer, "\nFile Path : ");
						strcat(buffer, p_path);

						bukanawal++;
					}
					// printf("%s", buffer);
					write(sock, buffer, 1024);
					memset(buffer, '0', 1024);
				}
			}


			fclose(fp);
		}

	}
		
	//Free the socket pointer
	free(socket_desc);
	
	return 0;
}
