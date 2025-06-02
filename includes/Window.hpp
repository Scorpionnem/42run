/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 12:11:18 by mbatty            #+#    #+#             */
/*   Updated: 2025/05/22 17:09:14 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_HPP
# define WINDOW_HPP

# include "libs.hpp"
# include "Shader.hpp"
# include "Font.hpp"

class	Window
{
	public:
		Window();
		~Window();
		bool	up(void);
		GLFWwindow	*getWindowData(void);
		float		getDeltaTime(void);
		void		loopStart(void);
		void		loopEnd(Font &font, Shader &textShader);
		void		setIcon(const char *path);
		void		center();
		float		_deltaTime;
		float		_lastFrame;
		float		_currentFrame;
	private:
		GLFWwindow	*_windowData;
		std::string displayFPS(Font &font, Shader &textShader);
};

#endif