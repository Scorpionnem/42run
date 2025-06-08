/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:25:07 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/08 02:09:46 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Game.hpp"


Game::Game() : skybox({SKYBOX_PATHES})
{
	shaders.load({"mesh", MESH_VERT_SHADER, MESH_FRAG_SHADER});
	shaders.load({"gui", GUI_VERT_SHADER, GUI_FRAG_SHADER});
	shaders.load({"text", TEXT_VERT_SHADER, TEXT_FRAG_SHADER});
	shaders.load({"skybox", SKYBOX_VERT_SHADER, SKYBOX_FRAG_SHADER});
	shaders.load({"fullbright", MESH_VERT_SHADER, MESH_FULLBRIGHT_SHADER});
	shaders.get("text")->use();
	shaders.get("text")->setInt("tex0", 0);
    shaders.get("mesh")->use();
	shaders.get("mesh")->setInt("tex0", 0);
	shaders.get("skybox")->use();
	shaders.get("skybox")->setInt("skybox", 0);

	meshManager.load("models/scorpionem/scorpionem.obj");
	meshManager.load("models/school_entrance.obj");
	meshManager.load("models/clustermiddle.obj", "models/bricks.bmp");
	meshManager.load("models/clusterexit.obj", "models/bricks.bmp");
	meshManager.load("models/corridor.obj", "models/bricks.bmp");
	
	loadUIs();
}

Game::~Game()
{
}

void	quitGame()
{
	g_Game->started = false;
	g_Game->diedTime = 0;
	g_Game->died = false;
	g_Game->world.speed = 10;
	if (g_Game->distance > g_Game->bestDistance)
		g_Game->bestDistance = g_Game->distance;
	if (g_Game->collectibles > g_Game->bestCollecCount)
		g_Game->bestCollecCount = g_Game->collectibles;
	g_Game->totalDistance += g_Game->distance;
	g_Game->distance = 0;
	g_Game->collectibles = 0;
	g_Game->pause();
}

void Game::logic()
{
	camera.pos.x = 0;
	camera.pos.y = 3.5;
	camera.pos.z = 3.5;
	camera.pitch = -15;
	if (died && glfwGetTime() - diedTime > 1)
		quitGame();
	if (hasPowerup && glfwGetTime() - powerupTime > 2)
		hasPowerup = false;
	if (paused || died)
		return ;
	distance += world.speed * window.getDeltaTime();
	player.update(window.getDeltaTime());
	if (!hasPowerup && player.isDead(world))
	{
		died = true;
		diedTime = glfwGetTime();
	}
	if (player.doesCollect(world))
	{
		totalCollectibles++;
		collectibles++;
	}
	if (player.doesGetPowerup(world))
	{
		hasPowerup = true;
		powerupTime = glfwGetTime();
	}
	world.update(window.getDeltaTime());
}

