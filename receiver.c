#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 256
#define LOG_FILE "feed.log"

void log_to_file(const char *message) {
  FILE *logfile = fopen(LOG_FILE, "a");
  if (logfile == NULL) {
    perror("Error opening log file");
    exit(1);
  }
  fprintf(logfile, "%s", message);
  fclose(logfile);
}

int main() {
  int sockfd, newsockfd;
  struct sockaddr_in serv_addr, cli_addr;
  socklen_t clilen;
  char buffer[BUFFER_SIZE];
  int n;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Error opening socket");
    exit(1);
   }

   memset(&serv_addr, 0, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(SERVER_PORT);

   if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("Error on binding");
      exit(1);
  }

  listen(sockfd, 5);
  clilen = sizeof(cli_addr);
  
  printf("Server listening on port %d\n", SERVER_PORT);
  
  while ((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen))) {
    if (newsockfd < 0) {
      perror("Error on accept");
      continue;
     }

    printf("Connection established with client\n");

    while ((n = read(newsockfd, buffer, BUFFER_SIZE - 1)) > 0) {
	   buffer[n] = '\0';
	   printf("%s", buffer); 
           log_to_file(buffer);
     }

     if (n < 0) {
       perror("Error reading from socket"); 
     }

     close(newsockfd);

    }
  close(sockfd);
  return 0;
}
