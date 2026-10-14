#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

static const char HOSTNAME[] = "localhost";
#define PORTNO 52001
#define MAX_MSG_SIZE 300

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int emptyrun=0;
    EMPTYRUN: int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[MAX_MSG_SIZE];
    
    if (argc < 2) {
       fprintf(stderr,"usage %s SYNC_SKIP\n", argv[0]);
       exit(0);
    }
    int SYNC_SKIP = atoi(argv[1]);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);   
    if (sockfd < 0) 
        error("ERROR opening socket");
        
    server = gethostbyname(HOSTNAME);
    
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
   
    if(emptyrun==0)
        printf("Please enter the message:");
    bzero(buffer,MAX_MSG_SIZE);
    char finbuffer[MAX_MSG_SIZE];
    bzero(finbuffer,MAX_MSG_SIZE);
    if(emptyrun==0){
        fgets(buffer,MAX_MSG_SIZE-1,stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        memmove(finbuffer+SYNC_SKIP, buffer, strlen(buffer));
        memset(finbuffer, ' ', SYNC_SKIP);
    }
     
    n = write(sockfd,finbuffer,MAX_MSG_SIZE);

    if (n < 0) 
         error("ERROR writing to socket");
    close(sockfd);
    
    if(emptyrun==1)
        return 0;
    emptyrun=1;
    goto EMPTYRUN; //hotfix to enable arrival of messages to server, TODO: find cause of fails and delete this hotfix
    
    return 0;
}

