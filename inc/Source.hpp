
#ifndef		SOURCE_HPP
# define	SOURCE_HPP

# include <iostream>

class Source
{
public:
	int				x;
	int				y;
	int				w;
	int				h;
	double			inc;

	Source(int const &x, int const &y, int const &w, int const &h, double const &inc);
	~Source(void);

	Source				&operator=(Source const &rhs);

private:
	Source(void);
	Source(Source const &src);
};

std::ostream				&operator<<(std::ostream &o, Source const &i);

#endif
