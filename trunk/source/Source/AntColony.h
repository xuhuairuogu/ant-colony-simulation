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

#include <vector>
#include <iostream>
#include <fstream>
#include "Ant.h"
#include "Home.h"
class Grid;
class FoodManager;

class AntColony
{
public:
	AntColony(void);
	AntColony(uint BugTexture, Grid* grid, FoodManager* food_manager);
	virtual ~AntColony(void);

	virtual void Draw();
	virtual void Update(float delta_time);
	virtual Home* SetHome(uint home_texture, int2 pos, int start_food);
	virtual void AddAntsNearHome(uint count) = 0;

protected:
	bool AddAnt(Ant* add_ant, int2 pos);	

protected:
	vector<Ant*> m_ants;
	Home* m_home;
	Grid* m_grid;
	FoodManager* m_food_manager;
	uint m_bug_texture;

private:
	void Log(float delta_time);
	float m_delta_log;
	float m_total_log;
	std::ofstream m_log_file;
};

