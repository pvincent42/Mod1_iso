
#include "Cell.hpp"

Cell::Cell(void) : source(0), max_gwl(10000), _groundLevel(0.0f), _waterLevel(0.0f)
{
	return ;
}

Cell::Cell(Cell const &src) : source(0), max_gwl(10000), _groundLevel(0.0f), _waterLevel(0.0f)
{
	*this = src;
	return ;
}

Cell::~Cell(void)
{
	return ;
}

double const
&Cell::getGL(void) const
{
	return (this->_groundLevel);
}

double const
&Cell::getWL(void) const
{
	return (this->_waterLevel);
}

void
Cell::setGL(double const &groundLevel)
{
	this->_groundLevel = groundLevel;
}

void
Cell::setWL(double const &waterLevel)
{
	this->_waterLevel = waterLevel;
}

Cell
&Cell::operator=(Cell const &rhs)
{
	if (this != &rhs)
	{
		// copy members here
	}
	return (*this);
}

std::ostream
&operator<<(std::ostream &o, Cell const &i)
{
	o	<< "Cell: " << &i;
	return (o);
}
