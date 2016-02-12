/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player_SFML.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdubray <mdubray@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/16 16:41:12 by mdubray           #+#    #+#             */
/*   Updated: 2015/08/11 15:51:43 by mdubray          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_SFML_H
# define PLAYER_SFML_H

# include <iostream>
# include <string>
# include <vector>

# include <SFML/Window.hpp>
# include <SFML/Graphics.hpp>

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
		sf::Font			_font;
		sf::RenderWindow	_win;
};

extern "C"
{
	Player		*createPlayer(int x, int y);
	Player		*copyPlayer(int x, int y, std::vector<int> x_pos, std::vector<int> y_pos, int size, bool alive, int score, int direction, int x_pos_food, int y_pos_food);
	void		deletePlayer(Player *player);
}

#endif

