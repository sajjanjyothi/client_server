INCLUDE_DIR = ./include
CC = gcc
OUTPUT = server
OBJECTFILES = src/server.o src/server_log.o src/server_config.o 
LIBS = -lpthread
CFLAGS = -I $(INCLUDE_DIR)  $(LIBS)

all:$(OUTPUT)
src/%.o : src/%.c 
	$(CC) -c -o $@ $^ $(CFLAGS)

$(OUTPUT):$(OBJECTFILES) 
	$(CC) -o $@ $(OBJECTFILES) $(CFLAGS)

clean:
	rm -rf $(OBJECTFILES) $(OUTPUT)