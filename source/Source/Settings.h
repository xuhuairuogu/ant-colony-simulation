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
#include <string>

enum ColonyType
{
	VectorAnts,
	PheromoneAnts
};

class Settings
{
public:
	Settings(void);
	virtual ~Settings(void);

	// General Simulation settings
	static float SIMULATION_SPEED;
	static uint RANDOM_SEED;
	static ColonyType COLONY_TYPE;
	static bool FIXED_STEP_ENABLED;
	static float DELTA_TIME;

	// Log file information
	static std::string LOG_OUTPUT_FILE;
	static float LOG_FREQUENCY;
	static float LOG_DURATION;

	// Grid appearence settings
	static bool RENDER_GRID_LINES;
	static bool RENDER_HOME_TRAIL;
	static bool RENDER_FOOD_TRAIL;
	static float4 HOME_TRAIL_COLOR;
	static float4 FOOD_TRAIL_COLOR;

	// Ant settings
	static uint SMELL_RANGE;
	static float ENERGY_CONSUMPTION;
	static float ANT_SPEED;
	static float HUNGRY_LEVEL;
	static float RANDOM_FACTOR;
	static uint START_ANT_COUNT;
	static float MAX_ENERGY;
	static float MAX_FOOD_STORAGE;

	// Pheromone parameters
	static float MAX_PHEROMONE_LEVEL;

	// Obstacle parameters
	static uint START_OBSTACLE_COUNT;
	static uint MIN_OBSTACLE_ARRAY;
	static uint MAX_OBSTACLE_ARRAY;

	// Home Trail
	static float HOME_TRAIL_DEPOSIT_RATE;
	static float HOME_TRAIL_FORGET_RATE;
	static float HOME_ALPHA;
	static float HOME_BETA;

	// Food Trail
	static float FOOD_TRAIL_DEPOSIT_RATE;
	static float FOOD_TRAIL_FORGET_RATE;
	static float FOOD_ALPHA;
	static float FOOD_BETA;	

	// Food Settings
	static uint MIN_FOOD;
	static uint MAX_FOOD;
	static uint START_FOOD_COUNT;
	static uint MIN_FOOD_ARRAY;
	static uint MAX_FOOD_ARRAY;

	// Read all settings from file
	static bool ReadSettings(const char* file_name);

};

