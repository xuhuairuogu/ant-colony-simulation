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

#include "ObstacleManager.h"
#include "Grid.h"
#include "Settings.h"

ObstacleManager::ObstacleManager(void) :
	m_grid(0),
	m_texture(0),
	m_obstacles()
{
}

ObstacleManager::ObstacleManager(uint texture, Grid* grid) :
	m_grid(grid),
	m_texture(texture),
	m_obstacles()
{
}

ObstacleManager::~ObstacleManager(void)
{
}

void ObstacleManager::Draw()
{
	for each(Obstacle* o in m_obstacles)	
	{
		o->Draw();
	}
}

void ObstacleManager::Update(float delta_time)
{
	for each(Obstacle* o in m_obstacles)
	{
		o->Update(delta_time);
	}
}

bool ObstacleManager::PutObstacleArray(uint texture, const int2& pos)
{
	return PutObstacleRecursive(pos, 0, rnd(Settings::MIN_OBSTACLE_ARRAY, Settings::MAX_OBSTACLE_ARRAY));
}

bool ObstacleManager::PutObstacleRecursive(const int2& pos, uint step, uint maxStep)
{
	if (!m_grid->IfPositionValid(pos)) return false;
	Obstacle* o = new Obstacle(m_texture, m_grid);
	if (!m_grid->AddGridObject(o, pos)) delete o;
	else m_obstacles.push_back(o);
	if (step >= maxStep) return true; else ++step;

	int2 rndDir;
	while ((rndDir = int2(rnd(-1, 1), rnd(-1, 1))) == int2(0, 0));
	return PutObstacleRecursive(pos + rndDir, step, maxStep);
}

