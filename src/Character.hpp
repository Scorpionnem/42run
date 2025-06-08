/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Character.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 22:59:17 by mbatty            #+#    #+#             */
/*   Updated: 2025/06/08 23:05:38 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHARACTER_HPP
# define CHARACTER_HPP

# include "libs.hpp"
# include "Mesh.hpp"

enum CharactersID
{
	SCORPIONEM,
	ELEO,
	THEWORFER27,
	MAX_CHARACTER
};

class	Character
{
	public:
		Character(CharactersID ID, Mesh *mesh, Mesh *sneakMesh)
		{
			this->ID = ID;
			this->mesh = mesh;
			this->sneakMesh = sneakMesh;
		}
		~Character(){}
		Mesh			*mesh;
		Mesh			*sneakMesh;
		CharactersID	ID;

		bool			owned = false;
		bool			equipped = false;
};

#endif
