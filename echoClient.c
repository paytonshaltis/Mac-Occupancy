#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];
	
    //basic check of the arguments
    //additional checks can be inserted
    if (argc !=2) {
        perror("Usage: TCPClient <IP address of the server"); 
        exit(1);
    }
	
    //Create a socket for the client
    //If sockfd<0 there was an error in the creation of the socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) <0) {
        perror("Problem in creating the socket");
        exit(2);
    }
	
    //Creation of the socket
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port =  htons(SERV_PORT); //convert to big-endian order
    
    //Connection of the client to the socket 
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
        perror("Problem in connecting to the server");
        exit(3);
    }
    
    // Create a pointer to 4096 bytes of space for hostname
    char* hostname = (char*)malloc(4096 * sizeof(char));
    gethostname(hostname, 4096);

    // Send the host name
    send(sockfd, hostname, strlen(hostname), 0);
    if (recv(sockfd, recvline, MAXLINE,0) == 0){
        //error: server terminated prematurely
        perror("The server terminated prematurely"); 
        exit(4);
    }
    printf("String received from the server: %s", recvline);
    
    // get rid of the pesky end-of-file characters
    char* hostnameRecvd = (char*)malloc(4096 * sizeof(char));
    char temp;
    int i = 0;
    for(; i < 63; i++) {
        temp = recvline[i];
        if(temp <= 0) {
            break;
        }
        else {
            hostnameRecvd[i] = temp;
        }
    }
    hostnameRecvd[++i] = 0;

    // Log this
    char* command1 = "echo ";
    char* command2 = " >> ~/Desktop/status.log";
    char command[256] = "";
    strcat(command, command1);
    strcat(command, hostnameRecvd);
    strcat(command, command2);
    system(command);

    // free our heap data
    free(hostname);
    free(hostnameRecvd);

    /* MODIFICATION 9/18/21
    while (fgets(sendline, MAXLINE, stdin) != NULL) {
        
        send(sockfd, sendline, strlen(sendline), 0);
            
        if (recv(sockfd, recvline, MAXLINE,0) == 0){
            //error: server terminated prematurely
            perror("The server terminated prematurely"); 
            exit(4);
        }
        printf("%s", "String received from the server: ");
        fputs(recvline, stdout);

        // clear the sendline and recvline buffers
        memset(sendline, 0, MAXLINE);
        memset(recvline, 0, MAXLINE);
    }
    */

    exit(0);
}
