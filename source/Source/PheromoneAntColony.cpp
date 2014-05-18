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

#include "PheromoneAntColony.h"


PheromoneAntColony::PheromoneAntColony(void)
{
}

PheromoneAntColony::PheromoneAntColony(uint BugTexture, Grid* grid, FoodManager* food_manager) :
	AntColony(BugTexture, grid, food_manager)
{
}

PheromoneAntColony::~PheromoneAntColony(void)
{
}

void PheromoneAntColony::Update(float delta_time)
{
	AntColony::Update(delta_time);
}

void PheromoneAntColony::AddAntsNearHome(uint count)
{
	for (uint i = 0; i < count; ++i) 
	{
		AddPheromoneAnt(m_home->GetPos());
	}
}

PheromoneAnt* PheromoneAntColony::AddPheromoneAnt(int2 pos)
{
	PheromoneAnt* new_ant = new PheromoneAnt(m_bug_texture, 0.0f, m_grid, m_home, m_food_manager);
	//new_ant->SetTarget(pos + int2(rnd(-3, 3),rnd(-3, 3)));
	if (AddAnt(new_ant, pos)) 
	{
		new_ant->MoveRandomly();
		return new_ant;
	}
	else
	{
		delete new_ant;
		return 0;
	}
}