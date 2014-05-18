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
#include "gridobject2d.h"
#include "ManagedProperty.h"
class Home;
class Food;
class FoodManager;
class GridCell;

class Ant :
	public GridObject2D
{
	//Methods
public:
	Ant(void);
	Ant(uint texture, float angle, Grid* grid, Home* home, FoodManager* food_manager);
	virtual ~Ant(void);

	virtual GridObjResult Update(float delta_time);
	float GetEnergy() { return m_energy.Value(); };
	float GetFoodStorage() { return m_foodStorage.Value(); };

	void SetTarget(int2 new_target);
	GridObjResult MoveRandomly();
	int GetNearTileIndex(int2 near_tile);

protected:
	bool TakeFood();
	bool DepositeFood();
	bool EatFoodStorage();
	bool EatHomeFood();
	Home* IsAtHome();
	Food* IsAtFood();
	bool ReachedTarget() { return m_position == m_movingTarget; };

private:
	int2 BFS_start(const int2& start, const int2& target);
	float BFS_next(GridCell* next, const int2& target);
	bool IfScanned(GridCell* next);
	int2 gotoTarget(int2 target);

	//Properties
private:
	ManagedProperty<float> m_energy;
	ManagedProperty<float> m_foodStorage;

	int2 m_movingTarget;
	int2 m_lastRandomDir;

	std::vector<GridCell*> m_scanned;
	std::vector<GridCell*> m_currentQueue;
	std::vector<GridCell*> m_nextQueue;

protected:
	Home* m_home;
	FoodManager* m_foodManager;
	
};

static int2 NearTiles[8] = { int2(-1,-1), int2(0,-1), int2(1,-1), int2(1,0), int2(1,1), int2(0,1), int2(-1,1), int2(-1,0) };
