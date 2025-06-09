/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InterfaceManager.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 22:00:44 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/09 11:31:43 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACEMANAGER_HPP
# define INTERFACEMANAGER_HPP

# include "Interface.hpp"
# include "CharacterManager.hpp"

enum InterfaceID
{
	MAIN_INTERFACE,
	QUESTS_INTERFACE,
	CHARACTERS_INTERFACE,
	PAUSE_INTERFACE,
	GAME_INTERFACE,
	MAX_INTERFACE //Not used as an interface id, its only to create the array of interfaces in InterfaceManager
};

class	InterfaceManager
{
	public:
		InterfaceManager()
		{
			for (int i = 0; i < MAX_INTERFACE; i++)
				interfaces[i] = new Interface();
			current = interfaces[MAIN_INTERFACE];
		}
		~InterfaceManager()
		{
			for (auto *interface : interfaces)
				delete interface;
		}
		void	update()
		{
			if (current)
				current->update();
		}
		void	draw()
		{
			if (current)
				current->draw();
		}
		Interface *get(const InterfaceID id)
		{
			if (id >= MAX_INTERFACE)
				return (NULL);
			return (interfaces[id]);
		}
		std::array<Interface *, MAX_INTERFACE>	interfaces;
		Interface								*current;
};

extern InterfaceManager	*INTERFACES_MANAGER;

void	resetGame(void *ptr);

inline void	gotoInterface(void *data)
{
	INTERFACES_MANAGER->current = static_cast<Interface *>(data);
}

//Functions under here are to build and use interfaces

extern vec3	CHARACTER_DISPLAY_POS;

inline void	buildMainInterface(InterfaceManager *interfaces)
{
	Interface	*main = interfaces->get(MAIN_INTERFACE);

	main->onUpdate = [](void *data)
	{
		(void)data;
		Interface	*main = INTERFACES_MANAGER->get(MAIN_INTERFACE);
		Text	*distance = static_cast<Text *>(main->elements[0]);
		Text	*collectibles = static_cast<Text *>(main->elements[1]);
		
		distance->label = "best distance " + std::to_string((int)GAME->bestDistance);
		distance->size.x = distance->label.size() * distance->size.y;
		collectibles->label = "best collectibles " + std::to_string((int)GAME->bestCollecCount);
		collectibles->size.x = collectibles->label.size() * collectibles->size.y;
	};
	main->elements.push_back(new Text(UIAnchor::CENTER, "best distance", vec2(0, -115), vec2(25, 25)));
	main->elements.push_back(new Text(UIAnchor::CENTER, "best collectibles", vec2(0, -85), vec2(25, 25)));
	
	main->elements.push_back(new Image(UIAnchor::TOP_CENTER, TEXTURE_MANAGER->get(ICON_PATH), vec2(0, 75), vec2(200, 200)));

	main->addButton(UIAnchor::CENTER_HALF_RIGHT, "characters", vec2(85, 110), vec2(200, 100), gotoInterface, interfaces->get(CHARACTERS_INTERFACE));

	main->addButton(UIAnchor::CENTER, "start", vec2(0, 0), vec2(350, 100), resetGame, NULL);
	main->addButton(UIAnchor::CENTER, "quests", vec2(0, 110), vec2(350, 100), gotoInterface, interfaces->get(QUESTS_INTERFACE));
	main->addButton(UIAnchor::CENTER, "quit game", vec2(0, 220), vec2(350, 100), [](void *data){(void)data;glfwSetWindowShouldClose(GAME->window.getWindowData(), true);}, NULL);
}

inline void	buildQuestsInterface(InterfaceManager *interfaces)
{
	Interface	*quests = interfaces->get(QUESTS_INTERFACE);

	quests->elements.push_back(new Text(UIAnchor::CENTER, "quests", vec2(0, -225), vec2(200, 50)));

	quests->onUpdate = [](void *data)
	{
		(void)data;
		Interface	*game = INTERFACES_MANAGER->get(QUESTS_INTERFACE);
		Text	*quest1 = static_cast<Text *>(game->elements[1]);
		Text	*quest2 = static_cast<Text *>(game->elements[2]);
		Text	*quest3 = static_cast<Text *>(game->elements[3]);
		
		quest1->label = "travel 1000 meters: " + std::to_string((int)((GAME->totalDistance / 1000.0f) * 100)) + "%";
		quest2->label = "collect 125 collectibles: " + std::to_string((int)((GAME->totalCollectibles / 125.f) * 100)) + "%";
		quest3->label = "play 100 times: " + std::to_string(GAME->timesPlayed) + "%";

		quest1->size.x = quest1->label.size() * quest1->size.y;
		quest2->size.x = quest2->label.size() * quest2->size.y;
		quest3->size.x = quest3->label.size() * quest3->size.y;
	};
	quests->elements.push_back(new Text(UIAnchor::CENTER, "quest1", vec2(0, -105), vec2(30, 30)));
	quests->elements.push_back(new Text(UIAnchor::CENTER, "quest2", vec2(0, 0), vec2(30, 30)));
	quests->elements.push_back(new Text(UIAnchor::CENTER, "quest3", vec2(0, 105), vec2(30, 30)));
	quests->addButton(UIAnchor::BOTTOM_CENTER, "done", vec2(0, -75), vec2(350, 100), gotoInterface, interfaces->get(MAIN_INTERFACE));
}