void	Game::keyHook()
{
	if (paused || died)
		return ;

	if (!player.sneaking && player.onGround && glfwGetKey(window.getWindowData(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		player.timeFallStart = glfwGetTime();
		player.velocity.y = 13;
	}
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		player.sneaking = true;
	else
		player.sneaking = false;
		
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_A) == GLFW_PRESS)
		player.pos.x -= 6 * window.getDeltaTime();
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_D) == GLFW_PRESS)
		player.pos.x += 6 * window.getDeltaTime();

	if (player.pos.x > 3)
		player.pos.x = 3;
	if (player.pos.x < -3)
		player.pos.x = -3;
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
	// Shader	*guiShader = shaders.get("gui");

    glDisable(GL_DEPTH_TEST);

	if (died)
		font.putString("you died", *textShader, vec2(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 50), vec2(300, 100));

	if (paused && started)
	{
        //Centers the pause interface
		Interface	*pause = ui.get("pause");
		pause->buttons[0].pos = vec2((SCREEN_WIDTH / 2) - 100, 50);
		pause->buttons[1].pos = vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2));
		pause->buttons[2].pos = vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) + 80);
		pause->buttons[3].pos = vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) + 160);
		// ui.draw("pause", window.getWindowData(), *guiShader, font, *textShader);
	}
	if (!started)
	{
		Interface	*start = ui.get("start");
		std::string	bestdistancemeter = "best distance: " + toString((int)bestDistance);
		std::string	bestcollectiblesmeter = "best collectibles: " + toString((int)bestCollecCount);
		std::string	distancequest = "travel 1000 meters: " + toString((int)((totalDistance / 1000.f) * 100.f)) + "%";
		std::string	collecquest = "collect 125 collectibles: " + toString((int)((totalCollectibles / 125.f) * 100.f)) + "%";
		font.putString(bestdistancemeter, *textShader, vec2((SCREEN_WIDTH / 2) - (bestdistancemeter.size() * (TERMINAL_CHAR_SIZE * 2) / 2), (SCREEN_HEIGHT / 2) - 100), vec2(bestdistancemeter.size() * (TERMINAL_CHAR_SIZE * 2), (TERMINAL_CHAR_SIZE * 2)));
		font.putString(bestcollectiblesmeter, *textShader, vec2((SCREEN_WIDTH / 2) - (bestcollectiblesmeter.size() * (TERMINAL_CHAR_SIZE * 2) / 2), (SCREEN_HEIGHT / 2) - 140), vec2(bestcollectiblesmeter.size() * (TERMINAL_CHAR_SIZE * 2), (TERMINAL_CHAR_SIZE * 2)));
		font.putString(std::string("quests: "), *textShader, vec2((SCREEN_WIDTH / 2) - (std::string("quests: ").size() * (TERMINAL_CHAR_SIZE * 2) / 2), (SCREEN_HEIGHT) - 140), vec2(std::string("quests: ").size() * (TERMINAL_CHAR_SIZE * 2), (TERMINAL_CHAR_SIZE * 2)));
		font.putString(distancequest, *textShader, vec2((SCREEN_WIDTH / 2) - (distancequest.size() * (TERMINAL_CHAR_SIZE * 2) / 2), (SCREEN_HEIGHT) - 100), vec2(distancequest.size() * (TERMINAL_CHAR_SIZE * 2), (TERMINAL_CHAR_SIZE * 2)));
		font.putString(collecquest, *textShader, vec2((SCREEN_WIDTH / 2) - (collecquest.size() * (TERMINAL_CHAR_SIZE * 2) / 2), (SCREEN_HEIGHT) - 60), vec2(collecquest.size() * (TERMINAL_CHAR_SIZE * 2), (TERMINAL_CHAR_SIZE * 2)));	
		start->buttons[0].pos = vec2((SCREEN_WIDTH / 2) - 100, 50);
		start->buttons[1].pos = vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2));
		start->buttons[2].pos = vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) + 80);
		// ui.draw("start", window.getWindowData(), *guiShader, font, *textShader);
		camera.yaw += 4 * window.getDeltaTime();
		return ;
	}

	std::string	distancemeter = "distance: " + toString((int)distance);
	std::string	collectiblesmeter = "collectibles: " + toString((int)collectibles);
	font.putString(distancemeter, *textShader, vec2(0, 0), vec2(distancemeter.size() * TERMINAL_CHAR_SIZE * 2, TERMINAL_CHAR_SIZE * 2));
	font.putString(collectiblesmeter, *textShader, vec2(0, 50), vec2(collectiblesmeter.size() * TERMINAL_CHAR_SIZE * 2, TERMINAL_CHAR_SIZE * 2));

	displayFPS();
    glEnable(GL_DEPTH_TEST);
}

void	Game::setShaderValues()
{
	Shader	*textShader = shaders.get("text");
	Shader	*meshShader = shaders.get("mesh");
	Shader	*fullBrightShader = shaders.get("fullbright");

	textShader->use();
	textShader->setFloat("time", glfwGetTime());
	textShader->setFloat("SCREEN_WIDTH", SCREEN_WIDTH);
	textShader->setFloat("SCREEN_HEIGHT", SCREEN_HEIGHT);
	textShader->setVec3("color", vec3(0.1, 1.0, 0.2));
	textShader->setBool("rainbow", hasPowerup);
	meshShader->use();
	meshShader->setVec3("viewPos", camera.pos);
	meshShader->setFloat("time", glfwGetTime());
	meshShader->setFloat("ambientStrength", 0.5);
	camera.setViewMatrix(*meshShader);
	fullBrightShader->use();
	camera.setViewMatrix(*fullBrightShader);
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
	g_Game->camera.yaw = -90;
	resumeGame();
}

void	Game::loadUIs()
{
	//Loads pause interface
	ui.createInterface("pause");
	ui.addButtonToInterface("pause", Button("", 200, 200, vec2((SCREEN_WIDTH / 2) - 100, 50), do_nothing, ui.iconTexture, ui.iconTexture));
	ui.addButtonToInterface("pause", Button("resume", 250, 75, vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) + 80), resumeGame, ui.buttonTexture, ui.buttonPressedTexture));
	ui.addButtonToInterface("pause", Button("stop", 250, 75, vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) + 240), quitGame, ui.buttonTexture, ui.buttonPressedTexture));

	ui.createInterface("start");
	ui.addButtonToInterface("start", Button("", 200, 200, vec2((SCREEN_WIDTH / 2) - 100, 50), do_nothing, ui.iconTexture, ui.iconTexture));
	ui.addButtonToInterface("start", Button("start", 250, 75, vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) - 80), resetGame, ui.buttonTexture, ui.buttonPressedTexture));
	ui.addButtonToInterface("start", Button("quit", 250, 75, vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) + 80), closeWindow, ui.buttonTexture, ui.buttonPressedTexture));
}
