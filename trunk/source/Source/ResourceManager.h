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

#include <string>
#include <vector>
#include "BasicMath.h"
using namespace std;

class Cube;

class ResourceManager
{
public:
	static uint LoadTexture(const char* name);

private:
	ResourceManager();
	static uint LoadTextureBMP(const char* name);
	static uint LoadTexturePNG(const char* name);
};

// Textures
static uint FoodTexture;
static uint BugTexture;
static uint HomeTexture;
static uint WallTexture;
static uint PheromoneTexture;

