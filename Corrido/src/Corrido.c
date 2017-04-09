/*
 ============================================================================
 Name        : Corrido.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include "lib.h"

#define CANTCONEXIONES 16
#define PORT 3885

typedef struct{
	uint32_t load;
	char nombre[20];
}Miauri;

int main(void) {
	struct sigaction sa;
	int server,cliente,yes= 1;
	struct sockaddr_in miData, conexionEntrante;
	miData = initAddr(INADDR_ANY,PORT);
	server = initSocket();
	if (bind(server, (struct sockaddr*)&miData, sizeof(struct sockaddr_in)) == -1) {
		perror("bind");
		exit(1);
	}
	if (setsockopt(server,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}

	if (listen(server, CANTCONEXIONES) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // Eliminar procesos muertos
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	int num =sizeof(struct sockaddr_in);
	Miauri aux;
	while(1) { // main accept() loop

		if ((cliente = accept(server, (struct sockaddr *)&conexionEntrante,&(num))) == -1) {
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n", inet_ntoa(conexionEntrante.sin_addr));
		int bytesRecibidos = recv(cliente, &aux, sizeof(Miauri), 0);
		if(bytesRecibidos <= 0){
			perror("Desconectado");
			return 1;
		}
		printf("Nro:%d\nNombre:%s\n",aux.load,aux.nombre);
	}


	return EXIT_SUCCESS;
}
