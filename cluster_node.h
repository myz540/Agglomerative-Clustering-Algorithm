/*
Mike Zhong
91.201-Rumshisky
Project1_2, Due: 11/26/15
*/

#ifndef CLUSTER_NODE_H
#define CLUSTER_NODE_H

#include "point_class.h"
#include <set>

using namespace std;

class Distance_Node
{
	public:
		// Constructor, copy constructor, overloaded assignment, and overloaded < operator
		Distance_Node(double _distance = 0, int _merged_index_1 = 0, int _merged_index_2 = 0);
		Distance_Node(const Distance_Node& rhs);
		Distance_Node& operator=(const Distance_Node& rhs);

		friend bool operator<(const Distance_Node& lhs, const Distance_Node& rhs);

		// distance between the cluster nodes at any two indices in the tree
		double distance;
		int merged_index_1;
		int merged_index_2;
};

class Cluster_Node
{
	public:
		static int Current_Index;

		vector<Point> point_cluster;			// contains all the points in the cluster
		
		int Index_Of_Node;						// contains the index of the given node in the vector
		bool can_merge;							// determines whether the node is active or not
		int dimension;							// dimension of all the points in the node

		// Computes the distance between two cluster nodes and returns it, as well as the cluster node indices as a Distance_Node
		friend Distance_Node get_distance_clusters(Cluster_Node& x, Cluster_Node& y, int index_of_x, int index_of_y);	
};

class Cluster
{
	public:
		vector<Cluster_Node> tree;				// vector to store the tree and use to generate dendogram
		set<Distance_Node> min_distance_set;	// ordered set of all Distance Nodes, minimum distance will be at the front of the set

		int num_of_points;						// number of points read from input.txt
		int num_of_nodes;						// number of cluster nodes in the tree, starts as num_of_points and increments 1 with each merge
		int num_of_clusters;					// number of active cluster nodes in the tree, starts as num_of_points and decrements 1 with each merge
		
		void print_min_distance_set();			// prints all the distance nodes in the min_distance_set
		
		void initialize_distance_set();			// called only by read_points() function, this will initialize the min_distance_set

		void print_cluster();					// prints all nodes in the dendogram

		void print_active_clusters_nodes();		// prints only active cluster nodes, eligible for merging		

		void merge_clusters();					// finds shortest distance between a pair of clusters and merges them

		void agglomerate(int _K);				// calls merge_clusters() until there are K active clusters remaining
};

#endif

