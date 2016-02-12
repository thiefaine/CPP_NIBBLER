/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player_GLFW.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdubray <mdubray@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/10 19:52:48 by mdubray           #+#    #+#             */
/*   Updated: 2015/08/13 15:11:08 by mdubray          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Player_GLFW.hpp"

Player::Player(int x, int y): _x_pos(4), _y_pos(4), _size(10), _alive(true), _score(1), _direction(2), _x_pos_food(0), _y_pos_food(0)
{
	if (!glfwInit())
	{
		std::cout << "Library not loaded" << std::endl;
		exit(EXIT_FAILURE);
	}
	_win = glfwCreateWindow(x + _size, y + _size, "Snake glfw", NULL, NULL);
	if (!_win)
	{
		std::cout << "Window not loaded" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(_win);
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST); // Depth Testing
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// positionnement du reste du corps du snake
	for (size_t i = 0; i < _x_pos.size(); i++)
	{
		_x_pos[i] = ((x / _size) / 2) * _size;
		_y_pos[i] = ((y / _size) / 2) * _size;
	}
	//_win.setFramerateLimit(10);
	this->generateFood();
}
Player::Player(int x, int y, std::vector<int> x_pos, std::vector<int> y_pos, int size, bool alive, int score, int direction, int x_pos_food, int y_pos_food): _x_pos(x_pos), _y_pos(y_pos), _size(size), _alive(alive), _score(score), _direction(direction), _x_pos_food(x_pos_food), _y_pos_food(y_pos_food)
{
	if (!glfwInit())
	{
		std::cout << "Library not loaded" << std::endl;
		exit(EXIT_FAILURE);
	}
	_win = glfwCreateWindow(x + _size, y + _size, "Snake glfw", NULL, NULL);
	if (!_win)
	{
		std::cout << "Window not loaded" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(_win);
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST); // Depth Testing
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}


Player::Player(Player const & cpy)
{
	*this = cpy;
}

Player &	Player::operator=(Player const & rhs)
{
	if (this != &rhs)
	{
		this->_x_pos = rhs._x_pos;
		this->_y_pos = rhs._y_pos;
		this->_alive = rhs._alive;
		this->_score = rhs._score;
		this->_direction = rhs._direction;
		this->_x_pos_food = rhs._x_pos_food;
		this->_y_pos_food = rhs._y_pos_food;
	}
	return (*this);
}

Player::~Player()
{
	glfwDestroyWindow(_win);
	glfwTerminate();
}

void		Player::get_input(int *gui)
{
	/*	UP & DOWN ARE REVERSE BEACAUSE THE ORIGIN ARE DIFFERENT */
	if (!glfwWindowShouldClose(_win))
	{
		glfwPollEvents();
		if (glfwGetKey(_win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			_alive = false;
			glfwDestroyWindow(_win);
			glfwTerminate();
		}
		else if (glfwGetKey(_win, GLFW_KEY_1) == GLFW_PRESS)
			*gui = 0;
		else if (glfwGetKey(_win, GLFW_KEY_2) == GLFW_PRESS)
			*gui = 1;
		else if (glfwGetKey(_win, GLFW_KEY_3) == GLFW_PRESS)
			*gui = 2;
		else if (glfwGetKey(_win, GLFW_KEY_LEFT) == GLFW_PRESS && _direction != 2)
			_direction = 0;
		else if (glfwGetKey(_win, GLFW_KEY_DOWN) == GLFW_PRESS && _direction != 3)
			_direction = 1;
		else if (glfwGetKey(_win, GLFW_KEY_RIGHT) == GLFW_PRESS && _direction != 0)
			_direction = 2;
		else if (glfwGetKey(_win, GLFW_KEY_UP) == GLFW_PRESS && _direction != 1)
			_direction = 3;
	}
}

