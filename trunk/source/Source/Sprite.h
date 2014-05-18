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
#include "QuadSheet.h"
#include "Vertex.h"
#include <vector>

using namespace std;

enum SpriteState
{
	normal,
	fadingIn,
	fadingOut,
	floating
};

class Sprite
{
public:

	Sprite();
	Sprite(uint texture, const float2 &position, float radius, int quadSheetQuanInTex);
	Sprite(uint texture, const float2 &position, float radius);
	//Sprite(const Sprite& copySprite);
	~Sprite(void);

	const bool Update(float timeDelta);
	const bool Update(float timeDelta, const float2 &position);
	const bool Update(float timeDelta, int quadSheetID);
	const bool Update(float timeDelta, float angle);
	const bool Update(float timeDelta, const float2 &position, int quadSheetID, float angle);
	const bool Update(float timeDelta, const float2 &position, float angle);

	void Draw(void);

	void AddQuadSheet(int startQuad, int endQuad, float animInterval);
	void AddQuadSheet(int startQuad);

	void FadeOut(float fadeOutTime);
	void FlashStart(float fadeInTime, float fadeOutTime);
	void FadeIn(float fadeInTime);
	void StartFloat(float speedMax);

	const float4& GetRect() const;
	void SetPos(const float2& new_pos) { m_position = new_pos; };
	void SetColor(const float4& new_color) { m_color = new_color; };
	void SetLayerDepth(float new_depth) { m_layerDepth = new_depth; };

private:

	// General Properties
	SpriteState m_state;
	float2 m_position;
	float2 m_size;
	float m_angle;
	float4 m_color;
	float m_layerDepth;
	
	// Quad sheet information
	vector<QuadSheet> m_quadSheet;
	uint m_currentQuadSheet;
	int m_quadSheetQuanInTex;

	// Texture information	
	uint m_texture;
	float m_textureQuadSize;
	float4 m_sourceRectangle;
	Vertex2D* m_vertices;

	// Fading state variables
	float m_fadeOutTime;
	float m_fadeInTime;
	float m_fadeTimeMax;

	// Floating state variables
	float m_speed;
	float m_speedMax;

	// Methods
	void CalculateSourceRect();
	void CreateVertices();
};

