# RPC-Protobuf

1. Generate C++ files from protobuf:
```Shell
$ protoc --cpp_out=./ example.proto
$ protoc --grpc_out=./ --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin example.proto
```
2. Compile C++ files：
```Shell
$ make
```
3. Run server and client separately in two shells:
```Shell
$ ./server
```
```Shell
$ ./client
```
