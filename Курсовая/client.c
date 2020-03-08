#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>


void game(int sockfd)
{
while (1);
}

	int main(int argc, char *argv[]) {
	    int sockfd = 0, n = 0;
	    char recvBuff[1024];
	    struct sockaddr_in serv_addr;
	    if(argc != 3) {
	        printf("\n Usage: %s <ip of server> \n",argv[0]);
	        return 1;
	    }
	    memset(recvBuff, '0',sizeof(recvBuff));
	    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	    {
	        printf("\n Error : Could not create socket \n");
	        return 1;
	    }
	    memset(&serv_addr, '0', sizeof(serv_addr));
	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_port = htons(atoi(argv[2]));
	    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
	    {
	        printf("\n inet_pton error occured\n");
	        return 1;
	    }
	    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	    {
	       printf("\n Error : Connect Failed \n");
	       return 1;
	    }
        int mod;
        read (sockfd, &mod, sizeof(int));
        mod = ntohs (mod);
        switch (mod)
        {
            case 1: 
            printf("Connection sucsess\n");
            printf("Choose gamemode (1-ManVsMan, 2-ManVsPS\n");
            scanf("%d", &mod);
	    mod = htons(mod);
            int check = 9;
            while (check!=0)
            {
            write (sockfd, &mod, sizeof(int));
            read (sockfd, &mod, sizeof(int));
            check = ntohs(mod);
	    mod = ntohs(mod);
            if (check==9)
            printf("Choose gamemode (1-ManVsMan, 2-ManVsPS\n");
            }
            printf("Sucsess\n");
            game(sockfd);
            break;
            case 2:
            printf("Connection sucsess\n");
            game(sockfd);
        }
    }
