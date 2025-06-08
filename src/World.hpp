/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:27:23 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/09 01:38:15 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORLD_HPP
# define WORLD_HPP

# include "Room.hpp"
# include "Game.hpp"

class   World
{
    public:
        World()
        : speed(10.0f)
		{
		}

		void	resetWorld()
		{
			for (auto it = rooms.begin(); it != rooms.end(); it++)
				delete *it;
			rooms.clear();
			speed = 0;
			int	offset = 0;
			speed = 10;
	    	std::srand(glfwGetTime());
			for (int i = 0; i < 8; i++)
			{
				rooms.push_back(new Room(NULL, vec3(0, 0, offset), Hardcoded));
				offset -= 8;
			}
			
			Mesh	*mesh;
			mesh = MESH_MANAGER->get("models/school_entrance.obj");
			rooms.push_back(new Room(mesh, vec3(0, 0, offset), Hardcoded));
		}
        ~World()
        {
			for (auto it = rooms.begin(); it != rooms.end(); it++)
				delete *it;
        }

		void	update(float deltaTime)
		{
			speed += 1 * deltaTime / 16;
			speed = clamp(speed, 5, 30);
    		for (auto it = rooms.begin(); it != rooms.end(); it++)
    		    (*it)->update(deltaTime, speed);
	
    		if (!rooms.empty() && rooms.front()->pos.z > 5)
			{
    		    delete rooms.front();
    		    rooms.pop_front();
    		    generateRoom(rooms.back()->type);
    		}
		}
		void	draw(Shader &shader)
		{
			for (auto it = rooms.begin(); it != rooms.end(); it++)
				(*it)->draw(shader);
		}

		float	speed;

		RoomType	chooseRoomType(RoomType prevRoomType)
		{
    		RoomType res = RoomType::Corridor;
			int	rand = std::rand() % 10;
			

			if (prevRoomType == Cluster1 && rand < 5)
				return (Cluster1Exit);
			else if (prevRoomType == Cluster1)
				return (Cluster1);
			if (prevRoomType == Cluster1Entrance)
				return (Cluster1);

			if (prevRoomType == Cluster2 && rand < 5)
				return (Cluster2Exit);
			else if (prevRoomType == Cluster2)
				return (Cluster2);
			if (prevRoomType == Cluster2Entrance)
				return (Cluster2);

			if (prevRoomType == Cluster3 && rand < 5)
				return (Cluster3Exit);
			else if (prevRoomType == Cluster3)
				return (Cluster3);
			if (prevRoomType == Cluster3Entrance)
				return (Cluster3);
				
			if (rand == 1)
				return (Cluster1Entrance);
			if (rand == 2)
				return (Cluster2Entrance);
			if (rand == 3)
				return (Cluster3Entrance);
			if (rand == 4)
				return (Cafetaria);
			return (res);
		}

		void	generateRoom(RoomType prevRoomType)
		{
			RoomType	type = chooseRoomType(prevRoomType);

    		std::string path = "models/corridor.obj";
    		std::string texPath = "models/bricks.bmp";
			if (type == Cluster1 || type == Cluster2 || type == Cluster3)
			{
				path = "models/clustermiddle.obj";
				texPath = "models/bricks.bmp";
			}
			if (type == Cluster1Exit || type == Cluster2Exit || type == Cluster3Exit)
			{
				path = "models/clusterexit.obj";
				texPath = "models/bricks.bmp";
			}
			if (type == Cluster1Entrance || type == Cluster2Entrance || type == Cluster3Entrance)
				texPath = "models/bricks.bmp";
			if (type == Corridor)
				texPath = "models/bricks.bmp";
			if (type == Cafetaria)
				texPath = "models/bricks.bmp";
			
    		Mesh* mesh = MESH_MANAGER->get(path, texPath);
			vec3	tmp = rooms.back()->pos;
			tmp.z -= 8;
    		Room* room = new Room(mesh, tmp, type);
    		rooms.push_back(room);
		}
		
		std::deque<Room *>	rooms;
};

#endif
