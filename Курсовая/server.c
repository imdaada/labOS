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

int getMode (int connfd)
{
    int type;
    read (connfd, &type, sizeof(int)); //ждём выбора режима игры (1 - с другим игроком, 2 - с компьютером)
    type = ntohs (type);
    return type;
}
void gamevsMan (int connfd1, int connfd2)
{

}

void gamevsPC (int connfd)
{

}


int main(int argc, char *argv[]) {
	    int listenfd = 0, connfd1 = 0, connfd2 = 0;
	    struct sockaddr_in serv_addr;
	    char sendBuff[1025];
	    time_t ticks;
	    listenfd = socket(AF_INET, SOCK_STREAM, 0);
	    memset(&serv_addr, '0', sizeof(serv_addr));
	    memset(sendBuff, '0', sizeof(sendBuff));

	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	    serv_addr.sin_port = htons(atoi(argv[1]));
	    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	    listen(listenfd, 10);
        while (1)
        {
            
            connfd1 = accept(listenfd, (struct sockaddr*)NULL, NULL); //ждем подключеня 1 клиента
            int mod=htons(1); 
            write (connfd1, &mod, sizeof(int)); // 1-клиент подключен как клиент1
            int i = 1;
            while (1)
            {
            int f = 0;
	    int j;
	    int i;
            switch (getMode(connfd1)) {
                case 1:
                i=htons(0); 
                write (connfd1, &i, sizeof(int)); //0- режим выбран успешно
                connfd2 = accept(listenfd, (struct sockaddr*)NULL, NULL); //ждем подключеня 2 клиента
                i=htons(2); 
                write (connfd2, &i, sizeof(int)); //2-клиент подключен как клиент2
                gamevsMan(connfd1, connfd2); 
                f=0;
                break;
                case 2:
                j=htons(0); 
                write (connfd1, &j, sizeof(int)); //0- режим выбран успешно
                gamevsPC(connfd1);
                f=0;
                break;
                default:
                j=htons(9); 
                write (connfd1, &j, sizeof(int)); //9 - ошибка выбора режима
                f = -1;
                break;
            }
            if (f==0) break;
            }



            close (connfd1);
            close (connfd2);
            break;
        }
}
