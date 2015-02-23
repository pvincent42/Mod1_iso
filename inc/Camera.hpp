
#ifndef		CAMERA_HPP
# define	CAMERA_HPP

# include <iostream>

# ifdef linux
#  include <GL/glu.h>
#  include <GL/gl.h>
#  include <SDL2/SDL.h>
# endif

# ifdef __APPLE__
#  include <openGL/glu.h>
#  include <openGL/gl.h>
#  include <SDL.h>
# endif

# ifdef __WIN32__
#  include <GL/gl.h>
#  include <windows.h>
#  include <SDL.h>
# endif

# define X_ROTATE		0b001
# define Y_ROTATE		0b010
# define Z_ROTATE		0b100

class Camera
{
public:
	Camera(void);
	virtual ~Camera(void);

	void				onMouseMotion(SDL_MouseMotionEvent const &e);
	void				onMouseButton(SDL_MouseButtonEvent const &e);
	void				onKeyboard(SDL_KeyboardEvent const &e);
	void				onMouseWheel(SDL_MouseWheelEvent const &e);
	void				look(void);
	void				setMotionSensivity(double sensivity);
	void				setScrollSensivity(double sensivity);
	double const		&getMotionSensivity(void) const;
	double const		&getScrollSensivity(void) const;
	bool const			&getHold(void) const;
	double const		&getDistance(void) const;
	double const		&getAngleY(void) const;
	double const		&getAngleZ(void) const;
	bool const			&getWheelUp(void) const;
	bool const			&getWheelDown(void) const;

	Camera				&operator=(Camera const &rhs);
private:
	int					_rotation;
	double				_motionSensivity;
	double				_scrollSensivity;
	bool				_hold;
	double				_distance;
	double				_angleY;
	double				_angleZ;
	bool				_wheelUp;
	bool				_wheelDown;

	Camera(Camera const &src);
};

std::ostream				&operator<<(std::ostream &o, Camera const &i);

#endif
