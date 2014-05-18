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

#ifdef _WIN32

#pragma once

#include <stdio.h>
#include <vector>
#include "BasicMath.h"
#include "Sprite.h"
#include "Grid.h"
#include "GXBaseTimer.h"
#include "FrameCounter.h"
#include "AntColony.h"
#include "FoodManager.h"
#include "ObstacleManager.h"
using namespace std;

#include "GXBaseContext.h"
#include "AntTweakBar/AntTweakBar.h"

class Game : public GXBaseContext	// GXBase specific
{
public:
	Game(void);
	~Game(void);

	virtual void OnCreate(void);
	virtual void OnDisplay(void);
	virtual void ClearScreen(void);
	void Update(void);

private:
	TwBar* m_bar;
	Grid* m_grid;
	GXBaseTimer* m_timer;
	FrameCounter* m_frameCounter;
	AntColony* m_antColony;
	FoodManager* m_foodManager;
	ObstacleManager* m_obstacles;

	float3 m_backColor;
};

// GXBase specific:
class MyApp : public App {
private:
	Game _wnd;
};

static MyApp theApp;

#endif