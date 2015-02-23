
#include "Map.hpp"
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <functional>
#include <chrono>

Map::Map(bool laValue, int source_number, bool snow, bool moche, bool wave)
{
	float			inc = SOURCE_POWER;
	float			init = 0.0f;
	int				x;
	int				y;
	int				i;
	Cell			*c;

/*	// this->sources[0] = new Source(3, 0, SIZE - 3, 1, inc);
	// this->sources[1] = new Source(0, 3, 1, SIZE - 3, inc);
	// this->sources[2] = new Source(3, SIZE - 1, SIZE - 3, SIZE, inc);
	// this->sources[3] = new Source(SIZE - 1, 3, SIZE, SIZE - 3, inc);
	this->sources[0] = NULL;
	this->sources[1] = NULL;
	this->sources[2] = NULL;
	this->sources[3] = NULL;*/
	this->lava = laValue;
	this->snow = snow;
	this->source_number = source_number;
	this->moche = moche;
	this->wave = wave;
	this->sources[0] = new Source(3, 0, SIZE - 3, 1, inc);
	this->sources[1] = new Source(0, 3, 1, SIZE - 3, inc);
	this->sources[2] = new Source(3, SIZE - 1, SIZE - 3, SIZE, inc);
	this->sources[3] = new Source(SIZE - 1, 3, SIZE, SIZE - 3, inc);
	this->total_size = SIZE * SIZE;
	this->a1 = new Cell[this->total_size];
	// initialize sources
	i = 0;
	x = 0;
	y = 0;
	if (this->lava == 0)
	{
		while (i < this->source_number)
		{
			if (this->sources[i] != NULL)
			{
				y = this->sources[i]->y;
				while (y < this->sources[i]->h)
				{
					x = this->sources[i]->x;
					while (x < this->sources[i]->w)
					{
						c = &this->a1[y * SIZE + x];
						c->setWL(c->getWL() + init);
						c->source = inc;
						x++;
					}
					y++;
				}
			}
			i++;
		}
	}
		// copy a1 -> a2
	/*	for (i = 0; i < this->total_size; ++i)
	{
		this->a2[i].source = this->a1[i].source;
		this->a2[i].setWL(this->a1[i].getWL());
	}*/
	return ;
}

Map::Map(Map const &src)
{
	*this = src;
	return ;
}

Map::~Map(void)
{
	delete [] this->a1;
	// delete [] this->a2;
	return ;
}

void
Map::applyColor(int h)
{
	float	test = (float(h) - GROUND) / 200;
	float	grey = (200 + float(h) - GROUND) / 255;
	if (h >= 125 + GROUND)
		glColor3f(1, 1, 1);
	else if (h >= 100 + GROUND)
		glColor3f(0.7, 0.7, 0.7);
	else if (h >= 50 + GROUND)
		glColor3f(0.8, 0.6, 0.53);
	else if (h >= -50 + GROUND)
		glColor3f(0.41, 0.80 + test , 0.12);
	else
		glColor3f(grey, grey, grey);
}

void
Map::generateRain(void)
{
	int				i;
	int				batch = random() % RAIN_BATCH;
	int				j;

	if (this->lava == 0)
	{
		i = 0;
		while (i < batch)
		{
			j = (random() % SIZE) * SIZE + (random() % SIZE);
			this->a1[j].setWL(this->a1[j].getWL() + RAIN_AMOUNT);
			++i;
		}
	}
}

void
Map::updateSource(void)
{
	int				ci = -1;	// neighbours index
	int				j;								// j terator
	int				x;								// x ter(min)ator
	int				y;								// y(mca) terator
	static double	water_level_source = 0;

	j = 0;
	x = 0;
	y = 0;
	while (j < source_number)
	{
		if (this->sources[j] != NULL)
		{
			y = this->sources[j]->y;
			while (y < this->sources[j]->h)
			{
				x = this->sources[j]->x;
				while (x < this->sources[j]->w)
				{
					ci = y * SIZE + x;
					if (this->a1[ci].getGL() + this->a1[ci].getWL() <= GROUND + water_level_source)
						this->a1[ci].setWL(this->a1[ci].getWL() + SOURCE_POWER);
					x++;
				}
				y++;
			}
		}
		j++;
	}
	water_level_source += SOURCE_POWER;
}

