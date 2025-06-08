/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MeshManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 21:02:44 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/08 23:13:42 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESHMANAGER_HPP
# define MESHMANAGER_HPP

#include "libs.hpp"
#include "Mesh.hpp"

class	MeshManager
{
	public:
		MeshManager(){}
		~MeshManager()
		{
			for (auto it = loadedMeshes.begin(); it != loadedMeshes.end(); it++)
				delete it->second;	
		}

		Mesh	*load(const std::string &path)
		{
			return (loadedMeshes.insert({path + MISSING_TEXTURE, new Mesh(path, MISSING_TEXTURE)}).first->second);
		}
		Mesh	*load(const std::string &path, const std::string &texPath)
		{
			return (loadedMeshes.insert({path + texPath, new Mesh(path, texPath)}).first->second);
		}
		Mesh	*get(const std::string &path)
		{
			std::map<std::string, Mesh*>::iterator	finder;
			finder = loadedMeshes.find(path + MISSING_TEXTURE);
			if (finder == loadedMeshes.end())
				return (load(path));
			return (finder->second);
		}
		Mesh	*get(const std::string &path, const std::string &texPath)
		{
			std::map<std::string, Mesh*>::iterator	finder;
			finder = loadedMeshes.find(path + texPath);
			if (finder == loadedMeshes.end())
				return (load(path, texPath));
			return (finder->second);
		}

		std::map<std::string, Mesh*>	loadedMeshes;
};

extern MeshManager	*MESH_MANAGER;

#endif
