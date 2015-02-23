
#include <math.h>
#include <cstdlib>
#include <ctime>
#include "Engine.hpp"

Engine::Engine(bool lava, int source_number, bool snow, bool moche, bool wave) : _map(lava, source_number, snow, moche, wave)
{
	return ;
}

Engine::Engine(Engine const &src) : _map(0, 0, 0, 0, 0)
{
	*this = src;
	return ;
}

Engine::~Engine(void)
{
	SDL_Quit();
	return ;
}

int
Engine::sdlError(int code)
{
	std::cerr << "(" << code << ") " << "SDL Error: " << SDL_GetError() << std::endl;
	return (code);
}

void
Engine::reshape(int const &x, int const &y)
{
	if (x < y)
		glViewport(0, (y - x) / 2, x, x);
	if (y < x)
		glViewport((x - y) / 2, 0, y, y);
}

void
Engine::compileDisplayList(void)
{
	this->groundList = glGenLists(1);
	if (this->groundList == 0)
	{
		std::cerr << "List error." << std::endl;
		return ;
	}
	glNewList(this->groundList, GL_COMPILE);
	this->_map.renderGround();
	glEndList();
}

void
Engine::generateTerrain(std::list<Point> const &p)
{
	float								tmpWeight = 0;
	float								result = 0;
	float								pond = 0;
	int									x;
	int									y;
	float								dis;
	int									b = 0;
	std::list<Point>::const_iterator	i;

	x = 0;
	while (x < SIZE)
	{
		y = 0;
		while (y < SIZE)
		{
			for (i = p.begin(); i != p.end(); ++i)
			{
				dis = sqrtf((x - (*i).x) * (x - (*i).x) + (y - (*i).y) * (y - (*i).y));
				pond += 1.0 / (dis * dis);
				tmpWeight += ((*i).z + GROUND) / (dis * dis);
				if ((*i).x == x && (*i).y == y)
					b = (*i).z + GROUND;
			}
			result = tmpWeight / pond;
			pond = 0;
			tmpWeight = 0;
			if (b != 0)
				result = b;
			if (isnan(result))
				result = GROUND;
			this->_map.a1[y * SIZE + x].setGL(result);
			y++;
			b = 0;
		}
		y = 0;
		x++;
	}
}

int
Engine::getDisplayMode(void)
{
	static int32_t		display_in_use = 0; /* Only using first display */
	int32_t				i;
	int32_t				display_mode_count;
	SDL_DisplayMode		mode;

	display_mode_count = SDL_GetNumDisplayModes(display_in_use);
	if (display_mode_count < 1)
		return (0);
	i = 0;
	while (i < display_mode_count)
	{
		if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0)
		{
			std::cerr << "SDL_GetDisplayMode failed: " << SDL_GetError() << std::endl;
			return (0);
		}
		if (i == 0)
		{
			this->window_width = mode.w;
			this->window_height = mode.h;
		}
		++i;
	}
	return (1);
}

int
Engine::init(void)
{
	srandom(time(NULL));
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return (sdlError(0));
	this->window_width = 1400;
	this->window_height = 1400;
	this->window = SDL_CreateWindow("Mod1",
									SDL_WINDOWPOS_UNDEFINED,
									SDL_WINDOWPOS_UNDEFINED,
									this->window_width,
									this->window_height,
									SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (this->window == NULL)
		return (sdlError(0));
	if (!(this->context = SDL_GL_CreateContext(this->window)))
		return (sdlError(0));
	glViewport(0, 0, this->window_width, this->window_height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	this->camera = new Camera();
	return (1);
}

void
Engine::renderAxes(void)
{
	static float const		x = 0;
	static float const		y = 0;
	static float const		z = 0;

	//render axes
	glBegin(GL_LINES);
	// X red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(x, 0.0f, 0.0f);
	glVertex3f(x + SIZE, 0.0f, 0.0f);
	// Y green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, y, 0.0f);
	glVertex3f(0.0f, y + SIZE, 0.0f);
	// Z blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, z);
	glVertex3f(0.0f, 0.0f, z + SIZE);
	glEnd();
}

void
Engine::render(void)
{
	float		a = this->window_width / this->window_height;

	// glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(240.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
	this->camera->look();
	// glRotatef(-this->_angleX, 0.0f, 1.0f, 0.0f);
	// glRotatef(this->_angleY, 1.0f, 0.0f, 0.0f);
	if (a >= 1.0f)
		glOrtho(-SIZE * a, SIZE * a, -SIZE, SIZE, SIZE, -SIZE);
	else
		glOrtho(-SIZE, SIZE, -SIZE / a, SIZE / a, SIZE, -SIZE);
	this->renderAxes();
	glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
	// render here
	glCallList(this->groundList);
	this->_map.renderWater();
	this->_map.renderWaterBorders();
	glFlush();
}

void
Engine::update(void)
{
	// update here
	if (this->_map.moche == 0)
		this->_map.update();
	else
		this->_map.waterInvasion();
}

void
Engine::loop(void)
{
	SDL_Event			event;
	int32_t				quit;
	uint32_t			start;
	uint32_t			result;
	uint32_t			frame_count;
	uint32_t			total_frame_time;
	uint32_t			average;
	std::stringstream	out;

	quit = 0;
	frame_count = 0;
	total_frame_time = 0;
	while (!quit)
	{
		start = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_MOUSEBUTTONDOWN:
					camera->onMouseButton(event.button);
					break;
				case SDL_MOUSEBUTTONUP:
					camera->onMouseButton(event.button);
				case SDL_MOUSEMOTION:
					camera->onMouseMotion(event.motion);
					break;
				case SDL_MOUSEWHEEL:
					camera->onMouseWheel(event.wheel);
					break;
				case SDL_KEYUP:
					camera->onKeyboard(event.key);
					break;
				case SDL_KEYDOWN:
					camera->onKeyboard(event.key);
					if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
						quit = 1;
					break;
			}
		}
		this->update();
		this->render();
		// SDL_Delay(500);
		// SDL_Delay(60 / 1000);
		frame_count++;
		result = SDL_GetTicks() - start;
		total_frame_time += result;
		average = total_frame_time / frame_count;
		out << "frame: " << frame_count << " took " << result << "ms, " << "average is: " << average;
		SDL_SetWindowTitle(this->window, out.str().c_str());
		out.str("");
		out.clear();
		SDL_GL_SwapWindow(this->window);
	}
}

Engine
&Engine::operator=(Engine const &rhs)
{
	if (this != &rhs)
	{
		// copy members here
	}
	return (*this);
}

std::ostream
&operator<<(std::ostream &o, Engine const &i)
{
	o	<< "Engine: " << &i;
	return (o);
}