// 0 = left
// 1 = down
// 2 = right
// 3 = up
void		Player::move()
{
	int		prev_tmp[2] = {_x_pos[0], _y_pos[0]};
	int		tmp[2] = {prev_tmp[0], prev_tmp[1]};

	// the head moves
	if (_direction == 0)
		setPosX(getPosX() - _size);
	else if (_direction == 1)
		setPosY(getPosY() + _size);
	else if (_direction == 2)
		setPosX(getPosX() + _size);
	else if (_direction == 3)
		setPosY(getPosY() - _size);

	// move all the coords to one after the other;
	int		i = 1;		//--> start to one after the head : 0 + 1
	while (i < _x_pos.size())
	{
		// take the coords of the previous one
		tmp[0] = prev_tmp[0];
		tmp[1] = prev_tmp[1];
		prev_tmp[0] = _x_pos[i];
		prev_tmp[1] = _y_pos[i];
		_x_pos[i] = tmp[0];
		_y_pos[i] = tmp[1];
		i++;
	}

	// Collision detection
	int		width(0);
	int		height(0);

	glfwGetFramebufferSize(_win, &width, &height);

	for (size_t i = 0; i < _x_pos.size(); i++)
	{
		if (i != 0 && this->getPosX() == _x_pos[i] && this->getPosY() == _y_pos[i])
			_alive = false;
		if (_x_pos[i] <= _size || _x_pos[i] >= (width - _size * 2) || _y_pos[i] <= _size || _y_pos[i] >= (height - _size * 2))
			_alive = false;
	}

	if (_alive && getPosX() == _x_pos_food && getPosY() == _y_pos_food)
	{
		_score += 10;
		this->grow();
	}
}
void	Player::grow()
{
	// add one element to the end of the vector with same position than the previous one
	_x_pos.push_back(_x_pos[_x_pos.size() - 1]);
	_y_pos.push_back(_y_pos[_y_pos.size() - 1]);

	this->generateFood();
}
void	Player::generateFood()
{
	int		width(0);
	int		height(0);

	glfwGetFramebufferSize(_win, &width, &height);

	int		x_rn = (rand()%((width / _size) - _size * 2) + _size) * _size;
	int		y_rn = (rand()%((height / _size) - _size * 2) + _size) * _size;
	bool	flag = false;

	while (flag == false)
	{
		x_rn = (rand()%((width / _size) - _size * 2) + _size) * _size;
		y_rn = (rand()%((height / _size) - _size * 2) + _size) * _size;
		flag = true;

		for (size_t i = 0; i < _x_pos.size(); i++)
		{
			if (y_rn == _y_pos[i] && x_rn == _x_pos[i])
				flag = false;
		}
	}
	_x_pos_food = x_rn;
	_y_pos_food = y_rn;

}
bool	Player::is_alive()
{
	return _alive;
}

void	Player::draw_cube(int x, int y)
{
	int		width(0);
	int		height(0);

	glfwGetFramebufferSize(_win, &width, &height);

	glBegin(GL_POLYGON); // back
	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(x, y, 0.f);
	glVertex3f(x, y + _size, 0.f);
	glVertex3f(x + _size, y + _size, 0.f);
	glVertex3f(x + _size, y, 0.f);
	glEnd();

	glBegin(GL_POLYGON); // front
	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(x, y, -_size);
	glVertex3f(x, y + _size, -_size);
	glVertex3f(x + _size, y + _size, -_size);
	glVertex3f(x + _size, y, -_size);
	glEnd();

	glBegin(GL_POLYGON); // left
	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(x, y, 0.f);
	glVertex3f(x, y + _size, 0.f);
	glVertex3f(x, y + _size, -_size);
	glVertex3f(x, y, -_size);
	glEnd();

	glBegin(GL_POLYGON); // right
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(x + _size, y, 0.f);
	glVertex3f(x + _size, y + _size, 0.f);
	glVertex3f(x + _size, y + _size, -_size);
	glVertex3f(x + _size, y, -_size);
	glEnd();

	glBegin(GL_POLYGON); // top
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(x, y, 0.f);
	glVertex3f(x + _size, y, 0.f);
	glVertex3f(x + _size, y, -_size);
	glVertex3f(x, y, -_size);
	glEnd();

	glBegin(GL_POLYGON); // bottom
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(x, y + _size, 0.f);
	glVertex3f(x + _size, y + _size, 0.f);
	glVertex3f(x + _size, y + _size, -_size);
	glVertex3f(x, y + _size, -_size);
	glEnd();

	static float alpha = 0;
	//glRotatef(alpha, 0, 0, 1);
	alpha += 0.1;
}

