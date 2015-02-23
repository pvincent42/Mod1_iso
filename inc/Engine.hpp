
#ifndef		ENGINE_HPP
# define	ENGINE_HPP

# include <iostream>
# include <stdint.h>
# include <string>
# include <sstream>
# include <list>
# include "Camera.hpp"
# include "Constants.hpp"
# include "Map.hpp"
# include "Point.hpp"

class Engine
{
private:
	Map					_map;

public:
	SDL_Window			*window;
	SDL_GLContext		context;
	uint32_t			window_width;
	uint32_t			window_height;
	Camera				*camera;
	GLuint				groundList;

	Engine(bool lava, int source_number, bool snow, bool moche, bool wave);
	Engine(Engine const &src);
	~Engine(void);

	void				compileDisplayList(void);
	void				reshape(int const &x, int const &y);
	int					sdlError(int code);
	int					getDisplayMode(void);
	int					init(void);
	void				update(void);
	void				render(void);
	void				renderAxes(void);
	void				loop(void);
	// float				getDistance(int x, int y, int x1, int y1);
	// void				getWeight(void);
	void				generateTerrain(std::list<Point> const &points);
	Engine				&operator=(Engine const &rhs);
};

std::ostream				&operator<<(std::ostream &o, Engine const &i);

#endif
