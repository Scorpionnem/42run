/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interface.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:44:12 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/03 11:49:40 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Interface.hpp"

void	Interface::update(GLFWwindow *window, Shader &guiShader, Font &font, Shader &textShader)
{
	double mouseX, mouseY;
	bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
		
	glfwGetCursorPos(window, &mouseX, &mouseY);

	guiShader.use();
	mat4 projection = ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	guiShader.setMat4("projection", projection);
		
	for (std::vector<Button>::iterator it = buttons.begin(); it != buttons.end(); it++)
	{
		it->texture.use();
		it->checkClick(vec2(mouseX, mouseY), mousePressed);
		it->draw(guiShader, font, textShader);
	}
	for (std::vector<Slider>::iterator it = sliders.begin(); it != sliders.end(); it++)
	{
		it->checkClick(vec2(mouseX, mouseY), mousePressed);
		it->drawBackground(guiShader);
		it->drawSlider(guiShader);
		it->drawStr(font, textShader);
	}
}
