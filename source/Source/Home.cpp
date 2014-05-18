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

#include "Home.h"

#pragma region Gang of three

Home::Home(void) :
	GridObject2D(),
	m_food_storage(0.0f, FLT_MAX)
{
}

Home::~Home(void)
{
}

Home::Home(uint home_texture, int2 position, int start_food, Grid* grid) :
	GridObject2D(position, 0.0f, grid),
	m_food_storage(start_food, FLT_MAX)
{
	m_sprite = new Sprite(home_texture, int2float2(m_position) + float2(0.5f, 0.5f), 0.5f);
}

#pragma endregion
