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

#include "Ant.h"
#include "Grid.h"
#include "Home.h"
#include "Settings.h"
#include <iostream>

#pragma region Gang of Three

Ant::Ant(void) :
	GridObject2D(),
	m_energy(Settings::MAX_ENERGY, Settings::MAX_ENERGY),
	m_foodStorage(0, Settings::MAX_FOOD_STORAGE),
	m_movingTarget(m_position),
	m_lastRandomDir(m_position),
	m_scanned(),
	m_currentQueue(),
	m_nextQueue(),
	m_home(0),
	m_foodManager(0)
{
}

Ant::Ant(uint texture, float angle, Grid* grid, Home* home, FoodManager* food_manager) :
	GridObject2D(int2(), angle, grid),
	m_energy(Settings::MAX_ENERGY, Settings::MAX_ENERGY),
	m_foodStorage(0, Settings::MAX_FOOD_STORAGE),
	m_movingTarget(m_position),
	m_lastRandomDir(m_position),
	m_scanned(),
	m_currentQueue(),
	m_nextQueue(),
	m_home(home),
	m_foodManager(food_manager)
{
	m_sprite = new Sprite(texture, float2(m_position.x + 0.5f, m_position.y + 0.5f), 0.5f, 2);
	m_sprite->AddQuadSheet(0, 3, 0.033f);
}

Ant::~Ant(void)
{
}

#pragma endregion

GridObjResult Ant::Update(float delta_time)
{	
	// Consume ant energy
	m_energy.Take(Settings::ENERGY_CONSUMPTION * delta_time);

	if (m_energy.Value() <= 0.0f)
	{
		// This ant is dead because of starvation
		m_grid->RemoveGridObject(this);
		return GridObjResult::Deleted;
	}

	GridObjResult result = GridObject2D::Update(delta_time);

	if (m_state == idle && m_position != m_movingTarget) Move(m_position + gotoTarget(m_movingTarget));

	return result;
}

// If ant stands on one gridcell with home, it would eat its food
bool Ant::EatHomeFood()
{
	Home* home = IsAtHome();
	if (home == 0) return false;

	m_energy << home->m_food_storage;
#ifdef _DEBUG
	std::cout << "Ant: " << m_foodStorage.Value() << " Home: " << home->m_food_storage.Value() << endl;
#endif
	return true;
}

// If ant stands on one gridcell with food source, it would take as much food as possible
bool Ant::TakeFood()
{
	Food* food = IsAtFood();
	if (food == 0) return false;

	m_foodStorage << *food;
	ResetLastPos();
#ifdef _DEBUG
	std::cout << "Ant: " << m_foodStorage.Value() << " Food: " << food->Value() << endl;
#endif
	return true;
}

// if ant stands on one gridcell with home, it would deposit all food to home
bool Ant::DepositeFood()
{
	Home* home = IsAtHome();
	if (home == 0) return false;

	home->m_food_storage << m_foodStorage;
	ResetLastPos();
#ifdef _DEBUG
	std::cout << "Ant: " << m_foodStorage.Value() << " Home: " << home->m_food_storage.Value() << endl;
#endif
	return true;
}

// Check if ant stand on the same gridcell with home and returns pointer to this home
Home* Ant::IsAtHome()
{
	GridCell* cell = m_grid->GetGridCell(m_position);
	if (cell->Count() <= 0) return 0;

	for (uint i = 0; i < cell->Count(); ++i)
	{
		Home* home = dynamic_cast<Home*>((*cell)[i]);
		if (home != 0) return home;
	}

	return 0;
}

// Check if ant stand on the same gridcell with food and returns pointer to this food
Food* Ant::IsAtFood()
{
	GridCell* cell = m_grid->GetGridCell(m_position);
	if (cell->Count() <= 0) return 0;

	for (uint i = 0; i < cell->Count(); ++i)
	{
		Food* food = dynamic_cast<Food*>((*cell)[i]);
		if (food != 0) return food;
	}

	return 0;
}

// Consume food from own food storage, returns true if consumed any;
bool Ant::EatFoodStorage()
{
	if (m_foodStorage.Value() > 0.1f)
	{
		m_energy << m_foodStorage;
		return true;
	}
	else
	{
		return false;
	}
}

