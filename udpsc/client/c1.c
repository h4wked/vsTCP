#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../datastruct.h"
#define PORT 4030

int main () {

//Varlist
struct sockaddr_in myaddr, src_addr;
int skt, rc, len, timeout;
datastruct messageStruct;
messageStruct.version_id = htons(1);
//socket erstellen
if ((skt = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	perror("Konnte kein socket erzeugen,\n");
	return -1;
	}
//socketaddress "füllen"
memset(&myaddr, 0, sizeof(struct sockaddr_in));
myaddr.sin_family = AF_INET;
myaddr.sin_port = htons(PORT);
myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//rc = bind(skt, (struct sockaddr *) &myaddr, sizeof(struct sockaddr_in));

char usrInput[1000];

len = sizeof(struct sockaddr_in);

timeout = 5; //in sec
fd_set fdset;
FD_ZERO( &fdset );
FD_SET( skt, &fdset );
struct timeval tv_timeout = { timeout, 0 };

int select_retval; 

char answ;

do{
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
    bzero((void *) &messageStruct, sizeof(messageStruct));
    printf("\nBitte geben Sie die Nachicht ein die Uebertragen werden soll!\n");
    scanf("%[^\t\n]", &usrInput);
    strcpy(messageStruct.data, usrInput);

    for (int i = 0; i < 3; i++) {
        (void) sendto(skt, &messageStruct, sizeof(messageStruct), 0, (const struct socketaddr *) &myaddr, sizeof(src_addr));
        tv_timeout.tv_sec = 5;	
        tv_timeout.tv_usec = 0;
        select_retval = select( skt+1, &fdset, NULL, NULL, &tv_timeout );


        if ( select_retval == -1 ) {
            perror("Select gescheitert");
        return EXIT_FAILURE;

        }else if (select_retval == 0) {
            if(i==2) {
                perror("Timeout!");
                return EXIT_FAILURE;
            }
            continue;
        }else{
        if(recvfrom(skt, &messageStruct, sizeof(messageStruct), 0, (const struct socketaddr *) &src_addr, &len))break;
        }
    }

    //Umwandlung Metadaten
    messageStruct.version_id = ntohs(messageStruct.version_id);
    printf("\nServerantwort: vs_id = %hu Antwortzeit = %dsec & %dusec \nNachricht = %s\n", messageStruct.version_id, 4-tv_timeout.tv_sec,1000000-tv_timeout.tv_usec, messageStruct.data);

    while(getchar() != '\n');
    printf("\nWeitere Nachicht senden?\n");
    answ = getchar();
    
    }while(answ != 'n'); 

return 0;
}


