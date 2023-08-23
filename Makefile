all: serverA.cpp serverB.cpp serverM.cpp client.cpp
	g++ -std=c++11 serverA.cpp -o serverA
	g++ -std=c++11 serverB.cpp -o serverB
	g++ -std=c++11 serverM.cpp -o serverM
	g++ -std=c++11 client.cpp -o client

clean: serverA serverB serverM client
	rm -r serverA serverB serverM client


