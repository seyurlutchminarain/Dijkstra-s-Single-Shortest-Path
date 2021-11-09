all: parallel_conway serial_conway serial_dijkstra parallel_dijkstra graph

parallel_conway: parallel_conway.cpp
	mpic++ parallel_conway.cpp -o parallel_conway

serial_conway: serial_conway.cpp
	g++ serial_conway.cpp -o serial_conway

serial_dijkstra: serial_dijkstra.cpp
	g++ serial_dijkstra.cpp -o serial_dijkstra

parallel_dijkstra: parallel_dijkstra.cpp
	mpic++ parallel_dijkstra.cpp -o parallel_dijkstra

graph: generateGraph.cpp
	g++ generateGraph.cpp -o generateGraph

clean:
	rm -f parallel_conway
	rm -f serial_conway
	rm -f serial_dijkstra
	rm -f parallel_dijkstra
	rm -f generateGraph