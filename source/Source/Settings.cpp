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

#include "Settings.h"
#include "rapidxml.hpp"
#include <iostream>
#include <fstream>
#include <vector>

using namespace rapidxml;
using namespace std;

Settings::Settings(void)
{
}

Settings::~Settings(void)
{
}

bool Settings::ReadSettings(const char* file_name)
{
	xml_document<> doc;
	xml_node<>* root_node;

	// Read the xml file into a vector
	ifstream inputxml(file_name);
	if (!inputxml.is_open()) 
	{
		cout << "Cannot load settings file: " << file_name << "; using default settings" << endl;
		return false;	// Failed to open a file, use default values
	}
	vector<char> buffer((istreambuf_iterator<char>(inputxml)), istreambuf_iterator<char>());
	buffer.push_back('\0');

	// Parse the buffer using the xml file parsing library into doc 
	doc.parse<0>(&buffer[0]);

	// Find our root node
	root_node = doc.first_node("Settings");

	// Read all settings values

	// General Simulation settings
	Settings::SIMULATION_SPEED = ctof(root_node->first_node("SIMULATION_SPEED")->value());
	Settings::RANDOM_SEED = ctoui(root_node->first_node("RANDOM_SEED")->value());
	Settings::COLONY_TYPE = static_cast<ColonyType>(ctoui(root_node->first_node("COLONY_TYPE")->value()));	// I imagibe this line could throw lots of exceptions
	Settings::FIXED_STEP_ENABLED = ctoui(root_node->first_node("FIXED_STEP_ENABLED")->value());
	Settings::DELTA_TIME = ctof(root_node->first_node("DELTA_TIME")->value());

	// Log file information
	Settings::LOG_OUTPUT_FILE = root_node->first_node("LOG_OUTPUT_FILE")->value();
	Settings::LOG_FREQUENCY = ctof(root_node->first_node("LOG_FREQUENCY")->value());
	Settings::LOG_DURATION = ctof(root_node->first_node("LOG_DURATION")->value());

	// Pheromone settings
	Settings::MAX_PHEROMONE_LEVEL = ctof(root_node->first_node("MAX_PHEROMONE_LEVEL")->value());

	// Grid appearance settings
	Settings::RENDER_HOME_TRAIL = ctoui(root_node->first_node("RENDER_HOME_TRAIL")->value());
	Settings::RENDER_FOOD_TRAIL = ctoui(root_node->first_node("RENDER_FOOD_TRAIL")->value());
	Settings::RENDER_GRID_LINES = ctoui(root_node->first_node("RENDER_GRID_LINES")->value());
	xml_node<>* home_color_node = root_node->first_node("HOME_TRAIL_COLOR");
	Settings::HOME_TRAIL_COLOR.r = ctof(home_color_node->first_node("R")->value());
	Settings::HOME_TRAIL_COLOR.g = ctof(home_color_node->first_node("G")->value());
	Settings::HOME_TRAIL_COLOR.b = ctof(home_color_node->first_node("B")->value());
	Settings::HOME_TRAIL_COLOR.a = ctof(home_color_node->first_node("A")->value());
	xml_node<>* food_color_node = root_node->first_node("FOOD_TRAIL_COLOR");
	Settings::FOOD_TRAIL_COLOR.r = ctof(food_color_node->first_node("R")->value());
	Settings::FOOD_TRAIL_COLOR.g = ctof(food_color_node->first_node("G")->value());
	Settings::FOOD_TRAIL_COLOR.b = ctof(food_color_node->first_node("B")->value());
	Settings::FOOD_TRAIL_COLOR.a = ctof(food_color_node->first_node("A")->value());

	// Ant settings
	Settings::SMELL_RANGE = ctoui(root_node->first_node("SMELL_RANGE")->value());
	Settings::ENERGY_CONSUMPTION = ctof(root_node->first_node("ENERGY_CONSUMPTION")->value());
	Settings::ANT_SPEED = ctof(root_node->first_node("ANT_SPEED")->value());
	Settings::RANDOM_FACTOR = ctof(root_node->first_node("RANDOM_FACTOR")->value());
	Settings::HUNGRY_LEVEL = ctof(root_node->first_node("HUNGRY_LEVEL")->value());
	Settings::START_ANT_COUNT = ctoui(root_node->first_node("START_ANT_COUNT")->value());
	Settings::MAX_ENERGY = ctof(root_node->first_node("MAX_ENERGY")->value());
	Settings::MAX_FOOD_STORAGE = ctof(root_node->first_node("MAX_FOOD_STORAGE")->value());

	// Food Settings
	Settings::MIN_FOOD = ctoui(root_node->first_node("MIN_FOOD")->value());
	Settings::MAX_FOOD = ctoui(root_node->first_node("MAX_FOOD")->value());
	Settings::START_FOOD_COUNT = ctoui(root_node->first_node("START_FOOD_COUNT")->value());
	Settings::MIN_FOOD_ARRAY = ctoui(root_node->first_node("MIN_FOOD_ARRAY")->value());
	Settings::MAX_FOOD_ARRAY = ctoui(root_node->first_node("MAX_FOOD_ARRAY")->value());

	// Obstacle parameters
	Settings::START_OBSTACLE_COUNT = ctoui(root_node->first_node("START_OBSTACLE_COUNT")->value());
	Settings::MIN_OBSTACLE_ARRAY = ctoui(root_node->first_node("MIN_OBSTACLE_ARRAY")->value());
	Settings::MAX_OBSTACLE_ARRAY = ctoui(root_node->first_node("MAX_OBSTACLE_ARRAY")->value());

	// Home Trail
	Settings::HOME_TRAIL_DEPOSIT_RATE = ctof(root_node->first_node("HOME_TRAIL_DEPOSIT_RATE")->value());
	Settings::HOME_TRAIL_FORGET_RATE = ctof(root_node->first_node("HOME_TRAIL_FORGET_RATE")->value());
	Settings::HOME_ALPHA = ctof(root_node->first_node("HOME_ALPHA")->value());
	Settings::HOME_BETA = ctof(root_node->first_node("HOME_BETA")->value());

	// Food Trail
	Settings::FOOD_TRAIL_DEPOSIT_RATE = ctof(root_node->first_node("FOOD_TRAIL_DEPOSIT_RATE")->value());
	Settings::FOOD_TRAIL_FORGET_RATE = ctof(root_node->first_node("FOOD_TRAIL_FORGET_RATE")->value());
	Settings::FOOD_ALPHA = ctof(root_node->first_node("FOOD_ALPHA")->value());
	Settings::FOOD_BETA = ctof(root_node->first_node("FOOD_BETA")->value());

	inputxml.close();
	return true;
}

