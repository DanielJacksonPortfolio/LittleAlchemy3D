#pragma once

#include <glm.hpp>

#include <vector>
#include <unordered_map>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>

#include "GUIQuad.h"

struct Coordi;
struct intPair;

struct Coordi
{
	int x = 0;
	int y = 0;
	Coordi(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

struct intPair
{
	int a = 0, b = 0;
	intPair(int x, int y)
	{
		this->a = x;
		this->b = y;
	}
};

enum TextRenderModes {SOLID,SHADED,BLENDED,WRAPPED};
enum TextDisplayModes {CUTOFF, DIMENSION_BASED,DISABLE_RESTRAINTS, CONTAINER};
enum DimensionType {DIMENSION_ABSOLUTE, DIMENSION_PERCENTAGE};
enum ColourMode {TRANSPARENT,FILLED};
enum DrawMode {HOVER,BASE};
enum ScrollMode {LIST,SMOOTH};
enum WidgetType {PAGE,BUTTON,TEXT,IMAGE,STATIC_LIST,SCROLLING_LIST,GRID,SEARCH};
enum AlignmentType {ALIGNMENT_ABSOLUTE,TOP_LEFT,TOP_RIGHT,CENTRE,TOP,BOTTOM_LEFT,BOTTOM_RIGHT,BOTTOM,LEFT,RIGHT};
enum PositionType {POSITION_ABSOLUTE, POSITION_PERCENTAGE_LEFT, POSITION_PERCENTAGE_RIGHT, POSITION_PERCENTAGE_CENTRE};
enum ImageLoadModes {STRETCH,ORIGINAL,EXACT};
enum RowModes { WIDGET, PIXEL, PERCENTAGE };
enum FunctionModes { VOID, STRING, INT };

static std::unordered_map<std::string, TextRenderModes>		TextRenderModesMap =	{ {"SOLID",SOLID},{"SHADED",SHADED},{"BLENDED",BLENDED},{"WRAPPED",WRAPPED } };
static std::unordered_map<std::string, ImageLoadModes>		ImageLoadModesMap =		{ {"STRETCH",STRETCH},{"ORIGINAL",ORIGINAL},{"EXACT",EXACT}};
static std::unordered_map<std::string, TextDisplayModes>	TextDisplayModesMap =	{ {"CUTOFF",CUTOFF},{"DIMENSION_BASED",DIMENSION_BASED},{"DISABLE_RESTRAINTS",DISABLE_RESTRAINTS},{"CONTAINER",CONTAINER } };
static std::unordered_map<std::string, DimensionType>		DimensionTypeMap =		{ {"DIMENSION_ABSOLUTE",DIMENSION_ABSOLUTE},{"DIMENSION_PERCENTAGE",DIMENSION_PERCENTAGE}};
static std::unordered_map<std::string, ColourMode>			ColourModeMap =			{ {"TRANSPARENT",TRANSPARENT},{"FILLED",FILLED} };
static std::unordered_map<std::string, DrawMode>			DrawModeMap =			{ {"HOVER",HOVER},{"BASE",BASE} };
static std::unordered_map<std::string, ScrollMode>			ScrollModeMap =			{ {"LIST",LIST},{"SMOOTH",SMOOTH} };
static std::unordered_map<std::string, AlignmentType>		AlignmentTypeMap =		{ {"BOTTOM_RIGHT",BOTTOM_RIGHT},{"BOTTOM",BOTTOM},{"BOTTOM_LEFT",BOTTOM_LEFT},{"CENTRE",CENTRE},{"RIGHT",RIGHT},{"LEFT",LEFT },{"TOP",TOP},{"TOP_RIGHT",TOP_RIGHT},{"TOP_LEFT",TOP_LEFT},{"ALIGNMENT_ABSOLUTE",ALIGNMENT_ABSOLUTE } };
static std::unordered_map<std::string, PositionType>		PositionTypeMap =		{ {"POSITION_ABSOLUTE",POSITION_ABSOLUTE},{"POSITION_PERCENTAGE_CENTRE",POSITION_PERCENTAGE_CENTRE},{"POSITION_PERCENTAGE_RIGHT",POSITION_PERCENTAGE_RIGHT},{"POSITION_PERCENTAGE_LEFT",POSITION_PERCENTAGE_LEFT } };
static std::unordered_map<std::string, glm::vec3>			Colours = { {"WHITE",glm::vec3(1.0f,1.0f,1.0f)}, {"BLACK",glm::vec3(0.0f,0.0f,0.0f)}, {"RED",glm::vec3(1.0f,0.0f,0.0f)}, {"BLUE",glm::vec3(0.0f,0.0f,1.0f)}, {"GREEN",glm::vec3(0.0f,1.0f,0.0f)}, {"YELLOW",glm::vec3(1.0f,1.0f,0.0f)}, {"CYAN",glm::vec3(0.0f,1.0f,1.0f)}, {"MAGENTA",glm::vec3(1.0f,0.0f,1.0f)}, { "PURPLE1", glm::vec3(0.2196078f, 0.0f, 0.1568627f) }, { "PURPLE2",glm::vec3(0.3215686f, 0.0627451f, 0.2470588f) }, { "YELLOW1",glm::vec3(0.9803922f, 0.6509804f, 0.1254902f) } };
static std::unordered_map<std::string, FunctionModes>		FunctionModesMap = { {"VOID", VOID}, {"STRING", STRING},{"INT",INT} };

class StaticMenuFuncs {
public:
	static int roundUp(int numToRound, int multiple)
	{
		if (multiple == 0)
		{
			return numToRound;
		}

		int const remainder = numToRound % multiple;
		if (remainder == 0)
		{
			return numToRound;
		}

		return numToRound + multiple - remainder;
	}

	static std::vector<std::string> SplitString(std::string stringToSplit, char delimeter)
	{
		std::stringstream stringStream(stringToSplit);
		std::string splitString;
		std::vector<std::string> splitStrings;
		while (std::getline(stringStream, splitString, delimeter))
			splitStrings.push_back(splitString);
		return splitStrings;
	}
};
