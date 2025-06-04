/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Room.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:31:26 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/04 14:33:07 by mbatty           ###   ########.fr       */
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

struct	Obstacle
{
	Mesh	*model;
	vec3	offset;
};

class	Room
{
	public:
		Mesh		*model;
		std::vector<Obstacle> obstacles;
		RoomType	type;
		vec3		pos;
		Room(Mesh *model, const vec3 &pos, RoomType type) : model(model), type(type), pos(pos)
		{
			int randomAmount = std::rand() % 5;
			int	neg = 1;
			for (int i = 0; i < randomAmount; i++)
			{
				rand() % 2 == 1 ? neg = 1 : neg = -1;
				obstacles.push_back({new Mesh("models/cube.obj", MISSING_TEXTURE), vec3((std::rand() % 4) * neg, 0, (std::rand() % 4) * neg)});
			}
			model->pos = pos;
		}
		~Room()
		{
			delete model;
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
				it->model->draw(shader);
			}
			model->pos = pos;
    	    model->draw(shader);
    	}
};

#endif
