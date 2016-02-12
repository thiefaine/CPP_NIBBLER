/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player_MLX.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdubray <mdubray@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/10 14:25:34 by mdubray           #+#    #+#             */
/*   Updated: 2015/08/10 18:33:27 by mdubray          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_MLX_H
# define PLAYER_MLX_H

# include <mlx.h>
# include <iostream>
# include <string>
# include <vector>

# include "IEntity.hpp"

class Player : public IEntity
{
	public:
		Player(int x, int y);
		Player(int x, int y, std::vector<int> x_pos, std::vector<int> y_pos, int size, bool alive, int speed, int direction, int x_pos_food, int y_pos_food);
		Player(Player const & cpy);
		virtual ~Player();

		Player &	operator=(Player const &);

		void				get_input(int *gui);
		void				move();
		void				grow();
		void				generateFood();
		void				draw_square(int x, int y, int color);
		int					display();

		int					getPosX();
		int					getPosY();
		int					getSize();
		bool				is_alive();
		int					getSpeed();
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
		int					_speed;
		int					_direction;
		int					_x_pos_food;
		int					_y_pos_food;
		int					_width;
		int					_height;
		void				*_mlx;
		void				*_win;
};

extern "C"
{
	Player		*createPlayer(int x, int y);
	Player		*copyPlayer(int x, int y, std::vector<int> x_pos, std::vector<int> y_pos, int size, bool alive, int speed, int direction, int x_pos_food, int y_pos_food);
	void		deletePlayer(Player *player);
}

#endif
