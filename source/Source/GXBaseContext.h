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

#include "GL/GXBase/GXBase.h"
#include "BasicMath.h"
#include "Camera.h"
using namespace gxbase;

class GXBaseContext : public GLWindow 
{
public:
	GXBaseContext(void);
	virtual ~GXBaseContext(void);

	// GLWindow.h handlers
	virtual void OnCreate(void);
	virtual void OnDestroy(void);
	virtual void OnIdle(void);
	virtual void OnKeyboard(int key, bool down);
	virtual void OnResize(int w, int h);
	virtual void OnMouseMove(int x, int y);
	virtual void OnMouseButton(MouseButton button, bool down);

	// Other helper methods
	virtual void PresentScreen(void);
	virtual void Print(const char* output, const float2& pos);
	virtual void Setup2D(void);
	LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

protected:
	Camera* m_camera;

	float2 m_mouse_old;
	bool m_camera_drag;
};

#endif