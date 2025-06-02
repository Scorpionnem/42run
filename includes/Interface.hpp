/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interface.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:21:35 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/02 10:12:19 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACE_HPP
# define INTERFACE_HPP

# include "Slider.hpp"

class	Interface
{
	public:
		Interface(){}
		~Interface(){}
		void	update(GLFWwindow *window, Shader &guiShader, Font &font, Shader &textShader);
		std::vector<Slider>	sliders;
		std::vector<Button>	buttons;
};

#endif