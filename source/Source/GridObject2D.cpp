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

#include "GridObject2D.h"
#include "Grid.h"
#include "Settings.h"

#pragma region Gang of three

GridObject2D::GridObject2D(void) :
	m_position(0,0),
	m_angle(0.0f),
	m_grid(0),
	m_sprite(0),
	m_state(GridObjectState::idle),
	m_last_pos(0,0),
	m_targetPos(0,0),
	m_moveTimerCurrent(0.0f),
	m_moveTimerMax(0.0f)
{
}

GridObject2D::GridObject2D(const int2& position, float angle, Grid* grid) :
	m_position(position),
	m_angle(angle),
	m_grid(grid),
	m_sprite(0),
	m_state(GridObjectState::idle),
	m_last_pos(position),
	m_targetPos(0,0),
	m_moveTimerCurrent(0.0f),
	m_moveTimerMax(0.0f)
{
}

GridObject2D::~GridObject2D(void)
{
	delete m_sprite;
}

#pragma endregion
#pragma region Setters

void GridObject2D::SetPos(const int2& new_pos)
{
	m_last_pos = m_position;
	m_position = new_pos;
	m_sprite->SetPos(float2(new_pos.x + 0.5f, new_pos.y + 0.5f));
}

#pragma endregion
#pragma region Public grid methods

// Set new position. New position should be adjacent to old position. (8 directions)
// If object currently is moving then this call would be ignored
GridObjResult GridObject2D::Move(const int2& newPos)
{
	if (m_state == GridObjectState::moving) return GridObjResult::CannotStartMoveInProgress;	// Should not move already
	if (m_position == newPos) return GridObjResult::CannotStartMoveSamePos;						// Can not be the same position
	if (length(m_position - newPos) >= 2) return GridObjResult::CannotStartMoveNotAsjacentTile;	// Not asjacent tile
	if (!m_grid->IfPositionValid(newPos)) return GridObjResult::CannotStartMoveInvalidPos;		// Invalid position
	if (m_grid->IfObstacle(newPos)) return GridObjResult::CannotStartMoveObstacle;				// Obstacle

	m_state = GridObjectState::moving;
	m_moveTimerMax = dis(int2float2(m_position), int2float2(newPos)) * Settings::ANT_SPEED;
	m_moveTimerCurrent = 0.0f;
	m_targetPos = newPos;

	return GridObjResult::StartedMoving;
}

// Removes object from grid (does not removed from memory)
GridObjResult GridObject2D::Remove(void)
{
	if (m_grid->RemoveGridObject(this)) return GridObjResult::Removed;
	else return GridObjResult::CannotRemoveError;
}

void GridObject2D::Rotate(float delta_angle)
{
	// Rotates object on given angle
	m_angle += delta_angle;
}

GridObjResult GridObject2D::Update(float delta_time)
{
	switch (m_state)
	{
	case idle:
		if (m_sprite != 0) m_sprite->Update(delta_time);
		return GridObjResult::Default;
	case moving:
		return UpdateMovement(delta_time);
	case deceased:
		if (m_sprite != 0) m_sprite->Update(delta_time);
		return GridObjResult::Removed;
	default:
		return GridObjResult::Default;
	}
}

void GridObject2D::Draw()
{
	if (m_sprite != 0) m_sprite->Draw();
}

#pragma endregion
#pragma region Private methods

GridObjResult GridObject2D::UpdateMovement(float delta_time)
{
	m_moveTimerCurrent += delta_time;
	if (m_moveTimerCurrent >= m_moveTimerMax)
	{
		// End movement, relocate to another gridcell
		m_state = GridObjectState::idle;
		m_grid->MoveGridObject(this, m_targetPos);
		m_sprite->Update(delta_time);
		return GridObjResult::Moved;
	}
	else
	{
		int2 moveDirection = m_targetPos - m_position;
		float a = singned_angle(float2(0.0f, -1.0f), int2float2(moveDirection));
		float2 moveDelta = float2(static_cast<float>(moveDirection.x), static_cast<float>(moveDirection.y)) * (m_moveTimerCurrent / m_moveTimerMax);
		m_sprite->Update(delta_time, float2(static_cast<float>(m_position.x), static_cast<float>(m_position.y)) + moveDelta + float2(0.5f, 0.5f), a);
		return GridObjResult::Default;
	}	
}

#pragma endregion