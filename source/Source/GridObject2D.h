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
#include "Sprite.h"
class Grid;

enum GridObjectState
{
	idle,
	moving,
	deceased
};

enum GridObjResult
{
	Default,
	Moved,
	Removed,
	Deleted,
	StartedMoving,
	CannotStartMoveInProgress,
	CannotStartMoveSamePos,
	CannotStartMoveNotAsjacentTile,
	CannotStartMoveInvalidPos,
	CannotStartMoveObstacle,

	CannotRemoveError,
	CannotDeleteError
};

class GridObject2D
{
public:
	GridObject2D(void);
	GridObject2D(const int2& position, float angle, Grid* grid);
	virtual ~GridObject2D(void);

	// Getters
	const int2& GetLastPos(void) { return m_last_pos; };
	const int2& GetPos(void) const { return m_position; };
	float GetAngle(void) const { return m_angle; };
	GridObjectState GetState(void) const { return m_state; };

	// Pure virtual
	virtual void Draw();
	virtual GridObjResult Update(float delta_time);

	// Modifiers/ Methods
	GridObjResult Move(const int2&);
	GridObjResult Remove();
	void Rotate(float delta_angle);
	void SetPos(const int2& new_pos);
	void ResetLastPos(void) { m_last_pos = m_position; };

private:
	GridObjResult UpdateMovement(float delta_time);

protected:
	int2 m_position;
	float m_angle;		

	Grid* m_grid;
	Sprite* m_sprite;
	GridObjectState m_state;

private:
	int2 m_last_pos;
	int2 m_targetPos;
	float m_moveTimerCurrent;
	float m_moveTimerMax;
};

