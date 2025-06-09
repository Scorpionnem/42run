/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:33:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/09 11:42:35 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

#include "Game.hpp"
#include "InterfaceManager.hpp"
#include "CharacterManager.hpp"
#include "UIElement.hpp"

float	FOV = 65;
float	SCREEN_WIDTH = 1100;
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

Font				*FONT;
Game				*GAME;
MeshManager			*MESH_MANAGER;
ShaderManager		*SHADER_MANAGER;
TextureManager		*TEXTURE_MANAGER;
CharacterManager	*CHARACTER_MANAGER;
InterfaceManager	*INTERFACES_MANAGER;

void	resumeGame()
{
	GAME->resume();
}

void	closeWindow()
{
	glfwSetWindowShouldClose(GAME->window.getWindowData(), true);
}

void	key_hook(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)window;(void)key;(void)scancode;(void)action;(void)mods;

	if (GAME->started && key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (GAME->paused)
			GAME->resume();
		else
			GAME->pause();
	}
	if (GAME->paused)
		return ;
}

void	resetGame(void *ptr)
{
	(void)ptr;
	GAME->world.resetWorld();
	GAME->player.pos = vec3(0);
	GAME->player.velocity = vec3(0);
	GAME->started = true;
	GAME->camera.yaw = -90;
	INTERFACES_MANAGER->current = NULL;
	resumeGame();
}

void	buildShaders()
{
	SHADER_MANAGER->load({"mesh", MESH_VERT_SHADER, MESH_FRAG_SHADER});
	SHADER_MANAGER->load({"gui", GUI_VERT_SHADER, GUI_FRAG_SHADER});
	SHADER_MANAGER->load({"text", TEXT_VERT_SHADER, TEXT_FRAG_SHADER});
	SHADER_MANAGER->load({"skybox", SKYBOX_VERT_SHADER, SKYBOX_FRAG_SHADER});
	SHADER_MANAGER->load({"fullbright", MESH_VERT_SHADER, MESH_FULLBRIGHT_SHADER});
	SHADER_MANAGER->get("text")->use();
	SHADER_MANAGER->get("text")->setInt("tex0", 0);
    SHADER_MANAGER->get("mesh")->use();
	SHADER_MANAGER->get("mesh")->setInt("tex0", 0);
	SHADER_MANAGER->get("skybox")->use();
	SHADER_MANAGER->get("skybox")->setInt("skybox", 0);
}

int	main(void)
{
	try {
		
		Game	game;
		GAME = &game;

		Font	font;
		FONT = &font;

		MeshManager	meshes;
		MESH_MANAGER = &meshes;
		
		ShaderManager	shaders;
		SHADER_MANAGER = &shaders;
		buildShaders();
		
		TextureManager	textures;
		TEXTURE_MANAGER = &textures;

		InterfaceManager	interfaces;
		INTERFACES_MANAGER = &interfaces;
		build(INTERFACES_MANAGER);

		CharacterManager	characters;
		CHARACTER_MANAGER = &characters;

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
			
			if (!GAME->started)
				GAME->camera.pitch = 0;
			else
				GAME->camera.pitch = -15;

			//Swaps buffers and get key actions
			game.keyHook();
			game.window.loopEnd();
		}
	} catch (const std::exception &e) {
		std::cerr << "An error occurred: " << e.what() << std::endl;
		return (1);
	}
}
