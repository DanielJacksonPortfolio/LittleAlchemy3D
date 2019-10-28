#include "Callbacks.h"

namespace Callbacks
{
	namespace
	{
		Game * game = nullptr;
	}
	void Init(Game * gl)
	{
		game = gl;
	}

	void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		game->FramebufferSizeCallback(window, width, height);
	}

	void MouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		game->MouseCallback(window, xpos, ypos);
	}

	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		game->ScrollCallback(window, xoffset, yoffset);
	}

	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		game->MouseButtonCallback(window, button, action, mods);
	}
}