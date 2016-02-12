/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player_SDL.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdubray <mdubray@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/16 16:41:01 by mdubray           #+#    #+#             */
/*   Updated: 2015/08/11 15:52:05 by mdubray          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_SDL_H
# define PLAYER_SDL_H

# include <iostream>
# include <string>
# include <vector>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>

# include <SDL.h>
# include <SDL_ttf.h>

# include "IEntity.hpp"

class Player : public IEntity
{
	public:
		Player(int x, int y);
		Player(int x, int y, std::vector<int> x_pos, std::vector<int> y_pos, int size, bool alive, int score, int direction, int x_pos_food, int y_pos_food);
		Player(Player const & cpy);
		virtual ~Player();

		Player &	operator=(Player const &);

		void				get_input(int *gui);
		void				move();
		void				grow();
		void				generateFood();
		int					display();

		int					getPosX();
		int					getPosY();
		int					getSize();
		bool				is_alive();
		int					getScore();
		int					getDirection();
		int					getPosXFood();
		int					getPosYFood();
		std::vector<int>	getVectorX();
		std::vector<int>	getVectorY();

		void				setPosX(int x);
		void				setPosY(int y);

	private:
		Player();
		std::vector<int>	_x_pos;
		std::vector<int>	_y_pos;
		int					_size;
		bool				_alive;
		int					_score;
		int					_direction;
		int					_x_pos_food;
		int					_y_pos_food;
		TTF_Font			*_font;
		SDL_Window			*_win;
		SDL_Renderer		*_renderer;
};

extern "C"
{
	Player		*createPlayer(int x, int y);
	Player		*copyPlayer(int x, int y, std::vector<int> x_pos, std::vector<int> y_pos, int size, bool alive, int score, int direction, int x_pos_food, int y_pos_food);
	void		deletePlayer(Player *player);
}

#endif

