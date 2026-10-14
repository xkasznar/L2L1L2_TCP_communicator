#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h> //lowercase conversion

#define MAX_MSG_SIZE 256 //new rule about message size
#define PORTNO 52001

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char portstr[10];


    char buffer[MAX_MSG_SIZE];
    if (argc != 2) {
       fprintf(stderr,"usage %s hostname\n", argv[0]);
       exit(0);
    }

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
         
    if (sockfd < 0) 
        error("ERROR opening socket");
        

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }        

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(PORTNO);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
   

    printf("client sending message:");
    bzero(buffer,MAX_MSG_SIZE);
    fgets(buffer,MAX_MSG_SIZE,stdin);
    
    /*for(int i = 0; i < strlen(buffer); i++){ //editing of text to lowercase
        buffer[i] = tolower(buffer[i]); //editing of text to lowercase
    }*/
    
    buffer[strcspn(buffer, "\n")] = '\0'; //hotfix to fix trailing '\n'
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,MAX_MSG_SIZE);
    n = read(sockfd,buffer,MAX_MSG_SIZE);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("client received reply from server:%s\n",buffer);
    close(sockfd);
    return 0;
}

