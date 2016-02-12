/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player_SDL.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdubray <mdubray@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/16 16:40:51 by mdubray           #+#    #+#             */
/*   Updated: 2015/08/12 20:06:55 by mdubray          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Player_SDL.hpp"

Player::Player(int x, int y): _x_pos(4), _y_pos(4), _size(10), _alive(true), _score(1), _direction(2), _x_pos_food(0), _y_pos_food(0), _win(NULL), _renderer(0)
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		std::cout << "SDL could not be initialize. SDL_error : " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		_win = SDL_CreateWindow("Snake sdl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x + _size, y + _size, SDL_WINDOW_SHOWN);
		if (!_win)
		{
			std::cout << "Window could not be created. SDL_error : " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}
		_renderer = SDL_CreateRenderer(_win, -1, 0);
		if (!_renderer)
		{
			std::cout << "Failed to create renderer : " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}
		SDL_RenderSetLogicalSize( _renderer, x, y);
		SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );
	}

	// INIT TTF
	if (TTF_Init() != 0)
	{
		std::cerr << "TTF_Init() Failed: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	if (!(_font = TTF_OpenFont("LEDCalculator.ttf", _size * 1.5)))
	{
		std::cout << "Can't load the font" << std::endl;
		this->~Player();
	}

	// positionnement du reste du corps du snake
	for (size_t i = 0; i < _x_pos.size(); i++)
	{
		_x_pos[i] = ((x / _size) / 2) * _size;
		_y_pos[i] = ((y / _size) / 2) * _size;
	}
	//_win.setFramerateLimit(10);
	this->generateFood();
}

Player::Player(int x, int y, std::vector<int> x_pos, std::vector<int> y_pos, int size, bool alive, int score, int direction, int x_pos_food, int y_pos_food): _x_pos(x_pos), _y_pos(y_pos), _size(size), _alive(alive), _score(score), _direction(direction), _x_pos_food(x_pos_food), _y_pos_food(y_pos_food), _win(NULL), _renderer(0)
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		std::cout << "SDL could not be initialize. SDL_error : " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		_win = SDL_CreateWindow("Snake sdl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x + _size, y + _size, SDL_WINDOW_SHOWN);
		if (!_win)
		{
			std::cout << "Window could not be created. SDL_error : " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}
		_renderer = SDL_CreateRenderer(_win, -1, 0);
		if (!_renderer)
		{
			std::cout << "Failed to create renderer : " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}
		SDL_RenderSetLogicalSize( _renderer, x, y);
		SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );
	}

	// INIT TTF
	if (TTF_Init() != 0)
	{
		std::cerr << "TTF_Init() Failed: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	if (!(_font = TTF_OpenFont("LEDCalculator.ttf", _size * 1.5)))
	{
		std::cout << "Can't load the font" << std::endl;
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
	TTF_Quit();
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_win);
	SDL_Quit();
}

void		Player::get_input(int *gui)
{
	int		dir = _direction;
	SDL_Event evenements;

	while (SDL_PollEvent(&evenements))
	{
		switch(evenements.type)
		{
			case SDL_QUIT:
				_alive = false;
				SDL_DestroyRenderer(_renderer);
				SDL_DestroyWindow(_win);
				SDL_Quit();
				break;
	    	case SDL_KEYDOWN:
	    		if (evenements.key.keysym.sym == SDLK_ESCAPE)
	    		{
					_alive = false;
	    			SDL_DestroyRenderer(_renderer);
					SDL_DestroyWindow(_win);
					SDL_Quit();
	    		}
	    		else if (evenements.key.keysym.sym == SDLK_1)
					*gui = 0;
	    		else if (evenements.key.keysym.sym == SDLK_2)
					*gui = 1;
	    		else if (evenements.key.keysym.sym == SDLK_3)
					*gui = 2;
	    		else if (evenements.key.keysym.sym == SDLK_LEFT && dir != 2)
 					_direction = 0;
	    		else if (evenements.key.keysym.sym == SDLK_RIGHT && dir != 0)
 					_direction = 2;
	    		else if (evenements.key.keysym.sym == SDLK_UP && dir != 1)
 					_direction = 3;
	    		else if (evenements.key.keysym.sym == SDLK_DOWN & dir != 3)
 					_direction = 1;
				break;
			default:
				break;
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
	int		i = 1;
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
	int		x_size;
	int		y_size;
	SDL_GetWindowSize(_win, &x_size, &y_size);

	for (size_t i = 0; i < _x_pos.size(); i++)
	{
		if (i != 0 && this->getPosX() == _x_pos[i] && this->getPosY() == _y_pos[i])
			_alive = false;
		if (_x_pos[i] <= _size || _x_pos[i] >= (x_size - _size * 2) || _y_pos[i] <= _size || _y_pos[i] >= (y_size - _size * 2))
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
	int		x_size;
	int		y_size;
	SDL_GetWindowSize(_win, &x_size, &y_size);

	int		x_rn = (rand()%((x_size / _size) - _size * 2) + _size) * _size;
	int		y_rn = (rand()%((y_size / _size) - _size * 2) + _size) * _size;
	bool	flag = false;

	while (flag == false)
	{
		x_rn = (rand()%((x_size / _size) - _size * 2) + _size) * _size;
		y_rn = (rand()%((y_size / _size) - _size * 2) + _size) * _size;
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
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	SDL_RenderClear(_renderer);
	SDL_Rect	rectangle;

	int		x_size;
	int		y_size;
	SDL_GetWindowSize(_win, &x_size, &y_size);

	//draw snake
	for (size_t i = 0; i < _x_pos.size(); i++)
	{
		rectangle.x = _x_pos[i];
		rectangle.y = _y_pos[i];
		rectangle.w = _size;
		rectangle.h = _size;
		if (i == 0)
			SDL_SetRenderDrawColor( _renderer, 21, 200, 145, 255 );
		else
			SDL_SetRenderDrawColor( _renderer, 255, 0, i * (255 / _x_pos.size()), 255 );
		SDL_RenderFillRect(_renderer, &rectangle);
	}
	//draw food
	rectangle.x = _x_pos_food;
	rectangle.y = _y_pos_food;
	SDL_SetRenderDrawColor( _renderer, 0, 0, 255, 255 );
	SDL_RenderFillRect(_renderer, &rectangle);

	//draw border
	rectangle.x = _size * 2;
	rectangle.y = _size * 2;
	rectangle.w = (x_size) - _size * 4;
	rectangle.h = (y_size) - _size * 4;
	SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 255 );
	SDL_RenderDrawRect(_renderer, &rectangle);

	//draw hud
	SDL_Color White = {255, 0, 0};
	std::string str("Score : " + std::to_string(_score));
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(_font, str.c_str(), White);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(_renderer, surfaceMessage);

	SDL_Rect Message_rect;
	Message_rect.x = _size * 2;
	Message_rect.y = _size / 3;
	Message_rect.w = 80;
	Message_rect.h = _size * 2;
	SDL_RenderCopy(_renderer, Message, NULL, &Message_rect);

	SDL_RenderPresent(_renderer);
	SDL_UpdateWindowSurface(_win);
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