int		Player::display()
{
	int		height(0);
	int		width(0);

	glfwGetFramebufferSize(_win, &width, &height);
	glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION_MATRIX);
	glLoadIdentity();
	gluPerspective( 100, static_cast<double>(width) / static_cast<double>(height), 0.9, 5);
	glOrtho(0, width, height, 0, 0, -_size * 50);
	glMatrixMode(GL_MODELVIEW_MATRIX);

	//draw snake
	for (size_t i = 0; i < _x_pos.size(); i++)
		this->draw_cube(_x_pos[i], _y_pos[i]);

	//draw food
	glBegin(GL_POLYGON); // bottom
	if (width != 0 && height != 0)
		glColor3f(static_cast<double>(_x_pos_food) / width, static_cast<double>(_y_pos_food) / height, 1.0);
	else
		glColor3f(_x_pos_food, _y_pos_food, 1.0);
	glVertex3f(_x_pos_food, _y_pos_food, 0.f);
	glVertex3f(_x_pos_food + _size, _y_pos_food, 0.f);
	glVertex3f(_x_pos_food + _size, _y_pos_food + _size, 0.f);
	glVertex3f(_x_pos_food, _y_pos_food + _size, 0.f);
	glEnd();

	//draw border
	glLineWidth(1);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	//first line
	glColor3f(1.0, 0.0, 0.0);
	glVertex2d(_size * 2, _size * 2);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2d(width - _size * 2, _size * 2);

	//second line
	glColor3f(0.0, 1.0, 0.0);
	glVertex2d(width - _size * 2, _size * 2);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2d(width - _size * 2, height - _size * 2);

	//third line
	glColor3f(0.0, 0.0, 1.0);
	glVertex2d(width - _size * 2, height - _size * 2);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2d(_size * 2, height - _size * 2);

	//fourth line
	glColor3f(1.0, 1.0, 1.0);
	glVertex2d(_size * 2, height - _size * 2);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2d(_size * 2, _size * 2);
	glEnd();

	//draw hud
	std::string	str = "Score : " + std::to_string(_score);
	glRasterPos2f(_size, _size);
	for (int i = 0; str[i]; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, static_cast<int>(str[i]));

	glfwSwapBuffers(_win);

	return (0);
}

//GETTERS
int		Player::getPosX()
{
	return (_x_pos[0]);
}
int		Player::getPosY()
{
	return (_y_pos[0]);
}
int					Player::getSize()
{
	return (_size);
}
int					Player::getScore()
{
	return (_score);
}
int					Player::getDirection()
{
	return (_direction);
}
int					Player::getPosXFood()
{
	return (_x_pos_food);
}
int					Player::getPosYFood()
{
	return (_y_pos_food);
}
std::vector<int>	Player::getVectorX()
{
	return (_x_pos);
}
std::vector<int>	Player::getVectorY()
{
	return (_y_pos);
}

//SETTERS
void	Player::setPosX(int x)
{
	_x_pos[0] = x;
}
void	Player::setPosY(int y)
{
	_y_pos[0] = y;
}

Player		*createPlayer(int x, int y)
{
	return new Player(x, y);
}

Player		*copyPlayer(int x, int y, std::vector<int> x_pos, std::vector<int> y_pos, int size, bool alive, int score, int direction, int x_pos_food, int y_pos_food)
{
	return new Player(x, y, x_pos, y_pos, size, alive, score, direction, x_pos_food, y_pos_food);
}

void		deletePlayer(Player *player)
{
	delete player;
}
