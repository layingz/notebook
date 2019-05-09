module go_grpc

go 1.12

require (
	github.com/golang/protobuf v1.3.1 // indirect
	google.golang.org/grpc v1.20.1 // indirect
	grpc/pb v0.0.0
)

replace grpc/pb => ./pb
