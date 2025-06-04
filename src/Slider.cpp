/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Slider.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 13:04:21 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/03 11:37:07 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Slider.hpp"

GLuint sliderVAO = 0, sliderVBO = 0;

Slider::~Slider()
{
    if (sliderVAO != 0)
    {
        if (DEBUG)
            std::cout << "Destroying slider quad" << std::endl;
        glDeleteBuffers(1, &sliderVBO);
        glDeleteVertexArrays(1, &sliderVAO);
        sliderVBO = 0;
        sliderVAO = 0;
    }
}

Slider::Slider(std::string str, float width, float height, vec2 pos, Texture &sliderTexture, Texture &sliderPressedTexture, Texture &backgroundTexture)
: backgroundTexture(backgroundTexture), sliderTexture(sliderTexture), sliderPressedTexture(sliderPressedTexture)
{
    this->str = str;
    this->pos = pos;
    this->width = width;
    this->height = height;

    this->sliderWidth = width / 4;
    this->sliderHeight = height;
    this->sliderPos = vec2(pos.x + width / 2, pos.y);
    this->minCenter = pos.x + sliderWidth * 0.5f;
    this->maxCenter = pos.x + width - sliderWidth * 0.5f;
    setSlider(0.5);
}

void Slider::initSliderModel()
{
    if (sliderVAO != 0) return;
    
    if (DEBUG)
        std::cout << "Loading slider quad" << std::endl;

    float vertices[] = {
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
		0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
    };

    glGenVertexArrays(1, &sliderVAO);
    glGenBuffers(1, &sliderVBO);

    glBindVertexArray(sliderVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sliderVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void    Slider::checkClick(vec2 mousePos, bool mousePressed)
{
    this->sliderPos.y = pos.y;
    this->setSlider(this->value);
    this->minCenter = pos.x + sliderWidth * 0.5f;
    this->maxCenter = pos.x + width - sliderWidth * 0.5f;
    
    bool inside = isInside(this->pos, mousePos, this->width, this->height);
    
    isSliderClicked = false;
    if (inside && mousePressed)
    {
        sliderPos.x = mousePos.x - (sliderWidth / 2);
        isSliderClicked = true;
    }
    if (sliderPos.x > pos.x + width - (sliderWidth))
        sliderPos.x = pos.x + width - (sliderWidth);
    if (sliderPos.x < pos.x)
        sliderPos.x = pos.x;
    float sliderCenter = sliderPos.x + (sliderWidth / 2);
    float normalizedValue = (sliderCenter - minCenter) / (maxCenter - minCenter);
    normalizedValue = clamp(normalizedValue, 0.0f, 1.0f);
            
    value = normalizedValue;
}

void    Slider::setSlider(float value)
{
    value = clamp(value, 0.0f, 1.0f);
    this->value = value;
    
    float minCenter = pos.x + (sliderWidth / 2);
    float maxCenter = pos.x + width - (sliderWidth / 2);
    float centerX = minCenter + value * (maxCenter - minCenter);

    sliderPos.x = centerX - (sliderWidth / 2);
}

void    Slider::drawBackground(Shader &shader)
{
    initSliderModel();
    shader.use();
    backgroundTexture.use();
    
    mat4 model = translate(mat4(1.0f), vec3(pos.x, pos.y, 0.0f));
    model = scale(model, vec3(width, height, 1.0f));
        
    shader.setMat4("model", model);
        
    glBindVertexArray(sliderVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void    Slider::drawSlider(Shader &shader)
{
    initSliderModel();
    shader.use();
    if (isSliderClicked)
        sliderPressedTexture.use();
    else
        sliderTexture.use();
        
    mat4 model = translate(mat4(1.0f), vec3(sliderPos.x, sliderPos.y, 0.0f));
    model = scale(model, vec3(sliderWidth, sliderHeight, 1.0f));
        
    shader.setMat4("model", model);
        
    glBindVertexArray(sliderVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void    Slider::drawStr(Font &font, Shader &textShader)
{
    font.putString(this->str, textShader, vec2(this->pos.x, this->pos.y - height / 8), vec2(width, height));
}
        