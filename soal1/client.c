#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
  
int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

	char choice[1024];
	while (1)
	{
		// memset(buffer, '0', 1024);
        char username[1024];
        char password[1024];
		valread = read(sock, buffer, 1024);
		printf("%s", buffer);
        memset(buffer, '0', 1024);

		scanf("%s", choice);
        write(sock, choice, strlen(choice));
        if (!(strcmp(choice, "1")))
        {
            valread = read(sock, buffer, 1024);
		    printf("%s", buffer);
            scanf("%s", username);
            write(sock, username, 1024);

            valread = read(sock, buffer, 1024);
		    printf("%s", buffer); 
            scanf("%s", password);
            write(sock, password, 1024);
        }
        else if (!(strcmp(choice, "2")))
        {
            while (1)
            {
                char aut[1024];
                valread = read(sock, buffer, 1024);
                printf("%s", buffer);
                scanf("%s", username);
                write(sock, username, 1024);

                valread = read(sock, buffer, 1024);
                printf("%s", buffer); 
                scanf("%s", password);
                write(sock, password, 1024);

                // while(aut != 0)
                // {

                // }

                valread = read(sock, aut, 1024);
                while (!strcmp(aut, "1"))
                {
                    printf("\nLOGIN BERASIL\n");    
                    valread = read(sock, buffer, 1024);
                    printf("%s", buffer);
                    memset(buffer, '0', 1024);
                    scanf("%s", choice);
                    write(sock, choice, 1024);
                    if (!strcmp(choice, "add"))
                    {
                        char kirim[1024] = {0};
                        valread = read(sock, buffer, 1024);
                        printf("%s", buffer);
                        memset(buffer, '0', 1024);
                        scanf("%s", kirim);
                        write(sock, kirim, 1024);
                        memset(kirim, '0', 1024);
                        
                        valread = read(sock, buffer, 1024);
                        printf("%s", buffer);
                        memset(buffer, '0', 1024);
                        scanf("%s", kirim);
                        write(sock, kirim, 1024);
                        memset(kirim, '0', 1024);
                        
                        valread = read(sock, buffer, 1024);
                        printf("%s", buffer);
                        scanf("%s", kirim);
                        write(sock, kirim, 1024);
                        memset(kirim, '0', 1024);
                    }
                    break;
                }
            }

        }
	}
	
    return 0;
}