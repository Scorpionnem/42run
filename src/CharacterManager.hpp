/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CharacterManager.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 22:59:58 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/08 23:49:40 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHARACTERMANAGER_HPP
# define CHARACTERMANAGER_HPP

# include "Character.hpp"
# include "MeshManager.hpp"

class	CharacterManager
{
	public:
		CharacterManager()
		{
			for (int i = 0; i < MAX_CHARACTER; i++)
			{
				switch (i)
				{
					case SCORPIONEM:
					{
						characters[i] = new Character((CharactersID)i, MESH_MANAGER->get("models/scorpionem/scorpionem.obj"), MESH_MANAGER->get("models/scorpionem/scorpionem.obj"));
						break ;
					}
					case ELEO:
					{
						characters[i] = new Character((CharactersID)i, MESH_MANAGER->get("models/mix/mix.obj"), MESH_MANAGER->get("models/mix/mix.obj"));
						break;
					}
					case THEWORFER27:
					{
						characters[i] = new Character((CharactersID)i, MESH_MANAGER->get("models/scorpionem/scorpionem.obj"), MESH_MANAGER->get("models/scorpionem/scorpionem.obj"));
						break;
					}
					default:
						characters[i] = new Character((CharactersID)i, MESH_MANAGER->get("models/scorpionem/scorpionem.obj"), MESH_MANAGER->get("models/scorpionem/scorpionem.obj"));
				}
			}
			current = characters[0];
			current->equipped = true;
		}
		~CharacterManager()
		{
			for (auto *character : characters)
				delete character;	
		}
		void	useNext()
		{
			CharactersID ID = current->ID;

			ID = static_cast<CharactersID>((static_cast<int>(ID) + 1) % MAX_CHARACTER);
			current = characters[ID];
		}
		void usePrevious()
		{
			int ID = static_cast<int>(current->ID) - 1;

			if (ID < 0)
				ID = MAX_CHARACTER - 1;

			current = characters[ID];
		}
		void	useEquiped()
		{
			for (auto *character : characters)
			{
				if (character->equipped)
				{
					current = character;
					break ;
				}
			}
		}
		void	equipCurrent()
		{
			for (auto *character : characters)
				character->equipped = false;
			current->equipped = true;
		}

		std::array<Character *, MAX_CHARACTER>	characters;
		Character								*current;
};

extern CharacterManager	*CHARACTER_MANAGER;

#endif
