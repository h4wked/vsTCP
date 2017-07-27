#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 4030
#define MAXSOCKETS 50
#define BUFFSIZE 1024

int main (int argc, char * argv[]) {
	//Variablenliste
    int version;
    if( (version = atoi(argv[1]) == 0) ) {
        perror("invalid version option! using ipv4!");
    }
    struct sockaddr_in myAddr, cliAddr;
    struct sockaddr_in6 myAddr6, cliAddr6;
	int lfd; 
	char * name_array[100];
	memset(name_array, 0, sizeof(name_array));
	int newfd, maxfd, rc, currentWriter;
	int recbytes, sendbytes;
	char buf[BUFFSIZE];
	int nfd[MAXSOCKETS], nfdcpy[MAXSOCKETS];
    int opt = 1;
	fd_set fdset, read_fdset;
	//socket binden
    if(argc < 2){
        memset( &myAddr, 0, sizeof( struct sockaddr_in ));
        myAddr.sin_addr.s_addr = INADDR_ANY;
        myAddr.sin_family = AF_INET;
        myAddr.sin_port = htons( PORT );
        lfd = socket( AF_INET, SOCK_STREAM, 0);
        if( setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
                perror("setsockopt failed");
                return EXIT_FAILURE;
        }
        rc = bind( lfd, (struct sockaddr *) &myAddr, sizeof( struct sockaddr_in ));
        printf("Socket binded.\nUsing IPv4!\n");
    
    }else{
        memset( &myAddr, 0, sizeof( struct sockaddr_in ));
        myAddr6.sin6_addr = in6addr_any;
        myAddr6.sin6_family = AF_INET6;
        myAddr6.sin6_port = htons( PORT );
        lfd = socket( AF_INET6, SOCK_STREAM, 0);
        if( setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
                perror("setsockopt failed");
                return EXIT_FAILURE;
        }

        rc = bind( lfd, (struct sockaddr *) &myAddr6, sizeof( struct sockaddr_in6 ));
        printf("Socket binded.\nUsing IPv6!\n");
    
    }
	if ( rc < 0 ) {
		perror("Problem opening Socket!\n");
		exit( 1 );
	}

	if( listen( lfd, 10 ) == -1 ) {
		perror( "Listen Failed!!");
		exit(1);
	}

	printf(  "Listen Established!\n");
	maxfd = lfd;
    int clilen;
    if( version == 6 ) {
	    clilen = sizeof( cliAddr6 );
    }else{
	    clilen = sizeof( cliAddr );
    }
    FD_SET( lfd, &fdset );

	while ( 1 ) {
		read_fdset = fdset;
		 if(select(maxfd+1, &read_fdset, NULL, NULL, NULL) == -1)	{
    		 	perror("Server-select() error lol!");
    			exit(1);
		}
		//printf("Server-select() is OK...\n");

		for( int i = 0; i <= maxfd; i++ ) { //Running through connections
			bzero( buf, sizeof(buf) );

			if( FD_ISSET( i, &read_fdset ) ) {

					if( i == lfd ) { //Handling new connection
                        if( version == 6 ) {
							newfd = accept( lfd, (struct sockaddr *) &cliAddr, &clilen );
                        }else{
							newfd = accept( lfd, (struct sockaddr *) &cliAddr6, &clilen );
                        }
						if ( newfd < 0 ) {
							perror( "Error Server-Accept!\n" );
							exit(1);
						}else{
							printf("Server-accept successful! Welcome %d\n", newfd);
							FD_SET( newfd, &fdset );
							if( newfd > maxfd ) {
								maxfd = newfd;
							}
						newfd = 0;
						}
					}else{ //Handling data to be read/end of connections
						bzero( buf, BUFFSIZE );
						
						recbytes = read( i, buf, sizeof(buf));
						//Namenshandling
						if(name_array[i] == NULL){
							name_array[i] = malloc(recbytes+1);
							strcpy(name_array[i], buf);
							continue;			
						}
						
						if( recbytes == 0 ) { //connection closed
							printf("socket %d hung up\n", i );
							if( i == maxfd) maxfd--;
							close( i );
							FD_CLR( i, &fdset );
							free(name_array[i]);
						}else if(recbytes == -1) {
							perror( "recv failed" );
						}else{
							printf("Client socket nr. %d \n", i );
							//We got data
							printf( "%s \n", buf);
							for( int j = 3; j <= maxfd; j++) {
								if( FD_ISSET( j, &fdset ) ) {
										if( (j != lfd) && (j != i) ) { //senden an alle ausser selbst & sender
											if( currentWriter != i ) {  //Namen senden
                                                currentWriter = i;
                                                write( j, " ", 1);
                                                usleep(2);
                                                write( j, name_array[i], sizeof(name_array[i]) );
                                                usleep(2);
                                                write( j, " ", 1);
                                            }
                                            usleep(2);
											sendbytes = write( j, buf, recbytes);
											//printf( "%s\n",buf );
										}
								}
							}//end going through sends
						}//end of sending
					}//end Handling recv
			}//end handling of FD_ISSET
		} //end running trough connections
	} //end while
	return 0;
}


