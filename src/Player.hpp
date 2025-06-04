/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 10:00:22 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/04 10:01:23 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_HPP
# define PLAYER_HPP

# include "libs.hpp"
# include "Shader.hpp"
# include "Mesh.hpp"

class   Player
{
    public:
        Player() : mesh("models/scorpionem/scorpionem.obj", MISSING_TEXTURE), sneakmesh("models/mix/mix.obj", MISSING_TEXTURE)
        {
            this->pos = vec3(0, 0, 0);
        }
        ~Player()
        {

        }
        void    update(float deltaTime)
        {
            double  timeSinceFall = glfwGetTime() - timeFallStart;
            float gravityMultiplier = timeSinceFall > 0.2f ? 1.5f : 1.0f;
            velocity.y -= 30 * gravityMultiplier * deltaTime;
            this->pos.y += this->velocity.y * deltaTime;
            onGround = false;
            if (this->pos.y <= 0)
            {
                this->velocity.y = 0;
                this->pos.y = 0;
                onGround = true;
            }
            mesh.pos = this->pos;
            sneakmesh.pos = this->pos;
        }
        void    draw(Shader &shader)
        {
            if (sneaking)
                sneakmesh.draw(shader);
            else
                mesh.draw(shader);
        }

        Mesh    mesh;
        Mesh    sneakmesh;
        vec3    pos;
        vec3    velocity;
        bool    onGround = true;
        bool    sneaking = false;
        double  timeSinceFall = 0;
        double  timeFallStart = 0;
};

#endif
