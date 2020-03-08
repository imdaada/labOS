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

int checkWin (int *mas)
{
    if((mas[0]==1)&&(mas[1]==1)&&(mas[2]==1))
    {
        return 1;
    }
    if((mas[3]==1)&&(mas[4]==1)&&(mas[5]==1))
    {
        return 1;
    }
    if((mas[6]==1)&&(mas[7]==1)&&(mas[8]==1))
    {
        return 1;
    }
    if((mas[0]==1)&&(mas[3]==1)&&(mas[6]==1))
    {
        return 1;
    }
    if((mas[1]==1)&&(mas[4]==1)&&(mas[7]==1))
    {
        return 1;
    }
    if((mas[2]==1)&&(mas[5]==1)&&(mas[8]==1))
    {
        return 1;
    }
    if((mas[0]==1)&&(mas[4]==1)&&(mas[8]==1))
    {
        return 1;
    }
    if((mas[2]==1)&&(mas[4]==1)&&(mas[6]==1))
    {
        return 1;
    }
    if((mas[0]==2)&&(mas[1]==2)&&(mas[2]==2))
    {
        return 2;
    }
    if((mas[3]==2)&&(mas[4]==2)&&(mas[5]==2))
    {
        return 2;
    }
    if((mas[6]==2)&&(mas[7]==2)&&(mas[8]==2))
    {
        return 2;
    }
    if((mas[0]==2)&&(mas[3]==2)&&(mas[6]==2))
    {
        return 2;
    }
    if((mas[1]==2)&&(mas[4]==2)&&(mas[7]==2))
    {
        return 2;
    }
    if((mas[2]==2)&&(mas[5]==2)&&(mas[8]==2))
    {
        return 2;
    }
    if((mas[0]==2)&&(mas[4]==2)&&(mas[8]==2))
    {
        return 2;
    }
    if((mas[2]==2)&&(mas[4]==2)&&(mas[6]==2))
    {
        return 2;
    }
    if((mas[0]!=0)&&(mas[1]!=0)&&(mas[2]!=0)&&(mas[3]!=0)&&(mas[4]!=0)&&(mas[5]!=0)&&(mas[6]!=0)&&(mas[7]!=0)&&(mas[8]!=2))
    {
        return 3;
    }
    return -1;
            
}
void gamevsMan (int connfd1, int connfd2)
{
        int mas[9]; //игровое поле
        for (int i=0; i<9; i++)
        {
            mas[i]=0;
        }
        tohtons(mas);
        write (connfd1, mas, 9*sizeof(int));
        write (connfd2, mas, 9*sizeof(int));
        tontohs(mas);
        int q=0;
        int ind;
        int r[2];
        while(1)
        {
            if(q%2==0) //ход 1 игока
            {
                ind=htons(1);
                write (connfd1, &ind, sizeof(int));
                ind=htons(0);
                write (connfd2, &ind, sizeof(int));
                while (1)
                {
                    read (connfd1, r, 2*sizeof(int));
                    r[0]=ntohs(r[0]);
                    r[1]=ntohs(r[1]);
                    if (mas[(r[0]-1)*3+(r[1]-1)]==0)
                    {
                        ind=htons(0);
                        write (connfd1, &ind, sizeof(int)); //0-успешный ход
                        mas[(r[0]-1)*3+(r[1]-1)]=1;
                        tohtons(mas);
                        write (connfd1, mas, 9*sizeof(int));
                        write (connfd2, mas, 9*sizeof(int));
                        tontohs(mas);
                        break;
                    }
                    else
                    {
                        ind=htons(9);
                        write (connfd1, &ind, sizeof(int)); //9-неправильный ход
                    }

                }
            }
            else //ход 2 игока
            {
                ind=htons(1);
                write (connfd2, &ind, sizeof(int));
                ind=htons(0);
                write (connfd1, &ind, sizeof(int));
                while (1)
                {
                    read (connfd2, r, 2*sizeof(int));
                    r[0]=ntohs(r[0]);
                    r[1]=ntohs(r[1]);
                    if (mas[(r[0]-1)*3+(r[1]-1)]==0)
                    {
                        ind=htons(0);
                        write (connfd2, &ind, sizeof(int)); //0-успешный ход
                        mas[(r[0]-1)*3+(r[1]-1)]=2;
                        tohtons(mas);
                        write (connfd1, mas, 9*sizeof(int));
                        write (connfd2, mas, 9*sizeof(int));
                        tontohs(mas);
                        break;
                    }
                    else
                    {
                        ind=htons(9);
                        write (connfd2, &ind, sizeof(int)); //9-неправильный ход
                    }
                }
            }
            q++;
            if (checkWin(mas)==1)
            {
                ind=htons(2);
                write (connfd1, &ind, sizeof(int)); //2 - код победы
                ind=htons(3);
                write (connfd2, &ind, sizeof(int)); //3 - код проигрыша
		break;
            }
            else if (checkWin(mas)==2)
            {
                ind=htons(2);
                write (connfd2, &ind, sizeof(int)); //2 - код победы
                ind=htons(3);
                write (connfd1, &ind, sizeof(int)); //3 - код проигрыша
		break;
            }
            else if (checkWin(mas)==3)
            {
                ind=htons(4);
                write (connfd1, &ind, sizeof(int)); //4 - ничья
                write (connfd2, &ind, sizeof(int)); 
		break;
            }
        }

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
            //break;
        }
}
