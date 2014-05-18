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

#include "Grid.h" 
#include <typeinfo>
#include <iostream>
#include "GL/GXBase/GXBase.h"
#include "ResourceManager.h"
#include "Settings.h"

Grid::Grid(void) :
	m_size(NUM_COLUMNS_X, NUM_ROWS_Y),
	m_pheromones(0),
	m_grid_lines(0),
	m_render_grid_lines(false)
{
	// Assign all grid object to null
	for (int i = 0; i < NUM_COLUMNS_X; ++i)
	{
		for (int j = 0; j < NUM_ROWS_Y; ++j)
		{
			m_grid2D[i][j] = new GridCell(int2(i, j));
		}
	}

	GridLineRenderingEnabled(true);
}
Grid::~Grid(void)
{
	// Remove all known grid object
	for (int i = 0; i < NUM_COLUMNS_X; ++i)
	{
		for (int j = 0; j < NUM_ROWS_Y; ++j)
		{
			delete m_grid2D[i][j];
		}
	}

	GridLineRenderingEnabled(true);
	delete m_pheromones;
}

bool Grid::RemoveGridObject(GridObject2D* remove_me)
{
	GridCell* cell = GetGridCell(remove_me->GetPos());
	return cell->Remove(remove_me);
}

bool Grid::DeleteGridObject(GridObject2D* delete_me)
{
	GridCell* cell = GetGridCell(delete_me->GetPos());
	return cell->Delete(delete_me);
}

GridCell* Grid::GetGridCell(const int2& pos) const
{
	if (!IfPositionValid(pos)) return 0;
	return m_grid2D[pos.x][pos.y];
}

bool Grid::AddGridObject(GridObject2D* object_add, const int2& pos)
{
	GridCell* cell = GetGridCell(pos);
	bool add_result = cell->Add(object_add);
	if (!add_result) return false;

	object_add->SetPos(pos);
	return true;
}

bool Grid::IfGridCellEmpty(const int2& pos) const
{
	GridCell* cell = GetGridCell(pos);
	return cell->Count() <= 0 ? true : false;
}

bool Grid::IfPositionValid(const int2& pos) const
{
	if (pos.x < 0 || pos.y < 0 || pos.x > NUM_COLUMNS_X - 1 || pos.y > NUM_ROWS_Y - 1) return false;
	else return true;
}

void Grid::Draw()
{
	glPushMatrix();
	{		
		glTranslatef(-NUM_COLUMNS_X / 2.0f, -NUM_ROWS_Y / 2.0f, 0.0f);

		if (Settings::RENDER_GRID_LINES)
		{
			glDisable(GL_TEXTURE_2D);

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, sizeof(LineVertex2D), &m_grid_lines->m_first[0].x);
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(3, GL_FLOAT, sizeof(LineVertex2D), &m_grid_lines->m_first[0].r);
			glDrawArrays(GL_LINES, 0, m_grid_lines->m_size);
		
			glEnable(GL_TEXTURE_2D);
		}

		//for (int i = 0; i < NUM_COLUMNS_X; ++i) {
		//	for (int j = 0; j < NUM_ROWS_Y; ++j) {
		//		m_grid2D[i][j]->Draw();
		//	}
		//}
		m_pheromones->Draw();
	}
	glPopMatrix();
}

void Grid::Update(float delta)
{
	for (int i = 0; i < NUM_COLUMNS_X; ++i) {
		for (int j = 0; j < NUM_ROWS_Y; ++j) {
			m_grid2D[i][j]->Update(delta);
		}
	}	
}

