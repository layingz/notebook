import grpc
from pb import client_pb2_grpc, client_pb2

endpoint = "127.0.0.1:9099"

def client():
    conn = grpc.insecure_channel(endpoint)
    client = client_pb2_grpc.WaiterStub(channel = conn)
    resp = client.ExecCmd(client_pb2.Cmd(type=1, param ="exec"))
    print("recv:" + resp.r)

if __name__ == "__main__":
    client()