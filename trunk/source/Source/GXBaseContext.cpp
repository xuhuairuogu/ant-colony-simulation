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

#include "GXBaseContext.h"
#include "AntTweakBar/AntTweakBar.h"
#include "GL/GXBase/glex.h"

#include <GL\GLUT\glut.h>

GXBaseContext::GXBaseContext(void) :
	m_camera(new Camera(float2(0.0f, 0.0f), 0.0f, 1.0f)),
	m_mouse_old(0.0f, 0.0f),
	m_camera_drag(0)
{
	SetSize(800, 600);
}

GXBaseContext::~GXBaseContext(void) 
{
}


// GLWindow event handlers
void GXBaseContext::OnCreate(void) 
{
	Setup2D();

	//Setup TweakBar
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(Width(), Height());
}

void GXBaseContext::OnDestroy(void)
{
	TwTerminate();
	delete m_camera;
}

void GXBaseContext::OnResize(int w, int h)
{
	GLWindow::OnResize(w, h);

	Setup2D();

	//REsize tweakbar
	TwWindowSize(w, h);
}

void GXBaseContext::OnIdle(void)
{
	Redraw();
}

void GXBaseContext::OnKeyboard(int key, bool down)
{
	if (down)
	{
		int kmod = 0;

		if(GetShiftKey())	kmod |= TW_KMOD_SHIFT;
		if(GetCtrlKey())	kmod |= TW_KMOD_CTRL;
		if(GetAltKey())		kmod |= TW_KMOD_ALT;

		if( (kmod&TW_KMOD_CTRL) && (key>0 && key<27) )  // CTRL special case
			key += 'a'-1;

		if (!TwKeyPressed(key, kmod))
		{
			// Process keyboard event inside application
			if(VK_UP==key)if(m_camera->m_zoom--<=1)m_camera->m_zoom=1;
			if(VK_DOWN==key)if(m_camera->m_zoom++>=100)m_camera->m_zoom=100;
		}
	}
}

void GXBaseContext::OnMouseMove(int x, int y)
{
	if (!TwMouseMotion(x, Height() - y))
	{
		float2 mouse_pos = float2(Width() - x, y);
		float2 windows_size = float2(Width(), Height());

		// Process mouse movement inside application
		if (m_camera_drag)
		{
			float2 mouse_offset = mouse_pos - m_mouse_old;
			float2 mouse_offset_normalized = mouse_offset / windows_size;
			float2 mouse_offset_zoomed = mouse_offset_normalized;

			m_camera->m_pos -= mouse_offset_zoomed;
		}

		m_mouse_old = float2(Width() - x, y);
	}
}

void GXBaseContext::OnMouseButton(MouseButton button, bool down)
{
	TwMouseAction twaction = down ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;

	ETwMouseButtonID twbutton;

	switch (button){
		case gxbase::GLWindow::MBLeft:		twbutton = TW_MOUSE_LEFT;	break;
		case gxbase::GLWindow::MBMiddle:	twbutton = TW_MOUSE_MIDDLE;	break;
		case gxbase::GLWindow::MBRight:		twbutton = TW_MOUSE_RIGHT;	break;
		default: twbutton = TW_MOUSE_LEFT;	break;
	}

	if (!TwMouseButton(twaction, twbutton))
	{
		// Process mouse button inside application
		if (down) m_camera_drag = true;
		else m_camera_drag = false;
	}
}

// Other methods
void GXBaseContext::PresentScreen(void) {
	SwapBuffers();
}

LRESULT CALLBACK WndProc( HWND, UINT message, WPARAM wParam, LPARAM)
{
	switch (message)
	{
	case WM_MOUSEWHEEL:
		GET_WHEEL_DELTA_WPARAM(wParam);
		if (wParam > 0)
		{
			int i = 5;
		}
		else
		{
			int i = 6;
		}
		break;
	default:
		break;
	}

	return 0;
}

// Coordinates for text is 0-1 0-1
void GXBaseContext::Print(const char* output, const float2& pos) 
{
	glDisable(GL_BLEND);

	glRasterPos2f(pos.x, pos.y);
	Printf("%s", output);

	glEnable(GL_BLEND);
}

void GXBaseContext::Setup2D(void)
{
	//Setup 2D
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();

	float aspect = Aspect();
	//glOrtho(-0.5f * aspect, 0.5f * aspect, 0.5f, -0.5f, -1.0, 1.0);
	glOrtho(-0.5f, 0.5f, 0.5f, -0.5f, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
}

#endif