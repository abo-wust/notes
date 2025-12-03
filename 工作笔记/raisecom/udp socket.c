/*
 * File : client.c
 * Author : Amine Amanzou
 *
 * Created : 4th January 2013
 *
 * Under GNU Licence
 */

#include <stdio.h>
#include <stdlib.h>

// Time function, sockets, htons... file stat
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>

// File function and bzero
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

/* Taille du buffer utilise pour envoyer le fichier
 * en plusieurs blocs
 */
#define BUFFERT 512

/* Commande pou génerer un fichier de test
 * dd if=/dev/urandom of=fichier count=8
 */

/* Declaration des fonctions*/
int duration (struct timeval *start,struct timeval *stop, struct timeval *delta);
int create_client_socket (int port, char* ipaddr);

struct sockaddr_in sock_serv;

int main (int argc, char**argv){
	struct timeval start, stop, delta;
    int sfd,fd;
    char buf[BUFFERT];
    off_t count=0, m,sz;//long
	long int n;
    int l=sizeof(struct sockaddr_in);
	struct stat buffer;
    
	if (argc != 4){
		printf("Error usage : %s <ip_serv> <port_serv> <filename>\n",argv[0]);
		return EXIT_FAILURE;
	}
    
    sfd=create_client_socket(atoi(argv[2]), argv[1]);
    
	if ((fd = open(argv[3],O_RDONLY))==-1){
		perror("open fail");
		return EXIT_FAILURE;
	}
    
	//taille du fichier
	if (stat(argv[3],&buffer)==-1){
		perror("stat fail");
		return EXIT_FAILURE;
	}
	else
		sz=buffer.st_size;
    
	//preparation de l'envoie
	bzero(&buf,BUFFERT);
    
	gettimeofday(&start,NULL);
    n=read(fd,buf,BUFFERT);
	while(n){
		if(n==-1){
			perror("read fails");
			return EXIT_FAILURE;
		}
		m=sendto(sfd,buf,n,0,(struct sockaddr*)&sock_serv,l);
		if(m==-1){
			perror("send error");
			return EXIT_FAILURE;
		}
		count+=m;
		//fprintf(stdout,"----\n%s\n----\n",buf);
		bzero(buf,BUFFERT);
        n=read(fd,buf,BUFFERT);
	}
	//read vient de retourner 0 : fin de fichier
	
	//pour debloquer le serv
	m=sendto(sfd,buf,0,0,(struct sockaddr*)&sock_serv,l);
	gettimeofday(&stop,NULL);
	duration(&start,&stop,&delta);
    
	printf("Nombre d'octets transférés : %lld\n",count);
	printf("Sur une taille total de : %lld \n",sz);
	printf("Pour une durée total de : %ld.%d \n",delta.tv_sec,delta.tv_usec);
    
    close(sfd);
    close(fd);
	return EXIT_SUCCESS;
}

/* Fonction permettant le calcul de la durée de l'envoie */
int duration (struct timeval *start,struct timeval *stop,struct timeval *delta)
{
    suseconds_t microstart, microstop, microdelta;
    
    microstart = (suseconds_t) (100000*(start->tv_sec))+ start->tv_usec;
    microstop = (suseconds_t) (100000*(stop->tv_sec))+ stop->tv_usec;
    microdelta = microstop - microstart;
    
    delta->tv_usec = microdelta%100000;
    delta->tv_sec = (time_t)(microdelta/100000);
    
    if((*delta).tv_sec < 0 || (*delta).tv_usec < 0)
        return -1;
    else
        return 0;
}

/* Fonction permettant la creation d'un socket
 * Renvoie un descripteur de fichier
 */
int create_client_socket (int port, char* ipaddr){
    int l;
	int sfd;
    
	sfd = socket(AF_INET,SOCK_DGRAM,0);
	if (sfd == -1){
        perror("socket fail");
        return EXIT_FAILURE;
	}
    
    //preparation de l'adresse de la socket destination
	l=sizeof(struct sockaddr_in);
	bzero(&sock_serv,l);
	
	sock_serv.sin_family=AF_INET;
	sock_serv.sin_port=htons(port);
    if (inet_pton(AF_INET,ipaddr,&sock_serv.sin_addr)==0){
		printf("Invalid IP adress\n");
		return EXIT_FAILURE;
	}
    
    return sfd;
}





