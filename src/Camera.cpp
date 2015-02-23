
#include "Camera.hpp"

Camera::Camera(void)
	:	_rotation(Z_ROTATE),
		_motionSensivity(0.3),
		_scrollSensivity(0.1),
		_hold(false),
		_distance(2),
		_angleY(0),
		_angleZ(0),
		_wheelUp(false),
		_wheelDown(false)
{
	return ;
}

Camera::~Camera(void)
{
	return ;
}

void
Camera::onMouseMotion(SDL_MouseMotionEvent const &e)
{
	if (_hold)
	{
		_angleZ += e.xrel * _motionSensivity;
		_angleY += e.yrel * _motionSensivity;
		if (_angleY > 90)
			_angleY = 90;
		else if (_angleY < -90)
			_angleY = -90;
	}
}

void
Camera::onMouseButton(SDL_MouseButtonEvent const &e)
{
	if (e.button == SDL_BUTTON_LEFT)
	{
		if (_hold && e.type == SDL_MOUSEBUTTONUP)
			_hold = false;
		else if (!_hold && e.type == SDL_MOUSEBUTTONDOWN)
			_hold = true;
	}
	else if (_wheelUp && e.type == SDL_MOUSEBUTTONDOWN)
	{
		_distance -= _scrollSensivity;
		if (_distance < 0.1)
			_distance = 0.1;
	}
	else if (_wheelDown && e.type == SDL_MOUSEBUTTONDOWN)
		_distance += _scrollSensivity;
}

void
Camera::onMouseWheel(SDL_MouseWheelEvent const &e)
{
	if (e.y < 0)
	{
		_wheelDown = true;
		_wheelUp = false;
	}
	else
	{
		_wheelUp = true;
		_wheelDown = false;
	}
}

void
Camera::onKeyboard(SDL_KeyboardEvent const &e)
{
	if (e.type == SDL_KEYDOWN)
	{
		if (e.keysym.sym == SDLK_HOME)
		{
			_angleY = 0;
			_angleZ = 0;
		}
		e.keysym.sym == SDLK_x ? _rotation |= X_ROTATE : _rotation &= ~X_ROTATE;
		e.keysym.sym == SDLK_y ? _rotation |= Y_ROTATE : _rotation &= ~Y_ROTATE;
		e.keysym.sym == SDLK_z ? _rotation |= Z_ROTATE : _rotation &= ~Z_ROTATE;
	}
}

void
Camera::look()
{
	// gluLookAt(0.1, 0, 0, 0, 0, 0, 1, 1, 1);
	 // glRotated(_angleY, 0, 1, 0);
	// glRotated(_angleZ, _rotation & X_ROTATE, 0, 0);
	// glRotated(_angleY, 0, 1, 0);
	glRotated(_angleZ, 0, 0, 1);
}

double const
&Camera::getMotionSensivity(void) const
{
	return (_motionSensivity);
}

double const
&Camera::getScrollSensivity(void) const
{
	return (_scrollSensivity);
}

bool const
&Camera::getHold(void) const
{
	return (_hold);
}

double const
&Camera::getDistance(void) const
{
	return (_distance);
}

double const
&Camera::getAngleY(void) const
{
	return (_angleY);
}

double const
&Camera::getAngleZ(void) const
{
	return (_angleZ);
}

bool const
&Camera::getWheelUp(void) const
{
	return (_wheelUp);
}

bool const
&Camera::getWheelDown(void) const
{
	return (_wheelDown);
}

Camera
&Camera::operator=(Camera const &rhs)
{
	if (this != &rhs)
	{
		// copy members here
	}
	return (*this);
}

std::ostream
&operator<<(std::ostream &o, Camera const &i)
{
	o	<< "Camera: " << &i;
	return (o);
}
