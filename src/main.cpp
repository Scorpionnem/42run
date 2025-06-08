/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:33:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/08 02:07:07 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

#include "Game.hpp"

float	FOV = 65;
float	SCREEN_WIDTH = 1000;
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

MeshManager	*g_meshManager;
ShaderManager	*g_shaderManager;

#include "AButton.hpp"

void	test(void *ptr)
{
	std::cout << ptr << std::endl;
}

void	resetGame(void *ptr)
{
	(void)ptr;
	g_Game->world.resetWorld();
	g_Game->player.pos = vec3(0);
	g_Game->player.velocity = vec3(0);
	g_Game->started = true;
	g_Game->camera.yaw = -90;
	resumeGame();
}

void	sliderTest(float val, void *ptr)
{
	(void)ptr;
	std::cout << val << std::endl;
}

int	main(void)
{
	try {
		
		Game	game;
		g_Game = &game;
		g_meshManager = &game.meshManager;
		g_shaderManager = &game.shaders;

		std::vector<AButton *>	buttons;
		buttons.push_back(new xButton(UIAnchor::TOP_CENTER, "", vec2(0, 75), vec2(200, 200), test, NULL));

		buttons.push_back(new xButton(UIAnchor::CENTER_HALF_RIGHT, "cosmetics", vec2(85, 110), vec2(200, 100), test, &game));
		buttons.push_back(new xButton(UIAnchor::CENTER_HALF_LEFT, "powerups", vec2(-85, 110), vec2(200, 100), test, &game));

		buttons.push_back(new xButton(UIAnchor::CENTER, "start", vec2(0, 0), vec2(350, 100), resetGame, NULL));
		buttons.push_back(new xButton(UIAnchor::CENTER, "quests", vec2(0, 110), vec2(350, 100), test, &game));
		buttons.push_back(new xButton(UIAnchor::CENTER, "options", vec2(-87.5, 220), vec2(170, 100), test, &game));
		buttons.push_back(new xButton(UIAnchor::CENTER, "quit game", vec2(87.5, 220), vec2(170, 100), test, &game));

		for (auto *button : buttons)
		{
			button->setAssets(new Texture(BUTTON_PATH), new Texture(BUTTON_PRESSED_PATH), g_shaderManager->get("gui"), g_shaderManager->get("text"), &game.font);
			if (xSlider* slider = dynamic_cast<xSlider*>(button))
				slider->setAssets(NULL, NULL, new Texture(SLIDER_BG_PATH), 0, NULL, NULL, NULL);
		}

		buttons[0]->setAssets(new Texture(ICON_PATH), new Texture(BUTTON_PRESSED_PATH), g_shaderManager->get("gui"), g_shaderManager->get("text"), &game.font);

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
			// game.drawUI();

			double mouseX, mouseY;
			bool mousePressed = glfwGetMouseButton(game.window.getWindowData(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
			glfwGetCursorPos(game.window.getWindowData(), &mouseX, &mouseY);

			glDisable(GL_DEPTH_TEST);
			for (auto *button : buttons)
			{
				button->update(vec2(mouseX, mouseY), mousePressed);
				button->draw();
			}
			glEnable(GL_DEPTH_TEST);

			//Swaps buffers and get key actions
			game.keyHook();
			game.window.loopEnd();
		}
	} catch (const std::exception &e) {
		std::cerr << "An error occurred: " << e.what() << std::endl;
		return (1);
	}
}
