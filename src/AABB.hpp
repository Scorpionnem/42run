/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AABB.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 21:45:21 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/06 12:13:31 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AABB_HPP
# define AABB_HPP

#include "Mesh.hpp"
#include "Game.hpp"
#include "MeshManager.hpp"

class AABB
{
	public:
    	vec3 center;
    	vec3 halfSize;
	
    	AABB(vec3 center, vec3 size)
		{
			this->halfSize.x = size.x / 2;
			this->halfSize.y = size.y / 2;
			this->halfSize.z = size.z / 2;
			this->center.x = center.x;
			this->center.y = center.y + this->halfSize.y;
			this->center.z = center.z;
		}
};

inline void	drawHitbox(Shader &shader, vec3 pos, vec3 size)
{
	(void)shader;
	Mesh	*mesh = g_meshManager->get("models/cube.obj", "textures/mbatty.bmp");

	mesh->pos = pos;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	mesh->draw(*g_shaderManager->get("fullbright"), size);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

inline void resolveAABBCollision(AABB &player, const AABB &other)
{
	vec3 delta = {
		player.center.x - other.center.x,
		player.center.y - other.center.y,
		player.center.z - other.center.z
	};

	vec3 overlap = {
		(player.halfSize.x + other.halfSize.x) - std::abs(delta.x),
		(player.halfSize.y + other.halfSize.y) - std::abs(delta.y),
		(player.halfSize.z + other.halfSize.z) - std::abs(delta.z)
	};

	if (overlap.x < overlap.y && overlap.x < overlap.z)
		player.center.x += (delta.x < 0 ? -overlap.x : overlap.x);
	else if (overlap.y < overlap.z)
		player.center.y += (delta.y < 0 ? -overlap.y : overlap.y);
	else
		player.center.z += (delta.z < 0 ? -overlap.z : overlap.z);
}

inline bool doesCollide(const AABB &a, const AABB &b)
{
    return (std::abs(a.center.x - b.center.x) <= (a.halfSize.x + b.halfSize.x)) &&
           (std::abs(a.center.y - b.center.y) <= (a.halfSize.y + b.halfSize.y)) &&
           (std::abs(a.center.z - b.center.z) <= (a.halfSize.z + b.halfSize.z));
}

#endif