inline void	charactersOnDraw(void *data)
{
	(void)data;

	Mesh	*character = CHARACTER_MANAGER->current->mesh;
	glEnable(GL_DEPTH_TEST);
	character->pos = GAME->camera.pos + CHARACTER_DISPLAY_POS * 4;
	character->draw(*SHADER_MANAGER->get("mesh"));
	glDisable(GL_DEPTH_TEST);
}

inline void	charactersOnUpdate(void *data)
{
	(void)data;
	Button	*buyButton = static_cast<Button *>(INTERFACES_MANAGER->get(CHARACTERS_INTERFACE)->elements[2]);

	vec3	direction;
	direction.x = cos(toRadians(GAME->camera.yaw)) * cos(toRadians(GAME->camera.pitch));
	direction.y = sin(toRadians(GAME->camera.pitch));
	direction.z = sin(toRadians(GAME->camera.yaw)) * cos(toRadians(GAME->camera.pitch));
	CHARACTER_DISPLAY_POS = direction.normalize();
	CHARACTER_DISPLAY_POS.y -= 0.15; //Magic number to moove it down a little

	if (CHARACTER_MANAGER->current->equipped)
		buyButton->label = "equipped";
	else
		buyButton->label = "equip";
}

inline void	equipCharacter(void *data)
{
	(void)data;
	CHARACTER_MANAGER->equipCurrent();
}

inline void	buildCharactersInterface(InterfaceManager *interfaces)
{
	Interface	*characters = interfaces->get(CHARACTERS_INTERFACE);

	characters->onUpdate = charactersOnUpdate;
	characters->onDraw = charactersOnDraw;
	characters->addButton(UIAnchor::CENTER_HALF_LEFT, "previous", vec2(0, 0), vec2(200, 100), [](void *data){(void)data;CHARACTER_MANAGER->usePrevious();}, NULL);
	characters->addButton(UIAnchor::CENTER_HALF_RIGHT, "next", vec2(0, 0), vec2(200, 100), [](void *data){(void)data;CHARACTER_MANAGER->useNext();}, NULL);
	characters->addButton(UIAnchor::BOTTOM_CENTER, "buy", vec2(0, -180), vec2(200, 100), equipCharacter, NULL);
	characters->addButton(UIAnchor::BOTTOM_CENTER, "done", vec2(0, -75), vec2(350, 100), gotoInterface, interfaces->get(MAIN_INTERFACE));
}

inline void	buildPauseInterface(InterfaceManager *interfaces)
{
	Interface	*pause = interfaces->get(PAUSE_INTERFACE);

	pause->addButton(UIAnchor::CENTER, "resume", vec2(0, 0), vec2(200, 100), [](void *data){(void)data;GAME->resume();}, NULL);
	pause->addButton(UIAnchor::CENTER, "quit", vec2(0, 105), vec2(200, 100), [](void *data){(void)data;quitGame();}, NULL);
}

inline void	buildGameInterface(InterfaceManager *interfaces)
{
	Interface	*game = interfaces->get(GAME_INTERFACE);

	game->onUpdate = [](void *data)
	{
		(void)data;
		Interface	*game = INTERFACES_MANAGER->get(GAME_INTERFACE);
		Text	*distance = static_cast<Text *>(game->elements[0]);
		Text	*collectibles = static_cast<Text *>(game->elements[1]);
		
		distance->label = "distance " + std::to_string((int)GAME->distance);
		distance->size.x = distance->label.size() * distance->size.y;
		collectibles->label = "collectibles " + std::to_string((int)GAME->collectibles);
		collectibles->size.x = collectibles->label.size() * collectibles->size.y;
	};
	game->elements.push_back(new Text(UIAnchor::TOP_LEFT, "distance", vec2(0, 0), vec2(35, 35)));
	game->elements.push_back(new Text(UIAnchor::TOP_LEFT, "collectibles", vec2(0, 40), vec2(35, 35)));
}

inline void	build(InterfaceManager *interfaces)
{
	buildMainInterface(interfaces);
	buildQuestsInterface(interfaces);
	buildCharactersInterface(interfaces);
	buildPauseInterface(interfaces);
	buildGameInterface(interfaces);
}

#endif
