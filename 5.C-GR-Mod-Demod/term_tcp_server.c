#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

#define PORTNO 52002
#define MAX_MSG_SIZE 300
#define DEBUG 1

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void altprint(char buffer[]){
    for(int i = 0; i < MAX_MSG_SIZE; i++){
        unsigned char c = (unsigned char)buffer[i];
        if(isprint(c))
            putchar(c);
        else
            putchar('X');
        if(i != 0 && (i+1)%100 == 0)
            putchar('\n');
    }
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd;
     socklen_t clilen;
     
     char buffer[MAX_MSG_SIZE];
     
     struct sockaddr_in serv_addr, cli_addr;
     
     int n;
    if (argc < 2) {
       fprintf(stderr,"usage %s SYNC_SKIP\n", argv[0]);
       exit(0);
    }
    int SYNC_SKIP = atoi(argv[1]);

     sockfd = socket(AF_INET, SOCK_STREAM, 0);

     if (sockfd < 0) 
        error("ERROR opening socket");
        
     int optval = 1; //hotfix bind failure
     setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)); //hotfix bind failure

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    
    serv_addr.sin_port = htons(PORTNO);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
        sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
     listen(sockfd,5);
    clilen = sizeof(cli_addr);
    
    LISTEN_AGAIN:
    
    newsockfd = accept(sockfd, 
             (struct sockaddr *) &cli_addr, 
             &clilen);
     if (newsockfd < 0)
          error("ERROR on accept");
          
     bzero(buffer,MAX_MSG_SIZE);
     n = read(newsockfd,buffer,MAX_MSG_SIZE-1);
     
     if (n < 0) error("ERROR reading from socket");

     if(DEBUG == 0)
         printf("Here is the message:%s\n",buffer+SYNC_SKIP+8);
     else{
         printf("Here is the message:\n");
         altprint(buffer);
         printf("First 10 bytes: [");
         for(int i = 0; i<10;i++)
             printf("%d,", buffer[i] );
         printf("]\n");
         printf("Last 10 bytes: [");
         for(int i = MAX_MSG_SIZE-10; i<MAX_MSG_SIZE;i++)
             printf("%d,", buffer[i] );
         printf("]\n");
     }
     
     close(newsockfd);

     goto LISTEN_AGAIN;

     close(sockfd);
     return 0; 
}

