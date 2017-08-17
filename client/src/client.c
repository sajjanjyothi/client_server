#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <server.h>

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    Message_t message;
    char recvBuff[MAX_MESSAGE_LENGTH];
    struct sockaddr_in serv_addr; 
    char serverIPAddress[MAX_IP_ADDR_LENGTH];
    int serverPort = DEFAULT_PORT;
    int operation = -1;
    char messageString[MAX_MESSAGE_LENGTH] = {0};
    int opt = -1;

    strcpy(serverIPAddress, "127.0.0.1");
    strcpy(messageString,"TEST_SERVER");


    while ( (opt = getopt(argc, argv, ":s:p:o:m:h")) != -1)
    {
        switch (opt)
        {
        case 's':
            strcpy(serverIPAddress, optarg);
            break;
        case 'p':
            serverPort = atoi(optarg);
            break;
        case 'o':
            operation = atoi(optarg);
            break;
        case 'm':
            strcpy(messageString,optarg);
            break;
        case 'h':
        default:
            printf("Usage %s -s <server ip address> -p <port> -o <operation> -m <message>\n", argv[0]);
            exit(1);
            break;
        }
    }

    if( operation == -1)
    {
        printf("Usage %s -s <server ip address> -p <port> -o <operation> -m <message>\n", argv[0]);
        exit(1);
    }

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("\n Error : Could not create socket \n");
        return (-1);
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serverPort); 

    if(inet_pton(AF_INET, serverIPAddress, &serv_addr.sin_addr)<=0)
    {
        perror("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       perror("\n Error : Connect Failed \n");
       return 1;
    } 

    memset(&message,0,sizeof(message));

    message.operation = operation;
    strcpy(message.message,messageString); 

    if( write( sockfd, &message, sizeof(message)) < 0)
    {
        perror("Server write failed");
        return -1;
    }
    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        printf("%s\n", recvBuff);
    } 

    if(n < 0)
    {
        printf("\n Read error \n");
    } 

    return 0;
}