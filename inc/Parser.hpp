
#ifndef		PARSER_HPP
# define	PARSER_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <list>
# include "Point.hpp"

class Parser
{
public:
	int					count;
	std::list<Point>	points;

	Parser(void);
	~Parser(void);

	bool							checkFileName(std::string const &filename);
	template<typename T> bool		error(std::string const missing, T const near);
	bool							parsePoints(std::string const &filename);

	Parser				&operator=(Parser const &rhs);

private:
	Parser(Parser const &src);
};

std::ostream				&operator<<(std::ostream &o, Parser const &i);

#endif
