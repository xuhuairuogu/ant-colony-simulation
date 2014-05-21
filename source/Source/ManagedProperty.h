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

// This class is cool template class for manageable property.
// Think of this as a barrel - it could contain some value(items) inside and it has maximum capacity.
// When putting something into it it makes sure it does not exceed maximum capacity,
// When taking something from it, it only gives the amount it could. 

template <class T> class ManagedProperty
{
	T m_value;
	T MAX_VALUE;
	T MIN_VALUE;	// Always zero

public:

	ManagedProperty(T value, T maxValue) : m_value(value), MAX_VALUE(maxValue), MIN_VALUE(0) { }

	// Returns current value
	T Value() { return m_value; }

	// Returns free space in container
	T FreeSpace() { return MAX_VALUE - m_value; }

	// Adds new value to current value. Returns remainder.
	T Add(T add_value)
	{
		if (T <= 0) throw std::exception("Can't add zero/negative amount");

		m_value += add_value;
		if (m_value > MAX_VALUE)
		{
			T remainder = m_value - MAX_VALUE;
			m_value = MAX_VALUE;
			return remainder;
		}
		else return static_cast<T>(0);
	}

	// Requests given value from storage. Return maximum ammoun it could give (0 if nothing)
	T Take(T request)
	{
		if (T <= 0) throw std::exception("Can't add zero/negative amount");

		T amount_given;

		if (request > m_value)
		{
			amount_given = m_value - MIN_VALUE;
			m_value = MIN_VALUE;
		}
		else
		{
			amount_given = request;
			m_value -= request;
		}

		return amount_given;
	}

	bool Empty() { if(abs(m_value - MIN_VALUE) < 0.001f) return true; else return false; }

	bool Full() { if(abs(m_value - MAX_VALUE) < 0.001f) return true; else return false; }
};

template <class T>
ManagedProperty<T>& operator<<(ManagedProperty<T>& to_add, ManagedProperty<T>& to_take)
{
	// That is how maximum object can take to be 100% full
	T request = to_add.FreeSpace();

	// That is how much object we are taking from can give
	T can_give = to_take.Take(request);

	// Adding value we got to object we want to fill
	to_add.Add(can_give);

	return to_add;
}

template <class T>
ManagedProperty<T>& operator>>(ManagedProperty<T>& to_take, ManagedProperty<T>& to_add)
{
	// That is how maximum object can take to be 100% full
	T request = to_add.FreeSpace();

	// That is how much object we are taking from can give
	T can_give = to_take.Take(request);

	// Adding value we got to object we want to fill
	to_add.Add(can_give);

	return to_take;
}