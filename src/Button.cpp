/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Button.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 12:21:13 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/03 11:36:55 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Button.hpp"

GLuint buttonVAO = 0, buttonVBO = 0;

bool isInside(vec2 buttonPos, vec2 mousePos, float width, float height)
{
    return mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + width && mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + height;
}

Button::~Button()
{
    if (buttonVAO != 0)
    {
        if (DEBUG)
            std::cout << "Destroying button quad" << std::endl;
        glDeleteBuffers(1, &buttonVBO);
        glDeleteVertexArrays(1, &buttonVAO);
        buttonVBO = 0;
        buttonVAO = 0;
    }
}

Button::Button(std::string str, float width, float height, vec2 pos, std::function<void()> function, Texture &texture, Texture &pressedTexture)
: texture(texture), pressedTexture(pressedTexture)
{
    this->pos = pos;
    this->width = width;
    this->height = height;
    onClick = function;
    this->str = str;
}

void Button::draw(Shader &shader, Font &font, Shader &textShader)
{
    initButtonModel();
    shader.use();
    
    mat4 model = translate(mat4(1.0f), vec3(pos.x, pos.y, 0.0f));
    model = scale(model, vec3(width, height, 1.0f));
    
    shader.setMat4("model", model);
    
    glBindVertexArray(buttonVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    font.putString(this->str, textShader, vec2(this->pos.x, this->pos.y - height / 8), vec2(width, height));
}

void    Button::checkClick(vec2 mousePos, bool mousePressed)
{
    bool inside = isInside(this->pos, mousePos, this->width, this->height);
    
    if (mousePressed)
    {
    	wasPressedInside = inside;
    	if (inside)
    		this->pressedTexture.use();
    }
    else
    {
    	if (wasPressedInside && inside)
    		this->onClick();
    	wasPressedInside = false;
    }
}

void Button::initButtonModel()
{
    if (buttonVAO != 0) return;
    
    if (DEBUG)
        std::cout << "Loading button quad" << std::endl;

    float vertices[] = {
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
		0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
    };

    glGenVertexArrays(1, &buttonVAO);
    glGenBuffers(1, &buttonVBO);

    glBindVertexArray(buttonVAO);
    glBindBuffer(GL_ARRAY_BUFFER, buttonVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}