void
Map::updateLava(void)
{
	int			x;
	int			y;
	int			ci;
	int			lava_update = 0;
	static int	lava_rythm = 0;

	if (lava_rythm % LAVA_PULSE == 0)
	{
		while (lava_update < LAVA_SPEED)
		{
			y = 0;
			while (y < SIZE)
			{
				x = 0;
				while (x < SIZE)
				{
					ci = y * SIZE + x;
					if (this->a1[ci].getGL() >= 100 + GROUND)
						this->a1[ci].setWL(this->a1[ci].getWL() + LAVA_POWER);
				++x;
				}
			++y;
			}
			lava_update++;
		}
	lava_rythm = 1;
	}
	else
		lava_rythm++;
		// copy a2 into a1
		// for (i = 0; i < this->total_size; ++i)
			// this->a1[i].setWL(this->a2[i].getWL());
}

void
Map::waterInvasion(void)
{
	int				x;
	int				y;
	int				ci;
	static float	level = -200.0f + GROUND;

	y = 0;
	while (y < SIZE)
	{
		x = 0;
		while (x < SIZE)
		{
			ci = y * SIZE + x;
			if (this->a1[ci].getGL() + this->a1[ci].getWL() <= level)
				this->a1[ci].setWL(this->a1[ci].getWL() + 0.5f);
		++x;
		}
	++y;
	}
	level += 0.5f;
	// copy a2 into a1
	// for (i = 0; i < this->total_size; ++i)
	// this->a1[i].setWL(this->a2[i].getWL());
}


void
Map::update(void)
{
	// iterators
	int			x;
	int			y;
	int			i;

	int			ci[5] = {-1, -1, -1, -1, -1};	// index

	double		gwl[5] = {};					// ground + water level
	double		gwlmax;							// ground + water level of highest neighbour

	double		diff;							// gwl difference between current cell and highest neighbour
	double		gld;							// ground level difference
	double		wld;							// water level difference
	double		trsf;							// water transfer to `count` neighbours
	int			count;							// number of neighbours to update
	static int	source_pulse = 0;
	static int	rain_pulse = 0;
	int			water_update = 0;

	if (source_pulse % PULSE == 0 && this->lava == 0 && this->source_number != 0)
	{
		this->updateSource();
		source_pulse = 1;
	}
	else if (this->lava == 0 && this->source_number != 0)
		++source_pulse;
	else if (this->lava == 1)
		this->updateLava();
	else if (this->source_number == 0)
	{
		if (rain_pulse % RAIN_PULSE == 0)
		{
			this->generateRain();
			rain_pulse = 1;
		}
		else
			++rain_pulse;
	}
	if (this->snow == 1)
		return ;
	while (water_update < WATER_SPEED)
	{
		y = 0;
		while (y < SIZE)
		{
			x = 0;
			while (x < SIZE)
			{
				ci[0] = y * SIZE + x;
				if (this->a1[ci[0]].getWL() > 0)
				{
					// get index of neighbours
					ci[1] = y - 1 >= 0 ? (y - 1) * SIZE + x : -1;
					ci[2] = x + 1 < SIZE ? y * SIZE + x + 1 : -1;
					ci[3] = y + 1 < SIZE ? (y + 1) * SIZE + x : -1;
					ci[4] = x - 1 >= 0 ? y * SIZE + x - 1 : -1;
					// get neighbours ground + water level
					i = 0;
					while (i < 5)
					{
						if (ci[i] != -1)
							gwl[i] = this->a1[ci[i]].getGL() + this->a1[ci[i]].getWL();
						else
							gwl[i] = 300000.0; // high value
						++i;
					}
					// reset neighbours count and highest ground + water level (gwlm)
					count = 0;
					gwlmax = 0.0;
					// get highest neighbour and set number and pointers of transferable neighbours
					i = 1;
					while (i < 5)
					{
						if (gwl[0] > gwl[i] && this->a1[ci[i]].source != SOURCE_POWER)
						{
							if (gwlmax < gwl[i])
								gwlmax = gwl[i];
							++count;
						}
						else
							ci[i] = -1;
						++i;
					}
					// do water transfer if one neighbour or more
					if (count > 0)
					{
						// get difference between ground + water levels of current cell and highest neighbour
						diff = gwl[0] - gwlmax;
						// get ground level difference
						gld = diff - this->a1[ci[0]].getWL();
						// get water level difference
						wld = gld > 0 ? (diff - gld) : diff;
						// get water to transfer into each neighbour
						trsf = wld / count;
						if (trsf > 0.000000000000001)
						{
							if (this->a1[ci[0]].getWL() > wld)
							{
								this->a1[ci[0]].setWL(this->a1[ci[0]].getWL() - wld);
								i = 1;
								while (i < 5)
								{
									if (ci[i] != -1)// && this->a1[ci[i]].source != SOURCE_POWER)
									{
										if (this->lava == 1 || this->source_number == 0 || this->wave == 1)
											this->a1[ci[i]].setWL(this->a1[ci[i]].getWL() + trsf);
										else
											this->a1[ci[i]].setWL(this->a1[ci[i]].getWL() + wld);
									}
									++i;
								}
							}
						}
					}
				}
				++x;
			}
			++y;
		}
		// copy a2 into a1
		// for (i = 0; i < this->total_size; ++i)
			// this->a1[i].setWL(this->a2[i].getWL());
		water_update++;
	}
}

