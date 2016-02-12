/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdubray <mdubray@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/11 18:05:47 by mdubray           #+#    #+#             */
/*   Updated: 2015/08/11 18:06:53 by mdubray          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include <iostream>
# include <string>

class Game {
	public:
		Game();
		Game(Game const & cpy);
		~Game();

		Game &	operator=(Game const &);

	private:
		bool		_pause;


};

#endif

