/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Slider.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 12:25:11 by mbatty            #+#    #+#             */
/*   Updated: 2025/05/28 11:47:44 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SLIDER_HPP
# define SLIDER_HPP

# include "Button.hpp"

extern GLuint sliderVAO;
extern GLuint sliderVBO;

class Slider
{
    public:
        ~Slider();
        Slider(std::string str, float width, float height, vec2 pos, Texture &sliderTexture, Texture &sliderPressedTexture, Texture &backgroundTexture);
        void    drawBackground(Shader &shader);
        void    drawSlider(Shader &shader);
        void    drawStr(Font &font, Shader &textShader);
        void    checkClick(vec2 mousePos, bool mousePressed);
        void    setSlider(float value);

        void initSliderModel();

        Texture &backgroundTexture;
        Texture &sliderTexture;
        Texture &sliderPressedTexture;

        float   width;
        float   height;
        
        float   sliderWidth;
        float   sliderHeight;
        bool    isSliderClicked = false;
        float   value = 0;
        float   minCenter;
        float   maxCenter;
        vec2   pos;
        vec2   sliderPos;
        std::string str;
};

#endif