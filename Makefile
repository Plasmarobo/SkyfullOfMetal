CC=g++
AR=ar
ARFLAGS=rvs
CFLAGS=-c -Wall -std=c++11
LDFLAGS=

CORE_SOURCE=./src/common/*.cpp
CORE_SOURCE=$(wildcard ./src/common/*/*.cpp)

CLIENT_SOURCE=./src/client/*.cpp
CLIENT_SOURCE=$(wildcard ./src/client/*/*.cpp)

SERVER_SOURCE=./src/server/*.cpp
SERVER_SOURCE=$(wildcard ./src/server/*/*.cpp)

TEST_SOURCE=./src/test/*.cpp

CORE_OBJECTS=$(CORE_SOURCE:.cpp=.o)
CLIENT_OBJECTS=$(CLIENT_SOURCE:.cpp=.o)
SERVER_OBJECTS=$(SERVER_SOURCE:.cpp=.o)
TEST_OBJECTS=$(TEST_SOURCE:.cpp=.o)

INCLUDE=-I./libraries/Catch/include
INCLUDE=-I./libraries/asio/asio/include
INCLUDE=-I./src/common
INCLUDE=-I./src/client
INCLUDE=-I./src/server
INCLUDE=-I./src/tests

LIBDIR=lib
BINDIR=bin

CLIENT=$(BIRDIR)/som_client
SERVER=$(BINDIR)/som_server
TESTS=$(BINDIR)/tests
LIBCORE=$(LIBDIR)/libsomcore.a

all: $(CLIENT) $(SERVER) $(TESTS) $(LIBCORE)

$(LIBCORE): $(CORE_OBJECTS)
  $(AR) $@ $(CORE_OBJECTS)

$(CLIENT): $(LIBCORE) $(CLIENT_OBJECTS)
  $(CC) -l$(LIBCORE) $(CLIENT_OBJECTS) -o $(CLIENT)

$(SERVER): $(LIBCORE) $(SERVER_OBJECTS)
  $(CC) -l$(LIBCORE) $(SERVER_OBJECTS) -o $(SERVER)

$(TESTS): $(LIBCORE) $(SERVER_OBJECTS) $(CLIENT_OBJECTS) $(TEST_OBJECS)
  $(CC) -l$(LIBCORE) $(TEST_OBJECTS) $(SERVER_OBJECTS) $(CLIENT_OBJECTS) -o $(TEST)

.cpp.o:
  $(CC) $(CFLAGS) $< -o $@
