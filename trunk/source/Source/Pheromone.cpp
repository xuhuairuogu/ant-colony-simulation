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

#include "Pheromone.h"
#include "Settings.h"

Pheromone::Pheromone(void) :
	GridObject2D(),
	m_type(PheromoneType::home),
	m_strength(0.0f)
{
}

Pheromone::Pheromone(uint texture, PheromoneType type, float strength, Grid* grid) :
	GridObject2D(int2(0,0), 0.0f, grid),
	m_type(type),
	m_strength(strength) 
{
	m_sprite = new Sprite(texture, float2(0,0), 0.5f);
	m_sprite->SetLayerDepth(-1.0f);
	SetColor();
}

Pheromone::~Pheromone(void)
{
}

GridObjResult Pheromone::Update(float delta_time)
{
	if (m_type == food) m_strength -= m_strength * Settings::FOOD_TRAIL_FORGET_RATE * delta_time;
	if (m_type == home) m_strength -= m_strength * Settings::HOME_TRAIL_FORGET_RATE * delta_time;
	SetColor();

	if (m_strength <= 0.0f) m_strength = 0.0f;
	return GridObject2D::Update(delta_time);
}

void Pheromone::Draw()
{
	if (m_strength > 0.001f) GridObject2D::Draw();
}

float Pheromone::Deposit(float amount)
{
	m_strength += amount;
	if (m_strength > 1.0f) m_strength = 1.0f;
	SetColor();

	return m_strength;
}

void Pheromone::SetColor()
{
	// Change sprite colour/alpha
	const float4* color;

	switch (m_type)
	{
	case home:
		color = &Settings::HOME_TRAIL_COLOR;
		break;
	case food:
		color = &Settings::FOOD_TRAIL_COLOR;
		break;
	default:
		color = &Settings::HOME_TRAIL_COLOR;
		break;
	}

	m_sprite->SetColor(*color * (m_strength / Settings::MAX_PHEROMONE_LEVEL));
}