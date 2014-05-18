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

#include "PheromoneManager.h"
#include "Grid.h"
#include "Settings.h"

PheromoneManager::PheromoneManager(void) :
	m_texture(0),
	m_grid(0),
	m_pheromones(0)
{
}

PheromoneManager::PheromoneManager(uint texture, Grid* grid) :
	m_texture(texture),
	m_grid(grid),
	m_pheromones(0)
{
}

PheromoneManager::~PheromoneManager(void)
{
}

void PheromoneManager::Update(float delta_time)
{
	for each(Pheromone* p in m_pheromones)
	{
		p->Update(delta_time);
	}
}

void PheromoneManager::Draw()
{
	for each(Pheromone* p in m_pheromones)
	{
		if (p->GetType() == food && Settings::RENDER_FOOD_TRAIL) p->Draw();
		if (p->GetType() == home && Settings::RENDER_HOME_TRAIL) p->Draw();
	}
}

Pheromone* PheromoneManager::AddPheromone(float strength, PheromoneType type)
{
	Pheromone* new_pheromone = CreatePheromone(strength, type);
	m_pheromones.push_back(new_pheromone);
	return new_pheromone;
}

Pheromone* PheromoneManager::AddPheromone(float strength, PheromoneType type, const int2& pos)
{
	Pheromone* new_pheromone = CreatePheromone(strength, type);
	m_pheromones.push_back(new_pheromone);
	m_grid->AddGridObject(new_pheromone, pos);
	return new_pheromone;
}

Pheromone* PheromoneManager::CreatePheromone(float strength, PheromoneType type)
{
	return new Pheromone(m_texture, type, strength, m_grid);
}