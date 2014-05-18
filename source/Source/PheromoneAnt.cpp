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
	// Ooops, some memory leaked
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

		// Moved to the new position, leave pheromone trail
		DepositPheromone();
	}
	else
	{
		TakeFood();
		DepositeFood();
		EatFoodStorage();
		EatHomeFood();
	}

	CalculateNextAction();

	return result;
}

void PheromoneAnt::DepositPheromone()
{
	float foodStorage = GetFoodStorage();
	float energy = GetEnergy();

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

int2 PheromoneAnt::SmellPheromone(PheromoneType type)
{
	uint range = Settings::SMELL_RANGE;
	int2 startPos = m_position - int2(range, range);
	int2 endPos = m_position + int2(range, range);
	float2 dir = float2(0, 0);
	bool smell_found = false;

	for (int x = startPos.x; x < endPos.x; ++x)
	{
		for (int y = startPos.y; y < endPos.y; ++y)
		{
			int2 current = int2(x, y);
			// Check if position is valid
			if (!m_grid->IfPositionValid(current)) continue;

			// Check if within the smell range
			// maybe...

			// Add new smell directory to average smell dir
			// TODO. Adds smell fadeout according to distance
			float strength = m_grid->GetPheromoneStrength(current, type);
			if (strength > 0.01f)
			{
				dir += int2float2(current - m_position) * strength;
				smell_found = true;
			}
		}
	}

	if (!smell_found) return int2(0, 0);

	float bestAngle = PI_F;
	int2 bestDir = int2(0,0);
	for each(int2 near_tile in NearTiles)
	{
		float a = angle(int2float2(near_tile), dir) < bestAngle;
		if (a < bestAngle && m_grid->IfPositionValid(m_position + near_tile) && (m_position + near_tile) != GetLastPos())
		{
			bestAngle = a;
			bestDir = near_tile;
		}
	}

	return bestDir;
}

void PheromoneAnt::CalculateNextAction()
{
	if (m_state == GridObjectState::moving) return;

	if (GetFoodStorage() > 0.01f * Settings::MAX_FOOD_STORAGE)
	{
		// GOING HOME TO DEPOSIT FOOD (Following home trail)

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
	else if (GetEnergy() > Settings::MAX_ENERGY * Settings::HUNGRY_LEVEL)
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
	else if (GetEnergy() < Settings::MAX_ENERGY * Settings::HUNGRY_LEVEL)
	{
		// VERY HUNGRY, RETURN HOME

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
	float best_str = 0.01f;
	int2 best_dir = int2(0,0); 

	std::vector<int2> to_scan;
	if (GetLastPos() == m_position)
	{
		for each(int2 near_tile in NearTiles) to_scan.push_back(near_tile);
	}
	else
	{
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
		if (!m_grid->IfPositionValid(m_position + near_tile)) continue;
		if (m_grid->IfObstacle(m_position + near_tile)) continue;
		if ((m_position + near_tile) == GetLastPos()) continue;

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

		// Find attractiveness coefficient according to pheromone intensity in all directions
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

		// Compare is it better than another directions
		if (trail_str > best_str)
		{
			// Add random factor to movements
			// For example, if ant have choise of two path of relatively equals pheromone trail strenght
			// it could follow lesser strenght trail, thouse "trying" another path.
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