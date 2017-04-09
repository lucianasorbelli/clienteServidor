
struct sockaddr_in initAddr(in_addr_t address,int puerto){
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	// Ordenación de máquina
	my_addr.sin_port = htons(puerto);
	// short, Ordenación de la red
	my_addr.sin_addr.s_addr = address;
	if(my_addr.sin_addr.s_addr == -1){
		perror("BroadcastError\n");
		exit(1);
	}
	memset(&(my_addr.sin_zero), '\0', 8);
	return my_addr;
}

void sigchld_handler(int s)
{
	while(wait(NULL) > 0);
}

int initSocket(void){
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);// ¡Comprueba que no hay errores!
	if(sockfd ==-1 ){
		perror("Socket:");
		exit(1);
	}
	return sockfd;
}
