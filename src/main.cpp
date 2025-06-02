/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:33:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/02 17:40:48 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

#include "Camera.hpp"
#include "Shader.hpp"

#include "Mesh.hpp"
#include "Light.hpp"
#include "Terminal.hpp"
#include "Button.hpp"
#include "Slider.hpp"
#include "Window.hpp"
#include "Interface.hpp"
#include "Skybox.hpp"
#include "InterfaceFunctions.hpp"

float	FOV = 70;
#define MAX_FOV 100
float	SCREEN_WIDTH = 800;
float	SCREEN_HEIGHT = 800;
float	RENDER_DISTANCE = 1000;

vec3	backgroundColor(0.05, 0.05, 0.05);

vec3	mesh_pos;
float	mesh_roll;
vec3	playerPos(0);
vec3	playerMotion(0);
bool	onGround = true;

int		interpolate = 0;
int		skybox_interpolate = 0;
float	ambientStrength = 0.2;

bool	F1 = false;
bool	F3 = false;
bool	lock_fps = true;
bool	rainbow = false;
bool	camera_toggle = false;
bool	light_move = true;
bool	apply_normal = false;
bool	mesh_spin = true;

unsigned int	TOTAL_VERTICES = 0;
unsigned int	TOTAL_TRIANGLES = 0;

bool	paused = false;
double	pause_time = 0;

int	shaderEffect = 0;
int	interface = 0;

void	interpolateTo(float &float1, float &float2, float deltaTime)
{
	float1 += (1.0f - float1) * 2.0f * deltaTime;
	if (float1 > 0.99f)
		float1 = 1.0f;

	float2 = 1.0f - float1;
}

std::string	toString(int nbr)
{
	std::stringstream	strs;
	strs << nbr;
	return (strs.str());
}

void	frame_key_hook(Window &window)
{
	if (isTerminalOn)
		return ;
	
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_C) == GLFW_PRESS)
		window.center();

	float cameraSpeed = 15.0f * window.getDeltaTime();
	float	speedBoost = 1.0f;

	if (paused)
		return ;

	if (glfwGetKey(window.getWindowData(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		speedBoost = 20.0f;
	
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_W) == GLFW_PRESS)
		pos = pos + front * (cameraSpeed * speedBoost);
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_S) == GLFW_PRESS)
		pos = pos - front * (cameraSpeed * speedBoost);
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_SPACE) == GLFW_PRESS)
		pos = pos + up * (cameraSpeed * speedBoost);
	if (onGround && glfwGetKey(window.getWindowData(), GLFW_KEY_J) == GLFW_PRESS)
	{
		playerMotion.y = 10;
	}
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		pos = pos - up * (cameraSpeed * speedBoost);
		
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_A) == GLFW_PRESS)
		pos = pos -  front.cross(up).normalize() * (cameraSpeed * speedBoost);
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_D) == GLFW_PRESS)
		pos = pos + front.cross(up).normalize() * (cameraSpeed * speedBoost);
		
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_LEFT) == GLFW_PRESS)
		yaw -= (10.0f * cameraSpeed) * 1.f;
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_RIGHT) == GLFW_PRESS)
		yaw += (10.0f * cameraSpeed) * 1.f;
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_UP) == GLFW_PRESS)
		pitch += (10.0f * cameraSpeed) * 1.f;
	if (glfwGetKey(window.getWindowData(), GLFW_KEY_DOWN) == GLFW_PRESS)
		pitch -= (10.0f * cameraSpeed) * 1.f;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}

void	pause_render()
{
	if (!paused)
	{
		F1 = false;
		pause_time = glfwGetTime();
		paused = true;
		goto_pause_interface();
	}
	else
	{
		paused = false;
		goto_main_interface();
	}
}

void	key_hook(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)window;(void)key;(void)scancode;(void)action;(void)mods;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		pause_render();
	}
	if (paused)
		return ;
		
	if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
		F3 = !F3;
}

