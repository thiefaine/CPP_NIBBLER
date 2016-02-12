/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player_MLX.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdubray <mdubray@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/10 16:50:37 by mdubray           #+#    #+#             */
/*   Updated: 2015/08/10 18:33:24 by mdubray          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include "Player_MLX.hpp"

Player::Player(int x, int y): _x_pos(4), _y_pos(4), _size(10), _alive(true), _speed(1), _direction(2), _x_pos_food(0), _y_pos_food(0), _width(x), _height(y), _mlx(mlx_init()), _win(mlx_new_window(_mlx, x, y, const_cast<char *>("Snake mlx")))
{
	// positionnement du reste du corps du snake
	for (size_t i = 0; i < _x_pos.size(); i++)
	{
		_x_pos[i] = ((_width / _size) / 2) * _size;
		_y_pos[i] = ((_height / _size) / 2) * _size;
	}
	this->generateFood();
}
Player::Player(int x, int y, std::vector<int> x_pos, std::vector<int> y_pos, int size, bool alive, int speed, int direction, int x_pos_food, int y_pos_food): _x_pos(x_pos), _y_pos(y_pos), _size(size), _alive(alive), _speed(speed), _direction(direction), _x_pos_food(x_pos_food), _y_pos_food(y_pos_food), _width(x), _height(y), _mlx(mlx_init()), _win(mlx_new_window(_mlx, x, y, const_cast<char *>("Snake mlx")))
{
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
		this->_speed = rhs._speed;
		this->_direction = rhs._direction;
		this->_x_pos_food = rhs._x_pos_food;
		this->_y_pos_food = rhs._y_pos_food;
		this->_width = rhs._width;
		this->_height = rhs._height;
	}
	return (*this);
}

Player::~Player()
{
}

void		Player::get_input(int *gui)
{
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
	for (size_t i = 0; i < _x_pos.size(); i++)
	{
		if (i != 0 && this->getPosX() == _x_pos[i] && this->getPosY() == _y_pos[i])
			_alive = false;
		if (_x_pos[i] <= _size || _x_pos[i] >= (_width - _size * 2) || _y_pos[i] <= _size || _y_pos[i] >= (_height - _size * 2))
			_alive = false;
	}

	if (_alive && getPosX() == _x_pos_food && getPosY() == _y_pos_food)
		this->grow();
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
	int		x_rn = (rand()%((_width / _size) - _size * 2) + _size) * _size;
	int		y_rn = (rand()%((_height / _size) - _size * 2) + _size) * _size;
	bool	flag = false;

	while (flag == false)
	{
		x_rn = (rand()%((_width / _size) - _size * 2) + _size) * _size;
		y_rn = (rand()%((_height / _size) - _size * 2) + _size) * _size;
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

void	Player::draw_square(int x, int y, int color)
{
	for(int i = x; i < (x + _size); i++)
	for(int j = y; j < (y + _size); j++)
	{
		mlx_pixel_put(_mlx, _win, i, j, color);
	}
}

int		Player::display()
{
	mlx_expose_hook(_win, NULL, NULL);
	mlx_clear_window(_mlx, _win);
	for (size_t i = 0; i < _x_pos.size(); i++)
	{
		this->draw_square(_x_pos[i], _y_pos[i], 0xff0000);
	}
	this->draw_square(_x_pos_food, _y_pos_food, 0x00ff00);
	mlx_loop(_mlx);
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
int					Player::getSpeed()
{
	return (_speed);
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

Player		*copyPlayer(int x, int y, std::vector<int> x_pos, std::vector<int> y_pos, int size, bool alive, int speed, int direction, int x_pos_food, int y_pos_food)
{
	return new Player(x, y, x_pos, y_pos, size, alive, speed, direction, x_pos_food, y_pos_food);
}

void		deletePlayer(Player *player)
{
	delete player;
}
