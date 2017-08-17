#ifndef _SERVER_H_
#define _SERVER_H_

#define SERVER_LOG "server.log"
#define LOG_LENGTH 255
#define MAX_IP_ADDR_LENGTH 32
#define MAX_MESSAGE_LENGTH 255
#define DEFAULT_PORT 9999
#define TRUE 1
/*
    Message type
*/
typedef enum
{
    READ = 1,
    MODIFY ,
    TERMINATE
}OperationType_t;

/* Actual message */
typedef struct Message
{
    OperationType_t operation;
    char message[255];
}Message_t;

#endif //_SERVER_H_