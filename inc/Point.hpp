
#ifndef		POINT_HPP
# define	POINT_HPP

# include <iostream>

class Point
{
public:
	float			x;
	float			y;
	float			z;

	Point(float const x, float const y, float const z);
	~Point(void);

	Point				&operator=(Point const &rhs);
private:
	Point(void);
	Point(Point const &src);
};

std::ostream				&operator<<(std::ostream &o, Point const &i);

#endif
