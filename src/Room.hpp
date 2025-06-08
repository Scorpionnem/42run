/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Room.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:31:26 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/09 01:34:22 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOM_HPP
# define ROOM_HPP

# include "libs.hpp"
# include "Mesh.hpp"
# include "Shader.hpp"
# include "AABB.hpp"
# include "Player.hpp"

enum RoomType
{
    Hardcoded,
	Corridor,
	Cafetaria,
	Cluster1Entrance,
	Cluster1,
	Cluster1Exit,
	Cluster2Entrance,
	Cluster2,
	Cluster2Exit,
	Cluster3Entrance,
	Cluster3,
	Cluster3Exit
};

struct	Object
{
	Mesh	*model;
	vec3	hitboxSize;
	vec3	offset;
	vec3	scale;
	bool	draw;
};

class	Room
{
	public:
		Mesh		*model;
		std::vector<Object> obstacles;
		std::vector<Object> collectibles;
		std::vector<Object> powerups;
		RoomType	type;
		vec3		pos;
		Room(Mesh *model, const vec3 &pos, RoomType type) : model(model), type(type), pos(pos)
		{
			vec3	obPos;
			int	neg = -1;
			rand() % 2 == 1 ? neg = 1 : neg = -1;
			if (model && rand() % 4 == 1)
			{
				obstacles.push_back({MESH_MANAGER->get("models/bar_horizontal.obj"), vec3(10, 1, 1), vec3(0, rand() % 30 / 10.f, 2), vec3(1, 1, 1), true});
				obPos = this->pos + obstacles.back().offset; 
				if (!canSpawn(AABB(obPos, obstacles.back().hitboxSize)))
					obstacles.pop_back();
			}
			if (model && rand() % 2)
			{
				obstacles.push_back({MESH_MANAGER->get("models/bar.obj"), vec3(1, 10, 1), vec3(neg * rand() % 30 / 10.f, 0, 0), vec3(1, 1, 1), true});
				obPos = this->pos + obstacles.back().offset; 
				if (!canSpawn(AABB(obPos, obstacles.back().hitboxSize)))
					obstacles.pop_back();
			}
			if (model && rand() % 2)
			{
				obstacles.push_back({MESH_MANAGER->get("models/cluster2cube.obj"), vec3(1, 1, 1), vec3(neg * rand() % 30 / 10.f, 0, 0), vec3(1, 1, 1), true});
				obPos = this->pos + obstacles.back().offset; 
				if (!canSpawn(AABB(obPos, obstacles.back().hitboxSize)))
					obstacles.pop_back();
			}
			if (model && rand() % 2)
			{
				obstacles.push_back({MESH_MANAGER->get("models/cluster2cube.obj"), vec3(1, 1, 1), vec3(rand() % 30 / 10.f, 0, 2), vec3(1, 1, 1), true});
				obPos = this->pos + obstacles.back().offset; 
				if (!canSpawn(AABB(obPos, obstacles.back().hitboxSize)))
					obstacles.pop_back();
			}
			if (model && rand() % 2)
			{
				obstacles.push_back({MESH_MANAGER->get("models/cluster1post.obj"), vec3(3, 1, 1), vec3(neg * rand() % 30 / 10.f, 0, -2), vec3(1, 1, 1), true});
				obPos = this->pos + obstacles.back().offset; 
				if (!canSpawn(AABB(obPos, obstacles.back().hitboxSize)))
					obstacles.pop_back();
			}
			if (model && rand() % 2)
			{
				obstacles.push_back({MESH_MANAGER->get("models/cluster1post.obj"), vec3(3, 1, 1), vec3(neg * rand() % 30 / 10.f, 0, -2), vec3(1, 1, 1), true});
				obPos = this->pos + obstacles.back().offset; 
				if (!canSpawn(AABB(obPos, obstacles.back().hitboxSize)))
					obstacles.pop_back();
			}
			collectibles.push_back({MESH_MANAGER->get("models/125.obj", "models/125.bmp"), vec3(0.8, 0.8, 0.8), vec3(neg * (rand() % 30 / 10.f), 0, neg * (rand() % 30 / 10.f)), vec3(1, 1, 1), true});
			if (rand() % 4 == 1)
				powerups.push_back({MESH_MANAGER->get("models/125.obj", "models/mylittleponey.bmp"), vec3(0.8, 0.8, 0.8), vec3(neg * (rand() % 30 / 10.f), 0, neg * (rand() % 30 / 10.f)), vec3(1, 1, 1), true});
		}
		~Room()
		{
		}
		
		void update(float deltaTime, float speed)
		{
			pos.z += speed * deltaTime;
    	}

    	void draw(Shader& shader)
		{
			if (model)
			{
				model->pos = pos;
    	    	model->draw(shader);
			}
		
			for (auto it = obstacles.begin(); it != obstacles.end(); it++)
			{
				if (it->model)
				{
					it->model->pos = pos + it->offset;
					if (it->model->pos.z > 1)
						continue ;
					it->model->draw(shader);
				}
				if (DEBUG)
					drawHitbox(shader, pos + it->offset, it->hitboxSize);
			}
			for (auto it = collectibles.begin(); it != collectibles.end(); it++)
			{
				if (!it->draw)
					continue ;
				it->model->pos = pos + it->offset;
				it->model->rotateIntensity = vec3(0, 1, 0);
				it->model->draw(shader, vec3(4, 4, 4));
				if (DEBUG)
					drawHitbox(shader, pos + it->offset, it->hitboxSize);
			}
			for (auto it = powerups.begin(); it != powerups.end(); it++)
			{
				if (!it->draw)
					continue ;
				it->model->pos = pos + it->offset;
				it->model->rotateIntensity = vec3(0, 1, 0);
				it->model->draw(shader, vec3(4, 4, 4));
			}
    	}
		bool	canSpawn(AABB hitbox)
		{
			for (auto it = obstacles.begin(); it != obstacles.end(); it++)
			{
				vec3	obPos = this->pos + it->offset;
				if (it != obstacles.begin() && doesCollide(hitbox, AABB(obPos, it->hitboxSize)))
					return (false);
			}
			return (true);
		}
		float	distance(vec3 p1, vec3 p2)
		{
			return (std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2) + std::pow(p1.z - p2.z, 2)));
		}
};

#endif
