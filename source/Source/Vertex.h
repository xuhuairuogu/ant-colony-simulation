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

typedef struct {
	float x;		// position x
	float y;		// position y
	float z;		// position z
	float u;		// texture u
	float v;		// texture v
	float r;		// colour r
	float g;		// colour g
	float b;		// colour b
	float a;		// colour alfa
} Vertex2D;

typedef struct {
	float x;
	float y;
	float r;		// colour r
	float g;		// colour g
	float b;		// colour b
} LineVertex2D;

// Dynamic C-style array
template <class T> struct DynamicArray {
	DynamicArray() :
		m_first(0),
		m_size(0)
	{
	}
	DynamicArray(T* in_first, uint in_size) :
		m_first(in_first),
		m_size(in_size)
	{

	}
	DynamicArray(const DynamicArray<T>& copy_me)
	{
		m_size = copy_me.m_size;
		m_first = new T[m_size];
		for (int i = 0; i < m_size; ++i)
		{
			m_first[i] = copy_me.m_first[i];
		}
	}
	DynamicArray<T>& operator= (const DynamicArray<T>& copy_me)
	{
		if (this != &copy_me)
		{
			delete[] m_first;
			m_size = copy_me.m_size;
			m_first = new T[m_size];
			for (int i = 0; i < m_size; ++i)
			{
				m_first[i] = copy_me.m_first[i];
			}
		}
		return *this;
	}

	~DynamicArray()
	{
		delete[] first;
	}

	T* m_first;	// C-style array
	uint m_size;
};