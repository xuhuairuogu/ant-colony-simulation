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

#include "PheromoneAnt.h"
#include "Settings.h"
#include "Grid.h"
#include "Pheromone.h"
#include "Home.h"

PheromoneAnt::PheromoneAnt(void)
{
}

PheromoneAnt::PheromoneAnt(uint texture, float angle, Grid* grid, Home* home, FoodManager* food_manager) :
	Ant(texture, angle, grid, home, food_manager)
{

}

PheromoneAnt::~PheromoneAnt(void)
{
}

GridObjResult PheromoneAnt::Update(float delta_time)
{
	GridObjResult result;

	if ((result = Ant::Update(delta_time)) == GridObjResult::Moved)
	{
		TakeFood();
		DepositeFood();
		EatFoodStorage();
		EatHomeFood();

		// Just moved to the new position, leave pheromone trail
		DepositPheromone();
	}

	CalculateNextAction();

	return result;
}

void PheromoneAnt::DepositPheromone()
{
	float foodStorage = this->GetFoodStorage();
	float energy = this->GetEnergy();

	// If ant is carrying food it leaves food trails, otherwise leaves home trail
	if (foodStorage > 0)
	{
		// Leave food pheromobe trail
		m_grid->DepositPheromone(m_position, PheromoneType::food, (GetFoodStorage() / Settings::MAX_FOOD_STORAGE) * Settings::FOOD_TRAIL_DEPOSIT_RATE);
	}
	else
	{
		// Leave home pheromone trail
		m_grid->DepositPheromone(m_position, PheromoneType::home, (GetEnergy() / Settings::MAX_ENERGY) * Settings::HOME_TRAIL_DEPOSIT_RATE);
	}
}

void PheromoneAnt::CalculateNextAction()
{
	if (m_state == GridObjectState::moving) return; // If ant is moving at the moment it can't decide on next movement yet

	if (this->GetFoodStorage() > 0.01f * Settings::MAX_FOOD_STORAGE)
	{
		// GOING HOME TO DEPOSIT FOOD (Following home trail)

		// See if there is home close to us
		if (dis(m_position, m_home->GetPos()) <= Settings::SMELL_RANGE)
		{
			SetTarget(m_home->GetPos());
			return;
		}

		// Follow home trail
		int2 home_trail = this->FollowTrail(PheromoneType::home, m_home->GetPos());
		if (home_trail != int2(0,0))
		{
			SetTarget(m_position + home_trail);
		}
		else MoveRandomly();

		return;
	}
	else if (this->GetEnergy() > Settings::MAX_ENERGY * Settings::HUNGRY_LEVEL)
	{
		// TRYING TO FIND FOOD (Following food trail)

		// See if there is any food close to us
		int2 target = m_position;
		Food* f = m_grid->ClosestFood(m_position, Settings::SMELL_RANGE);
		if (f != 0)
		{	
			SetTarget(f->GetPos());
			return;
		}

		// Follow food trail
		int2 food_trail = FollowTrail(PheromoneType::food, target);
		if (food_trail != int2(0,0))
		{
			SetTarget(m_position + food_trail);
		}
		else MoveRandomly();

		return;
	}
	else if (this->GetEnergy() < Settings::MAX_ENERGY * Settings::HUNGRY_LEVEL)
	{
		// VERY HUNGRY, RETURN HOME (Following home trail)

		// See if there is home close to us
		if (dis(m_position, m_home->GetPos()) <= Settings::SMELL_RANGE)
		{
			SetTarget(m_home->GetPos());
			return;
		}

		// Follow home trail
		int2 home_trail = FollowTrail(PheromoneType::home, m_home->GetPos());
		if (home_trail != int2(0,0))
		{
			SetTarget(m_position + home_trail);
		}
		else MoveRandomly();

		return;
	}

	return;
}

int2 PheromoneAnt::FollowTrail(PheromoneType type, int2 target)
{
	float best_str = 0.01f;		// Best pheromone strength
	int2 best_dir = int2(0,0);	// Best direction to target

	std::vector<int2> to_scan;
	if (this->GetLastPos() == m_position)
	{
		// If have not moved since last turn (when spawned, after reached destination, like home and food) 
		// Put all 8 direction into temporary scan list ...
		for each(int2 near_tile in NearTiles) to_scan.push_back(near_tile);
	}
	else
	{
		// ... otherwise put three directions for potential movement: left, right and forward
		// Find out straight moving direction
		int2 straight = m_position - GetLastPos();
		int straight_index = GetNearTileIndex(straight);
		if (straight_index == -1) throw new exception("Ant straight direction index can't be found");

		// Left direction
		int left_index = straight_index - 1;
		if (left_index == -1) left_index = 7;
		int2 left = NearTiles[left_index];

		// Right direction
		int right_index = straight_index + 1;
		if (right_index == 8) right_index = 0;
		int2 right = NearTiles[right_index];

		// Make a nice list to iterate thought it
		to_scan.push_back(straight);
		to_scan.push_back(left);
		to_scan.push_back(right);
	}

	for each(int2 near_tile in to_scan)
	{
		if (!m_grid->IfPositionValid(m_position + near_tile)) continue;	// Position Invalid
		if (m_grid->IfObstacle(m_position + near_tile)) continue;		// Position is obstacle
		if ((m_position + near_tile) == GetLastPos()) continue;			// Position is where ant comed from (deprecated)

		// Find attractiveness coefficient according to ant movement direction
		// It should be from 0 (not attractive), to 1 (attractive)
		// It easy to find by calculating angle between possible move and target
		// Just need to normalize, as angle is from 0 to 3.14 (pi)
		float pos_d;
		if (target == m_position) pos_d = 1.0f;
		else
		{
			float2 dirToGo = int2float2(target) - int2float2(m_position);
			float a = angle(int2float2(near_tile), dirToGo);
			pos_d = 1 - (a / PI_F);
		}

		// Find attractiveness coefficient using pheromone intensity
		float phem_d = m_grid->GetPheromoneStrength(m_position + near_tile, type);

		// Find overall attractiveness for the grid position
		// Have to use different Alpha Beta parameters for different pheromone types
		float a, b;
		if (type == food) { a = Settings::FOOD_ALPHA; b = Settings::FOOD_BETA; }
		if (type == home) { a = Settings::HOME_ALPHA; b = Settings::HOME_BETA; }

		// Formula for overall attractiveness
		// Alpaha is pheromone intensity, and Beta is direction factor
		// Should have really wrote it somewhere else than source code
		float trail_str = (phem_d * a) * (pos_d * b);				 

		// Compare, is it better than another directions ?
		if (trail_str > best_str)
		{
			// Add random factor (decision factor) to movements
			// For example, if ant have choses of two paths of relatively equals pheromone trail strenght
			// it could follow lesser strenght trail, thus "trying" another path.
			float a = trail_str;
			float b = best_str * Settings::RANDOM_FACTOR;
			float r = rnd(a + b);

			if (r < trail_str)
			{
				best_str = trail_str;
				best_dir = near_tile;
			}
		}
	}

	return best_dir;
}