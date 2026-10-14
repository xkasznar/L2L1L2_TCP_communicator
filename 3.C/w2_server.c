#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_MSG_SIZE 10

void error(const char *);
int main(int argc, char *argv[])
{
   int sockfd, newsockfd, servlen, n;
   socklen_t clilen;
   struct sockaddr_un  cli_addr, serv_addr;
   char buf[MAX_MSG_SIZE];

   if ((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
       error("creating socket");
       
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sun_family = AF_UNIX;
   if(argc != 2){
       printf("averted seg fault, please provide socket\n");
       exit(1);
   }
   strcpy(serv_addr.sun_path, argv[1]);
   
   unlink(serv_addr.sun_path); //hotfix bind failure
   
   servlen=strlen(serv_addr.sun_path) + 
                     sizeof(serv_addr.sun_family);
   if(bind(sockfd,(struct sockaddr *)&serv_addr,servlen)<0)
       error("binding socket"); 

   listen(sockfd,5);
   
   LISTEN_AGAIN:
   clilen = sizeof(cli_addr);
   newsockfd = accept(
        sockfd,(struct sockaddr *)&cli_addr,&clilen);
   if (newsockfd < 0) 
        error("accepting");
   n=read(newsockfd,buf,MAX_MSG_SIZE);
     printf("Here is the message: %s\n",buf);
    for(int i = 0; i < strlen(buf); i++){ //editing of text to uppercase
        buf[i] = toupper(buf[i]); //editing of text to uppercase
    }

   write(newsockfd,buf,strlen(buf));
   close(newsockfd);
   
   goto LISTEN_AGAIN;
   close(sockfd);
   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

