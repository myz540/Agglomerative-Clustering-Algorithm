/*
Mike Zhong
91.201-Rumshisky
Project1_2, Due: 11/26/15
*/

#include "point_class.h"
#include "cluster_node.h"

using namespace std;

// reads in points and stores them as cluster nodes of size 1 in _cluster
void read_points(Cluster& _cluster)
{
	string temp_string;
	double real_number;
	vector<double> temp_point;
	Cluster_Node temp_cluster_node;

	std::cout << "Reading input now and initializing cluster nodes..." << endl;

	_cluster.print_min_distance_set();	

	// read in a line and store it in temp string until EOF
	while(getline(cin, temp_string)){
		istringstream string_buffer(temp_string);					// create istringstream object to store the string

		while(string_buffer >> real_number){						// read doubles from string buffer, store in vector<double> temp_point
			temp_point.push_back(real_number);
		}
		
		temp_cluster_node.point_cluster.push_back(temp_point);		// add point to tree as a cluster node of size 1
		_cluster.tree.push_back(temp_cluster_node);					// add cluster node to tree
		
		temp_point.clear();											// clear vector
		temp_cluster_node.point_cluster.clear();					// clear cluster

		// check to make sure points added have the same dimension as the first point added
		if(_cluster.tree[Cluster_Node::Current_Index].point_cluster[0].getDimension() != _cluster.tree[0].point_cluster[0].getDimension()){
			cerr << "Input points are not of the same dimension, exiting program" << endl;
			exit(3);
		}

		// initialize Cluster.Cluster_Node elements
		_cluster.tree[Cluster_Node::Current_Index].Index_Of_Node = Cluster_Node::Current_Index;
		_cluster.tree[Cluster_Node::Current_Index].can_merge = true;
		_cluster.tree[Cluster_Node::Current_Index].dimension = _cluster.tree[Cluster_Node::Current_Index].point_cluster[0].getDimension();

		++Cluster_Node::Current_Index;
	}

	// initialize Cluster elements (tree);
	_cluster.num_of_points = 
	_cluster.num_of_nodes = 
	_cluster.num_of_clusters = Cluster_Node::Current_Index;

	_cluster.initialize_distance_set();

	std::cout << "done reading and initializing points and cluster nodes" << endl;
	std::cout << "The current min_distance_set looks like this: " << endl;
	_cluster.print_min_distance_set();
}

int main(int argc, char* argv[])
{
	// make sure the number of command line arguments is correct
	if(argc != 2){
		cerr << "Invalid number of command line arguments" << endl;
		exit(1);
	}

	// create istringstream object and store the command line argument
	istringstream iss(argv[1]);

	// create a Cluster object, this will store the dendogram as it is built
	Cluster cluster;
	
	// attempt to store argument as an int in the num_of_points data member
	if(!(iss >> cluster.num_of_points)){
		cerr << "Invalid number of clusters requested" << endl;
		exit(2);
	}
	
	// create const int K, the number of active cluster nodes to agglomerate to
	const int K = cluster.num_of_points;

	// read input.txt
	read_points(cluster);
	
	cluster.print_cluster();

	// calls agglomerate function on cluster with argument K
	cluster.agglomerate(K);

	cout << "\n\n";
	cluster.print_active_clusters_nodes();

	return 0;
}
