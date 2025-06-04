/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:54:38 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/04 10:09:26 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"
#include "Camera.hpp"

vec3 camPos;
vec3 camFront;
vec3 camUp;

float	deltaTime = 0.0f;
float	lastFrame = 0.0f;

float camYaw = -90.0f;
float camPitch = 0.0f;

Camera::Camera()
{
	camPos = vec3(0.0f, 0.0f, 0.0f);
	camFront = vec3(0.0f, 0.0f, -1.0f);
	camUp = vec3(0.0f, 1.0f, 0.0f);
}

void	Camera::update()
{
	this->direction.x = cos(toRadians(camYaw)) * cos(toRadians(camPitch));
	this->direction.y = sin(toRadians(camPitch));
	this->direction.z = sin(toRadians(camYaw)) * cos(toRadians(camPitch));
	camFront = direction.normalize();
}

void	Camera::setViewMatrix(Shader &shader)
{
	mat4	view = this->getViewMatrix();
	mat4	projection = perspective(FOV, SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, RENDER_DISTANCE);

	shader.use();
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
}

mat4	Camera::getViewMatrix()
{
	return (lookAt(camPos, camPos + camFront, camUp));
}
