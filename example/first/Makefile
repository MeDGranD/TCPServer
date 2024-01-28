CXX = 		g++
CXX_FLAGS =	-std=c++17 -Wall

SRC =		../../src
BIN = 		bin
INCLUDE = 	../../include
SERVER = 	server.cpp
CLIENT = 	client.cpp
SEREXE = 	server
CLEXE = 	client

build : $(BIN)/$(SEREXE) | $(BIN)/$(CLEXE)

$(BIN)/$(SEREXE): $(SERVER) | $(BIN)
	@echo "Building server..."
	$(CXX) $(CXX_FLAGS) -o $@ -I $(INCLUDE) $(SERVER) $(SRC)/TCPServerBase.cpp

$(BIN)/$(CLEXE): $(CLIENT) | $(BIN)
	@echo "Building client..."
	$(CXX) $(CXX_FLAGS) -o $@ $(CLIENT)

$(BIN):
	mkdir $@