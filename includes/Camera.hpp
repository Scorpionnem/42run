/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:55:06 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/04 10:10:11 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "libs.hpp"
# include "Shader.hpp"

extern vec3 camPos;
extern vec3 camFront;
extern vec3 camUp;

extern float	deltaTime;
extern float	lastFrame;

extern float camYaw;
extern float camPitch;

class Camera
{
	public:
		Camera();
		void	update();
		mat4	getViewMatrix();
		vec3 direction;
		void	setViewMatrix(Shader &shader);
};

#endif
