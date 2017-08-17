#include <stdio.h>
#include <stdlib.h>
#include <server.h>

/* File contains functions for server log messages */

static FILE *logFP;

int initialise_log()
{
    
    logFP = fopen(SERVER_LOG, "a");
    if (logFP == NULL)
    {
        return -1;
    }
    return 0;
}


int logMessage(char *logmessage)
{
    fputs(logmessage, logFP);
    fflush(logFP);
    return 0;
}

int cleanUpLog()
{
    if (logFP)
    {
        fclose(logFP);
    }
    return 0;
}