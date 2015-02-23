
#include "Source.hpp"

Source::Source(int const &x, int const &y, int const &w, int const &h, double const &inc)
	: x(x), y(y), w(w), h(h), inc(inc)
{
	return ;
}

Source::~Source(void)
{
	return ;
}

Source
&Source::operator=(Source const &rhs)
{
	if (this != &rhs)
	{
		// copy members here
	}
	return (*this);
}

std::ostream
&operator<<(std::ostream &o, Source const &i)
{
	o	<< "Source: " << &i;
	return (o);
}
