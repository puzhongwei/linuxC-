#include<sys/socket>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char* argv[])
{
	if(argc <= 2)
	{
		printf("usage : %s ip_address port_number \n", basename(argv[0]));
		return 1;
	}

	const char * ip = argv[1];
	int port  = atoi(argv[2]);
	struct sockaddr server_address;
	bzero( &server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &server_address.sin_addr);
	server_address.sin_port = htons(port);
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(sockfd >= 0);
	if(connect(sockfd, (struct * sockaddr)&server_address, sizeof(server_address)) < 0)
	{
		printf("connect failed!\n");
	}
	else
	{
		const char * obb_data = "abc";
		const char * nomal_data = "123";
		send(sockfd, nomal_data, strlen(obb_data), MSG_OOB);
	}
}
