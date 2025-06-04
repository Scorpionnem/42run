/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:33:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/04 14:20:04 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

#include "Game.hpp"

float	FOV = 70;
float	SCREEN_WIDTH = 820;
float	SCREEN_HEIGHT = 900;
float	RENDER_DISTANCE = 1000;

bool	F1 = false;
bool	F3 = false;
bool	lock_fps = true;

unsigned int	TOTAL_VERTICES = 0;
unsigned int	TOTAL_TRIANGLES = 0;

void	interpolateTo(float &float1, float &float2, float deltaTime)
{
	float1 += (float2 - float1) * 4.0f * deltaTime;

	float2 = 1.0f - float1;
}

std::string	toString(int nbr)
{
	std::stringstream	strs;
	strs << nbr;
	return (strs.str());
}

std::string	toString(float nbr)
{
	std::stringstream	strs;
	strs << nbr;
	return (strs.str());
}

Game	*g_Game;

void	resumeGame()
{
	g_Game->resume();
}

void	closeWindow()
{
	glfwSetWindowShouldClose(g_Game->window.getWindowData(), true);
}

void	key_hook(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)window;(void)key;(void)scancode;(void)action;(void)mods;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (g_Game->paused)
			g_Game->resume();
		else
			g_Game->pause();
	}
	if (g_Game->paused)
		return ;
}

int	main(void)
{
	try {
		Game	game;
		g_Game = &game;

		while (game.window.up())
		{
			//Clear and get current frame + updates shaders accordingly
			game.window.loopStart();
			game.camera.update();
			game.setShaderValues();

			game.logic();

			//3D Objects drawing
			game.draw3D();

			//2D Objects drawing
			game.drawUI();

			//Swaps buffers and get key actions
			game.keyHook();
			game.window.loopEnd();
		}
	} catch (const std::exception &e) {
		std::cerr << "An error occurred: " << e.what() << std::endl;
		return (1);
	}
}
