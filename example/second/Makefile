CXX = 		g++
CXX_FLAGS =	-std=c++17 -Wall

SRC =		../../src
BIN = 		bin
INCLUDE = 	../../include
SERVER = 	server.cpp
SERVEXE =	server

build: $(BIN)/$(SERVEXE)

$(BIN)/$(SERVEXE): $(BIN)
	$(CXX) $(CXX_FLAGS) -o $@ -I $(INCLUDE) $(SERVER) $(SRC)/TCPServerBase.cpp $(SRC)/handlers/HTTPHandler.cpp

$(BIN):
	mkdir $@