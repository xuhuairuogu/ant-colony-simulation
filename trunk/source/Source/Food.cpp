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

#include "Food.h"

#pragma region Gang of three

Food::Food(void) :
	GridObject2D(),
	ManagedProperty(0, FLT_MAX)
{
	//m_sprite = new Sprite(FoodTexture, float2(m_position.x + 0.5f, m_position.y + 0.5f), 1.0f);
}

Food::Food(uint texture, int food_storage, Grid* grid) :
	GridObject2D(int2(), 0.0f, grid),
	ManagedProperty(food_storage, food_storage)
{
	m_sprite = new Sprite(texture, int2float2(m_position) + float2(0.5f, 0.5f), 0.5f);
}

Food::~Food(void)
{

}

#pragma endregion
#pragma region Public methods

GridObjResult Food::Update(float delta_time)
{
	return GridObject2D::Update(delta_time);
}

#pragma endregion
