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
#include "Pheromone.h"

class PheromoneManager
{
public:
	PheromoneManager(void);
	PheromoneManager(uint texture, Grid* grid);
	virtual ~PheromoneManager(void);

	virtual void Update(float delta_time);
	virtual void Draw();

	// Creates and adds pheromone to manager
	Pheromone* AddPheromone(float strength, PheromoneType type);
	// Creates ands pheromone to manager and grid
	Pheromone* AddPheromone(float strength, PheromoneType type, const int2& pos);
private:
	Pheromone* CreatePheromone(float strength, PheromoneType type);

private:
	uint m_texture;
	Grid* m_grid;
	std::vector<Pheromone*> m_pheromones;

};