// General Simulation settings
float Settings::SIMULATION_SPEED = 1.0f;
uint Settings::RANDOM_SEED = 0;
ColonyType Settings::COLONY_TYPE = ColonyType::PheromoneAnts;
bool Settings::FIXED_STEP_ENABLED = false;
float Settings::DELTA_TIME = 0.02;

// Log file information
std::string Settings::LOG_OUTPUT_FILE = "output.csv";
float Settings::LOG_FREQUENCY = 1.0f;
float Settings::LOG_DURATION = 600.0f;

// Pheromone settings
float Settings::MAX_PHEROMONE_LEVEL = 1.0f;

// Grid appearance settings
bool Settings::RENDER_HOME_TRAIL = false;
bool Settings::RENDER_FOOD_TRAIL = true;
bool Settings::RENDER_GRID_LINES = false;
float4 Settings::HOME_TRAIL_COLOR = float4(0.0f, 1.0f, 0.0f, 1.0f);
float4 Settings::FOOD_TRAIL_COLOR = float4(1.0f, 0.0f, 0.0f, 1.0f);

// Ant settings
uint  Settings::SMELL_RANGE = 2;
float Settings::ENERGY_CONSUMPTION = 0.02f;
float Settings::ANT_SPEED = 0.15f;
float Settings::RANDOM_FACTOR = 0.2f;
float Settings::HUNGRY_LEVEL = 0.3f;
uint Settings::START_ANT_COUNT = 200;
float Settings::MAX_ENERGY = 100.0f;
float Settings::MAX_FOOD_STORAGE = 50.0f;

// Food Settings
uint Settings::MIN_FOOD = 500;
uint Settings::MAX_FOOD = 5000;
uint Settings::START_FOOD_COUNT = 10;
uint Settings::MIN_FOOD_ARRAY = 20;
uint Settings::MAX_FOOD_ARRAY = 40;

// Obstacle parameters
uint Settings::START_OBSTACLE_COUNT = 100;
uint Settings::MIN_OBSTACLE_ARRAY = 10;
uint Settings::MAX_OBSTACLE_ARRAY = 15;

// Home Trail
float Settings::HOME_TRAIL_DEPOSIT_RATE = 0.5f;
float Settings::HOME_TRAIL_FORGET_RATE = 0.01f;
float Settings::HOME_ALPHA = 0.5f;
float Settings::HOME_BETA = 0.8f;

// Food Trail
float Settings::FOOD_TRAIL_DEPOSIT_RATE = 0.2f;
float Settings::FOOD_TRAIL_FORGET_RATE = 0.05f;
float Settings::FOOD_ALPHA = 0.5f;
float Settings::FOOD_BETA = 1.0f;

