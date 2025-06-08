/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:25:07 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/08 14:13:13 by mbatty           ###   ########.fr       */
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
	GAME->started = false;
	GAME->diedTime = 0;
	GAME->died = false;
	GAME->world.speed = 10;
	if (GAME->distance > GAME->bestDistance)
		GAME->bestDistance = GAME->distance;
	if (GAME->collectibles > GAME->bestCollecCount)
		GAME->bestCollecCount = GAME->collectibles;
	GAME->totalDistance += GAME->distance;
	GAME->distance = 0;
	GAME->collectibles = 0;
	GAME->pause();
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
	textShader->setVec3("color", vec3(1, 1, 1));
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
	GAME->world.resetWorld();
	GAME->player.pos = vec3(0);
	GAME->player.velocity = vec3(0);
	GAME->started = true;
	GAME->camera.yaw = -90;
	resumeGame();
}

void	Game::loadUIs()
{
}
