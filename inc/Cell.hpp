
#ifndef		CELL_HPP
# define	CELL_HPP

# include <iostream>
# include <stdint.h>
# include <mutex>
# include "Constants.hpp"

class Cell
{
public:
	float				source;
	double				max_gwl;
	std::mutex			m;

	Cell(void);
	Cell(Cell const &src);
	~Cell(void);

	Cell				&operator=(Cell const &rhs);

	double const		&getGL() const;
	double const		&getWL() const;

	void				setGL(double const &groundLevel);
	void				setWL(double const &waterLevel);

private:
	double				_groundLevel;
	double				_waterLevel;
};

std::ostream				&operator<<(std::ostream &o, Cell const &i);

#endif
