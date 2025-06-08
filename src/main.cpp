/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:33:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/08 16:13:35 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

#include "Game.hpp"

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

Game	*GAME;

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

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
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

MeshManager	*MESH_MANAGER;
ShaderManager	*SHADER_MANAGER;
TextureManager	*TEXTURE_MANAGER;

#include "UIElement.hpp"

void	test(void *ptr)
{
	std::cout << ptr << std::endl;
}

void	resetGame(void *ptr)
{
	(void)ptr;
	GAME->world.resetWorld();
	GAME->player.pos = vec3(0);
	GAME->player.velocity = vec3(0);
	GAME->started = true;
	GAME->camera.yaw = -90;
	resumeGame();
}

void	sliderTest(float val, void *ptr)
{
	(void)ptr;
	std::cout << val << std::endl;
}

class	Interface
{
	public:
		Interface(){}
		Interface(std::function<void(void*)> onUpdate, void *updateData)
		{
			this->onUpdate = onUpdate;
			this->updateData = updateData;
		}
		~Interface()
		{
			for (auto *element : elements)
				delete element;
		}

		void	addButton(UIAnchor anchor, std::string label, vec2 offset, vec2 size, std::function<void(void*)> onClick, void *clickData)
		{
			elements.push_back(new Button(anchor, label, offset, size, onClick, clickData));
		}
		void	addButton(std::string label, vec2 pos, vec2 size, std::function<void(void*)> onClick, void *clickData)
		{
			elements.push_back(new Button(label, pos, size, onClick, clickData));
		}
		void	addSlider(UIAnchor anchor, std::string label, vec2 offset, vec2 size, std::function<void(float, void*)> onClick, void *clickData)
		{
			elements.push_back(new Slider(anchor, label, offset, size, onClick, clickData));
		}
		void	addSlider(std::string label, vec2 pos, vec2 size, std::function<void(float, void*)> onClick, void *clickData)
		{
			elements.push_back(new Slider(label, pos, size, onClick, clickData));
		}

