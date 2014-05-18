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

#include "VectorAnt.h"
#include "Grid.h"
#include "Home.h"
#include "FoodManager.h"
#include "Settings.h"

#pragma region Gang of three

VectorAnt::VectorAnt(void) :
	Ant()
{
}

VectorAnt::VectorAnt(uint texture, float angle, Grid* grid, Home* home, FoodManager* food_manager) :
	Ant(texture, angle, grid, home, food_manager)
{
}

VectorAnt::~VectorAnt(void)
{
}

#pragma endregion

GridObjResult VectorAnt::Update(float delta_time)
{	
	GridObjResult result = Ant::Update(delta_time);

	if (ReachedTarget())	// Aproached destination
	{
		// Decide what to do on destination point
		TakeFood();
		DepositeFood();
		EatHomeFood();

		// Than decide on next action
		calculateNextAction();
	}

	checkIfHungry();

	return result;
}

void VectorAnt::calculateNextAction()
{
	// This is going to be advanced optimal path calculation
	// First have to look around and see (smell) if there is any food around
	std::vector<Food*> food_around = m_foodManager->FoodInRange(m_position, Settings::SMELL_RANGE);

	// If food storage is full, then go home
	if (GetFoodStorage() > Settings::MAX_FOOD_STORAGE * 0.8f) 
	{
		SetTarget(m_home->GetPos());
		return;
	}

	// If ant is low energy, then it must run to home straight away
	checkIfHungry();

	// For testing it will just go for nearest
	const Food* nearest_food = m_foodManager->NearestFood(m_position, Settings::SMELL_RANGE);
	// If any food has been found, go to it
	if (nearest_food != 0) {
		SetTarget(nearest_food->GetPos());
		return;
	}

	// Nothing else to do, choose random location to go
	MoveRandomly();
}

void VectorAnt::checkIfHungry()
{
	// If ant is starving then run to home to restore energy
	// First find distance between ant and home
	int2 d = m_home->GetPos() - m_position;
	int d2 = abs(d.x) + abs(d.y);
	if (GetEnergy() <= d2 * Settings::ENERGY_CONSUMPTION)
	{
		if (!EatFoodStorage())
		{
			SetTarget(m_home->GetPos());
			return;
		}
	}
}
