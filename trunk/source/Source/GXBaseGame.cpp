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

#include "GXBaseGame.h"
#include "ResourceManager.h"
#include "VectorAnt.h"
#include "Obstacle.h"
#include "Settings.h"
#include "VectorAntColony.h"
#include "PheromoneAntColony.h"

#include <time.h>
#include <string>
#include <iostream>
#include <fstream>

Game::Game(void) :
	m_bar(0),
	m_grid(0),
	m_timer(0),
	m_frameCounter(0),
	m_antColony(0),
	m_foodManager(0),
	m_obstacles(0),
	m_backColor(0.39f, 0.58f, 0.92f)
{
}

Game::~Game(void)
{
	//delete m_grid;
	delete m_frameCounter;
	delete m_timer;
	delete m_antColony;
	delete m_foodManager;
}

void Game::OnCreate(void)
{
	// Setip GXBase context (setting up canvas and 2D generally)
	GXBaseContext::OnCreate();

	// Read settings from XML file
	// Try to take XML file path from arguments
	if (theApp.ArgCount() >= 2) Settings::ReadSettings(theApp.Arg(1));
	else 
	{
		cout << "No input file detected in arguments, using default input.xml" << endl;
		Settings::ReadSettings("input.xml");
	}

	// Randomise (or use the seed)
	if (Settings::RANDOM_SEED == 0) srand((uint)time(NULL));
	else srand(Settings::RANDOM_SEED);

#pragma region Init Tweakbar
	// Create TweakBar
	m_bar = TwNewBar("TweakBar");
	TwDefine(" GLOBAL help='This bar would help to customize simulation parameters' ");	// Message added to the help bar.
	TwDefine(" TweakBar size='256 500' color='32 32 32' ");			// change default tweak bar size and color

	// Grid settings
    TwAddVarRW(m_bar, "Back colour", TW_TYPE_COLOR3F, &m_backColor, " group='Appearance' ");
	TwAddVarRW(m_bar, "Home trail colour", TW_TYPE_COLOR4F, &Settings::HOME_TRAIL_COLOR, " group='Appearance' ");
	TwAddVarRW(m_bar, "Food trail colour", TW_TYPE_COLOR4F, &Settings::FOOD_TRAIL_COLOR, " group='Appearance' ");
	TwAddVarRW(m_bar, "Grid lines", TW_TYPE_BOOL8  , &Settings::RENDER_GRID_LINES, "group='Appearance' ");
	TwAddVarRW(m_bar, "Home trail", TW_TYPE_BOOL8  , &Settings::RENDER_HOME_TRAIL, "group='Appearance' ");
	TwAddVarRW(m_bar, "Food trail", TW_TYPE_BOOL8  , &Settings::RENDER_FOOD_TRAIL, "group='Appearance' ");

	// Camera settings
	//TwAddVarRW(m_bar, "Rotation"   , TW_TYPE_FLOAT  , &m_camera->m_angle, " min=-3.14 max=3.14 step=0.04 group='Camera' ");
	TwAddVarRW(m_bar, "Zoom", TW_TYPE_FLOAT  , &m_camera->m_zoom, " min=1.00 max=100.0 step=1.0 group='Camera' ");
	TwAddVarRW(m_bar, "Simulation speed", TW_TYPE_FLOAT  ,&Settings::SIMULATION_SPEED, " min=0.1 max=10.0 step=0.05 group='Camera' ");

	// Ant settings
	TwAddVarRW(m_bar, "Energy consumption", TW_TYPE_FLOAT  , &Settings::ENERGY_CONSUMPTION, "min=0.0 max=1.0 step=0.01 group='Ants' ");
	TwAddVarRW(m_bar, "Ant smell range", TW_TYPE_UINT32  , &Settings::SMELL_RANGE, "min=1 max=100 step=1 group='Ants' ");
	TwAddVarRW(m_bar, "Ant move speed", TW_TYPE_FLOAT  , &Settings::ANT_SPEED, "min=0.0 max=2.5 step=0.01 group='Ants' ");
	TwAddVarRW(m_bar, "Random factor", TW_TYPE_FLOAT  , &Settings::RANDOM_FACTOR, "min=0.0 max=1.0 step=0.01 group='Ants' ");

	// Home Trail
	TwAddVarRW(m_bar, "Home Deposit Rate", TW_TYPE_FLOAT  , &Settings::HOME_TRAIL_DEPOSIT_RATE, "min=0.0 max=1.0 step=0.01 group='Home Trail' ");
	TwAddVarRW(m_bar, "Home Forget Rate", TW_TYPE_FLOAT, &Settings::HOME_TRAIL_FORGET_RATE, "min=0.0 max=1.0 step=0.01 group='Home Trail' ");
	TwAddVarRW(m_bar, "Home Alpha", TW_TYPE_FLOAT, &Settings::HOME_ALPHA, "min=0.0 max=1.0 step=0.01 group='Home Trail' ");
	TwAddVarRW(m_bar, "Home Beta", TW_TYPE_FLOAT, &Settings::HOME_BETA, "min=0.0 max=1.0 step=0.01 group='Home Trail' ");

	// Food Trail
	TwAddVarRW(m_bar, "Food Deposit Rate", TW_TYPE_FLOAT  , &Settings::FOOD_TRAIL_DEPOSIT_RATE, "min=0.0 max=1.0 step=0.01 group='Food Trail' ");
	TwAddVarRW(m_bar, "Food Forget Rate", TW_TYPE_FLOAT, &Settings::FOOD_TRAIL_FORGET_RATE, "min=0.0 max=1.0 step=0.01 group='Food Trail' ");
	TwAddVarRW(m_bar, "Food Alpha", TW_TYPE_FLOAT, &Settings::FOOD_ALPHA, "min=0.0 max=1.0 step=0.01 group='Food Trail' ");
	TwAddVarRW(m_bar, "Food Beta", TW_TYPE_FLOAT, &Settings::FOOD_BETA, "min=0.0 max=1.0 step=0.01 group='Food Trail' ");

#pragma endregion

	// Load textures
	FoodTexture = ResourceManager::LoadTexture("Textures\\food.png");
	BugTexture  = ResourceManager::LoadTexture("Textures\\bug.png");
	HomeTexture = ResourceManager::LoadTexture("Textures\\home.png");
	WallTexture = ResourceManager::LoadTexture("Textures\\wall.png");
	PheromoneTexture = ResourceManager::LoadTexture("Textures\\pheromone.png");

	// Grid init
	m_grid = new Grid();
	m_grid->InitPheromones(PheromoneTexture);
	m_grid->GridLineRenderingEnabled(true);

	// Put random obstacles
	m_obstacles = new ObstacleManager(WallTexture, m_grid);
	for (uint i = 0; i < Settings::START_OBSTACLE_COUNT; ++i)
	{
		m_obstacles->PutObstacleArray(WallTexture, int2(rnd(0, NUM_COLUMNS_X - 1), rnd(0, NUM_ROWS_Y - 1)));
	}

	// Create food manager
	m_foodManager = new FoodManager(m_grid, FoodTexture);
	for (uint i = 0; i < Settings::START_FOOD_COUNT; ++i)
	{
		m_foodManager->PutFoodArray(int2(rnd(0, NUM_COLUMNS_X - 1), rnd(0, NUM_ROWS_Y - 1)));
	}

	// Creat Vector Ant colony
	if (Settings::COLONY_TYPE == VectorAnts) m_antColony = new VectorAntColony(BugTexture, m_grid, m_foodManager);
	else if (Settings::COLONY_TYPE == PheromoneAnts) m_antColony = new PheromoneAntColony(BugTexture, m_grid, m_foodManager);

	// Create Home for colony (Make sure it is not inside obstacle)
	m_antColony->SetHome(HomeTexture, int2(NUM_COLUMNS_X / 2, NUM_ROWS_Y / 2), 100);
	// Create Ants around colony
	m_antColony->AddAntsNearHome(Settings::START_ANT_COUNT);

	// Timer and framecounter init
	m_timer = new GXBaseTimer();
	m_frameCounter = new FrameCounter();
	m_timer->Start();

	// Zoom camera to fit grid
	m_camera->m_zoom = NUM_ROWS_Y * 1.0f;
}

