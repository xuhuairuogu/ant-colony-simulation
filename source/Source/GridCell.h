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

#include "GridObject2D.h"
#include <vector>
class Pheromone;
class Food;
enum PheromoneType;

class GridCell
{
public:
	GridCell(void);
	GridCell(const int2& pos);
	virtual ~GridCell(void);

	void Update(float delta_time);
	void Draw(void);

private:
	GridCell(const GridCell& copy_me);
	GridCell& operator=(const GridCell& copy_me);
	std::vector<GridObject2D*>::iterator FindObject(GridObject2D* find_me);

private:
	std::vector<GridObject2D*> m_grid_objects;
	int2 m_pos;

public:
	// Getters
	uint Count(void) const { return m_grid_objects.size(); };
	GridObject2D* operator[](uint i) { return m_grid_objects[i];}
	const GridObject2D* operator[](uint i) const { return m_grid_objects[i];}
	const int2& GetPos() const { return m_pos; }; 
	int FindIndex(GridObject2D* find_me) const;

	// Methods
	bool Remove(GridObject2D* remove_me);
	bool Delete(GridObject2D* delete_me);
	bool Add(GridObject2D* add_me);
	bool Clear();
	bool DeleteAll();
	bool IfObstacle();
	Pheromone* GetPheromone(PheromoneType type);
	Food* GetFood();

};

