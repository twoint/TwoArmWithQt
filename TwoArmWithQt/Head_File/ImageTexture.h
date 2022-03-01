#pragma once
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iostream>

class ImageTexture
{
public:
	GLuint textureId;
	void createTexture(char const *filename);

};

