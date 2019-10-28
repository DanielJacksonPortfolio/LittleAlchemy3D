#pragma once

#include "Game.h"

class Game;//Forward declare Game

namespace Callbacks
{
	void Init(Game * gl);
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

}
