/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Light.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:41:09 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/02 13:07:54 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Light.hpp"

Light::~Light()
{
}

Light::Light()
{
    pos = vec3(0.0f);
    color = vec3(1.0f);
}

void	Light::update(Shader &shader)
{	
	pos = vec3(-16, 32, 0);
	shader.use();
	shader.setVec3("lightPos", pos);
	shader.setVec3("lightColor", color);
}