void	glBugReport(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
	(void)userParam;
	
	if (!DEBUG)
		return ;
	if (type == GL_DEBUG_TYPE_PERFORMANCE || severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		std::cout << "---WARNING---" << std::endl;
	else
		std::cout << "---ERROR---" << std::endl;
	std::cout << "source: " << source << std::endl;
	std::cout << "type: " << type << std::endl;
	std::cout << "id: " << id << std::endl;
	std::cout << "severity: " << severity << std::endl;
	std::cout << "length: " << length << std::endl;
	std::cout << "message: " << message << std::endl;
	std::cout << "-----------" << std::endl;
}

Window	*g_window;

void	close_window()
{
	glfwSetWindowShouldClose(g_window->getWindowData(), true);
}

void	resume_render()
{
	paused = false;
	interface = 0;
	glfwSetTime(pause_time);
}

//Lol, just for my dummy buttons because I dont want to do a put image function
void	do_nothing(){}

int	main(int ac, char **av)
{
	(void)av;
	if (ac != 1)
	{
		std::cerr << "Too many arguments" << std::endl;
		return (1);
	}
	try {
		Window		window;
		g_window = &window;
		
		Camera		camera;
		
		Shader		shader(MESH_VERT_SHADER, MESH_FRAG_SHADER);
		Shader		guiShader(GUI_VERT_SHADER, GUI_FRAG_SHADER);
		Shader		text_shader(TEXT_VERT_SHADER, TEXT_FRAG_SHADER);
		Shader		skybox_shader(SKYBOX_VERT_SHADER, SKYBOX_FRAG_SHADER);

		Texture		icon_texture(ICON_PATH);
		Texture		button_texture(BUTTON_PATH);
		Texture		button_pressed_texture(BUTTON_PRESSED_PATH);

		Light		light;
		
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(glBugReport, NULL);
		
		Font	font;
		
		Interface	pauseInterface;
		pauseInterface.buttons.push_back(Button("", 200, 200, vec2((SCREEN_WIDTH / 2) - 100, 50), do_nothing, icon_texture, icon_texture));
		pauseInterface.buttons.push_back(Button("resume", 250, 75, vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) - 80), resume_render, button_texture, button_pressed_texture));
		pauseInterface.buttons.push_back(Button("settings", 250, 75, vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2)), resume_render, button_texture, button_pressed_texture));
		pauseInterface.buttons.push_back(Button("quit", 250, 75, vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) + 80), close_window, button_texture, button_pressed_texture));
		
		pos = vec3(0, 6, 10);
		pitch = -20;
		
		Skybox	skybox({
			"textures/skybox/right.bmp",
    	    "textures/skybox/left.bmp",
    	    "textures/skybox/top.bmp",
    	    "textures/skybox/bottom.bmp",
    	    "textures/skybox/front.bmp",
    	    "textures/skybox/back.bmp"
    	});
		skybox_shader.use();
		skybox_shader.setInt("skybox", 0);
		
		shader.use();
		shader.setInt("tex0", 0);
		shader.setInt("tex1", 1);
		guiShader.use();
		guiShader.setInt("tex0", 0);
		text_shader.use();
		text_shader.setInt("tex0", 0);
		
		pause_render();
		
		Mesh	room;
		room.loadOBJ("models/plane.obj", MISSING_TEXTURE);
		room.pos = vec3(0, 0, 0);
		
		Mesh	player;
		player.loadOBJ("models/scorpionem/scorpionem.obj", "textures/mbatty.bmp");

		while (window.up())
		{
			window.loopStart();
	
			if (paused)
				glfwSetTime(pause_time);

			camera.update();
			camera.setViewMatrix(shader);
			light.update(shader);

			playerPos.y += playerMotion.y * window.getDeltaTime();
			playerMotion.y -= 9.8 * window.getDeltaTime();
			onGround = false;
			if (playerPos.y < 0)
			{
				onGround = true;
				playerMotion.y = 0;
				playerPos.y = 0;
			}
			player.pos = playerPos;

			text_shader.use();
			text_shader.setFloat("time", glfwGetTime());
			text_shader.setFloat("SCREEN_WIDTH", SCREEN_WIDTH);
			text_shader.setFloat("SCREEN_HEIGHT", SCREEN_HEIGHT);
			text_shader.setVec3("color", vec3(1.0, 1.0, 1.0));
			shader.use();
			shader.setFloat("ambientStrength", ambientStrength);
			shader.setVec3("viewPos", pos);
			shader.setFloat("time", glfwGetTime());

			skybox.draw(camera, skybox_shader);
				
			room.draw(shader);
			player.draw(shader);

			if (!F1)
			{
				if (interface == -1)
					pauseInterface.update(window.getWindowData(), guiShader, font, text_shader);
			}
			if (paused)
			{
				pauseInterface.buttons[0].pos = vec2((SCREEN_WIDTH / 2) - 100, 50);
				pauseInterface.buttons[1].pos = vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) - 80);
				pauseInterface.buttons[2].pos = vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2));
				pauseInterface.buttons[3].pos = vec2((SCREEN_WIDTH / 2) - 125, (SCREEN_HEIGHT / 2) + 80);
			}

			frame_key_hook(window);
			window.loopEnd(font, text_shader);
		}
	} catch (const std::exception &e) {
		std::cerr << "An error occurred: " << e.what() << std::endl;
		return (1);
	}
}