void Grid::GridLineRenderingEnabled(bool new_value)
{
	if (new_value != m_render_grid_lines) m_render_grid_lines = new_value;

	if (m_render_grid_lines && m_grid_lines == 0)
	{
		uint size = (NUM_COLUMNS_X + NUM_ROWS_Y + 2) * 2;
		LineVertex2D* grid_lines = new LineVertex2D[size];
		uint counter = 0;

		// Cycle through all grid pos to build all lines
		for (int i = 0; i <= NUM_COLUMNS_X; ++i)
		{
			grid_lines[counter].x = 1.0f * i;
			grid_lines[counter].y = 0.0f;
			grid_lines[counter].r = 0.0f;
			grid_lines[counter].g = 0.0f;
			grid_lines[counter].b = 0.0f;
			++counter;

			grid_lines[counter].x = 1.0f * i;
			grid_lines[counter].y = 1.0f * NUM_ROWS_Y;
			grid_lines[counter].r = 0.0f;
			grid_lines[counter].g = 0.0f;
			grid_lines[counter].b = 0.0f;
			++counter;
		}

		for (int j = 0; j <= NUM_ROWS_Y; ++j)
		{
			grid_lines[counter].x = 0.0f;
			grid_lines[counter].y = 1.0f * j;
			grid_lines[counter].r = 0.0f;
			grid_lines[counter].g = 0.0f;
			grid_lines[counter].b = 0.0f;
			++counter;

			grid_lines[counter].x = 1.0f * NUM_COLUMNS_X;
			grid_lines[counter].y = 1.0f * j;
			grid_lines[counter].r = 0.0f;
			grid_lines[counter].g = 0.0f;
			grid_lines[counter].b = 0.0f;
			++counter;				
		}

		if (counter != size) _ASSERT(true);

		m_grid_lines = new DynamicArray<LineVertex2D>(grid_lines, size);
	}
}

bool Grid::MoveGridObject(GridObject2D* move_me, const int2& new_pos)
{
	// Remove grid object from the grid cell
	bool remove_result = GetGridCell(move_me->GetPos())->Remove(move_me);
	if (!remove_result) return false;

	// Add grid object to the new cell
	bool add_result = GetGridCell(new_pos)->Add(move_me);
	if (!add_result) return false;

	return false;
}

bool Grid::IfObstacle(const int2& pos)
{
	GridCell* grid_cell = GetGridCell(pos);
	if (grid_cell == 0) return true;
	return GetGridCell(pos)->IfObstacle();
}

Food* Grid::ClosestFood(const int2& pos, uint range)
{
	int2 startPos = pos - int2(range, range);
	int2 endPos = pos + int2(range, range);
	Food* result = 0;
	int bestDist = INT_MAX;

	for (int x = startPos.x; x < endPos.x; ++x)
	{
		for (int y = startPos.y; y < endPos.y; ++y)
		{
			int2 current = int2(x, y);
			// Check if position is valid
			if (!IfPositionValid(current)) continue;

			// Check if there is food on this location
			Food* f = GetGridCell(current)->GetFood();
			if (f == 0) continue;

			// Check if within the smell range
			int dist = dis(pos, current);			
			if (dist > range) continue;

			// Check if closer that others
			if (dist <= bestDist)
			{
				result = f;
			}
		}	
	}

	return result;
}

// Init pheromone manager
void Grid::InitPheromones(uint texture)
{
	m_pheromones = new PheromoneManager(texture, this);
}

// Update Pheromones
void Grid::UpdatePheromones(float delta_time)
{
	m_pheromones->Update(delta_time);
}

// Get pheromone strength at given location
float Grid::GetPheromoneStrength(const int2& pos, PheromoneType type)
{
	Pheromone* p = GetGridCell(pos)->GetPheromone(type);
	if (p == 0) return 0.01f;
	else return p->GetStrength();
}

// Deposits pheromone at given location. Return new pheromone strength
float Grid::DepositPheromone(const int2& pos, PheromoneType type, float add_pheromone)
{
	Pheromone* p = GetGridCell(pos)->GetPheromone(type);
	if (p != 0) return p->Deposit(add_pheromone);
	
	// Create new pheromone at this position
	return m_pheromones->AddPheromone(add_pheromone, type, pos)->GetStrength();
}