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

#include "FoodManager.h"
#include "Grid.h"
#include "Settings.h"

#pragma region Gang of three

FoodManager::FoodManager(void) :
	m_grid(0),
	m_food_texture(0),
	m_food()
{
}

FoodManager::FoodManager(Grid* grid, uint food_texture) :
	m_grid(grid),
	m_food_texture(food_texture),
	m_food()
{
}

FoodManager::~FoodManager(void)
{
}

#pragma endregion

bool FoodManager::AddFood(int2 pos, int food_storage)
{
	Food* food_add = new Food(m_food_texture, food_storage, m_grid);

	if (!m_grid->AddGridObject(food_add, pos)) return false;	// Object failed to be added to grid

	m_food.push_back(food_add);
	return true;
}

// Returns nearest to given position food if it is in range
const Food* FoodManager::NearestFood(int2 pos, uint range)
{
	Food* nearest_food = 0;
	float smallest_dis = FLT_MAX;

	std::vector<Food*>::iterator current = m_food.begin();
	while (current != m_food.end())
	{
		float d = dis(int2float2(pos), int2float2((*current)->GetPos()));
		if (d < smallest_dis && d <= range)
		{
			nearest_food = (*current);
			smallest_dis = d;
		}
		++current;
	}

	return nearest_food;
}

// Update all food
void FoodManager::Update(float delta_time)
{
	uint i = 0;
	while (i < m_food.size())
	{
		Food* current = m_food[i];
		current->Update(delta_time);
		if (current->Empty()) 
		{
			m_food.erase(m_food.begin() + i);
			m_grid->DeleteGridObject(current);
		}
		else ++i;
	}
}

// Draw all food
void FoodManager::Draw()
{
	for each(Food* food in m_food)
	{
		food->Draw();
	}
}

// Checks if given food exists inside food manager
bool FoodManager::ValidateFood(Food* food)
{
	for each(Food* f in m_food) if(f == food) return true;
	return false;	// What the hell
}

// Returns list of all food in range
std::vector<Food*> FoodManager::FoodInRange(int2 pos, uint range)
{
	std::vector<Food*> result;

	std::vector<Food*>::iterator current = m_food.begin();
	while (current != m_food.end())
	{
		float d = dis(int2float2(pos), int2float2((*current)->GetPos()));
		if (d <= range)
		{
			result.push_back((*current));
		}
		++current;
	}

	return result;
}


bool FoodManager::PutFoodArray(const int2& pos)
{
	return PutFoodRecursive(pos, 0, rnd(Settings::MIN_FOOD_ARRAY, Settings::MAX_FOOD_ARRAY));
}

bool FoodManager::PutFoodRecursive(const int2& pos, uint step, uint maxStep)
{
	if (!m_grid->IfPositionValid(pos)) return false;
	Food* o = new Food(m_food_texture, rnd(Settings::MIN_FOOD, Settings::MAX_FOOD), m_grid);
	if (!m_grid->AddGridObject(o, pos)) delete o;
	else m_food.push_back(o);
	if (step >= maxStep) return true; else ++step;

	int2 rndDir;
	while ((rndDir = int2(rnd(-1, 1), rnd(-1, 1))) == int2(0, 0));
	return PutFoodRecursive(pos + rndDir, step, maxStep);
}

