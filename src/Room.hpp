/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Room.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:31:26 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/04 19:36:51 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOM_HPP
# define ROOM_HPP

# include "libs.hpp"
# include "Mesh.hpp"
# include "Shader.hpp"

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
		RoomType	type;
		vec3		pos;
		bool	drawModel;
		Room(Mesh *model, const vec3 &pos, RoomType type) : model(model), type(type), pos(pos), drawModel(true)
		{
			int randomAmount = rand() % 10;
			int	neg = 1;
			for (int i = 0; i < randomAmount; i++)
			{
				rand() % 2 == 1 ? neg = 1 : neg = -1;
				float	pos = std::rand() % 4 * neg;
				obstacles.push_back({new Mesh("models/cube.obj", MISSING_TEXTURE), vec3(pos, 0, pos), vec3(1), true});
			}
			randomAmount = rand() % 2;
			neg = 1;
			for (int i = 0; i < randomAmount; i++)
			{
				rand() % 2 == 1 ? neg = 1 : neg = -1;
				float	pos = std::rand() % 4 * neg;
				collectibles.push_back({new Mesh("models/cube.obj", "models/125.bmp"), vec3(pos, 0, pos), vec3(1), true});
			}
			model->pos = pos;
		}
		Room(Mesh *model, const vec3 &pos, RoomType type, bool draw) : model(model), type(type), pos(pos), drawModel(draw)
		{
			model->pos = pos;
		}
		~Room()
		{
			delete model;
			if (obstacles.size())
				for (auto it = obstacles.begin(); it != obstacles.end(); it++)
					delete it->model;
			if (collectibles.size())
				for (auto it = collectibles.begin(); it != obstacles.end(); it++)
					delete it->model;
		}
		
		void update(float deltaTime, float speed)
		{
			pos.z += speed * deltaTime;
			model->pos = pos;
    	}

    	void draw(Shader& shader)
		{
			for (auto it = obstacles.begin(); it != obstacles.end(); it++)
			{
				it->model->pos = pos + it->offset;
				it->model->draw(shader, it->scale);
			}
			for (auto it = collectibles.begin(); it != collectibles.end(); it++)
			{
				if (!it->draw)
					continue ;
				it->model->pos = pos + it->offset;
				it->model->draw(shader);
			}
			model->pos = pos;
			if (drawModel)
    	    	model->draw(shader);
    	}
		float	distance(vec3 p1, vec3 p2)
		{
			return (std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2) + std::pow(p1.z - p2.z, 2)));
		}
		bool	doesPlayerCollide(vec3 playerPos)
		{
			for (auto it = obstacles.begin(); it != obstacles.end(); it++)
			{
				if (distance(pos + it->offset, playerPos) < 1)
					return (true);
			}
			return (false);
		}
		bool	doesPlayerCollect(vec3 playerPos)
		{
			for (auto it = collectibles.begin(); it != collectibles.end(); it++)
			{
				if (distance(pos + it->offset, playerPos) < 1 && it->draw)
				{
					it->draw = false;
					return (true);
				}
			}
			return (false);
		}
};

#endif
