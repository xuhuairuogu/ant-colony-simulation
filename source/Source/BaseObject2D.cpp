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

#include "BaseObject2D.h"


BaseObject2D::BaseObject2D(void) :
	m_pos(0.0f, 0.0f),
	m_angle(0.0f),
	m_scale(1.0f, 1.0f)
{
}

BaseObject2D::BaseObject2D(const float2& pos, const float& angle, const float2& scale) :
	m_pos(pos),
	m_angle(angle),
	m_scale(scale)
{

}

BaseObject2D::~BaseObject2D(void)
{
}
