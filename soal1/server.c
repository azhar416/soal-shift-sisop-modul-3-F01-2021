#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
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
	char username[1024];
	char password[1024];
	char choice[1024] = {0};
	while (1)
	{
		sprintf(menu, "\nMain Menu\n1. REGISTER\n2. LOGIN\nCHOICE : \e[s\n\e[u");
		write(sock, menu, 1024); // send 1
		memset(menu, '0', 1024);

		valread = read(sock, choice, 1024); // recv 1
		if (valread == 0) return 0;

		printf("\nCHOICE : %s\n", choice);
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
			printf("Masuk 2\n");
			sprintf(menu, "Username : \e[s\n\e[u");
			write(sock, menu, 1024); 
			memset(menu, '0', 1024);
			valread = read(sock, username, 1024);
			if (valread == 0) return 0;

			sprintf(menu, "Password : \e[s\n\e[u");
			write(sock, menu, 1024); 
			memset(menu, '0', 1024);
			valread = read(sock, password, 1024); 
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
					while (1)
					{
						// char menu[1024] = {0};
						sprintf(menu, "LOGIN MENU\nadd\nCHOICE : \e[s\n\e[u");
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
							
							fprintf(fp_1, "%s\t%s\t%s", publisher, tahun, path);
							fclose(fp_1);
						}
					}
				}
			}
			fclose(fp);
		}

	}
		
	//Free the socket pointer
	free(socket_desc);
	
	return 0;
}
