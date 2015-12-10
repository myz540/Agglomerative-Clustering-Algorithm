/*
Mike Zhong
91.201-Rumshisky
Project1_2, Due: 11/26/15
*/

#include "cluster_node.h"
#include <cassert>

Distance_Node::Distance_Node(double _distance, int _merged_index_1, int _merged_index_2)
{
	distance = _distance;
	merged_index_1 = _merged_index_1;
	merged_index_2 = _merged_index_2;
}

Distance_Node::Distance_Node(const Distance_Node& rhs)
{
	distance = rhs.distance;
	merged_index_1 = rhs.merged_index_1;
	merged_index_2 = rhs.merged_index_2;
}

Distance_Node& Distance_Node::operator=(const Distance_Node& rhs)
{
	distance = rhs.distance;
	merged_index_1 = rhs.merged_index_1;
	merged_index_2 = rhs.merged_index_2;
	return *this;
}

//overloaded < operator for Distance nodes
bool operator<(const Distance_Node& lhs, const Distance_Node& rhs)
{
	return lhs.distance < rhs.distance;
}

// return the distance of any two Points
double get_distance_points(Point& x, Point& y)
{
	assert(x.getDimension() == y.getDimension());

	// sum up the square of the differences
	double sum_of_squares = 0;
	for(int i = 0; i < x.getDimension(); ++i){
		sum_of_squares += pow((x[i] - y[i]), 2);
	}

	// return the square root of that sum, giving the distance between two points
	return sqrt(sum_of_squares);
}

// Computes the distance between two cluster nodes and returns it, as well as the cluster node indices as a Distance_Node
Distance_Node get_distance_clusters(Cluster_Node& x, Cluster_Node& y, int index_of_x, int index_of_y)
{
	if(x.point_cluster.size() == 1 && y.point_cluster.size() == 1){
		Distance_Node temp_node(get_distance_points(x.point_cluster[0], y.point_cluster[0]), index_of_x, index_of_y);
		return temp_node;
	}

	else{
		// sum up the square of the differences
		double sum_of_distances = 0;
		double total_points = x.point_cluster.size() + y.point_cluster.size();
	
		for(Point px : x.point_cluster){
			for(Point py : y.point_cluster){
				sum_of_distances += get_distance_points(px, py);
			}
		}
	
		Distance_Node temp_node(sum_of_distances / total_points, index_of_x, index_of_y);
		return temp_node;
	}
}
// initialize static variable used to track the indices of the Cluster Nodes as they are added
int Cluster_Node::Current_Index = 0;

// called only by read_points() function, this will initialize the min_distance_set
void Cluster::initialize_distance_set()
{
	for(int i = 0; i < this->tree.size() - 1; ++i){
		for(int j = i + 1; j < tree.size(); ++j){

			this->min_distance_set.insert(get_distance_clusters(this->tree[i], this->tree[j], i, j));

		}
	}
}

// prints all the distance nodes in the min_distance_set
void Cluster::print_min_distance_set()
{
	if(this->min_distance_set.empty()){
		cout << "The Distance Set is Empty" << endl;
		return;
	}

	for(auto iter = this->min_distance_set.begin(); iter != this->min_distance_set.end(); ++iter){
		cout << "Distance: " << iter->distance;
		cout << ", Indices: " << iter->merged_index_1 << ", " << iter->merged_index_2 << endl;
	}
}

// finds the two active cluster nodes with the shortest distance and merges them into a new node, then pushes it back onto the tree
void Cluster::merge_clusters()
{
	cout << "\nMerge clusters called, the current min_distance_set looks like this: " << endl;
	this->print_min_distance_set();

	// iter points to the minimum distance node
	auto iter = this->min_distance_set.begin();
	// assign the index of the nodes to be merged and store them temporarily
	const int index1 = iter->merged_index_1;
	const int index2 = iter->merged_index_2;
	
	// create a new cluster node
	Cluster_Node new_cluster;

	// push back all the Points from the two cluster nodes that will merge
	for(Point x : this->tree[index1].point_cluster){
		new_cluster.point_cluster.push_back(x);
	}
	for(Point x : this->tree[index2].point_cluster){
		new_cluster.point_cluster.push_back(x);
	}

	// inactivate the two nodes that just merged, they cannot merge again
	this->tree[index1].can_merge = false;
	this->tree[index2].can_merge = false;
	
	// delete the distance node that was just used
	this->min_distance_set.erase(iter);

	// delete any distance node whose merged_index_1 or 2 are equal to the index of the nodes just merged
	for(auto iter = this->min_distance_set.begin(); iter != this->min_distance_set.end(); ){
		
		if(iter->merged_index_1 == index1 || 
		   iter->merged_index_2 == index1 || 
		   iter->merged_index_1 == index2 ||
		   iter->merged_index_2 == index2){
			  
			  this->min_distance_set.erase(iter++); 
		   }
		else{
			++iter;
		}
	}
	
	// initialize new cluster node
	new_cluster.Index_Of_Node = Cluster_Node::Current_Index;
	new_cluster.dimension = this->tree[0].dimension;
	new_cluster.can_merge = true;

	// update the tree
	this->tree.push_back(new_cluster);		// push back the new cluster node onto the tree
	this->num_of_clusters--;				// decrement number of active clusters
	this->num_of_nodes++;					// increment number of nodes

	// update the minimum distance set by adding new distance nodes
	for(int i = 0; i < this->tree.size() - 1; ++i){
		if(this->tree[i].can_merge){
			this->min_distance_set.insert(get_distance_clusters(this->tree[Cluster_Node::Current_Index], this->tree[i], Cluster_Node::Current_Index, i));
		}
	}
	
	// increment the static variable, Current_Index
	++Cluster_Node::Current_Index;

	cout << "The cluster nodes at index " << index1 << " and " << index2 << " were successfully merged and added to the tree" << endl;
	cout << "The new min_distance_heap looks like this: " << endl;
	this->print_min_distance_set();
}

// agglomerates the clusters into K active cluster nodes
void Cluster::agglomerate(int _K)
{
	cout << "\nAgglomerate called, merging clusters until only " << _K << " active cluster nodes remain" << endl;
	while(this->num_of_clusters > _K){
		// merge clusters and print new tree
		this->merge_clusters();
		this->print_cluster();
		cout << "number of points, active clusters, cluster nodes: " << 
			this->num_of_points << ", " <<
			this->num_of_clusters << ", " <<
			this->num_of_nodes << endl;
	}
	cout << _K << " active clusters reached, SUCCESS!";
}

// prints all the cluster nodes currently in the tree
void Cluster::print_cluster()
{
	cout << "printing all cluster nodes" << endl;
	int i = 0;
	for(Cluster_Node node : this->tree){
		cout << "Node #" << i << " contains " << node.point_cluster.size() << " points" << endl;
	
		for(Point x : node.point_cluster){
			cout << x << endl;
		}
		++i;
	}
}

void Cluster::print_active_clusters_nodes()
{
	cout << "printing only active cluster nodes" << endl;
	
	int i = 0;
	for(Cluster_Node node : this->tree){
		if(node.can_merge){
			cout << "Active Node #" << i << " contains " << node.point_cluster.size() << " points" << endl;
	
			for(Point x : node.point_cluster){
				cout << x << endl;
			}
			++i;
		}
	}
}