-------------------------------------------


/*
 * File : server.c
 * Author : Amine Amanzou
 *
 * Created : 4th January 2013
 *
 * Under GNU Licence
 */

#include <stdio.h>
#include <stdlib.h>

// Time function, sockets, htons... file stat
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>

// File function and bzero
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

/* Taille du buffer utilise pour envoyer le fichier
 * en plusieurs blocs
 */
#define BUFFERT 512

/* Commande pou génerer un fichier de test
 * dd if=/dev/urandom of=fichier count=8
 */

/* Declaration des fonctions*/
int duration (struct timeval *start,struct timeval *stop, struct timeval *delta);
int create_server_socket (int port);

struct sockaddr_in sock_serv,clt;

int main (int argc, char**argv){
    //Descripteur
	int fd, sfd;
    
	char buf[BUFFERT];
	off_t count=0, n; // long type
	char filename[256];
    unsigned int l=sizeof(struct sockaddr_in);
	
    // Variable pour la date
	time_t intps;
	struct tm* tmi;
    
	if (argc != 2){
		printf("Error usage : %s <port_serv>\n",argv[0]);
		return EXIT_FAILURE;
	}
    
    sfd = create_server_socket(atoi(argv[1]));
    
	intps = time(NULL);
	tmi = localtime(&intps);
	bzero(filename,256);
	sprintf(filename,"clt.%d.%d.%d.%d.%d.%d",tmi->tm_mday,tmi->tm_mon+1,1900+tmi->tm_year,tmi->tm_hour,tmi->tm_min,tmi->tm_sec);
	printf("Creating the output file : %s\n",filename);
    
	//ouverture du fichier
	if((fd=open(filename,O_CREAT|O_WRONLY|O_TRUNC,0600))==-1){
		perror("open fail");
		return EXIT_FAILURE;
	}
    
	//preparation de l'envoie
	bzero(&buf,BUFFERT);
    n=recvfrom(sfd,&buf,BUFFERT,0,(struct sockaddr *)&clt,&l);
	while(n){
		printf("%lld of data received \n",n);
		if(n==-1){
			perror("read fails");
			return EXIT_FAILURE;
		}
		count+=n;
		write(fd,buf,n);
		bzero(buf,BUFFERT);
        n=recvfrom(sfd,&buf,BUFFERT,0,(struct sockaddr *)&clt,&l);
	}
    
	printf("Nombre d'octets transférés : %lld \n",count);
    
    close(sfd);
    close(fd);
	return EXIT_SUCCESS;
}

/* Fonction permettant le calcul de la durée de l'envoie */
int duration (struct timeval *start,struct timeval *stop,struct timeval *delta)
{
    suseconds_t microstart, microstop, microdelta;
    
    microstart = (suseconds_t) (100000*(start->tv_sec))+ start->tv_usec;
    microstop = (suseconds_t) (100000*(stop->tv_sec))+ stop->tv_usec;
    microdelta = microstop - microstart;
    
    delta->tv_usec = microdelta%100000;
    delta->tv_sec = (time_t)(microdelta/100000);
    
    if((*delta).tv_sec < 0 || (*delta).tv_usec < 0)
        return -1;
    else
        return 0;
}

/* Fonction permettant la creation d'un socket et son attachement au systeme
 * Renvoie un descripteur de fichier dans la table de descripteur du processus
 * bind permet sa definition aupres du systeme
 */
int create_server_socket (int port){
    int l;
	int sfd;
    
	sfd = socket(AF_INET,SOCK_DGRAM,0);
	if (sfd == -1){
        perror("socket fail");
        return EXIT_FAILURE;
	}
    
    //preparation de l'adresse de la socket destination
	l=sizeof(struct sockaddr_in);
	bzero(&sock_serv,l);
	
	sock_serv.sin_family=AF_INET;
	sock_serv.sin_port=htons(port);
	sock_serv.sin_addr.s_addr=htonl(INADDR_ANY);
    
	//Affecter une identité au socket
	if(bind(sfd,(struct sockaddr*)&sock_serv,l)==-1){
		perror("bind fail");
		return EXIT_FAILURE;
	}
    
    
    return sfd;
}







