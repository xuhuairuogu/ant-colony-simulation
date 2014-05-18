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

#include "BasicMath.h"
#include "GridObject2D.h"
#include "GridCell.h"
#include "PheromoneManager.h"
#include "Food.h"

static const int NUM_COLUMNS_X = 100;
static const int NUM_ROWS_Y = 100;

class Grid
{
public:
	Grid(void);
	virtual ~Grid(void);

	virtual void Update(float delta);
	virtual void Draw();

	// This method would set grid line rendering to true the value you sent
	void GridLineRenderingEnabled(bool new_value);
	// Returns pointer to grid object at selected location
	GridCell* GetGridCell(const int2& pos) const;

	// Removes grid object from grid, but leaves it in memory
	bool RemoveGridObject(GridObject2D* remove_me);
	// Removes grid object from grid and deletes it's memory
	bool DeleteGridObject(GridObject2D* delete_me);
	// Moves grid object from one grid cell to another
	bool MoveGridObject(GridObject2D* move_me, const int2& new_pos);

	// Adds object to the grid. Returns true if object have been added.
	bool AddGridObject(GridObject2D* object_add, const int2& pos);
	// Validates grid pos. Returns true if grid position is valied.
	bool IfGridCellEmpty(const int2& pos) const;
	// Check if given grid position is valid
	bool IfPositionValid(const int2& pos) const;
	// Checks if given position is obstacle
	bool IfObstacle(const int2& pos);
	// Find closest food in range
	Food* ClosestFood(const int2& pos, uint range);

	// Init pheromone manager
	void InitPheromones(uint texture);
	// Update Pheromones
	void UpdatePheromones(float delta_time);
	// Get pheromone strength at given location
	float GetPheromoneStrength(const int2& pos, PheromoneType type);
	// Deposits pheromone at given location. Return new pheromone strength
	float DepositPheromone(const int2& pos, PheromoneType type, float add_pheromone);

private: // Members
	GridCell* m_grid2D[NUM_COLUMNS_X][NUM_ROWS_Y];
	int2 m_size;
	PheromoneManager* m_pheromones;

	// Grid Rendering
	DynamicArray<LineVertex2D>* m_grid_lines;
	bool m_render_grid_lines;

private: // Methods
	Grid(const Grid& copy_grid);
	Grid& operator= (const Grid& copy_grid);
	
};

