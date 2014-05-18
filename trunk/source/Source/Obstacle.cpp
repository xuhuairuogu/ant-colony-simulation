/* ----------------------------------------------------------------

	This file is part of modified version of Ant Colony Simulation 
	developed by Aleksejs Belezjaks for University of Hull 08341 Final Project 13/14 module.
	
	Copyright (C) 2014 Aleksejs Belezjaks http://alexbelezjaks.com
	
	Ant Colony Simulation is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ant Colony Simulation is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
	--------------------------------------------------------------- */

#include "Obstacle.h"

#pragma region Gang of three

Obstacle::Obstacle(void) : 
	GridObject2D()
{
}

Obstacle::Obstacle(uint texture, Grid* grid) :
	GridObject2D(int2(0,0), 0.0f, grid)
{
	m_sprite = new Sprite(texture, float2(0,0), 0.5f);
}

Obstacle::~Obstacle(void)
{
}

#pragma endregion