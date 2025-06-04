/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:25:07 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/04 14:55:10 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Game.hpp"

Game::Game() : skybox({SKYBOX_PATHES})
{
	shaders.load({"mesh", MESH_VERT_SHADER, MESH_FRAG_SHADER});
	shaders.load({"gui", GUI_VERT_SHADER, GUI_FRAG_SHADER});
	shaders.load({"text", TEXT_VERT_SHADER, TEXT_FRAG_SHADER});
	shaders.load({"skybox", SKYBOX_VERT_SHADER, SKYBOX_FRAG_SHADER});
	shaders.get("text")->use();
	shaders.get("text")->setInt("tex0", 0);
    shaders.get("mesh")->use();
	shaders.get("mesh")->setInt("tex0", 0);
	shaders.get("skybox")->use();
	shaders.get("skybox")->setInt("skybox", 0);
	
	loadUIs();
}

Game::~Game()
{
}

void Game::logic()
{
	camPos.x = 0;
	camPos.y = 3.5;
	camPos.z = 3.5;
	camPitch = -15;
	if (paused)
		return ;
	player.update(window.getDeltaTime());
	world.update(window.getDeltaTime());
}

void	Game::keyHook()
{
	if (paused)
		return ;

	if (player.onGround && glfwGetKey(window.getWindowData(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		player.timeFallStart = glfwGetTime();
		player.velocity.y = 12;
	}
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		player.sneaking = true;
	else
		player.sneaking = false;
		
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_A) == GLFW_PRESS)
		player.pos.x -= 6 * window.getDeltaTime();
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_D) == GLFW_PRESS)
		player.pos.x += 6 * window.getDeltaTime();

	if (player.pos.x > 2.5)
		player.pos.x = 2.5;
	if (player.pos.x < -2.5)
		player.pos.x = -2.5;
}

void	Game::draw3D()
{
	Shader	*skyboxShader = shaders.get("skybox");
	Shader	*meshShader = shaders.get("mesh");

	skybox.draw(camera, *skyboxShader);

	if (!started)
		return ;
    player.draw(*meshShader);
	world.draw(*meshShader);
}

void	Game::drawUI()
{
	Shader	*textShader = shaders.get("text");
	Shader	*guiShader = shaders.get("gui");

    glDisable(GL_DEPTH_TEST);

	if (paused && started)
	{
        //Centers the pause interface
		Interface	*pause = ui.get("pause");
		pause->buttons[0].pos = vec2((SCREEN_WIDTH / 2) - 100, 50);
		pause->buttons[1].pos = vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) - 80);
		pause->buttons[2].pos = vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2));
		pause->buttons[3].pos = vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) + 80);
		pause->buttons[4].pos = vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) + 160);
		ui.draw("pause", window.getWindowData(), *guiShader, font, *textShader);
	}
	if (!started)
	{
		ui.draw("start", window.getWindowData(), *guiShader, font, *textShader);
		camYaw += 4 * window.getDeltaTime();
		return ;
	}

	if (world.rooms.front()->type == Cluster1)
		font.putString("cluster1", *textShader, vec2(0, 120), vec2(300, 100));
	if (world.rooms.front()->type == Cluster1Entrance)
		font.putString("cluster1entrance", *textShader, vec2(0, 120), vec2(300, 100));
	if (world.rooms.front()->type == Cluster1Exit)
		font.putString("cluster1exit", *textShader, vec2(0, 120), vec2(300, 100));
	if (world.rooms.front()->type == Cluster2)
		font.putString("cluster2", *textShader, vec2(0, 120), vec2(300, 100));
	if (world.rooms.front()->type == Cluster2Entrance)
		font.putString("cluster2entrance", *textShader, vec2(0, 120), vec2(300, 100));
	if (world.rooms.front()->type == Cluster2Exit)
		font.putString("cluster2exit", *textShader, vec2(0, 120), vec2(300, 100));
	if (world.rooms.front()->type == Cluster3)
		font.putString("cluster3", *textShader, vec2(0, 120), vec2(300, 100));
	if (world.rooms.front()->type == Cluster3Entrance)
		font.putString("cluster3entrance", *textShader, vec2(0, 120), vec2(300, 100));
	if (world.rooms.front()->type == Cluster3Exit)
		font.putString("cluster3exit", *textShader, vec2(0, 120), vec2(300, 100));
	if (world.rooms.front()->type == Hardcoded)
		font.putString("hardcoded", *textShader, vec2(0, 120), vec2(300, 100));
	if (world.rooms.front()->type == RoomType::Cafetaria)
		font.putString("cafetaria", *textShader, vec2(0, 120), vec2(300, 100));
	if (world.rooms.front()->type == RoomType::Corridor)
		font.putString("corridor", *textShader, vec2(0, 120), vec2(300, 100));
		
	std::string	speedstr = "speed " + toString((float)world.speed);
	font.putString(speedstr, *textShader, vec2(0, 220), vec2(250, 100));

	std::string	playerPos = "x " + toString(camPos.x) + " y " + toString(camPos.y) + " z " + toString(camPos.z);
	font.putString(playerPos, *textShader, vec2(0, 0), vec2(250, 100));
	displayFPS();
    glEnable(GL_DEPTH_TEST);
}

