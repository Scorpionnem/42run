/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Terminal.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:19:57 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/01 17:09:32 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Terminal.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"

bool		isTerminalOn = false;
std::string	terminalInput;
std::string	terminalReturn;
float	terminalReturnTime = -1;
std::string::iterator	terminalCursor;

bool		terminalIgnoreNext = false;

std::vector<std::string>	history;
std::vector<std::string>::iterator	historyCursor;

void	setTerminalReturn(std::string str)
{
	terminalReturn = str;
	terminalReturnTime = glfwGetTime();
}

void	terminal_setting_command(std::istringstream	&iss)
{
	std::string	arg;

	if (iss >> arg)
	{
	}
	else
		setTerminalReturn("setting: Not enough arguments.");
}

void	terminal_culling_command(std::istringstream	&iss)
{
	std::string	arg;

	if (iss >> arg)
	{
		if (arg == "clockwise" || arg == "cw" || arg == "CW")
		{
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CW);
		}
		if (arg == "counter_clockwise" || arg == "ccw" || arg == "CCW")
		{
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
		}
		if (arg == "off")
			glDisable(GL_CULL_FACE);
		else
			setTerminalReturn("culling help : off : clockwise : counter_clockwise");
	}
	else
		setTerminalReturn("culling: not enough arguments.");
}

void	terminal_background_command(std::istringstream &iss)
{
	std::string	arg;

	if (iss >> arg)
	{
		if (arg == "set")
		{
			float r, g, b;
			if (iss >> r && iss >> g && iss >> b)
				backgroundColor = vec3(r, g, b);
			else
				setTerminalReturn("Error");
		}
		else
			setTerminalReturn("background help : set");
	}
	else
		setTerminalReturn("background: not enough arguments.");
}

void	terminal_render_command(std::istringstream &iss)
{
	std::string	arg;

	if (iss >> arg)
	{
		if (arg == "wireframe")
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else if (arg == "normal")
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			setTerminalReturn("render help : wireframe : normal");
	}
	else
		setTerminalReturn("render: Not enough arguments.");
}

void	terminal_execute_command(std::string str)
{
	if (!str.size())
		return ;
	std::istringstream	iss(str);
	std::string	command;

	iss >> command;
	if (command == "setting")
		terminal_setting_command(iss);
	else if (command == "culling")
		terminal_culling_command(iss);
	else if (command == "background")
		terminal_background_command(iss);
	else if (command == "render")
		terminal_render_command(iss);
	else if (command == "rainbow")
		rainbow = !rainbow;
	else if (command == "help")
	{
		setTerminalReturn("commands: background, culling, render, help");
	}
	else
		setTerminalReturn("command not found.");
}

void	terminal_special_keys(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)window;(void)key;(void)scancode;(void)action;(void)mods;
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		if (isTerminalOn)
		{
			if (key == GLFW_KEY_BACKSPACE && terminalInput.size())
			{
				if (terminalCursor - terminalInput.begin() - 1 >= 0)
				{
					terminalInput.erase(terminalCursor - terminalInput.begin() - 1, 1);
					terminalCursor--;
				}
			}
			if (key == GLFW_KEY_ENTER)
			{
				terminal_execute_command(terminalInput);
				isTerminalOn = false;
				history.push_back(terminalInput);
				historyCursor = history.end();
				terminalInput.clear();
			}
			if (key == GLFW_KEY_LEFT && terminalCursor != terminalInput.begin())
			{
				terminalCursor--;
				if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
				{
					while (terminalCursor != terminalInput.begin() && std::isalnum(*terminalCursor))
						terminalCursor--;
				}
			}
			if (key == GLFW_KEY_RIGHT && terminalCursor != terminalInput.end())
			{
				terminalCursor++;
				if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
				{
					while (terminalCursor != terminalInput.end() && std::isalnum(*terminalCursor))
						terminalCursor++;
				}
			}
			if (key == GLFW_KEY_END)
				terminalCursor = terminalInput.end();
			if (key == GLFW_KEY_HOME)
				terminalCursor = terminalInput.begin();
			if (key == GLFW_KEY_UP && historyCursor != history.begin())
			{
				historyCursor--;
				terminalInput = *historyCursor;
				terminalCursor = terminalInput.end();
			}
			if (key == GLFW_KEY_DOWN)
			{
				if (historyCursor != history.end() && historyCursor + 1 != history.end())
				{
					historyCursor++;
					terminalInput = *historyCursor;
				}
				else
				{
					terminalInput = "";
					historyCursor = history.end();
				}
				terminalCursor = terminalInput.end();
			}
		}
		else if (key == GLFW_KEY_T || key == GLFW_KEY_SLASH)
		{
			isTerminalOn = true;
			terminalIgnoreNext = true;
			terminalCursor = terminalInput.end();
		}
	}
}

void	terminal_keyboard_input(GLFWwindow *window, unsigned int key)
{
	(void)window;
	if (paused)
		return ;
	if (isTerminalOn && !terminalIgnoreNext)
	{
		if (key >= 32 && key <= 126)
		{
			terminalCursor = terminalInput.insert(terminalCursor, 1, (char)key);
			terminalCursor++;
		}
	}
	terminalIgnoreNext = false;
}
