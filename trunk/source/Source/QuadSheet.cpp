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

#include "BasicMath.h"
#include "QuadSheet.h"

//Constructors
QuadSheet::QuadSheet(void) : 
	m_start(0),
	m_end(0),
	m_animInterval(0),
	m_animated(true),
	m_counter(0),
	m_current(0)
{

}

QuadSheet::QuadSheet(int start, int end, float animInterval) : 
	m_start(start),
	m_end(end),
	m_animInterval(animInterval),
	m_animated(true),
	m_counter(0.0f),
	m_current(start)
{
}

QuadSheet::QuadSheet(int start) :
	m_start(start),
	m_end(start),
	m_animInterval(0.3333f),
	m_animated(false),
	m_counter(0.0f),
	m_current(start)
{
}

const int QuadSheet::GetCurrent() const
{
	return m_current;
}

const bool QuadSheet::Update(float timeDelta)
{
	if (!m_animated) return 0;

	m_counter += timeDelta;
	if (m_counter < m_animInterval) return 0;

	if (m_current == m_end)
	{
		m_current = m_start;
	}
	else
	{
		++m_current;
	}
	m_counter = 0;

	return 1;
}

QuadSheet::~QuadSheet(void)
{
}
