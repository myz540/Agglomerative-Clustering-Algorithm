/*
Mike Zhong
91.201-Rumshisky
Project1_2, Due: 11/26/15
*/

#include "point_class.h"

using namespace std;

Point::Point(int _dimension)
{
	for(int i = 0; i < _dimension; ++i){
		coordinates.push_back(0);
	}
}
		
Point::Point(vector<double> _coordinates)
{
	coordinates = _coordinates;
}
		
Point::Point(const Point& rhs)
{
	coordinates = rhs.coordinates;
}
		
Point& Point::operator=(const Point& rhs)
{
	this->coordinates = rhs.coordinates;
	return *this;
}
		
Point& Point::operator+=(const Point& rhs)
{
	assert(this->coordinates.size() == rhs.coordinates.size());
	for(int i = 0; i < rhs.coordinates.size(); ++i){
		this->coordinates[i] += rhs.coordinates[i];
	}
	return *this;
}
		
double& Point::operator[](const int index)
{
	assert(index >=0 && index < this->coordinates.size());
	return this->coordinates[index];
}

Point& Point::operator++()
{
	for(int i = 0; i < this->coordinates.size(); ++i){
		++this->coordinates[i];
	}
	return *this;
}

Point Point::operator++(int)
{
	Point temp = *this;
	for(int i = 0; i < this->coordinates.size(); ++i){
		++this->coordinates[i];
	}
	return temp;
}

ostream& operator<<(ostream& os, const Point& x)
{
	os << "(";
	for(int i = 0; i < x.coordinates.size() - 1; i++){
		os << x.coordinates[i] << ",";
	}
	os << x.coordinates.back() << ")";
	return os;
}

Point operator+(const Point& x, const Point& y)
{
	assert(x.coordinates.size() == y.coordinates.size());
	Point temp = x;
	temp += y;
	return temp;
}