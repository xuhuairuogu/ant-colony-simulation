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

#include "ResourceManager.h"
#include "GL/GXBase/GXBase.h"
#include "lodepng.h"
#include <iostream>

using namespace gxbase;

uint ResourceManager::LoadTexture(const char* name)
{
	std::string name_s = string(name);
	std::string file_extension = name_s.substr(name_s.find_last_of(".") + 1);

	if (file_extension == "png")
	{
		return LoadTexturePNG(name);
	}
	else if (file_extension == "bmp")
	{
		return LoadTextureBMP(name);
	}
	else
	{
		std::cout << "Unsupported file type passed into Load Texture method" << std::endl;
		return 0;
	}
}

uint ResourceManager::LoadTextureBMP(const char* name)
{   
	uint texture = 0;
	// set up texturing for linear mip-mapping
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLuint tex = 0;
	glGenTextures(1, &tex);
	texture = static_cast<uint>(tex);
	glBindTexture(GL_TEXTURE_2D, texture);

	//// load image
	Image img;
	img.Load(name);
	img.FlipY();
	// build 2D mip-maps from image
	img.gluBuild2DMipmaps();
	img.Free();	
		
	return texture;
}

uint ResourceManager::LoadTexturePNG(const char* name)
{
	// Load and decode image
	std::vector<unsigned char> image;
	uint width, height;
	uint error = lodepng::decode(image, width, height, name);

	// If there's an error, display it.
	if(error != 0) {
		std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
		return 0;
	}
		
	glEnable(GL_TEXTURE_2D);

	// Bing texture
	GLuint texture = 0;	
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );

	// Set up texturing for linear mip-mapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return static_cast<uint>(texture);
}