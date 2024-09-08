#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define INITIAL_PRICE 100.0
#define VOLATILITY 0.2
#define DRIFT 0.01
#define MIN_PRICE 1.0
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define SLEEP_TIME 1

double normal_random() {
  double u1 = rand() / (double)RAND_MAX;
  double u2 = rand() / (double)RAND_MAX;
  double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
  return z;
}

void generate_prices_to_socket(double initial_price, double volatility, double drift, double min_price, const char* server_ip, int server_port) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Error opening socket");
    exit(1);
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(server_port);
  inet_pton(AF_INET, server_ip, &serv_addr.sin_addr);

  if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Error connecting to server");
    close(sockfd);
    exit(1);
  }

  double price = initial_price; 
  char buffer[256];

  while (1) {
        snprintf(buffer, sizeof(buffer), "%.2f\n", price);
	send(sockfd, buffer, strlen(buffer), 0);

       double random_shock = normal_random() * volatility; 
       price = price * (1.0 + drift + random_shock);
      
       if (price < min_price) {
	price = min_price;
        }
	
	//snprintf(buffer, sizeof(buffer), "%.2f\n", price);
	//send(sockfd, buffer, strlen(buffer), 0);
       //printf("%.2f\n", price);
      sleep(SLEEP_TIME);
     }
  close(sockfd);
}

int main() { 
	srand(time(NULL));
	generate_prices_to_socket(INITIAL_PRICE, VOLATILITY, DRIFT, MIN_PRICE, SERVER_IP, SERVER_PORT);
	return 0;
}
