
#include "Point.hpp"

Point::Point(float const x, float const y, float const z)
	: x(x), y(y), z(z)
{
	return ;
}

Point::~Point(void)
{
	return ;
}

Point
&Point::operator=(Point const &rhs)
{
	if (this != &rhs)
	{
		// copy members here
	}
	return (*this);
}

std::ostream
&operator<<(std::ostream &o, Point const &i)
{
	o	<< "Point: x: " << i.x << ", y: " << i.y << ", z: " << i.z;
	return (o);
}
