/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Button.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:04:30 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/01 15:08:46 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUTTON_HPP
# define BUTTON_HPP

# include "libs.hpp"
# include "Shader.hpp"
# include "Font.hpp"
# include "Texture.hpp"

extern GLuint buttonVAO;
extern GLuint buttonVBO;

bool isInside(vec2 buttonPos, vec2 mousePos, float width, float height);

class   Button
{
    public:
        ~Button();
        Button(std::string str, float width, float height, vec2 pos, std::function<void()> function, Texture &texture, Texture &pressedTexture);
        void    draw(Shader &shader, Font &font, Shader &textShader);
        void    checkClick(vec2 mousePos, bool mousePressed);
        
        void    initButtonModel();
        
        float                   width;
        float                   height;
        float                   wasPressedInside = false;
        std::function<void()>   onClick;

        vec2   pos;
        std::string str;
        Texture &texture;
        Texture &pressedTexture;
    private:
};

#endif