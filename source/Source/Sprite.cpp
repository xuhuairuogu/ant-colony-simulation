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

#include "Sprite.h"
#include "GL/GXBase/GXBase.h"

#pragma region Constructors

Sprite::Sprite(void)
{
}

Sprite::Sprite(uint texture, const float2 &position, float radius, int quadSheetQuanInTex) :
		m_state(SpriteState::normal),
		m_position(position),
		m_size(radius * 2.0f, radius * 2.0f),
		m_angle(0.0f),
		m_color(1.0f, 1.0f, 1.0f, 1.0f),
		m_layerDepth(0.0f),

		m_quadSheet(0),
		m_currentQuadSheet(0),
		m_quadSheetQuanInTex(quadSheetQuanInTex),

		m_texture(texture),
		m_textureQuadSize(1.0f / m_quadSheetQuanInTex),
		m_sourceRectangle(0.0f, 0.0f, m_textureQuadSize, m_textureQuadSize),
		m_vertices(0),
		
		m_fadeOutTime(0.0f),
		m_fadeInTime(0.0f),		
		m_fadeTimeMax(0.0f),
		
		m_speed(0.0f),
		m_speedMax(0.0f)
{
	m_vertices = new Vertex2D[4];
	//m_color = float4(1.0f, 1.0f, 1.0f, 1.0f);
}

Sprite::Sprite(uint texture, const float2 &position, float radius) :
		m_state(SpriteState::normal),
		m_position(position),
		m_size(radius * 2.0f, radius * 2.0f),
		m_angle(0.0f),
		m_color(1.0f, 1.0f, 1.0f, 1.0f),
		m_layerDepth(0.0f),

		m_quadSheet(0),
		m_currentQuadSheet(0),
		m_quadSheetQuanInTex(1),

		m_texture(texture),
		m_textureQuadSize(1.0f / m_quadSheetQuanInTex),
		m_sourceRectangle(0.0f, 0.0f, m_textureQuadSize, m_textureQuadSize),
		m_vertices(0),
		
		m_fadeOutTime(0.0f),
		m_fadeInTime(0.0f),		
		m_fadeTimeMax(0.0f),
		
		m_speed(0.0f),
		m_speedMax(0.0f)
{
	AddQuadSheet(0);
	m_vertices = new Vertex2D[4];
	//m_texture = texture;
}
	
Sprite::~Sprite(void)
{
	//delete[] m_quadSheet;
	delete[] m_vertices;
	m_quadSheet.clear();
}

#pragma endregion
#pragma region Public Methods

void Sprite::CalculateSourceRect(void)
{
	m_sourceRectangle.r = (m_quadSheet[m_currentQuadSheet].GetCurrent() % m_quadSheetQuanInTex) * m_textureQuadSize;
	m_sourceRectangle.g = (m_quadSheet[m_currentQuadSheet].GetCurrent() / m_quadSheetQuanInTex) * m_textureQuadSize;
	m_sourceRectangle.b = m_sourceRectangle.r + m_textureQuadSize;
	m_sourceRectangle.a = m_sourceRectangle.g + m_textureQuadSize;
}

void Sprite::CreateVertices(void)
{
	// Creating sprite vertices
	m_vertices[0].x = -0.5f;
	m_vertices[0].y = -0.5f;
	m_vertices[0].z = m_layerDepth;
	m_vertices[0].u = m_sourceRectangle.x;
	m_vertices[0].v = m_sourceRectangle.y;
	m_vertices[0].r = m_color.r;
	m_vertices[0].g = m_color.g;
	m_vertices[0].b = m_color.b;
	m_vertices[0].a = m_color.a;

	m_vertices[1].x = 0.5f;
	m_vertices[1].y = -0.5f;
	m_vertices[1].z = m_layerDepth;
	m_vertices[1].u = m_sourceRectangle.z;
	m_vertices[1].v = m_sourceRectangle.y;
	m_vertices[1].r = m_color.r;
	m_vertices[1].g = m_color.g;
	m_vertices[1].b = m_color.b;
	m_vertices[1].a = m_color.a;

	m_vertices[2].x = -0.5f;
	m_vertices[2].y = 0.5f;
	m_vertices[2].z = m_layerDepth;
	m_vertices[2].u = m_sourceRectangle.x;
	m_vertices[2].v = m_sourceRectangle.w;
	m_vertices[2].r = m_color.r;
	m_vertices[2].g = m_color.g;
	m_vertices[2].b = m_color.b;
	m_vertices[2].a = m_color.a;

	m_vertices[3].x = 0.5f;
	m_vertices[3].y = 0.5f;
	m_vertices[3].z = m_layerDepth;
	m_vertices[3].u = m_sourceRectangle.z;
	m_vertices[3].v = m_sourceRectangle.w;
	m_vertices[3].r = m_color.r;
	m_vertices[3].g = m_color.g;
	m_vertices[3].b = m_color.b;
	m_vertices[3].a = m_color.a;
}

