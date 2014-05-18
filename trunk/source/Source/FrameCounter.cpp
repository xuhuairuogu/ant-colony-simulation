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

#include "FrameCounter.h"
#include "BasicMath.h"

FrameCounter::FrameCounter(void)
{
}

FrameCounter::~FrameCounter(void)
{
}

uint FrameCounter::Update(float timeDelta)
{
	uint i = 0;
	while (i < frameList.size())
	{		
		if ((frameList[i] += timeDelta) > 1.0f)
		{
			std::vector<float>::iterator eraseIt = frameList.begin() + i;
			frameList.erase(eraseIt);
		}
		else
		{
			++i;
		}
	}

	frameList.push_back(0.0f);

	return frameList.size();
}

uint FrameCounter::GetFPS()
{
	return frameList.size();
}