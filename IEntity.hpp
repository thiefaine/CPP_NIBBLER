/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEntity.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdubray <mdubray@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/02 14:29:03 by mdubray           #+#    #+#             */
/*   Updated: 2015/08/11 15:54:18 by mdubray          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IENTITY_H
# define IENTITY_H

# include <iostream>
# include <string>

class IEntity {
	public:
		virtual void				get_input(int *gui) = 0;
		virtual void				move() = 0;
		virtual void				grow() = 0;
		virtual void				generateFood() = 0;
		virtual int					display() = 0;

		virtual int					getPosX() = 0;
		virtual int					getPosY() = 0;
		virtual int					getSize() = 0;
		virtual bool				is_alive() = 0;
		virtual int					getScore() = 0;
		virtual int					getDirection() = 0;
		virtual int					getPosXFood() = 0;
		virtual int					getPosYFood() = 0;
		virtual std::vector<int>	getVectorX() = 0;
		virtual std::vector<int>	getVectorY() = 0;

		virtual void				setPosX(int x) = 0;
		virtual void				setPosY(int y) = 0;
};

#endif

