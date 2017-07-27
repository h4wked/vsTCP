#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>

#define PORT 4030
#define BUFFSIZE 1024

int main (int argc, char * argv[]) {

    int version; 
	fd_set master, readset;
	int sock, recbytes;
	char buf[BUFFSIZE];
	int buflen = sizeof( buf );
	struct sockaddr_in addr;
    struct sockaddr_in6 addr6;

    char choice;
    printf("Ipv6 benutzen? (y/n)\n");
    scanf("%c", &choice);
    if( choice == 'y' ) {
        version = 6;
	    addr6.sin6_family = AF_INET6;
	    addr6.sin6_port = htons( PORT );
    }else{
	    addr.sin_family = AF_INET;
	    addr.sin_port = htons( PORT );
    }
	printf("Geben Sie die IP des Servers ein!\n(0 = localhost)\n");
	char ip[20], name[40];
	scanf("%s", ip);
    if( version == 6 ) {
        if((strcmp(ip, "0\n") != 0 )){
            inet_pton(AF_INET6, "::1", &addr6.sin6_addr);
        }else{
            inet_pton(AF_INET6, ip,  &addr6.sin6_addr);
        }
    }else{
        if((strcmp(ip, "0\n") != 0 )){
            addr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
        }else{	
            addr.sin_addr.s_addr = inet_addr( ip );
        }
    }
	printf("Geben Sie ihren Namen ein!\n");
	scanf("%s", name);
					
	//sock erstellen

    if( version == 6 ) {
	    sock = socket( AF_INET6, SOCK_STREAM, 0 );
    }else{
        sock = socket( AF_INET, SOCK_STREAM, 0 );
    }
	if( sock < 0 ) {
		perror( "Creating Socket failed!" );
		exit( 1 );
	}

    if( version == 6 ) {
        if( connect( sock, ( struct sockaddr * ) &addr6, sizeof( addr6 ) ) < 0 ) {
            perror( "Binding Socket Failed" );
            exit( 1 );
        }else{
            printf("successfully connected!\n\n");
        }
    }else{
        if( connect( sock, ( struct sockaddr * ) &addr, sizeof( addr ) ) < 0 ) {
            perror( "Binding Socket Failed" );
            exit( 1 );
        }else{
            printf("successfully connected!\n\n");
        }
    }

	FD_SET( sock, &master );
	FD_SET( 0, &master );
	int maxfd = sock;
	write(sock, name, sizeof(name));

	//Senden der Nachicht

	while( 1 ) {
		bzero( buf, sizeof(buf));
		readset = master;
		select( maxfd+1, &readset, 0, 0, 0 );
		if( FD_ISSET( 0, &readset ) ) {

			//printf("Geben Sie die Nachicht ein die übertragen werden soll! \n");
			recbytes = read(0, buf, sizeof(buf));
			if( strcmp( buf, "quit\n") == 0 ) break;
			write( sock, buf, recbytes - 1 );
		}
		if( FD_ISSET( sock, &readset ) ) {
			if( (recbytes = read( sock, buf, sizeof(buf) )) == 0 ) {
				printf( "Server hung up!\n" );
				break;
			}
			printf( "%s\n", buf );
		}
	}

	//Beenden der Verbindung
	close( sock );

	return EXIT_SUCCESS;
}
