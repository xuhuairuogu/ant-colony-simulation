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

#pragma once

#include "Food.h"
class Grid;

class FoodManager
{
public:
	FoodManager(void);
	FoodManager(Grid* grid, uint food_texture);
	virtual ~FoodManager(void);

	void Draw();
	void Update(float delta_time);
	bool AddFood(int2 pos, int food_storage);
	const Food* NearestFood(int2 pos, uint range);
	std::vector<Food*> FoodInRange(int2 pos, uint range);

	Food* operator[](uint i) { return m_food[i];}
	const Food* operator[](uint i) const { return m_food[i];}
	// Put big obstacle array on grid
	bool PutFoodArray(const int2& pos);

private:
	bool ValidateFood(Food* food);

private:
	Grid* m_grid;
	uint m_food_texture;
	std::vector<Food*> m_food;

	// Put big obstacle array on grid
	bool PutFoodRecursive(const int2& pos, uint step, uint maxStep);
};

