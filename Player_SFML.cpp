/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player_SFML.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdubray <mdubray@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/16 16:41:19 by mdubray           #+#    #+#             */
/*   Updated: 2015/08/12 20:06:44 by mdubray          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Player_SFML.hpp"

Player::Player(int x, int y): _x_pos(4), _y_pos(4), _size(10), _alive(true), _score(0), _direction(2), _x_pos_food(0), _y_pos_food(0), _win(sf::VideoMode(x + _size, y + _size), "Snake sfml", sf::Style::Close | sf::Style::Titlebar)
{
	if (!_font.loadFromFile("LEDCalculator.ttf"))
	{
		std::cout << "Error can't load the font" << std::endl;
		this->~Player();
	}
	// positionnement du reste du corps du snake
	for (size_t i = 0; i < _x_pos.size(); i++)
	{
		_x_pos[i] = ((_win.getSize().x / _size) / 2) * _size;
		_y_pos[i] = ((_win.getSize().y / _size) / 2) * _size;
	}
	//_win.setFramerateLimit(10);
	this->generateFood();
}
Player::Player(int x, int y, std::vector<int> x_pos, std::vector<int> y_pos, int size, bool alive, int score, int direction, int x_pos_food, int y_pos_food): _x_pos(x_pos), _y_pos(y_pos), _size(size), _alive(alive), _score(score), _direction(direction), _x_pos_food(x_pos_food), _y_pos_food(y_pos_food), _win(sf::VideoMode(x + _size, y + _size), "Snake sfml", sf::Style::Close | sf::Style::Titlebar)
{
	if (!_font.loadFromFile("LEDCalculator.ttf"))
	{
		std::cout << "Error can't load the font" << std::endl;
		this->~Player();
	}
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
	_win.close();
}

void		Player::get_input(int *gui)
{
	int		dir = _direction;
	if (_win.isOpen())
	{
		sf::Event event;
		while (_win.pollEvent(event))
		{
			switch(event.type)
			{
				case sf::Event::Closed:
					_alive = false;
					_win.close();
					break;
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape)
					{
						_alive = false;
						_win.close();
					}
					else if (event.key.code == sf::Keyboard::Num1)
						*gui = 0;
					else if (event.key.code == sf::Keyboard::Num2)
						*gui = 1;
					else if (event.key.code == sf::Keyboard::Num3)
						*gui = 2;
					else if (event.key.code == sf::Keyboard::Left && dir != 2)
						_direction = 0;
					else if (event.key.code == sf::Keyboard::Down && dir != 3)
						_direction = 1;
					else if (event.key.code == sf::Keyboard::Right && dir != 0)
						_direction = 2;
					else if (event.key.code == sf::Keyboard::Up && dir != 1)
						_direction = 3;
					break;
				default:
					break;
			}
		}
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
	for (size_t i = 0; i < _x_pos.size(); i++)
	{
		if (i != 0 && this->getPosX() == _x_pos[i] && this->getPosY() == _y_pos[i])
			_alive = false;
		if (_x_pos[i] <= _size || _x_pos[i] >= (_win.getSize().x - _size * 2) || _y_pos[i] <= _size || _y_pos[i] >= (_win.getSize().y - _size * 2))
			_alive = false;
	}

	if (_alive && getPosX() == _x_pos_food && getPosY() == _y_pos_food)
	{
		this->grow();
		_score += 10;
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
	int		x_rn = (rand()%((_win.getSize().x / _size) - _size * 2) + _size) * _size;
	int		y_rn = (rand()%((_win.getSize().y / _size) - _size * 2) + _size) * _size;
	bool	flag = false;

	while (flag == false)
	{
		x_rn = (rand()%((_win.getSize().x / _size) - _size * 2) + _size) * _size;
		y_rn = (rand()%((_win.getSize().y / _size) - _size * 2) + _size) * _size;
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

int		Player::display()
{
	sf::RectangleShape	rectangle(sf::Vector2f(1, 1));
	sf::CircleShape		circle(_size / 2);
	sf::Text			text = sf::Text();
	sf::Font			font;

	_win.clear(sf::Color(255, 255, 255, 255));
	//draw snake
	for (size_t i = 0; i < _x_pos.size(); i++)
	{
		if (i == 0)
		{
			circle.setPosition(_x_pos[i], _y_pos[i]);
			circle.setFillColor(sf::Color(90, 78, 174, 255));
			_win.draw(circle);
		}
		else
		{
			rectangle.setSize(sf::Vector2f(_size, _size));
			rectangle.setPosition(_x_pos[i], _y_pos[i]);
			rectangle.setFillColor(sf::Color(255, i * (255 / _x_pos.size()), 0, 255));
			_win.draw(rectangle);
		}
	}
	//draw food
	rectangle.setFillColor(sf::Color(0, 255, 0, 255));
	rectangle.setPosition(_x_pos_food, _y_pos_food);
	_win.draw(rectangle);

	//draw border
	rectangle.setPosition(_size * 2, _size * 2);
	rectangle.setSize(sf::Vector2f(_win.getSize().x - _size * 4, _win.getSize().y - _size * 4));
	rectangle.setFillColor(sf::Color(0, 0, 0, 0));
	rectangle.setOutlineColor(sf::Color(0, 0, 0, 255));
	rectangle.setOutlineThickness(_size);
	_win.draw(rectangle);

	//draw hud
	text.setFont(_font);
	text.setString("Score : " + std::to_string(_score) + " ");
	text.setCharacterSize(_size * 1.5);
	text.setColor(sf::Color::Black);
	text.setPosition(_size, -_size / 2);
	_win.draw(text);

	_win.display();
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
