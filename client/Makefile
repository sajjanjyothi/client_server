INCLUDE_DIR = ../server/include
CC = gcc
OUTPUT = client
OBJECTFILES = src/client.o

CFLAGS = -I $(INCLUDE_DIR) 

all:$(OUTPUT)
src/%.o : src/%.c 
	$(CC) -c -o $@ $^ $(CFLAGS)

$(OUTPUT):$(OBJECTFILES) 
	$(CC) -o $@ $(OBJECTFILES) $(CFLAGS)

clean:
	rm -rf $(OBJECTFILES) $(OUTPUT)