// Return which direction it should go to reach its target
int2 Ant::gotoTarget(int2 target)
{
	float2 dirToGo = int2float2(target) - int2float2(m_position);
	int2 bestDir;
	float bestAngle = PI_F;

	// We need to calculate distance for all directions
	for (int i = 0; i < sizeof(NearTiles) / sizeof(int2); ++i)
	{
		int2 current = NearTiles[i];
		float a = angle(int2float2(current), dirToGo);

		bool obs = m_grid->IfObstacle(m_position + current);
		if (a > bestAngle || obs) continue;

		bestAngle = a;
		bestDir = current;
	}

	if (bestAngle >= PI_F * 0.5f)
	{
		// Stuck, have to find exit
		m_scanned.clear();
		m_currentQueue.clear();
		m_nextQueue.clear();

		int2 new_target = BFS_start(m_position, target);
		if (new_target == int2(-1, -1)) return bestDir;
		SetTarget(new_target);

		return gotoTarget(m_movingTarget);
	}

	return bestDir;
}

// Sets moving target. Ant would continue to move toward the target until in reaches it
void Ant::SetTarget(int2 new_target)
{
	if (m_grid->IfObstacle(new_target)) 
	{
		m_movingTarget = m_position;
		//m_antState = followPath;
	}
	else
	{
		m_movingTarget = new_target;
		//m_antState = followPath;
	}
}

// Move randomly
GridObjResult Ant::MoveRandomly()
{
	if (m_state == moving) return GridObjResult::Default;

	GridObjResult result = Default;
	do
	{
		if (rnd(0, 100) < 50) 
		{
			//while ((m_lastRandomDir = int2(rnd(-1, 1), rnd(-1, 1))) == int2(0, 0))
			int straight = GetNearTileIndex(m_lastRandomDir);

			if (rnd(0, 100) > 50)	
			{ // Move left
				int left = straight - 1;
				if (left < 0) left = 7;
				m_lastRandomDir = NearTiles[left];
				break;
			}
			else 
			{ // Move right
				int right = straight + 1;
				if (right > 7) right = 0;
				m_lastRandomDir = NearTiles[right];
				break;
			}

			if (m_lastRandomDir == int2(0,0))
			{
				m_lastRandomDir = NearTiles[rnd(0, 7)];
			}
		}

		result = Move(m_position + m_lastRandomDir);
	}
	while (result != StartedMoving);

	SetTarget(m_position + m_lastRandomDir);
	return result;
}

// Finds and returns position index inside NearTiles array
int Ant::GetNearTileIndex(int2 near_tile)
{
	for (uint i = 0; i < 8; ++i)
	{
		if (NearTiles[i] == near_tile)
		{
			return i;
		}
	}

	return -1;
}

// Hill climbing/breadth first search - for the case if ant is stuck and can't use simple "Best first" algorithm
// Scans next queue to find goal
float Ant::BFS_next(GridCell* next, const int2& target)
{
	m_scanned.push_back(next);

	for (int i = 0; i < 8; ++i)
	{
		int2 scan_pos = next->GetPos() + NearTiles[i];
		if (!m_grid->IfPositionValid(scan_pos)) continue;	// Position not valid
		
		GridCell* scan_grid = m_grid->GetGridCell(scan_pos);
		if (scan_grid->IfObstacle()) continue;
		if (!IfScanned(scan_grid)) m_nextQueue.push_back(scan_grid);
	}

	return dis(int2float2(next->GetPos()), int2float2(target));
}

// Main hill climbing algorithm method
int2 Ant::BFS_start(const int2& start, const int2& target)
{
	m_currentQueue.push_back(m_grid->GetGridCell(start));
	float start_dis = dis(int2float2(start), int2float2(target));

	while (true)
	{
		for each(GridCell* cell in m_currentQueue)
		{
			if (BFS_next(cell, target) < start_dis) return cell->GetPos();
		}

		if (m_nextQueue.size() == 0) return int2(-1, -1);

		m_currentQueue.clear();
		m_currentQueue = m_nextQueue;
		m_nextQueue.clear();
	}

	return int2(-1, -1);
}

// Check if specific cell have been scanned
bool Ant::IfScanned(GridCell* next)
{
	// Check if not inside scanned list, current queue or next queue
	// Brute force, extremely inefficient

	for each(GridCell* cell in m_scanned)
	{
		if (cell == next) return true;
	}
	
	for each(GridCell* cell in m_currentQueue)
	{
		if (cell == next) return true;
	}

	for each(GridCell* cell in m_nextQueue)
	{
		if (cell == next) return true;
	}

	return false;
}
