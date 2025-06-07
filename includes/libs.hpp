/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:33:10 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/06 12:09:52 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBS_HPP
# define LIBS_HPP

# define DEBUG 1

# include <iostream>
# include <string>
# include <fstream>
# include <sstream>
# include <istream>
# include <vector>
# include <map>
# include <cmath>
# include <unistd.h>
# include <exception>
# include <functional>
# include <array>
# include <algorithm>
# include <deque>

# include "vec2.hpp"
# include "vec3.hpp"
# include "vec4.hpp"
# include "mat4.hpp"

# include "strings.hpp"

# include "glad.h"
# include "glfw3.h"

std::string	toString(int nbr);
std::string	toString(float nbr);

# define WIN_NAME "42run"

extern float	SCREEN_WIDTH;
extern float	SCREEN_HEIGHT;
extern float	FOV;
extern float	RENDER_DISTANCE;
extern unsigned int	TOTAL_VERTICES;
extern unsigned int	TOTAL_TRIANGLES;
extern vec3	mesh_pos;
extern bool	rainbow;

extern bool	F1;
extern bool	F3;

extern bool	mesh_spin;

extern vec3	backgroundColor;

extern bool	lock_fps;
extern int	interface;
extern int	shaderEffect;
extern bool	light_move;
extern bool	apply_normal;
extern int	interpolate;
extern int	skybox_interpolate;
extern bool	camera_toggle;
extern bool	mesh_spin;
extern float ambientStrength;


# define TERMINAL_CHAR_SIZE 15
# define MAX_SHADER_EFFECT 3

inline float   clamp(float val, float min, float max)
{
    float   res = val;
    if (res < min)
        res = min;
    else if (res > max)
        res = max;
    return (res);
}

inline float   toRadians(float deg)
{
    return(deg * M_PI / 180.0f);
}

#endif
