/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UIManager.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:19:17 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/03 12:20:04 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UIMANAGER_HPP
# define UIMANAGER_HPP

# include "libs.hpp"
# include "Texture.hpp"
# include "Interface.hpp"

class	UIManager
{
	public:
		UIManager() : iconTexture(ICON_PATH), buttonTexture(BUTTON_PATH), buttonPressedTexture(BUTTON_PRESSED_PATH)
		{
		}
		~UIManager()
		{
			for (auto it = interfaces.begin(); it != interfaces.end(); it++)
				delete it->second;
		}
		Interface	*get(const std::string &id)
		{
			return (interfaces.find(id)->second);
		}
		void	draw(const std::string &id, GLFWwindow *window, Shader &guiShader, Font &font, Shader &textShader)
		{
			Interface *current = interfaces.find(id)->second;
			
			current->update(window, guiShader, font, textShader);
		}
		void	createInterface(const std::string &id)
		{
			interfaces.insert({id, new Interface()});
		}
		void	addButtonToInterface(const std::string &id, Button button)
		{
			Interface *current = interfaces.find(id)->second;

			current->buttons.push_back(button);
		}
		void	addSliderToInterface(const std::string &id, Slider slider)
		{
			Interface *current = interfaces.find(id)->second;

			current->sliders.push_back(slider);
		}
		Texture	iconTexture;
		Texture	buttonTexture;
		Texture	buttonPressedTexture;
	private:
		std::map<std::string, Interface *> interfaces;
};

#endif