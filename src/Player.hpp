/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 10:00:22 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/05 12:34:43 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_HPP
# define PLAYER_HPP

# include "libs.hpp"
# include "Shader.hpp"
# include "Mesh.hpp"
# include "World.hpp"

# include "AABB.hpp"

class   Player
{
    public:
        Player()
        {
            this->pos = vec3(0, 0, 0);
        }
        void    load()
        {
            mesh = g_meshManager->get("models/scorpionem/scorpionem.obj");
            sneakmesh = g_meshManager->get("models/scorpionem/scorpionem.obj");
            mesh->pos = pos;
            sneakmesh->pos = pos;
        }
        ~Player()
        {

        }
        void    update(float deltaTime)
        {
            load();

            if (sneaking)
                hitboxSize = vec3(.9, 1, .9);
            else
                hitboxSize = vec3(.9, 2, .9);

            double timeSinceFall = glfwGetTime() - timeFallStart;
            float gravityMultiplier = timeSinceFall > 0.2f ? 1.5f : 1.0f;

            velocity.y -= ((30 * gravityMultiplier) + (sneaking * 50)) * deltaTime;

            pos.y += velocity.y * deltaTime;
    
            onGround = false;
            if (pos.y <= 0.001f) {
                velocity.y = 0;
                pos.y = 0;
                onGround = true;
            }
        
            mesh->pos = pos;
            sneakmesh->pos = pos;
        }

        bool    isDead(World &world)
        {
            AABB playerHitbox(pos, hitboxSize);

            for (auto roomIt = world.rooms.begin(); roomIt != world.rooms.end(); roomIt++)
                for (auto it = (*roomIt)->obstacles.begin(); it != (*roomIt)->obstacles.end(); it++)
                    if (doesCollide(playerHitbox, AABB((*roomIt)->pos + it->offset, it->hitboxSize)))
                        return (true);
            return (false);
        }
        bool    doesCollect(World &world)
        {
            AABB playerHitbox(pos, hitboxSize);

            for (auto roomIt = world.rooms.begin(); roomIt != world.rooms.end(); roomIt++)
                for (auto it = (*roomIt)->collectibles.begin(); it != (*roomIt)->collectibles.end(); it++)
                    if (doesCollide(playerHitbox, AABB((*roomIt)->pos + it->offset, it->hitboxSize)) && it->draw)
                    {
                        it->draw = false;
                        return (true);
                    }
            return (false);
        }
        void    draw(Shader &shader)
        {
            if (DEBUG)
                drawHitbox(shader, pos, hitboxSize);
            if (sneaking)
                sneakmesh->draw(shader);
            else
                mesh->draw(shader);
        }

        Mesh    *mesh;
        Mesh    *sneakmesh;
        vec3    pos;
        vec3    velocity;
        vec3    hitboxSize;

        bool    onGround = true;
        bool    sneaking = false;
        double  timeSinceFall = 0;
        double  timeFallStart = 0;
};

#endif
