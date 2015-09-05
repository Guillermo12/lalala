#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define MYPORT 22225

int main()
{
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family= AF_INET;
	direccionServidor.sin_port=htons(MYPORT);
	direccionServidor.sin_addr.s_addr=INADDR_ANY;
	memset(&(direccionServidor.sin_zero),'\0',8);

	int servidor = socket(AF_INET,SOCK_STREAM,0);

	int activado=1;
	setsockopt(servidor,SOL_SOCKET,SO_REUSEADDR,&activado,sizeof(activado));

	if(bind(servidor,(struct sockaddr*)&direccionServidor,sizeof(struct sockaddr))!=0)
	{
		perror("Fallo el bind");
		return 1;
	}

	printf("Socket: %d\n",servidor);
	printf("Estoy escuchando y esperando\n");
	listen(servidor,10);

	//-----------------------------------------------------

	struct sockaddr_in direccionCliente;
	unsigned int tamanioDireccion = sizeof(struct sockaddr_in);
	int cliente = accept(servidor,(struct sockaddr*) &direccionCliente,&tamanioDireccion);

	printf("Puerto del cliente: %hu\n",htons(direccionCliente.sin_port));
	//printf("Direccion del cliente %lu\n",htonl(direccionCliente.sin_addr.s_addr));
	printf("Recibi una conexion en %d\n",cliente);

	send(cliente,"Enviame el header: ",20,0);

	//-------------------------------------------------------------

	char* arrTamanio=malloc(4);
		int tamanioEnviado = recv(cliente,arrTamanio,3,0);
		arrTamanio[tamanioEnviado]='\0';

		int tamanio= atoi(arrTamanio);
		char* buffer= malloc(tamanio);
		send(cliente,"Enviame el mensaje: ",21,0);

		int bytesRecibidos = recv(cliente,buffer,tamanio-1,MSG_WAITALL);
	    buffer[bytesRecibidos]='\0';

	    printf("Mensaje Recibido: %s",buffer);
		free(buffer);

		//for(;;);
		return 0;
}


