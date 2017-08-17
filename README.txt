README
======

Not implemented 
===============
Message length limited to 255 (Configurable this can be changed in header file)
variable length message is not implemented to keep the implementation simple

Plan was as below

This can be implemnted by adding message length to the server message data structure
typedef struct Message
{
    OperationType_t operation;
    char message[255];
}Message_t;

read first 2 integers(4+4 = 8 bytes) and identify the message length from there 
Allocate memory for message and read out message of that length.


HOW TO BUILD
============
folders are as below
-rw-r--r--. 1 sajjan sajjan  118 Aug 16 10:49 Makefile
drwxr-xr-x. 4 sajjan sajjan 4096 Aug 16 10:52 server
drwxr-xr-x. 4 sajjan sajjan 4096 Aug 16 10:52 client

project can be built as make clean; make (both client and server will be built)

server can be exucted as  ./server -p <port number> ( server -h will print the usage)
client can be executed as 
./client -s <ip address> -o <operation> -m <message>

Eg: ./client -s 127.0.0.1 -o 2 -m "Hello I am here Sky"

Sample logs and screen shots are also attaching along with this

server.log - This has all access details for the server 

eg: 127.0.0.1;READ;Tue Aug 15 19:15:37 2017
127.0.0.1;MODIFY;Wed Aug 16 09:35:51 2017

client_screenshot.png - client execution screenshot
server_screenshot.png -  server execution screenshot



