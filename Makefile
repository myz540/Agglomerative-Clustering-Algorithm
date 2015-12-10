CC = g++
CFLAGS = -g --std=c++11

project1_2: project1_2.o cluster_node.o point_class.o
	$(CC) -o project1_2 project1_2.o cluster_node.o point_class.o
project1_2.o: project1_2.cc cluster_node.h point_class.h
	$(CC) $(CFLAGS) -c -o project1_2.o project1_2.cc
cluster_node.o: cluster_node.cc cluster_node.h point_class.h
	$(CC) $(CFLAGS) -c -o cluster_node.o cluster_node.cc
point_class.o: point_class.cc point_class.h
	$(CC) $(CFLAGS) -c -o point_class.o point_class.cc
