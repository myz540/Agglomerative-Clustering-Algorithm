/*
Mike Zhong
91.201-Rumshisky
Project1_2, Due: 11/26/15
*/

#ifndef POINT_CLASS_H
#define POINT_CLASS_H

#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <sstream>
#include <iomanip>

using namespace std;

class Point
{
	public:
		Point(int _dimension = 2);			
		Point(vector<double> _coordinates);
		Point(const Point& rhs);
		
		Point& operator=(const Point& rhs);
		Point& operator+=(const Point& rhs);
		double& operator[](const int index);
		Point& operator++();
		Point operator++(int);
		
		~Point() {}
		
		inline int getDimension() {return this->coordinates.size();}
	
		friend ostream& operator<<(ostream& os, const Point& x);
		friend Point operator+(const Point& x, const Point& y);
	
	private:
		vector<double> coordinates;
};

#endif