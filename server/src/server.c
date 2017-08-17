#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <server.h>
#include <pthread.h>
#include <server_log.h>
#include <server_config.h>

#define MUTEX_LOCK() pthread_mutex_lock(&lockMutex)
#define MUTEX_UNLOCK() pthread_mutex_unlock(&lockMutex)
char SERVER_MESSAGE[MAX_MESSAGE_LENGTH] = {0};

//For synchronisation of SERVER_MESSAGE read/write
static pthread_mutex_t lockMutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct thread_arg
{
    int connfd;
    char ip[MAX_IP_ADDR_LENGTH];
} threadArg_t;

// Thread function to handle multiple clients at same time

void *handleMessage(void *arg)
{
    Message_t message = {0};

    time_t ticks;
    char log[LOG_LENGTH] = {0};
    threadArg_t arguement;
    int connfd = -1;
    char ip[MAX_IP_ADDR_LENGTH];

    memcpy(&arguement, arg, sizeof(arguement));
    connfd = arguement.connfd;
    strcpy(ip, arguement.ip);

    ticks = time(NULL);
    if (read(connfd, &message, sizeof(message)) < 0)
    {
        logMessage("Server: socket read failed");
        return NULL;
    }

    switch (message.operation)
    {
    case READ:
        MUTEX_LOCK();
        sprintf(log, "%s;%s;%s", ip, "READ", ctime(&ticks));
        logMessage(log);
        MUTEX_UNLOCK();
        if (write(connfd, SERVER_MESSAGE, strlen(SERVER_MESSAGE)) < 0)
        {
            logMessage("Server: write failed ");
        }
        break;
    case MODIFY:
        MUTEX_LOCK();
        sprintf(log, "%s;%s;%s", ip, "MODIFY", ctime(&ticks));
        logMessage(log);
        if (modifyMessage(message.message) < 0)
        {

            logMessage("Message modification failed");
            if (write(connfd, "FAILED", strlen("SUCCESS")) < 0)
            {
                logMessage("Server: write failed ");
            }
        }
        strcpy(SERVER_MESSAGE, message.message);
        MUTEX_UNLOCK();
        if (write(connfd, "SUCCESS", strlen("SUCCESS")) < 0)
        {
            logMessage("Server: write failed ");
        }
        break;
    case TERMINATE:
        sprintf(log, "%s;%s;%s", ip, "TERMINATE", ctime(&ticks));
        logMessage(log);
        exit(1); //Terminating server
        break;
    default:
        sprintf(log, "%s;%s;%s", ip, "UNKNOWN OPERATION", ctime(&ticks));
        logMessage(log);
        break;
    }
    close(connfd);
    return NULL;
}

int handleConnection(int listenfd)
{
    int connfd = -1;
    struct sockaddr_in clientAddr = {0};
    socklen_t clientaddr_size = sizeof(clientAddr);
    pthread_t thread_id;
    threadArg_t arg;

    while (TRUE)
    {
        connfd = accept(listenfd, (struct sockaddr *)&clientAddr, &clientaddr_size);
        if (connfd < 0)
        {
            perror("Invalid connection from client");
            continue;
        }
        arg.connfd = connfd;
        strcpy(arg.ip, inet_ntoa(clientAddr.sin_addr));
        if (pthread_create(&thread_id, NULL, handleMessage, (void *)&arg) < 0)
        {
            logMessage("client thread creation failed");
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int listenfd = 0;
    struct sockaddr_in serv_addr = {0};
    int enable = 1;
    int opt;
    int port = DEFAULT_PORT;
    pid_t process_id = 0;
    pid_t sid = 0;

    while ((opt = getopt(argc, argv, ":p:h")) != -1)
    {
        switch (opt)
        {
        case 'p':
            port = atoi(optarg);
            break;
        case 'h':
            printf("Help: Usage %s -p <port>\n", argv[0]);
            exit(1);
            break;
        default:
            printf("Usage %s -p <port>\n", argv[0]);
            exit(1);
            break;
        }
    }

    if (initialise_log() < 0)
    {
        perror("Log opening failed");
        return (-1);
    }

    if (initialiseMessage(SERVER_MESSAGE) < 0)
    {
        perror("Message initialisation failed");
        return (-1);
    }
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("Server: socket creation failed");
        return (-1);
    }

    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        perror("setsockopt(SO_REUSEADDR) failed");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Server: Bind failed");
        return (-1);
    }

    if (listen(listenfd, 10) < 0)
    {
        perror("Server : listen failed");
        return (-1);
    }

    if (port == DEFAULT_PORT)
    {
        printf("server running on port number %d , you can change this by giving -p <port number>\n", DEFAULT_PORT);
    }

    process_id = fork();

    if (process_id < 0)
    {
        printf("fork failed!\n");
        exit(1);
    }
    if (process_id > 0)
    {
        exit(0);
    }

    umask(0);

    sid = setsid();
    if (sid < 0)
    {
        exit(1);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    handleConnection(listenfd);

    if (cleanUpLog() < 0)
    {
        perror("cleanUpLog failed");
        return (-1);
    }
    return 0;
}