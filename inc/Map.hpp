
#ifndef		MAP_HPP
# define	MAP_HPP

# include <iostream>
# include <limits>
# include "Cell.hpp"
# include "Constants.hpp"
# include "Camera.hpp"
# include "Source.hpp"

class Map
{
public:
	Source			*sources[4];
	Cell			*a1;
	int				total_size;
	bool			lava;
	bool			snow;
	bool			moche;
	bool			wave;
	int				source_number;

	Map(bool lava, int source_number, bool snow, bool moche, bool wave);
	Map(Map const &src);
	~Map(void);

	Map				&operator=(Map const &rhs);

	void			applyColor(int h);
	void			updateSource(void);
	void			generateRain(void);
	void			update(void);
	void			renderWater(void);
	void			renderWaterBorders(void);
	void			updateLava(void);
	void			renderGround(void);
	void			waterInvasion(void);
};

std::ostream				&operator<<(std::ostream &o, Map const &i);

#endif
