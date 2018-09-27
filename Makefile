export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

CC = g++
CFLAGS = -std=c++11 -pthread -c
INCLUDES = -I/usr/local/include
LDFLAGS = -L/usr/local/lib `pkg-config --libs grpc++ grpc` -Wl,--no-as-needed -lprotobuf -lpthread -ldl -lssl -lgrpc++_reflection -Wl,--as-needed

SERVER_SOURCE = server.cc
CLIENT_SOURCE = client.cc
PROTO_SOURCE = example.grpc.pb.cc example.pb.cc

SERVER_OBJS = $(SERVER_SOURCE:.cc=.o)
CLIENT_OBJS = $(CLIENT_SOURCE:.cc=.o)
PROTO_OBJS = $(PROTO_SOURCE:.cc=.o)

SERVER_TARGET = server
CLIENT_TARGET = client

OBJS = $(SERVER_OBJS) $(CLIENT_OBJS) $(PROTO_OBJS)
TARGET = $(SERVER_TARGET) $(CLIENT_TARGET)

.PHONY: all  clean
all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(SERVER_OBJS) $(PROTO_OBJS)
	$(CC) -o $(SERVER_TARGET) $(LDFLAGS) $^
$(CLIENT_TARGET): $(CLIENT_OBJS) $(PROTO_OBJS)
	$(CC) -o $(CLIENT_TARGET) $(LDFLAGS) $^
.cc.o:
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) $^

clean:
	rm $(OBJS) $(TARGET)
