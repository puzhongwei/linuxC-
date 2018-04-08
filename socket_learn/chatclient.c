    #include <stdio.h>  
    #include <string.h>  
    #include <errno.h>  
    #include <sys/socket.h>  
    #include <resolv.h>  
    #include <stdlib.h>  
    #include <netinet/in.h>  
    #include <arpa/inet.h>  
    #include <unistd.h>  
    #include <sys/time.h>  
    #include <sys/types.h>  
       
    #define MAXBUF 1024  
      
    int main(int argc, char *argv[])  
    {  
        int s, len;  
        struct sockaddr_in addr;  
        char buffer[MAXBUF + 1];  
        fd_set afds, rfds;  
        struct timeval tv;  
          
        switch (argc)  
        {  
            case 3:  
                break;  
            default:  
                printf("usage: chatclient ip port\n");  
                exit(1);  
        }  
          
        if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)  
        {  
            printf("socket error!\n");  
            return -1;  
        }  
      
        addr.sin_family = AF_INET;  
        addr.sin_port = htons(atoi(argv[2]));  
        bzero((void*)&addr.sin_zero,8);  
        if(inet_aton(argv[1], (struct in_addr *) &addr.sin_addr.s_addr) == 0)  
        {  
            perror(argv[1]);  
            exit(errno);  
        }  
      
        if(connect(s, (struct sockaddr *) &addr, sizeof(addr)) != 0) {  
            perror("Connect ");  
            exit(errno);  
        }  
       
        printf("\nclient: ready for chatting!\n");  
        while(1)  
        {  
            int ret;  
            FD_ZERO(&rfds);  
            FD_ZERO(&afds);  
            FD_SET(0, &afds);  
            FD_SET(s, &afds);  
            tv.tv_sec = 1;  
            tv.tv_usec = 0; // set select time-out = 1s  
            memcpy(&rfds, &afds, sizeof(rfds));  
            ret = select(s + 1, &rfds, (fd_set *)0, (fd_set *)0, &tv);  
            if(ret < 0)  
            {  
                printf("client: select error %s", strerror(errno));  
                break;  
            }  
            else if(ret == 0)  
            {  
                continue;  
            }  
            else  
            {  
                if(FD_ISSET(0, &rfds)) //read user input process  
                {  
                    bzero(buffer, MAXBUF + 1);  
                    fgets(buffer, MAXBUF, stdin);  
                    if(!strncasecmp(buffer, "quit", 4))  
                    {  
                        printf("client: exit chatting!\n");  
                        break;  
                    }  
                    len = send(s, buffer, strlen(buffer) - 1, 0);  
                    if(len < 0)  
                    {  
                        printf("client: send failed\n");  
                        break;  
                    }  
                    else  
                        printf("client: %s\n",buffer);  
                }  
                if(FD_ISSET(s, &rfds)) //receive msg through socket  
                {  
                    bzero(buffer, MAXBUF + 1);  
                    len = recv(s, buffer, MAXBUF, 0);  
                    if(len > 0)  
                        printf("client recv:'%s'\n",buffer);  
                    else {  
                        if(len < 0)  
                            printf("client: recv failed\n");  
                        else  
                            printf("client: session end, because the other one quit\n");  
                        break;  
                    }  
                }  
            }  
        }  
        close(s);  
        return 0;  
    }  
