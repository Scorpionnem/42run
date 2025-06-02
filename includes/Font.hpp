/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Font.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:45:03 by mbatty            #+#    #+#             */
/*   Updated: 2025/05/28 12:53:31 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FONT_HPP
# define FONT_HPP

# include "libs.hpp"
# include "Shader.hpp"
# include "Texture.hpp"

extern GLuint fontVAO;
extern GLuint fontVBO;

class	Font
{
	public:
		~Font();
		Font();
		Texture	&operator[](char c);
		Texture	&getChar(char c);
		void	initFontModel();
		void    putChar(char c, Shader &shader, vec2 pos, vec2 size);
		void	putString(std::string str, Shader &shader, vec2 pos, vec2 size);

		std::array<Texture, 128> font;	
};

#endif