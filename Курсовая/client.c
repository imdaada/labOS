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

void tontohs (int*a)
{
    for (int i=0; i<9; i++)
        {
            a[i]=ntohs(a[i]);
        }
}
void tohtons (int*a)
{
    for (int i=0; i<9; i++)
        {
            a[i]=htons(a[i]);
        }
}
void printGame (int*mas)
{
    for (int i=0; i<9; i++)
    {
        if (mas[i]==0) printf(".");
        else if (mas[i]==1) printf("0");
        else printf("x");
        if ((i==2)||(i==5)||(i==8)) printf("\n");
    }
}

void game(int sockfd)
{
    int ind;
    int mas[9];
    read (sockfd, mas, 9*sizeof(int));
    tontohs (mas);
    printGame (mas);
    int r[2];
    while (1)
    {
        read (sockfd, &ind, sizeof(int));
        ind = ntohs (ind);
        if (ind == 1) //ход
        {
            while (1)
            {
                int check;
                printf ("Enter coordinates\n");
                scanf ("%d", &r[0]);
                scanf ("%d", &r[1]);
                r[0]=htons(r[0]);
                r[1]=htons(r[1]);
                write (sockfd, r, 2*sizeof(int));
                read (sockfd, &check, sizeof(int));
                if (check==0) break;
            }
            read (sockfd, mas, 9*sizeof(int));
            tontohs (mas);
            printGame (mas);
        }
        if (ind==0)
        {
            printf("Wait...\n");
            read (sockfd, mas, 9*sizeof(int));
            tontohs (mas);
            printGame (mas);
        }
        if (ind==2)
        {
            printf("You Win\n");
	    break;
        }
        if (ind==3)
        {
            printf("You Lose\n");
	    break;
        }
        if (ind==4)
        {
            printf("Stand-off\n");
	    break;
        }
    }
    
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
