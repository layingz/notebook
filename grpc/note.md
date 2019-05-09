1、下载protoc
  先安装Protobuf 编译器 protoc，下载地址：https://github.com/google/protobuf/releases
  将安装路径bin添加到path里面
  
2、golang准备
  // gRPC运行时接口编解码支持库
  go get -u github.com/golang/protobuf/proto
  // 从 Proto文件(gRPC接口描述文件) 生成 go文件 的编译器插件
  go get -u github.com/golang/protobuf/protoc-gen-go
  
  在protoc的bin里面go build github.com/golang/protobuf/proto
  
 3、python准备
  pip install grpcio
  pip install grpcio-tools
  
 4、生成go文件和pytho的grpc文件
  protoc --go_out=plugins=grpc:. server.proto
  python -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. client.proto 
  
 5、编写客户端服务端
