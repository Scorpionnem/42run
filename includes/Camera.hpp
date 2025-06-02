/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:55:06 by mbatty            #+#    #+#             */
/*   Updated: 2025/05/24 13:21:51 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "libs.hpp"
# include "Shader.hpp"

extern vec3 pos;
extern vec3 front;
extern vec3 up;

extern float	deltaTime;
extern float	lastFrame;

extern float yaw;
extern float pitch;

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