		void	draw()
		{
			for (auto *element : elements)
				element->draw();
		}
		void	update()
		{
			if (onUpdate)
				onUpdate(updateData);
			double mouseX, mouseY;
			bool mousePressed = glfwGetMouseButton(GAME->window.getWindowData(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
			glfwGetCursorPos(GAME->window.getWindowData(), &mouseX, &mouseY);
			for (auto *element : elements)
				element->update(vec2(mouseX, mouseY), mousePressed);
		}
		std::vector<UIElement *>	elements;
		std::function<void(void*)>	onUpdate = NULL;
		void						*updateData = NULL;
};

Interface	*currentInterface;

void	gotoInterface(void *data)
{
	currentInterface = static_cast<Interface *>(data);
}

std::vector<Mesh *>				skins;
std::vector<Mesh *>::iterator	currentSkin;

void	cosmeticInterfaceUpdate(void *data)
{
	vec3	direction;
	direction.x = cos(toRadians(GAME->camera.yaw)) * cos(toRadians(GAME->camera.pitch));
	direction.y = sin(toRadians(GAME->camera.pitch));
	direction.z = sin(toRadians(GAME->camera.yaw)) * cos(toRadians(GAME->camera.pitch));
	vec3 front = direction.normalize();
	front.y += 0.1;

	(void)data;
	glEnable(GL_DEPTH_TEST);
	GAME->player.load();
	GAME->player.mesh->pos = GAME->camera.pos + front * 4;
	GAME->player.mesh->draw(*SHADER_MANAGER->get("mesh"));
	glDisable(GL_DEPTH_TEST);
}

void	cosmeticInterfaceNext(void *data)
{
	(void)data;
}

void	cosmeticInterfacePrevious(void *data)
{
	(void)data;
}

int	main(void)
{
	try {
		
		Game	game;
		GAME = &game;
		MESH_MANAGER = &game.meshManager;
		SHADER_MANAGER = &game.shaders;
		TEXTURE_MANAGER = &game.textures;

		Interface	*mainInterface = new Interface();
		Interface	*optionsInterface = new Interface();
		Interface	*questsInterface = new Interface();
		Interface	*cosmeticsInterface = new Interface(cosmeticInterfaceUpdate, NULL);

		Interface	*deadInterface = new Interface();
		deadInterface->elements.push_back(new Image(UIAnchor::TOP_LEFT, TEXTURE_MANAGER->get("textures/blackhole.bmp"), vec2(0), vec2(SCREEN_WIDTH, SCREEN_HEIGHT)));

		skins.push_back(MESH_MANAGER->get("models/scorpionem/scorpionem.obj"));
		skins.push_back(MESH_MANAGER->get("models/mix/mix.obj"));
		currentSkin = skins.begin();

		mainInterface->elements.push_back(new Image(UIAnchor::TOP_CENTER, TEXTURE_MANAGER->get(ICON_PATH), vec2(0, 75), vec2(200, 200)));

		mainInterface->addButton(UIAnchor::CENTER_HALF_RIGHT, "characters", vec2(85, 110), vec2(200, 100), gotoInterface, cosmeticsInterface);
		mainInterface->addButton(UIAnchor::CENTER_HALF_LEFT, "powerups", vec2(-85, 110), vec2(200, 100), NULL, NULL);

		mainInterface->addButton(UIAnchor::CENTER, "start", vec2(0, 0), vec2(350, 100), resetGame, NULL);
		mainInterface->addButton(UIAnchor::CENTER, "quests", vec2(0, 110), vec2(350, 100), gotoInterface, questsInterface);
		mainInterface->addButton(UIAnchor::CENTER, "options", vec2(-87.5, 220), vec2(170, 100), gotoInterface, optionsInterface);
		mainInterface->addButton(UIAnchor::CENTER, "quit game", vec2(87.5, 220), vec2(170, 100), [](void *data){(void)data;glfwSetWindowShouldClose(GAME->window.getWindowData(), true);}, NULL);

		mainInterface->elements.push_back(new Text(UIAnchor::CENTER, "best score: 69420", vec2(0, -120), vec2(std::string("best score: 69420").size() * 25, 25)));

		optionsInterface->elements.push_back(new Text(UIAnchor::CENTER, "options", vec2(0, -225), vec2(200, 50)));

		optionsInterface->addSlider(UIAnchor::CENTER, "fov", vec2(-205, -105), vec2(400, 100), sliderTest, NULL);
		optionsInterface->addButton(UIAnchor::CENTER, "video settings", vec2(-205, 0), vec2(400, 100), NULL, NULL);
		optionsInterface->addButton(UIAnchor::CENTER, "resource pack", vec2(-205, 105), vec2(400, 100), NULL, NULL);

		optionsInterface->addButton(UIAnchor::CENTER, "controls", vec2(205, -105), vec2(400, 100), NULL, NULL);
		optionsInterface->addButton(UIAnchor::CENTER, "wip", vec2(205, 0), vec2(400, 100), NULL, NULL);
		optionsInterface->addButton(UIAnchor::CENTER, "game options", vec2(205, 105), vec2(400, 100), NULL, NULL);

		optionsInterface->addButton(UIAnchor::BOTTOM_CENTER, "done", vec2(0, -75), vec2(350, 100), gotoInterface, mainInterface);

		questsInterface->elements.push_back(new Text(UIAnchor::CENTER, "quests", vec2(0, -225), vec2(200, 50)));

		questsInterface->addButton(UIAnchor::CENTER, "quest1", vec2(-205, -105), vec2(400, 100), NULL, NULL);
		questsInterface->addButton(UIAnchor::CENTER, "quest2", vec2(-205, 0), vec2(400, 100), NULL, NULL);
		questsInterface->addButton(UIAnchor::CENTER, "quest3", vec2(-205, 105), vec2(400, 100), NULL, NULL);

		questsInterface->addButton(UIAnchor::CENTER, "quest4", vec2(205, -105), vec2(400, 100), NULL, NULL);
		questsInterface->addButton(UIAnchor::CENTER, "quest5", vec2(205, 0), vec2(400, 100), NULL, NULL);
		questsInterface->addButton(UIAnchor::CENTER, "quest6", vec2(205, 105), vec2(400, 100), NULL, NULL);
		questsInterface->addButton(UIAnchor::BOTTOM_CENTER, "done", vec2(0, -75), vec2(350, 100), gotoInterface, mainInterface);

		cosmeticsInterface->addButton(UIAnchor::BOTTOM_CENTER, "done", vec2(0, -75), vec2(350, 100), gotoInterface, mainInterface);
		cosmeticsInterface->addButton(UIAnchor::BOTTOM_CENTER, "buy", vec2(0, -180), vec2(200, 100), NULL, NULL);
		cosmeticsInterface->addButton(UIAnchor::CENTER_HALF_LEFT, "previous", vec2(0, 0), vec2(200, 100), cosmeticInterfacePrevious, NULL);
		cosmeticsInterface->addButton(UIAnchor::CENTER_HALF_RIGHT, "next", vec2(0, 0), vec2(200, 100), cosmeticInterfaceNext, NULL);

		currentInterface = mainInterface;

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

			glDisable(GL_DEPTH_TEST);
			if (!GAME->started && currentInterface)
			{
				currentInterface->update();
				currentInterface->draw();
			}
			if (GAME->died)
				deadInterface->draw();
			glEnable(GL_DEPTH_TEST);
			
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
