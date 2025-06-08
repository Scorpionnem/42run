/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:25:07 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/09 01:47:22 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Game.hpp"
#include "InterfaceManager.hpp"

Game::Game() : skybox({SKYBOX_PATHES})
{
	camera.pos.x = 0;
	camera.pos.y = 3.5;
	camera.pos.z = 3.5;
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
	INTERFACES_MANAGER->current = INTERFACES_MANAGER->get(MAIN_INTERFACE);
}

void Game::logic()
{
	INTERFACES_MANAGER->update();
	
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
	static bool prevSpacePressed = false;
	bool spacePressed = glfwGetKey(window.getWindowData(), GLFW_KEY_SPACE) == GLFW_PRESS;

	if (paused || died)
		return ;

	bool spaceJustPressed = spacePressed && !prevSpacePressed;

	std::cout << player.doubleJump << std::endl;
	if (!player.sneaking && player.onGround && spacePressed)
	{
		player.timeFallStart = glfwGetTime();
		player.velocity.y = 13;
	}
	else if (!player.sneaking && !player.onGround && player.doubleJump && spaceJustPressed)
	{
		player.timeFallStart = glfwGetTime();
		player.velocity.y = 11;
		player.doubleJump = false;
	}

	prevSpacePressed = spacePressed;
	
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
	Shader	*skyboxShader = SHADER_MANAGER->get("skybox");
	Shader	*meshShader = SHADER_MANAGER->get("mesh");

	skybox.draw(camera, *skyboxShader);
	
	if (!started)
		return ;
    player.draw(*meshShader);
	world.draw(*meshShader);
}

void	Game::drawUI()
{
    glDisable(GL_DEPTH_TEST);
	displayFPS();
	INTERFACES_MANAGER->draw();
    glEnable(GL_DEPTH_TEST);
}

void	Game::setShaderValues()
{
	Shader	*textShader = SHADER_MANAGER->get("text");
	Shader	*meshShader = SHADER_MANAGER->get("mesh");
	Shader	*fullBrightShader = SHADER_MANAGER->get("fullbright");

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
		fps = window.displayFPS(font, *SHADER_MANAGER->get("text"));
	font.putString(fps.c_str(), *SHADER_MANAGER->get("text"),
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

void    Game::resume()
{
	INTERFACES_MANAGER->current = INTERFACES_MANAGER->get(GAME_INTERFACE);
	paused = false;
}

void	Game::pause()
{
	INTERFACES_MANAGER->current = INTERFACES_MANAGER->get(PAUSE_INTERFACE);
	paused = true;
}
