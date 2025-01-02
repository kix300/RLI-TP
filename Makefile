# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Source files and targets
CLIENT_SRC = client_udp.c
SERVER_SRC = server_udp.c
PROXY_SRC = proxy.c

CLIENT_BIN = client
SERVER_BIN = server
PROXY_BIN = proxy

# Default target
all: $(CLIENT_BIN) $(SERVER_BIN) $(PROXY_BIN)

# Compile client
$(CLIENT_BIN): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC)

# Compile server
$(SERVER_BIN): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_SRC)

# Compile proxy
$(PROXY_BIN): $(PROXY_SRC)
	$(CC) $(CFLAGS) -o $(PROXY_BIN) $(PROXY_SRC)

# Clean build artifacts
clean:
	rm -f $(CLIENT_BIN) $(SERVER_BIN) $(PROXY_BIN)

.PHONY: all clean

