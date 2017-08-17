#include<server_config.h>
#include<stdio.h>
#include <string.h>
#include <server.h>
/* File contains functions for server persistant storage */
static FILE *cfgFP;
int initialiseMessage(char *message)
{
    if ( ( cfgFP =  fopen(SERVER_CONFIG_FILE,"r")) == NULL)
    {
        if ( (cfgFP =  fopen(SERVER_CONFIG_FILE,"w")) == NULL)
        {
            return -1;
        }
        else
        {
            fputs("HELLO", cfgFP);
            strcpy(message, "HELLO");
            fclose(cfgFP);
        }
    }
    else
    {
        fgets(message, MAX_MESSAGE_LENGTH, cfgFP);
        fclose(cfgFP);
    }

    return 0;
}

int modifyMessage(char *message)
{
    if ( ( cfgFP =  fopen(SERVER_CONFIG_FILE,"w")) != NULL)
    {
        if ( fputs(message,cfgFP) == EOF)
        {
            fclose(cfgFP);
            return -1;
        }
        fclose(cfgFP);
    }
    else
    {
        return (-1);
    }
    return 0;
}