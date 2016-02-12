# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mdubray <mdubray@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/07/04 22:13:55 by mdubray           #+#    #+#              #
#    Updated: 2015/08/11 17:57:59 by mdubray          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_SFML = Player_SFML.cpp
OBJ_SFML = $(SRC_SFML:.cpp=.o)

SRC_SDL = Player_SDL.cpp
OBJ_SDL = $(SRC_SDL:.cpp=.o)

SRC_MLX = Player_MLX.cpp
OBJ_MLX = $(SRC_MLX:.cpp=.o)

SRC_GLFW = Player_GLFW.cpp
OBJ_GLFW = $(SRC_GLFW:.cpp=.o)


NAME = snake

LIBSFML = libsfml.so

LIBSDL = libsdl.so

LIBMLX = libmlx.so

LIBGLFW = libglfw.so

.PHONY: all fclean clean re

all: $(NAME)

$(NAME):
	#compilation SFML
	g++ -shared -fPIC -o $(LIBSFML) $(SRC_SFML) -I ~/SFML/include -L ~/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system
	export DYLD_LIBRARY_PATH=~/SFML/lib

	#compilation SDL
	g++ -shared -fPIC -o $(LIBSDL) $(SRC_SDL) ~/Library/Frameworks/SDL2.framework/SDL2 ~/Library/Frameworks/SDL2_ttf.framework/SDL2_ttf -I ~/Library/Frameworks/SDL2.framework/Headers -I ~/Library/Frameworks/SDL2_ttf.framework/Headers -framework GLUT -framework OpenGL

	#compilation GLFW
	g++ -Wno-deprecated-declarations -shared -fPIC -o $(LIBGLFW) $(SRC_GLFW) /System/Library/Frameworks/GLUT.framework/GLUT -I ~/glfw-3.1.1/include -I /System/Library/Frameworks/GLUT.framework/Headers -L ~/glfw-3.1.1/build/src -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

	#compilation mlx
	#make -C ~/minilibx_macos/
	#gcc -shared -fPIC -o $(LIBMLX) $(SRC_MLX) -I ~/minilib_macos/ -I . -framework OpenGl -framework AppKit -lmlx -L .

	#compilation main
	g++ -o $(NAME) main.cpp

clean:
	rm -rf $(OBJ_SFML)
	rm -rf $(OBJ_SDL)
	rm -rf $(OBJ_MLX)
	rm -rf $(OBJ_GLFW)

fclean: clean
	rm -rf $(NAME)
	rm -rf $(LIBSFML)
	rm -rf $(LIBSDL)
	rm -rf $(LIBMLX)
	rm -rf $(LIBGLFW)

re: fclean all

