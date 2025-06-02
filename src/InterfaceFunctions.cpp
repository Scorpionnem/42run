/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InterfaceFunctions.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:28:45 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/01 17:35:37 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

void	toggle_camera()
{
	camera_toggle = !camera_toggle;
}

void	toggle_texture()
{
	interpolate = !interpolate;
}

void	toggle_skybox()
{
	skybox_interpolate = !skybox_interpolate;
}

void	toggle_rainbow()
{
	rainbow = !rainbow;
}

void	toggle_mesh_spin()
{
	mesh_spin = !mesh_spin;
}

void	toggle_apply_normal()
{
	apply_normal = !apply_normal;
}

void	toggle_light_move()
{
	light_move = !light_move;
}

void	change_shader()
{
	shaderEffect++;
	if (shaderEffect > MAX_SHADER_EFFECT)
		shaderEffect = 0;
}

void	toggle_fpscap()
{
	lock_fps = !lock_fps;
	if (lock_fps)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

void	goto_camera_interface()
{
	interface = 1;
}

void	goto_main_interface()
{
	interface = 0;
}

void	goto_model_interface()
{
	interface = 2;
}

void	goto_light_interface()
{
	interface = 3;
}

void	goto_settings_interface()
{
	interface = -2;
}

void	goto_pause_interface()
{
	interface = -1;
}
