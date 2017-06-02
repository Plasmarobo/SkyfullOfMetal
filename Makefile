CC=g++
AR=ar
ARFLAGS=-rcvs
CFLAGS=-Wall -std=c++11 -Wextra
LDFLAGS=-L./lib

INCLUDE=-Ilibraries/Catch/single_include
INCLUDE+=-Ilibraries/flatbuffers/include
INCLUDE+=-Ilibraries/asio/asio/include
INCLUDE+=-Isrc/common
INCLUDE+=-Isrc/client
INCLUDE+=-Isrc/server
INCLUDE+=-Isrc/test

LIBDIR=lib
BINDIR=bin
BUILDDIR=build

CLIENT=$(BINDIR)/som_client
SERVER=$(BINDIR)/som_server
TESTS=$(BINDIR)/tests
LIBCORE=$(LIBDIR)/libsomcore.a
LIBCORE_LINK=-lsomcore

CORE_SOURCE = $(wildcard src/common/*.cpp) $(wildcard src/common/*/*.cpp)

CLIENT_SOURCE = $(wildcard src/client/*.cpp) $(wildcard src/client/*/*.cpp)

SERVER_SOURCE = $(wildcard src/server/*.cpp) $(wildcard src/server/*/*.cpp)

TEST_SOURCE = $(wildcard src/test/*.cpp)

_CORE_OBJECTS = $(CORE_SOURCE:.cpp=.o)
_CLIENT_OBJECTS = $(CLIENT_SOURCE:.cpp=.o)
_SERVER_OBJECTS = $(SERVER_SOURCE:.cpp=.o)
_TEST_OBJECTS = $(TEST_SOURCE:.cpp=.o)

CORE_OBJECTS = $(patsubst %,$(BUILDDIR)/%,$(_CORE_OBJECTS))
CLIENT_OBJECTS = $(patsubst %,$(BUILDDIR)/%,$(_CLIENT_OBJECTS))
SERVER_OBJECTS = $(patsubst %,$(BUILDDIR)/%,$(_SERVER_OBJECTS))
TEST_OBJECTS = $(patsubst %,$(BUILDDIR)/%,$(_TEST_OBJECTS))

.PHONY: clean dirs

all: $(CLIENT) $(SERVER) $(TESTS) $(LIBCORE)
	echo "Building all"

tests: $(TESTS)
	@echo $(TEST_SOURCE)

client: $(CLIENT)

server: $(SERVER)

libcore: $(LIBCORE)

$(LIBCORE): $(CORE_OBJECTS)
	@echo Building $(LIBCORE)
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $^

$(CLIENT): $(LIBCORE) $(CLIENT_OBJECTS)
	@echo Building $(CLIENT)
	@mkdir -p $(@D)
	$(CC) $(CLIENT_OBJECTS) -o $@ $(CFLAGS) $(LDFLAGS) $(LIBCORE_LINK)

$(SERVER): $(LIBCORE) $(SERVER_OBJECTS)
	@echo Building $(SERVER)
	@mkdir -p $(@D)
	$(CC) $(SERVER_OBJECTS) -o $@ $(CFLAGS) $(LDFLAGS) $(LIBCORE_LINK)

$(TESTS): $(LIBCORE) $(TEST_OBJECTS)
	@echo Building $(TESTS)
	@mkdir -p $(@D)
	$(CC) $(TEST_OBJECTS) -o $@ $(CFLAGS) $(LIBFLAGS) $(LDFLAGS) $(LIBCORE_LINK)

$(BUILDDIR)/%.o : %.cpp
	@mkdir -p $(@D)
	$(CC) -c $(INCLUDE) -o $@ $< $(CFLAGS)

clean:
	rm $(CLIENT_OBJECTS) $(SERVER_OBJECTS) $(TEST_OBJECTS) $(CORE_OBJECTS) $(CLIENT) $(SERVER) $(TESTS) $(LIBCORE)