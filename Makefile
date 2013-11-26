CC=g++
CFLAGS=-c -Wall

CPPFLAGS += -isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include
CXXFLAGS += -g -Wall -Wextra -pthread
LDFLAGS=
GTEST_DIR = ./gtest
GMOCK_DIR = ./gmock

USER_DIR = .
TESTS = ServerClientTest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

GMOCK_HEADERS = $(GMOCK_DIR)/include/gmock/*.h \
                $(GMOCK_DIR)/include/gmock/internal/*.h \
                $(GTEST_HEADERS)
GMOCK_SRCS_ = $(GMOCK_DIR)/src/*.cc $(GMOCK_HEADERS)

SERVER_SOURCES= ServerMain.cpp Server.cpp SocketAdapter.cpp
SERVER_OBJECTS=$(SERVER_SOURCES:.cpp=.o)
SERVER_EXECUTABLE=server
CLIENT_SOURCES= ClientMain.cpp Client.cpp SocketAdapter.cpp
CLIENT_OBJECTS=$(CLIENT_SOURCES:.cpp=.o)
CLIENT_EXECUTABLE=client

all: $(CLIENT_SOURCES) $(CLIENT_EXECUTABLE) $(SERVER_EXECUTABLE) $(SERVER_SOURCES) $(TESTS)
	
$(SERVER_EXECUTABLE): $(SERVER_OBJECTS) 
	$(CC) $(LDFLAGS) $(SERVER_OBJECTS) -o $@


$(CLIENT_EXECUTABLE): $(CLIENT_OBJECTS) 
	$(CC) $(LDFLAGS) $(CLIENT_OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(CLIENT_OBJECTS) $(SERVER_OBJECTS) $(CLIENT_EXECUTABLE) $(SERVER_EXECUTABLE) gtest.a gtest_main.a gmock.a gmock_main.a ServerClientTest.o $(TESTS) gmock-all.o gmock_main.o gtest-all.o gtest_main.o


$(TESTS): ServerClientTest.o Server.o Client.o gtest_main.a gmock_main.a
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@


ServerClientTest.o : $(SERVER_SOURCES) $(CLIENT_SOURCES) ServerClientTest.cpp $(GTEST_HEADERS) $(GMOCK_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c ServerClientTest.cpp



gtest-all.o : $(GTEST_SRCS_)
	$(CC) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

gmock-all.o : $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
            -c $(GMOCK_DIR)/src/gmock-all.cc

gmock_main.o : $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
            -c $(GMOCK_DIR)/src/gmock_main.cc

gmock.a : gmock-all.o gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gmock_main.a : gmock-all.o gtest-all.o gmock_main.o
	$(AR) $(ARFLAGS) $@ $^