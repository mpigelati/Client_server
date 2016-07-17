#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "syslog.h"
#include "sys/types.h"    
#include "sys/socket.h"
#include "netinet/in.h"
#include "string.h"

int main (int argc, char* argv[])
{
	int socket_fd, client_sock, client, read_size;
	
	struct sockaddr_in server_add, client_add;
	
	char client_message[2000];

	//Socket
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd == -1)
	{
		printf ("could not crate sockect");
		return 1;
	}
	puts("Socket created\n");	

	server_add.sin_family= AF_INET;
	server_add.sin_addr.s_addr=INADDR_ANY;
	server_add.sin_port=htons(8888);

	//Bind	
	if(bind(socket_fd,(struct sockaddr *)&server_add, sizeof(server_add)) < 0 )
	{

		printf ("could not bind\n ");
		return 1;
	}
	puts("bind done\n");


	///Listen
	listen(socket_fd , 3);

	client = sizeof(client_add);


	if(client_sock= accept(socket_fd, (struct sockaddr *)&client_add, (socklen_t*)&client)==-1)
		//if (client_sock < 0)
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
