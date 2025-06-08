/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:14:48 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/08 12:44:41 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_HPP
# define GAME_HPP

void	resumeGame();

void	closeWindow();
# include "MeshManager.hpp"
# include "ShaderManager.hpp"
# include "TextureManager.hpp"

extern MeshManager	*MESH_MANAGER;
extern ShaderManager	*SHADER_MANAGER;
extern TextureManager	*TEXTURE_MANAGER;

# include "libs.hpp"
# include "Window.hpp"
# include "Skybox.hpp"
# include "Mesh.hpp"
# include "Player.hpp"

# include "World.hpp"

# define SKYBOX_PATHES "textures/skybox/right.bmp","textures/skybox/left.bmp","textures/skybox/top.bmp","textures/skybox/bottom.bmp","textures/skybox/front.bmp","textures/skybox/back.bmp"

inline void	do_nothing(){}

class	Game
{
	public:
		Game();
		~Game();
		void	logic();
		void	loadUIs();
		void	draw3D();
		void	drawUI();
		void	displayFPS();
		void	setShaderValues();
		void	keyHook();

		void	pause();
		void	resume();

		Window	window;
		Camera	camera;

		std::string	fps = "0 fps";

		TextureManager	textures;
		ShaderManager	shaders;
		MeshManager		meshManager;
        World           world;
        Player          player;

		Skybox	skybox;
		Font	font;

		bool	paused = true;
		bool	started = false;
		bool	died = false;
		double	diedTime = 0;
		double	pauseTime = 0;
		float	distance = 0;
		double	totalDistance = 0;
		unsigned int		collectibles = 0;
		unsigned int		bestCollecCount = 0;
		unsigned int		totalCollectibles = 0;
		int		bestDistance = 0;
		bool	hasPowerup = false;
		double	powerupTime = 0;
};

extern Game	*GAME;

inline  void    Game::resume()
{
	paused = false;
}

inline void	Game::pause()
{
	paused = true;
}

#endif
