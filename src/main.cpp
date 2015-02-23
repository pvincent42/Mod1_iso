
#include <iostream>
#include <limits.h>
#include <cstdlib>
#include <ctime>
#include "Engine.hpp"
#include "Parser.hpp"

int
main(int argc, char **argv)
{
	bool	lava;
	bool	snow;
	bool	moche;
	bool	wave;
	int		source;
	Parser	parser;

	lava = 0;
	source = 0;
	snow = 0;
	moche = 0;
	wave = 0;
	if (argc >= 3)
	{
		if (argv[1][0] == 'l')
			lava = 1;
		else if (isdigit(argv[1][0]))
		{
			source = argv[1][0] - 48;
			if (source < 1 || source > 4)
				source = 0;
		}
		else if (argv[1][0] == 's')
			snow = 1;
		else if (argv[1][0] == 'm')
			moche = 1;
		else if (argv[1][0] == 'w')
		{
			wave = 1;
			source = 4;
		}
	}
	Engine		engine(lava, source, snow, moche, wave);
	srandom(time(NULL));
	if (argc >= 3)
	{
		if (!parser.checkFileName(argv[2]))
		{
			std::cerr << argv[2] << " is not a mod1 file !" << std::endl;
			return (0);
		}
		if (!parser.parsePoints(argv[2]))
			return (0);
	}
	else
	{
		std::cerr << "I need an option and a valid mod1 map." << std::endl;
		std::cerr << "./mod1 [options] [map]" << std::endl;
		std::cerr << "options :\n    l = lava\n    0 = rain\n    [1-4] = source number\n    m = water from depth\n    s = snow\n    w = wave\n    Wrong option will go for rain. Yes. Rain." << std::endl;
		return (0);
	}
	if (!engine.init())
		return (0);
	engine.generateTerrain(parser.points);
	engine.compileDisplayList();
	engine.loop();
	return (0);
}
