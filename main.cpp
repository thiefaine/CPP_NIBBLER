/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdubray <mdubray@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/02 13:59:12 by mdubray           #+#    #+#             */
/*   Updated: 2015/08/12 16:58:35 by mdubray          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

#include <time.h>
#include <cstdlib>
#include <unistd.h>
#include <dlfcn.h>
#include <vector>

#include "IEntity.hpp"

#define MS_PER_UPDATE 1

void	dlerror_wrapper(void)
{
	std::cerr << "Error : " << dlerror() << std::endl;
	exit(EXIT_FAILURE);
}

void	*switch_handle(int gui)
{
	void	*dl_handle;

	if (gui == 0)
	{
		if (!(dl_handle = dlopen("libsfml.so", RTLD_LAZY | RTLD_LOCAL)))
			dlerror_wrapper();
	}
	else if (gui == 1)
	{
		if (!(dl_handle = dlopen("libsdl.so", RTLD_LAZY | RTLD_LOCAL)))
			dlerror_wrapper();
	}
	else if (gui == 2)
	{
		if (!(dl_handle = dlopen("libglfw.so", RTLD_LAZY | RTLD_LOCAL)))
			dlerror_wrapper();
	}
	return (dl_handle);
}

void	init_gui(int gui, int x, int y)
{
	void		*dl_handle;
	IEntity		*(*PlayerCreator)(int x,int y);
	IEntity		*(*PlayerCopy)(int x, int y, std::vector<int> x_pos, std::vector<int> y_pos, int size, bool alive, int speed, int direction, int x_pos_food, int y_pos_food);
	void		(*PlayerDestructor)(IEntity *);

	//Detection librairie a utiliser
	dl_handle = switch_handle(gui);

	//Construction du player
	PlayerCreator = (IEntity *(*)(int x, int y)) dlsym(dl_handle, "createPlayer");
	if (!PlayerCreator)
		dlerror_wrapper();

	PlayerDestructor = (void(*)(IEntity*)) dlsym(dl_handle, "deletePlayer");
	if (!PlayerDestructor)
		dlerror_wrapper();

	IEntity		*p;
	p = PlayerCreator(x, y);

	//Boucle de jeu
	int		prev_gui = gui;

	p->display();

	while (p->is_alive())
	{
		usleep(100000 - p->getScore() * 200);

		//get the inputs
		p->get_input(&gui);

		//If you switch the shared library
		if (gui != prev_gui)
		{
			//get all datas from game
			std::vector<int> x_pos = p->getVectorX();
			std::vector<int> y_pos = p->getVectorY();
			int				size = p->getSize();
			bool			alive = p->is_alive();
			int				score = p->getScore();
			int				direction = p->getDirection();
			int				x_pos_food = p->getPosXFood();
			int				y_pos_food = p->getPosYFood();

			//change library
			PlayerDestructor(p);
			dlclose(dl_handle);

			dl_handle = switch_handle(gui);

			PlayerCopy = (IEntity *(*)(int x, int y, std::vector<int> x_pos, std::vector<int> y_pos, int size, bool alive, int score, int direction, int x_pos_food, int y_pos_food)) dlsym(dl_handle, "copyPlayer");
			if (!PlayerCopy)
				dlerror_wrapper();

			PlayerDestructor = (void(*)(IEntity*)) dlsym(dl_handle, "deletePlayer");
			if (!PlayerDestructor)
				dlerror_wrapper();

			p = PlayerCopy(x, y, x_pos, y_pos, size, alive, score, direction, x_pos_food, y_pos_food);
			prev_gui = gui;
		}

		//update the positions
		p->move();

		//display the elements
		p->display();
	}

	//Destruction du player
	if (p)
		PlayerDestructor(p);
	dlclose(dl_handle);
}

int		main(int ac, char *av[])
{
	int		gui(0);
	int		width(0);
	int		height(0);

	// Error handling
	if (ac < 3 || ac >= 5)
	{
	   	std::cout << "\033[31mWrong number of argument\033[0m" << std::endl << "> ./snake WIDTH HEIGHT [library.so]" << std::endl;
	   	return 0;
	}
	if (ac == 3)
		gui = 0;
	else if (ac == 4 && strcmp(av[3], "libsfml.so") == 0)
		gui = 0;
	else if (ac == 4 && strcmp(av[3], "libsdl.so") == 0)
		gui = 1;
	else if (ac == 4 && strcmp(av[3], "libglfw.so") == 0)
		gui = 2;

	width = atoi(av[1]);
	height = atoi(av[2]);

	if (width < 300 || height < 300)
	{
		std::cout << "Game area is too small. It must be greater or equal than 300" << std::endl;
		return 0;
	}
	else if (width > 1000 || height > 1000)
	{
		std::cout << "Game area is toot big. It must be smaller than 1000" << std::endl;
		return 0;
	}

	srand(time(0));
	init_gui(gui, width, height);

	return(0);
}