void
Map::renderWater(void)
{
	int					x;
	int					y;
	double				gx[4];
	double				gy[4];
	double				gz[4];
	int					i[4];
	float				test;
	static int const	s = SIZE / 2;

	glBegin(GL_TRIANGLES);
	y = 0;
	while (y < SIZE - 1)
	{
		x = 0;
		while (x < SIZE - 1)
		{
			gx[0] = x - s;
			gy[0] = y - s;
			gx[1] = x + 1 - s;
			gy[1] = y - s;
			gx[2] = x + 1 - s;
			gy[2] = y + 1 - s;
			gx[3] = x - s;
			gy[3] = y + 1 - s;
			i[0] = y * SIZE + x;
			i[1] = y * SIZE + x + 1;
			i[2] = (y + 1) * SIZE + x + 1;
			i[3] = (y + 1) * SIZE + x;
			gz[0] = this->a1[i[0]].getWL();
			gz[1] = this->a1[i[1]].getWL();
			gz[2] = this->a1[i[2]].getWL();
			gz[3] = this->a1[i[3]].getWL();
			test = gz[0] + gz[1] + gz[2] + gz[3];
			test /= COLOR_ATTENUATION;
			if (this->lava == 1)
				glColor3f(1.5 - test, 0.4 - test, 0.1);
			else if (this-> snow == 1)
				glColor3f(1, 1, 1);
			else if (this->source_number == 0)
				glColor3f(0.5 - test, 0.5 - test, 1);
			else if (this->wave == 1)
				glColor3f(1 - test * 10, 1 - test * 10, 1);
			else
				glColor3f(1 - test, 1 - test, 1);
			if (gz[0] > 0.0 && gz[1] > 0.0 && gz[3] > 0.0)
			{
				glVertex3d(gx[0], gy[0], this->a1[i[0]].getGL() + gz[0] - Z_DECREMENT);
				glVertex3d(gx[1], gy[1], this->a1[i[1]].getGL() + gz[1] - Z_DECREMENT);
				glVertex3d(gx[3], gy[3], this->a1[i[3]].getGL() + gz[3] - Z_DECREMENT);
			}
			if (gz[1] > 0.0 && gz[2] > 0.0 && gz[3] > 0.0)
			{
				glVertex3d(gx[1], gy[1], this->a1[i[1]].getGL() + gz[1] - Z_DECREMENT);
				glVertex3d(gx[2], gy[2], this->a1[i[2]].getGL() + gz[2] - Z_DECREMENT);
				glVertex3d(gx[3], gy[3], this->a1[i[3]].getGL() + gz[3] - Z_DECREMENT);
			}
			if (gz[0] > 0.0 && gz[1] > 0.0 && gz[2] > 0.0)
			{
				glVertex3d(gx[0], gy[0], this->a1[i[0]].getGL() + gz[0] - Z_DECREMENT);
				glVertex3d(gx[1], gy[1], this->a1[i[1]].getGL() + gz[1] - Z_DECREMENT);
				glVertex3d(gx[2], gy[2], this->a1[i[2]].getGL() + gz[2] - Z_DECREMENT);
			}
			if (gz[0] > 0.0 && gz[2] > 0.0 && gz[3] > 0.0)
			{
				glVertex3d(gx[0], gy[0], this->a1[i[0]].getGL() + gz[0] - Z_DECREMENT);
				glVertex3d(gx[2], gy[2], this->a1[i[2]].getGL() + gz[2] - Z_DECREMENT);
				glVertex3d(gx[3], gy[3], this->a1[i[3]].getGL() + gz[3] - Z_DECREMENT);
			}
			x += 1;
		}
		y += 1;
	}
	glEnd();
}

