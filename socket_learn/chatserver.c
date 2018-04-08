/**************************************************/
/* Chat - Server                                  */
/**************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
 
#define MAXBUF 1024
#define QLEN 5
 
int main(int argc, char *argv[])
{
	fd_set afds, rfds;
	struct sockaddr_in addr_host, addr_client;
    int msock, ssock;
    socklen_t len;
    unsigned int port;
    char buf[MAXBUF + 1];
    struct timeval tv;
    
	switch (argc)
	{
		case 2:
			port = atoi(argv[1]);
			break;
		default:
			printf("usage: chatserver port\n");
			exit(1);
	}
	
    msock=socket(AF_INET,SOCK_STREAM,0);
	if(msock<0)
	{
		printf("socket error!\n");
		return -1;
	}

    addr_host.sin_family = AF_INET;
	addr_host.sin_addr.s_addr=htonl(INADDR_ANY);
    addr_host.sin_port = htons(port);
	bzero((void *)&addr_host.sin_zero, 8);
   
 
    if(bind(msock,(struct sockaddr*)&addr_host,sizeof(addr_host))<0)
	{
		close(msock);
		printf("bind error!\n");
		return -1;
	}
	if(listen(msock,QLEN)<0)
	{
        printf("LISTEN error");
		return -1;
    }
 
    while(1)
	{
        printf("\nserver: waiting...\n");
        len = sizeof(struct sockaddr);
        if((ssock =accept(msock, (struct sockaddr *) &addr_client, &len)) == -1) {
            perror("accept");
            exit(errno);
        }
		else
            printf("server: got connection from %s, port %d, socket %d\n",
                   inet_ntoa(addr_client.sin_addr),
                   ntohs(addr_client.sin_port), ssock);
        printf("\nserver: ready for chatting!\n");
        while(1)
		{
			int ret;
            FD_ZERO(&rfds);
			FD_ZERO(&afds);
			FD_SET(0, &afds);
			FD_SET(ssock, &afds);
            tv.tv_sec = 1;
            tv.tv_usec = 0; // set select time-out = 1s
			memcpy(&rfds, &afds, sizeof(rfds));
            ret = select(ssock + 1, &rfds, (fd_set *)0, (fd_set *)0, &tv);
            if(ret < 0)
			{

            }
			else if(ret == 0)
			{
                continue;
            }
			else
			{
                if(FD_ISSET(0, &rfds)) //read user input process
				{
                    bzero(buf, MAXBUF + 1);
                    fgets(buf, MAXBUF, stdin);
                    if(!strncasecmp(buf, "quit", 4))
					{                        printf("server: exit chatting!\n");
                        break;
                    }
                    len = send(ssock, buf, strlen(buf) - 1, 0);
                    if(len > 0)
                        printf("server:%s\n",buf);
                    else
					{
                        printf("server:send failed\n");
                       break;
                    }
                }
                if(FD_ISSET(ssock, &rfds)) //receive msg through socket
				{
                bzero(buf, MAXBUF + 1);
                    len = recv(ssock, buf, MAXBUF, 0);
                    if(len > 0)
                        printf("server recv:'%s'\n",buf);
                    else
					{
                        if(len < 0)
                            printf("server:recv failed\n");
                        else
                            printf("server: session end, because the other one quit\n");
                        break;
                    }
                }
            }
        }
        close(ssock);
		//decide whether continue chatting
        printf("Continue chatting?(no=>exit)");
        fflush(stdout);
        bzero(buf, MAXBUF + 1);
        fgets(buf, MAXBUF, stdin);
        if(!strncasecmp(buf, "no", 2))
		{
            printf("server: Terminating chat!\n");
            break;
        }
    }
 
    close(msock);
    return 0;
}
