.PHONY:all
all:udp_client udp_server

udp_server:Main.cc
	g++ -o $@ $^ -std=c++11

udp_client:UdpClient.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -rf udp_server