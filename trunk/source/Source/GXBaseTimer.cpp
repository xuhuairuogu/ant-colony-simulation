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

#include "GXBaseTimer.h"

GXBaseTimer::GXBaseTimer(void)
{
}

GXBaseTimer::~GXBaseTimer(void)
{
}

void GXBaseTimer::Start()
{
	m_running = true;

	QueryPerformanceCounter(&last);
}

void GXBaseTimer::Stop()
{
	m_running = false;
}

float GXBaseTimer::GetSeconds()
{
	if(m_running)
    {
		QueryPerformanceFrequency(&frequency);//finds current clock frequency
		QueryPerformanceCounter(&now);//finds current clock cycle

		float timeSecs = (now.QuadPart - last.QuadPart) / static_cast<float>(frequency.QuadPart);//total clock cycles/frequency

		last = now;

		return timeSecs;
    }
    else
    {
        return 0;
    }
}

#endif