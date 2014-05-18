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

#include "AntColony.h"
#include "Grid.h"
#include "Settings.h"
#include "Pheromone.h"

#pragma region Gang of three

AntColony::AntColony(void) :
	m_ants(0),
	m_home(0),
	m_grid(0),
	m_food_manager(0),
	m_bug_texture(0),
	m_delta_log(0),
	m_total_log(0),
	m_log_file()
{
}

AntColony::~AntColony(void)
{
	m_log_file.close();
}

AntColony::AntColony(uint BugTexture, Grid* grid, FoodManager* food_manager) :
	m_ants(0),
	m_home(0),
	m_grid(grid),
	m_food_manager(food_manager),
	m_bug_texture(BugTexture),
	m_delta_log(0),
	m_total_log(0),
	m_log_file()
{
	m_log_file.open(Settings::LOG_OUTPUT_FILE);
}

#pragma endregion

// Default update for ant colony
void AntColony::Update(float delta_time)
{
	for (uint i = 0; i < m_ants.size(); ++i)
	{
		GridObjResult result = m_ants[i]->Update(delta_time);

		switch (result)
		{
		case Deleted:
			delete m_ants[i];
			m_ants.erase(m_ants.begin() + i);
			--i;
			break;
		default:
			break;
		}
	}

	m_home->Update(delta_time);

	Log(delta_time);
}

// Draw all ant and home
void AntColony::Draw()
{
	for each(Ant* ant in m_ants)
	{
		ant->Draw();
	}

	m_home->Draw();
}

// Adds ant into colony and grid as well, return false if not added
bool AntColony::AddAnt(Ant* add_ant, int2 pos)
{
	for each (Ant* ant in m_ants) if (ant == add_ant) return false;

	if (!m_grid->AddGridObject(add_ant, pos)) return false;
	m_ants.push_back(add_ant);
	return true;
}

// Add home to chosen position (or moves it). Checks home is not inside obstacle
Home* AntColony::SetHome(uint home_texture, int2 pos, int start_food)
{
	if (m_home == 0) delete m_home;

	Home* new_home = new Home(home_texture, pos, start_food, m_grid);
	while (!m_grid->AddGridObject(new_home, pos)) 
	{
		pos += NearTiles[rnd(0, 8)];
	}

	m_home = new_home;
	m_grid->DepositPheromone(m_home->GetPos(), PheromoneType::home, 1000000.0f);
	return m_home;
}

void AntColony::Log(float delta_time)
{
	m_delta_log += delta_time;
	m_total_log += delta_time;

	if (m_delta_log > Settings::LOG_FREQUENCY && m_total_log < Settings::LOG_DURATION)
	{
		m_log_file << m_home->m_food_storage.Value() << ", " << m_ants.size();

		for each(Ant* ant in m_ants)
		{
			m_log_file << ", " << ant->GetEnergy();
		}

		m_log_file << endl;

		m_delta_log -= Settings::LOG_FREQUENCY;
		return;
	}

	if (m_total_log > Settings::LOG_DURATION)
	{
		m_log_file.close();
	}
}