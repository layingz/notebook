package main

import (
	"context"
	"fmt"
	"google.golang.org/grpc"
	"google.golang.org/grpc/reflection"
	"hci.macrosan.com/hci/hci_golang/pb"
	"net"
)

const (
	port = "127.0.0.1:9099"
)

type server struct{}

func (s *server) ExecCmd(ctx context.Context, cmd *pb.Cmd) (*pb.Rsp, error){
	fmt.Printf("type:%d, param:%s\n", cmd.Type, cmd.Param)
	return &pb.Rsp{R:"ok"}, nil
}


func main() {
	l, err := net.Listen("tcp", port)
	if err != nil {
		fmt.Println("listen port failed")
	}
	var s *grpc.Server
	s = grpc.NewServer()

	pb.RegisterWaiterServer(s, &server{})

	reflection.Register(s)

	err = s.Serve(l)
	if err != nil {
		fmt.Println("server failed!")
	}
}