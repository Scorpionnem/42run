/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InterfaceFunctions.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:34:53 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/01 17:36:53 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACEFUNCTIONS_HPP
# define INTERFACEFUNCTIONS_HPP

void	toggle_camera();
void	toggle_texture();
void	toggle_skybox();
void	toggle_rainbow();
void	toggle_mesh_spin();
void	toggle_apply_normal();
void	toggle_light_move();
void	change_shader();
void	toggle_fpscap();

void	goto_camera_interface();
void	goto_main_interface();
void	goto_model_interface();
void	goto_light_interface();
void	goto_settings_interface();
void	goto_pause_interface();

#endif