################################
/************************************************************************
File name - lfy_udp_server.c
Purpose - An udp server, which accepts data sent by a client,
and echoes back the same data to the client.
To compile - gcc lfy_udp_server.c -o myserver
To run - ./server 
Port Number of  server hardcoded as 11710 (can be changed)
************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<errno.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<sys/wait.h>
#include	<fcntl.h>
#include	<signal.h>

#define BUFSIZE 512
#define MYPORT 11710
#define MAXNAME 100 
int main(int argc,  char *argv[] )

{
	int	sd, numbytes,bytessent, ret;
	struct	sockaddr_in
		serveraddress,cliaddr;
	socklen_t length;
	char clientname[MAXNAME],datareceived[BUFSIZE];

	sd = socket( AF_INET, SOCK_DGRAM, 0 );
	if(0 > sd ) 
	{
		perror( "socket" );
		exit( 1 );
	}
	

	memset( &serveraddress, 0, sizeof(serveraddress) );
	memset( &cliaddr, 0, sizeof(cliaddr) );
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(MYPORT);//PORT NO
	serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);//IP ADDRESS
	ret=bind(sd,(struct sockaddr*)&serveraddress,sizeof(serveraddress));
	if(0 > ret)
	{
		perror("Bind Fails:");
		exit(1);
	}
	for(;;)
	{
		printf("UDP Server:  Waiting for client data\n");
		length=sizeof(cliaddr);
		/*Received a datagram*/
		numbytes = recvfrom(sd,datareceived,BUFSIZE,0,
				(struct sockaddr*)&cliaddr, &length);
		if (0 > numbytes)
		{
			perror("Error while receiving:");
			exit(1);
		}
		/*Printing client's address*/
		printf("Data Received from %s\n",
				inet_ntop(AF_INET,&cliaddr.sin_addr,
					clientname,sizeof(clientname)));
		
		/*Sending the Received datagram back*/
		datareceived[numbytes]='\0';
		printf("Server Receives: %s\n",datareceived);
		bytessent = sendto(sd,datareceived,numbytes,0,(struct sockaddr *)&cliaddr,length);
		if (0 > bytessent)
		{
			perror("Error while sending:");
			exit(1);
		}
	}
}





-------------------


/************************************************************************
File name - lfy_udp_server.c
Purpose - An udp server, which accepts data sent by a client,
and echoes back the same data to the client.
To compile - gcc lfy_udp_server.c -o myserver
To run - ./server 
Port Number of  server hardcoded as 11710 (can be changed)
************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<errno.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<sys/wait.h>
#include	<fcntl.h>
#include	<signal.h>

#define BUFSIZE 512
#define MYPORT 11710
#define MAXNAME 100 
int main(int argc,  char *argv[] )

{
	int	sd, numbytes,bytessent, ret;
	struct	sockaddr_in
		serveraddress,cliaddr;
	socklen_t length;
	char clientname[MAXNAME],datareceived[BUFSIZE];

	sd = socket( AF_INET, SOCK_DGRAM, 0 );
	if(0 > sd ) 
	{
		perror( "socket" );
		exit( 1 );
	}
	

	memset( &serveraddress, 0, sizeof(serveraddress) );
	memset( &cliaddr, 0, sizeof(cliaddr) );
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(MYPORT);//PORT NO
	serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);//IP ADDRESS
	ret=bind(sd,(struct sockaddr*)&serveraddress,sizeof(serveraddress));
	if(0 > ret)
	{
		perror("Bind Fails:");
		exit(1);
	}
	for(;;)
	{
		printf("UDP Server:  Waiting for client data\n");
		length=sizeof(cliaddr);
		/*Received a datagram*/
		numbytes = recvfrom(sd,datareceived,BUFSIZE,0,
				(struct sockaddr*)&cliaddr, &length);
		if (0 > numbytes)
		{
			perror("Error while receiving:");
			exit(1);
		}
		/*Printing client's address*/
		printf("Data Received from %s\n",
				inet_ntop(AF_INET,&cliaddr.sin_addr,
					clientname,sizeof(clientname)));
		
		/*Sending the Received datagram back*/
		datareceived[numbytes]='\0';
		printf("Server Receives: %s\n",datareceived);
		bytessent = sendto(sd,datareceived,numbytes,0,(struct sockaddr *)&cliaddr,length);
		if (0 > bytessent)
		{
			perror("Error while sending:");
			exit(1);
		}
	}
}





