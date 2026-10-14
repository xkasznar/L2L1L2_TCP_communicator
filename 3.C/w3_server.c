#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //ipv6
#include <ctype.h>

#define PORTNO 5001
#define USE_IPV6 0

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd;
     socklen_t clilen;
     char buffer[256];
     
     struct sockaddr_in serv_addr, cli_addr;
     struct sockaddr_in6 serv_addr6, cli_addr6;
     
     int n;
     if(USE_IPV6 == 0)
         sockfd = socket(AF_INET, SOCK_STREAM, 0);
     else
         sockfd = socket(AF_INET6, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
        
     int optval = 1; //hotfix bind failure
     setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)); //hotfix bind failure
     
    if(USE_IPV6 == 0){
        bzero((char *) &serv_addr, sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(PORTNO);
        if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) 
            error("ERROR on binding");
    }
    else{   
        bzero((char *) &serv_addr6, sizeof(serv_addr6));

        serv_addr6.sin6_family = AF_INET6;
        serv_addr6.sin6_addr = in6addr_any;
        serv_addr6.sin6_port = htons(PORTNO);
        if (bind(sockfd, (struct sockaddr *) &serv_addr6,
            sizeof(serv_addr6)) < 0) 
        error("ERROR on binding");
    }
  
     listen(sockfd,5);
     LISTEN_AGAIN:
     if(USE_IPV6 == 0){
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     }
     else{
        clilen = sizeof(cli_addr6);
        newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr6, 
                 &clilen);
     }
     if (newsockfd < 0)
          error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
     
     
    for(int i = 0; i < strlen(buffer); i++){ //editing of text to uppercase
        buffer[i] = toupper(buffer[i]); //editing of text to uppercase
    }
     n = write(newsockfd,buffer,strlen(buffer));
     if (n < 0) error("ERROR writing to socket");
     close(newsockfd);
     
     goto LISTEN_AGAIN;
     close(sockfd);
     return 0; 
}

