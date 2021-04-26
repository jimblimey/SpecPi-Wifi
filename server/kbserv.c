#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <wiringPi.h>
#define MAX 80
#define PORT 16384
#define SA struct sockaddr

const int datalines[5] = {26,19,13,6,5};
const int addresslines[8] = {25,24,23,22,27,18,17,4};

const char* keys[8][5] = {	
	{"1","2","3","4","5"},
	{"Q","W","E","R","T"},
	{"A","S","D","F","G"},
	{"0","9","8","7","6"},
	{"P","O","I","U","Y"},
	{"LEFTSHIFT","Z","X","C","V"},
	{"ENTER","L","K","J","H"},
	{"SPACE","LEFTCTRL","M","N","B"}
}; 

char keytrack[8][5] = {
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0}
};

void ProcessKeys(int sockfd) {
  int isFree; 
  int isChanged = 1;

  char buf[5];
  bzero(buf,5);
  
  char sbuf[40];
  bzero(sbuf, 40);
  
  printf("Reading hello from client.. ");
  read(sockfd,buf,5);
  printf("Done! (%s)\n",buf);
        
 while(1) {
    // Only write to socket if there's a change     
    if(isChanged == 1) {
      printf("Writing to socket.. ");
      write(sockfd, keytrack, sizeof(keytrack));
      isChanged = 0;
      printf("Done!\n");
    }    
    for(int i=0;i<8;i++) {
      digitalWrite(addresslines[i], LOW);
      for(int j=0;j<5;j++) {
        isFree = digitalRead(datalines[j]);
        // Key down
        if(isFree == 0 && keytrack[i][j] == 0) {
          printf("Pressed %s\n",keys[i][j]);
          keytrack[i][j] = 1;
          isChanged = 1;
        }
        // Key up
        else if(isFree == 1 && keytrack[i][j] == 1) {
          printf("Released %s\n",keys[i][j]);
          keytrack[i][j] = 0;
          isChanged = 1;
        }
      }
      digitalWrite(addresslines[i], HIGH);
      //usleep(100);
      delay(1);
    }        
  }    
}
  

int main() {    
  // Prepare GPIO
  wiringPiSetupGpio();

  for(int i=0;i<8;i++) {
    pinMode(addresslines[i],OUTPUT);
    digitalWrite(addresslines[i],HIGH);
  }

  for(int i=0;i<5;i++) {
    pullUpDnControl(datalines[i],2);
  }    
    
  // Prepare sockets
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;
  
  // socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd == -1) {
    printf("Error: Could not create socket\n");
    exit(0);
  }

  bzero(&servaddr, sizeof(servaddr));
  
  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  // Binding newly created socket to given IP and verification
  if((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
    printf("Error: Could not bind socket\n");
    exit(0);
  }

  // Now server is ready to listen and verification
  if((listen(sockfd, 5)) != 0) {
    printf("Error: could not listen on port %d\n", PORT);
    exit(0);
  }
  else {  
    printf("Waiting for connection on port %d..\n", PORT);
  }
  len = sizeof(cli);
  
  // Accept the data packet from client and verification
  connfd = accept(sockfd, (SA*)&cli, &len);
  if(connfd < 0) {
    printf("Error: connection to client failed\n");
    exit(0);
  }
  else {
    printf("Connection established!\n");
  }  

  ProcessKeys(connfd);
  
  printf("Should never reach here!\n");
  
  // After chatting close the socket
  close(sockfd);
}
