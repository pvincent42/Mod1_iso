
#include <sys/stat.h>
#include "Parser.hpp"

Parser::Parser(void)
{
	return ;
}

Parser::~Parser(void)
{
	return ;
}

bool
Parser::checkFileName(std::string const &filename)
{
	if (filename.find(".mod1") == std::string::npos)
		return (false);
	return (true);
}

template<typename T> bool
Parser::error(std::string const missing, T const near)
{
	std::cerr	<< "Parser: format error ! missing `"
				<< missing
				<< "` near `"
				<< near
				<< "`"
				<< std::endl;
	return (false);
}

bool
Parser::parsePoints(std::string const &filename)
{
	float			x, y, z;
	char			c;
	struct stat		buffer;

	if (stat(filename.c_str(), &buffer) != 0)
	{
		std::cerr << "Can't open file: " << filename << std::endl;
		return (0);
	}
	std::fstream	filestream(filename, std::ios_base::in);
	while (filestream >> c)
	{
		if (c == '(')
		{
			filestream >> x;
			filestream >> c;
			if (c != ',')
				return (error(",", x));
			filestream >> y;
			filestream >> c;
			if (c != ',')
				return (error(",", y));
			filestream >> z;
			z = z == 0 || z == 0.0 ? 0.1 : z;
			filestream >> c;
			if (c != ')')
				return (error(")", z));
			points.emplace_back(x, y, z);
		}
		else
			break ;
	}
	filestream.close();
	return (true);
}

Parser
&Parser::operator=(Parser const &rhs)
{
	if (this != &rhs)
	{
		// copy members here
	}
	return (*this);
}

std::ostream
&operator<<(std::ostream &o, Parser const &i)
{
	o	<< "Parser: " << &i;
	return (o);
}
