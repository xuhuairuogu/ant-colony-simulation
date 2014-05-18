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

#include "GridCell.h"
#include "Obstacle.h"
#include "Pheromone.h"
#include "Food.h"

#pragma region Gang of three

GridCell::GridCell(void) :
	m_grid_objects(),
	m_pos(-1, -1)
{
}

GridCell::GridCell(const int2& pos) :
	m_grid_objects(),
	m_pos(pos)
{
}

GridCell::~GridCell(void)
{
	std::vector<GridObject2D*>::iterator current = m_grid_objects.begin();
	while (current != m_grid_objects.end())
	{
		delete (*current);
		++current;
	}
}

#pragma endregion
#pragma region Getters

int GridCell::FindIndex(GridObject2D* find_me) const
{
	for (uint i = 0; i < m_grid_objects.size(); ++i)
	{
		if (m_grid_objects[i] == find_me) return i;
	}
	return -1;
}

std::vector<GridObject2D*>::iterator GridCell::FindObject(GridObject2D* find_me)
{
	std::vector<GridObject2D*>::iterator current = m_grid_objects.begin();
	while (current != m_grid_objects.end())
	{
		if ((*current) == find_me)
		{
			return current;
		}
		++current;
	}

	return m_grid_objects.end();
}

#pragma endregion
#pragma region Public methods

// Removes Grid object from Grid Cell but leaves it in memory
bool GridCell::Remove(GridObject2D* remove_me)
{
	std::vector<GridObject2D*>::iterator iterator = FindObject(remove_me);
	if (iterator == m_grid_objects.end()) return false;	// Nothing found

	m_grid_objects.erase(iterator);
	return true;
}

// Removes Grid object from grid cell and clears the momory. Removes
bool GridCell::Delete(GridObject2D* delete_me)
{
	std::vector<GridObject2D*>::iterator iterator = FindObject(delete_me);
	if (iterator == m_grid_objects.end()) return false;	// Nothing found

	delete (*iterator);
	m_grid_objects.erase(iterator);
	return true;
}

// Add grid object to grid cell. Returns false if object already added.
bool GridCell::Add(GridObject2D* add_me)
{
	std::vector<GridObject2D*>::iterator iterator = FindObject(add_me);
	if (iterator != m_grid_objects.end()) return false;	// Nothing found

	if (IfObstacle()) return false;

	m_grid_objects.push_back(add_me);
	add_me->SetPos(m_pos);
	return true;
}

// Clears all grid object from grid cell, but leaves them in memory.
bool GridCell::Clear()
{
	for (int i = m_grid_objects.size() - 1; i = 0; --i)
	{
		m_grid_objects.erase(m_grid_objects.begin() + i);
	}

	return true;
}

// Removes all grid object from grid cell and clears the momory.
bool GridCell::DeleteAll()
{
	for (int i = m_grid_objects.size() - 1; i = 0; --i)
	{
		delete (*(m_grid_objects.begin() + i));
		m_grid_objects.erase(m_grid_objects.begin() + i);
	}	

	return true;
}

// Check if there is obstacle on this gridcell
bool GridCell::IfObstacle()
{
	std::vector<GridObject2D*>::iterator current = m_grid_objects.begin();
	while (current != m_grid_objects.end())
	{
		Obstacle* o = dynamic_cast<Obstacle*>(*current);
		if (o != 0) return true;
		++current;
	}

	return false;
}

// Update
void GridCell::Update(float delta_time)
{
	for (uint i = 0; i < m_grid_objects.size(); ++i)
	{
		GridObjResult result = m_grid_objects[i]->Update(delta_time);
		
		if (result == Moved || result == Removed || result == GridObjResult::Deleted) --i;
	}
}

// Draw all sh*t inside grid cell
void GridCell::Draw(void)
{
	std::vector<GridObject2D*>::iterator current = m_grid_objects.begin();
	while (current != m_grid_objects.end())
	{
		(*current)->Draw();
		++current;
	}
}

// Return pheromone on cell if there is any, return 0 if not
Pheromone* GridCell::GetPheromone(PheromoneType type)
{
	std::vector<GridObject2D*>::iterator current = m_grid_objects.begin();
	while (current != m_grid_objects.end())
	{
		Pheromone* o = dynamic_cast<Pheromone*>(*current);
		if (o != 0) if (o->GetType() == type) return o;
		++current;
	}

	return 0;
}

Food* GridCell::GetFood()
{
	std::vector<GridObject2D*>::iterator current = m_grid_objects.begin();
	while (current != m_grid_objects.end())
	{
		Food* f = dynamic_cast<Food*>(*current);
		if (f != 0) return f;
		++current;
	}

	return 0;
}

#pragma endregion