void Game::Update(void)
{
	float delta_time = m_timer->GetSeconds();
	m_frameCounter->Update(delta_time);

	if (Settings::FIXED_STEP_ENABLED) delta_time = Settings::DELTA_TIME;
	delta_time *= Settings::SIMULATION_SPEED;

	//m_grid->Update(delta_time);
	m_grid->UpdatePheromones(delta_time);

	// Update Grid objects
	m_foodManager->Update(delta_time);	
	m_antColony->Update(delta_time);
}

void Game::OnDisplay(void)
{
	Update();

	ClearScreen();

	glPushMatrix();
	{
		// Move camera
		glRotatef(RadianToDegree(m_camera->m_angle), 0.0f, 0.0f, 1.0f);		
		glTranslatef(m_camera->m_pos.x, m_camera->m_pos.y, 0.0f);	
		glScalef((1.0f / m_camera->m_zoom) / Aspect(), 1.0f / m_camera->m_zoom, 1.0f);
		
		// Draw field
		m_grid->Draw();
		glPushMatrix();
		{		
			glTranslatef(-NUM_COLUMNS_X / 2.0f, -NUM_ROWS_Y / 2.0f, 0.0f);
			m_obstacles->Draw();
			m_foodManager->Draw();
			m_antColony->Draw();
		}
		glPopMatrix();
	}
	glPopMatrix();

	// Draw UI	
	char* fps = new char[10]; 
	sprintf(fps, "%d", m_frameCounter->GetFPS());
	Print(fps, float2(0.45f, -0.45f));
	TwDraw();  

	PresentScreen();
}

void Game::ClearScreen(void)
{
	glClearColor(m_backColor.r, m_backColor.g, m_backColor.b, 1.0f);	// Cornflower Blue
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

#endif
#pragma region Tweak Bar Callbacks

void TW_CALL SetGridRenderCallback(const void *value, void *clientData)
{
	//m_grid->GridLineRenderingEnabled(static_cast<bool>(new_value));
}

void TW_CALL GetGridRenderCallback(void *value, void *clientData)
{
	//*(bool*)return_value = true;
}

#pragma endregion