void	Game::setShaderValues()
{
	Shader	*textShader = shaders.get("text");
	Shader	*meshShader = shaders.get("mesh");

	textShader->use();
	textShader->setFloat("time", glfwGetTime());
	textShader->setFloat("SCREEN_WIDTH", SCREEN_WIDTH);
	textShader->setFloat("SCREEN_HEIGHT", SCREEN_HEIGHT);
	textShader->setVec3("color", vec3(1.0, 1.0, 1.0));
	meshShader->use();
	meshShader->setVec3("viewPos", camPos);
	meshShader->setFloat("time", glfwGetTime());
	meshShader->setFloat("ambientStrength", 0.5);
	camera.setViewMatrix(*meshShader);
}

void	Game::displayFPS()
{
	if ((int)window._lastFrame != (int)window._currentFrame)
		fps = window.displayFPS(font, *shaders.get("text"));
	font.putString(fps.c_str(), *shaders.get("text"),
		vec2(SCREEN_WIDTH - fps.length() * TERMINAL_CHAR_SIZE, TERMINAL_CHAR_SIZE * 0),
		vec2(fps.length() * TERMINAL_CHAR_SIZE, TERMINAL_CHAR_SIZE));
}

void	resetGame()
{
	g_Game->world.resetWorld();
	g_Game->player.pos = vec3(0);
	g_Game->player.velocity = vec3(0);
	g_Game->started = true;
	camYaw = -90;
	resumeGame();
}

void	quitGame()
{
	g_Game->started = false;
	g_Game->pause();
}

void	Game::loadUIs()
{
	//Loads pause interface
	ui.createInterface("pause");
	ui.addButtonToInterface("pause", Button("", 200, 200, vec2((SCREEN_WIDTH / 2) - 100, 50), do_nothing, ui.iconTexture, ui.iconTexture));
	ui.addButtonToInterface("pause", Button("resume", 250, 75, vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) - 80), resumeGame, ui.buttonTexture, ui.buttonPressedTexture));
	ui.addButtonToInterface("pause", Button("settings", 250, 75, vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2)), resumeGame, ui.buttonTexture, ui.buttonPressedTexture));
	ui.addButtonToInterface("pause", Button("stop", 250, 75, vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) + 80), quitGame, ui.buttonTexture, ui.buttonPressedTexture));

	ui.createInterface("start");
	ui.addButtonToInterface("start", Button("", 200, 200, vec2((SCREEN_WIDTH / 2) - 100, 50), do_nothing, ui.iconTexture, ui.iconTexture));
	ui.addButtonToInterface("start", Button("start", 250, 75, vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2)), resetGame, ui.buttonTexture, ui.buttonPressedTexture));
	ui.addButtonToInterface("start", Button("settings", 250, 75, vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) + 80), do_nothing, ui.buttonTexture, ui.buttonPressedTexture));
	ui.addButtonToInterface("start", Button("quit", 250, 75, vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) + 160), closeWindow, ui.buttonTexture, ui.buttonPressedTexture));
}