void
Map::renderWaterBorders(void)
{
	int				x;
	int				y;
	const int		s = SIZE / 2;

	glBegin(GL_QUADS);
	x = 0;
	while (x < SIZE - 1)
	{
		// glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3d(x - s, -s, this->a1[x].getGL() + this->a1[x].getWL() - Z_DECREMENT);
		glVertex3d(x + 1 - s, -s, this->a1[x + 1].getGL() + this->a1[x + 1].getWL() - Z_DECREMENT);
		glVertex3d(x + 1 - s, -s, this->a1[x + 1].getGL() - Z_DECREMENT);
		glVertex3d(x - s, -s, this->a1[x].getGL() - Z_DECREMENT);

		// glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3d(x - s, s - 1, this->a1[(SIZE - 1) * SIZE + x].getGL() + this->a1[(SIZE - 1) * SIZE + x].getWL() - Z_DECREMENT);
		glVertex3d(x + 1 - s, s - 1, this->a1[(SIZE - 1) * SIZE + x + 1].getGL() + this->a1[(SIZE - 1) * SIZE + x + 1].getWL() - Z_DECREMENT);
		glVertex3d(x + 1 - s, s - 1, this->a1[(SIZE - 1) * SIZE + x + 1].getGL() - Z_DECREMENT);
		glVertex3d(x - s, s - 1, this->a1[(SIZE - 1) * SIZE + x].getGL() - Z_DECREMENT);
		++x;
	}
	y = 0;
	while (y < SIZE - 1)
	{
		// glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3d(-s, y - s, this->a1[y * SIZE + SIZE].getGL() + this->a1[y * SIZE].getWL() - Z_DECREMENT);
		glVertex3d(-s, y + 1 - s, this->a1[(y + 1) * SIZE].getGL() + this->a1[(y + 1) * SIZE].getWL() - Z_DECREMENT);
		glVertex3d(-s, y + 1 - s, this->a1[(y + 1) * SIZE].getGL() - Z_DECREMENT);
		glVertex3d(-s, y - s, this->a1[y * SIZE].getGL() - Z_DECREMENT);

		// glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3d(s - 1, y - s, this->a1[y * SIZE + SIZE - 1].getGL() + this->a1[y * SIZE + SIZE - 1].getWL() - Z_DECREMENT);
		glVertex3d(s - 1, y + 1 - s, this->a1[(y + 1) * SIZE + SIZE - 1].getGL() + this->a1[(y + 1) * SIZE + SIZE - 1].getWL() - Z_DECREMENT);
		glVertex3d(s - 1, y + 1 - s, this->a1[(y + 1) * SIZE + SIZE - 1].getGL() - Z_DECREMENT);
		glVertex3d(s - 1, y - s, this->a1[y * SIZE + SIZE - 1].getGL() - Z_DECREMENT);
		++y;
	}
	glEnd();
}

void
Map::renderGround(void)
{
	int					x;
	int					y;
	double				gx[4];
	double				gy[4];
	double				gz[4];
	static int const	s = SIZE / 2;

	glBegin(GL_TRIANGLES);
	y = 0;
	while (y < SIZE - 1)
	{
		x = 0;
		while (x < SIZE - 1)
		{
			gx[0] = x - s;
			gy[0] = y - s;
			gz[0] = this->a1[y * SIZE + x].getGL() - Z_DECREMENT;
			gx[1] = x + 1 - s;
			gy[1] = y - s;
			gz[1] = this->a1[y * SIZE + x + 1].getGL() - Z_DECREMENT;
			gx[2] = x + 1 - s;
			gy[2] = y + 1 - s;
			gz[2] = this->a1[(y + 1) * SIZE + x + 1].getGL() - Z_DECREMENT;
			gx[3] = x - s;
			gy[3] = y + 1 - s;
			gz[3] = this->a1[(y + 1) * SIZE + x].getGL() - Z_DECREMENT;
			// first triangle
			this->applyColor(gz[0] + Z_DECREMENT);
			glVertex3d(gx[0], gy[0], gz[0]);
			this->applyColor(gz[1] + Z_DECREMENT);
			glVertex3d(gx[1], gy[1], gz[1]);
			this->applyColor(gz[3] + Z_DECREMENT);
			glVertex3d(gx[3], gy[3], gz[3]);
			// second triangle
			this->applyColor(gz[1] + Z_DECREMENT);
			glVertex3d(gx[1], gy[1], gz[1]);
			this->applyColor(gz[2] + Z_DECREMENT);
			glVertex3d(gx[2], gy[2], gz[2]);
			this->applyColor(gz[3] + Z_DECREMENT);
			glVertex3d(gx[3], gy[3], gz[3]);
			++x;
		}
		++y;
	}
	glEnd();
}

Map
&Map::operator=(Map const &rhs)
{
	if (this != &rhs)
	{
		// copy members here
	}
	return (*this);
}

std::ostream
&operator<<(std::ostream &o, Map const &i)
{
	o	<< "Map: " << &i;
	return (o);
}
