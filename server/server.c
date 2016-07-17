#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "syslog.h"
#include "sys/types.h"    
#include "sys/socket.h"
#include "netinet/in.h"
#include "string.h"

int main (int argc, char *argv[])
{
	int socket_desc, client_sock, c, read_size;
	
	struct sockaddr_in server, client;
	
	char client_message[2000];

	//Socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc == -1)
	{
		printf ("could not crate sockect");
		return 1;
	}
	puts("Socket created\n");	

	server.sin_family      = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port        = htons(8888);

	//Bind	
	if(bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf ("could not bind\n ");
		return 1;
	}
	puts("bind done\n");

	///Listen
	listen(socket_desc , 3);

	c = sizeof(struct sockaddr_in);


	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}

	while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
	{
		//printf("write_message%s",client_message);
		//Send the message back to client
		write(client_sock , client_message , strlen(client_message));
	}

	if(read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}

	return 0;


}