void Sprite::AddQuadSheet(int startQuad)
{
	m_quadSheet.push_back(QuadSheet(startQuad));
	m_currentQuadSheet = m_quadSheet.size() - 1;
	CalculateSourceRect();
}

void Sprite::AddQuadSheet(int startQuad, int endQuad, float animInterval)
{
	m_quadSheet.push_back(QuadSheet(startQuad, endQuad, animInterval));
	m_currentQuadSheet = m_quadSheet.size() - 1;
	CalculateSourceRect();
}

void Sprite::FadeOut(float fadeOutTime)
{
	m_state = SpriteState::fadingOut;
	m_fadeOutTime = fadeOutTime;
	m_fadeTimeMax = fadeOutTime;
}

void Sprite::FadeIn(float fadeInTime)
{
	m_state = SpriteState::fadingIn;
	m_fadeInTime = 0;
	m_fadeTimeMax = fadeInTime;
}

void Sprite::FlashStart(float fadeInTime, float fadeOutTime)
{
	m_state = SpriteState::fadingIn;
	m_fadeInTime = 0;
	m_fadeTimeMax = fadeInTime;
	m_fadeOutTime = fadeOutTime;	
}

void Sprite::StartFloat(float speedMax)
{
	m_speedMax = speedMax;
	m_speed = 0.0f;
	m_state = floating;
}

const float4& Sprite::GetRect() const
{
	float4* rect = new float4(m_position.x - m_size.x / 2, m_position.y - m_size.y / 2, m_size.x, m_size.y);
	return *rect;
}

#pragma endregion
#pragma region Draw Methods

void Sprite::Draw(void)
{
	CreateVertices();

	glPushMatrix();
	{
		glTranslatef(m_position.x, m_position.y, 1.0f);

		//Because stupid old OpenGL receiving angle in degrees
		float angleInDegrees = RadianToDegree(m_angle);
		glRotatef(angleInDegrees, 0.0f, 0.0f, 1.0f);

		glScalef(m_size.x, m_size.y, 1.0f);

		glBindTexture(GL_TEXTURE_2D, m_texture);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex2D), &m_vertices[0].x);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex2D), &m_vertices[0].u);

		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, sizeof(Vertex2D), &m_vertices[0].r);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	glPopMatrix();

	// Not sure these two lines are necessary
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}

#pragma endregion
#pragma region Update Methods

const bool Sprite::Update(float timeDelta)
{
	if (m_quadSheet[m_currentQuadSheet].Update(timeDelta))
	{
		CalculateSourceRect();
	}

	switch (m_state)
	{
		case SpriteState::normal:
			return 0;
		case SpriteState::fadingOut:
			m_fadeOutTime -= timeDelta;
			if (m_fadeOutTime >= 0)
			{
				m_color.a = 1.0f * (m_fadeOutTime / m_fadeTimeMax);
				//float color = 255 * (m_fadeOutTime / m_fadeTimeMax);
				//m_color = float4(color, color, color, color);
				return 0;
			}
			else return 1;
		case SpriteState::fadingIn:
			m_fadeInTime += timeDelta;
			if (m_fadeInTime <= m_fadeTimeMax)
			{
				m_color.a = 1.0f * (m_fadeInTime / m_fadeTimeMax);
				//m_color = float4(color, color, color, color);
				return 0;
			}
			else
			{
				m_color = float4(1.0f, 1.0f, 1.0f, 1.0f);
				if (m_fadeOutTime > 0.1f)
				{
					FadeOut(m_fadeOutTime);
				}
				return 0;
			}
		case SpriteState::floating:
			m_speed += m_speedMax * 1.8f * timeDelta;
			if (abs(m_speed) > abs(m_speedMax)) m_speedMax = -m_speedMax;
			m_position.y += m_speed;
			return 0;
		default:
			return 0;

	}
}

const bool Sprite::Update(float timeDelta, const float2 &position)
{
	m_position = position;

	return Update(timeDelta);
}

const bool Sprite::Update(float timeDelta, int quadSheetID)
{
	if (quadSheetID != m_currentQuadSheet)
	{
		m_currentQuadSheet = quadSheetID;
		CalculateSourceRect();
	}

	return Update(timeDelta);
}

const bool Sprite::Update(float timeDelta, float angle)
{
	m_angle = angle;

	return Update(timeDelta);
}

const bool Sprite::Update(float timeDelta, const float2 &position, int quadSheetID, float angle)
{
	if (quadSheetID != m_currentQuadSheet)
	{
		m_currentQuadSheet = quadSheetID;
		CalculateSourceRect();
	}

	m_position = position;
	m_angle = angle;

	return Update(timeDelta);
}

const bool Sprite::Update(float timeDelta, const float2 &position, float angle)
{
	m_position = position;
	m_angle = angle;

	return Update(timeDelta);
}

#pragma endregion