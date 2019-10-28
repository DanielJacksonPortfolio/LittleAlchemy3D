#include "Menu.h"

Menu::Menu(GLFWwindow* window, ElementManager* elementManager, ItemManager* itemManager)
{
	this->window = window;
	this->elementManager = elementManager;
	this->itemManager = itemManager;
	LoadContent();
}

Menu::~Menu()
{

}

bool Menu::LoadTextures() {
	Textures.insert({ "PACMAN", new GUITexture("res/images/gui_textures/pacman_map.png") });
	Textures.insert({ "EARTH", new GUITexture("res/images/gui_textures/earth.png") });
	Textures.insert({ "FIRE", new GUITexture("res/images/gui_textures/fire.png") });
	Textures.insert({ "PENCIL", new GUITexture("res/images/gui_textures/pencil.png") });
	Textures.insert({ "LA2", new GUITexture("res/images/gui_textures/la2.png") });
	Textures.insert({ "TOOL", new GUITexture("res/images/gui_textures/tool.png") });
	Textures.insert({ "TIME", new GUITexture("res/images/gui_textures/time.png") });
	Textures.insert({ "SEARCH", new GUITexture("res/images/gui_textures/search.png") });
	return true;
}

bool Menu::LoadFonts() {
	Font* newFont = new Font();
	newFont->LoadFont("monofont");
	Fonts.insert({ "DEFAULT", newFont });
	return true;
}

bool Menu::LoadContent()
{
	LoadTextures();
	LoadFonts();

	vFunctionMap["Null"] = [&, this]() { Null(); };
	vFunctionMap["Quit"] = [&, this]() { Quit(); };
	vFunctionMap["Settings"] = [&, this]() { Settings(); };
	vFunctionMap["Play Game"] = [&, this]() { PlayGame(); };
	vFunctionMap["Load Menu"] = [&, this]() { LoadMenu(); };
	vFunctionMap["Load Save"] = [&, this]() { LoadSave(); };
	vFunctionMap["Resume"] = [&, this]() { Resume(); };
	vFunctionMap["Main Menu"] = [&, this]() { MainMenu(); };
	sFunctionMap["Spawn Element"] = [&, this](std::string string) { SpawnElement(string); };
	sFunctionMap["Null"] = [&, this](std::string string) { Null(string); };
	sFunctionMap["Set Save"] = [&, this](std::string string) { SetSave(string); };

	widgetList["SCREEN"] = new Widget();
	{
	Add("SCREEN", "MAINMENU", PAGE, { {"colourMode", "TRANSPARENT"} });
	Add("MAINMENU", "MENUBOX", STATIC_LIST, { {"width", "45"}, {"height", "45"}, {"backgroundColour", "PURPLE1"}, {"positionX", "50"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "50"}, {"positionModeY", "POSITION_PERCENTAGE_LEFT"} });
	Add("MAINMENU", "TITLETEXT1", TEXT, { {"targetTextHeight", "150"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", " LITTLE"}, {"positionX", "5"}, {"positionModeX", "POSITION_PERCENTAGE_LEFT"}, {"positionY", "10"}, {"positionModeY", "POSITION_PERCENTAGE_CENTRE"}, {"width", "40"}, {"height", "20"}, {"baseColour", "YELLOW1"} });
	Add("MAINMENU", "TITLETEXT2", TEXT, { {"targetTextHeight", "150"}, 	{"textDisplayMode", "DIMENSION_BASED"}, {"text", "ALCHEMY"}, {"positionX", "95"}, {"positionModeX", "POSITION_PERCENTAGE_RIGHT"}, {"positionY", "10"}, {"positionModeY", "POSITION_PERCENTAGE_CENTRE"}, {"width", "40"}, {"height", "20"} , {"baseColour", "YELLOW1"} });
	Add("MAINMENU", "TITLETEXT3", TEXT, { {"textDisplayMode", "DIMENSION_BASED"}, {"text", "3D"}, {"positionX", "50"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "18"}, {"positionModeY", "POSITION_PERCENTAGE_LEFT"}, {"width", "25"}, {"height", "25"} , {"baseColour", "YELLOW1"} });
	Add("MAINMENU", "LA2", IMAGE, { {"width", "175"}, {"height", "175"}, {"widthMode", "DIMENSION_ABSOLUTE"}, {"heightMode", "DIMENSION_ABSOLUTE"}, {"imageName", "LA2"}, {"positionX", "50"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "10"}, {"positionModeY", "POSITION_PERCENTAGE_CENTRE"}, {"imageLoadMode", "EXACT"}, {"rectW", "225"}, {"rectH", "225"}, {"rectX", "0"}, {"rectY", "0"} });
	Add("MAINMENU", "SMALLPRINT", TEXT, { {"textDisplayMode", "DIMENSION_BASED"}, {"text", "Created by; Danny Jackson, Fergus Griggs, Niha Patel"}, {"positionX", "1"}, {"positionModeX", "POSITION_PERCENTAGE_LEFT"}, {"positionY", "99"}, {"positionModeY", "POSITION_PERCENTAGE_RIGHT"}, {"width", "40"}, {"height", "5"}, {"baseColour", "YELLOW1"} });
	Add("MENUBOX", "NEWGAMEBUTTON", BUTTON, { {"width", "95"}, {"height", "15"}, {"vFunctionID", "Play Game"}, {"backgroundColour", "PURPLE2"} });
	Add("NEWGAMEBUTTON", "NEWGAMETEXT", TEXT, { {"targetTextHeight", "35"}, {"textDisplayMode", "CONTAINER"}, {"text", "NEW GAME"}, {"alignment", "CENTRE"}, {"baseColour", "YELLOW1"} });
	Add("NEWGAMEBUTTON", "NEWGAMEIMAGE1", IMAGE, { {"width", "65"}, {"height", "65"}, {"widthMode", "DIMENSION_ABSOLUTE"}, {"heightMode", "DIMENSION_ABSOLUTE"}, {"imageName", "EARTH"}, {"positionX", "25"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "50"}, {"positionModeY", "POSITION_PERCENTAGE_CENTRE"}, {"imageLoadMode", "STRETCH"} });
	Add("NEWGAMEBUTTON", "NEWGAMEIMAGE2", IMAGE, { {"width", "65"}, {"height", "65"}, {"widthMode", "DIMENSION_ABSOLUTE"}, {"heightMode", "DIMENSION_ABSOLUTE"}, {"imageName", "EARTH"}, {"positionX", "75"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "50"}, {"positionModeY", "POSITION_PERCENTAGE_CENTRE"}, {"imageLoadMode", "STRETCH"} });
	Add("MENUBOX", "LOADGAMEBUTTON", BUTTON, { {"width", "95"}, {"height", "15"}, {"vFunctionID", "Load Menu"}, {"backgroundColour", "PURPLE2"} });
	Add("LOADGAMEBUTTON", "LOADGAMETEXT", TEXT, { {"targetTextHeight", "35"}, {"textDisplayMode", "CONTAINER"}, {"text", "LOAD GAME"}, {"alignment", "CENTRE"}, {"baseColour", "YELLOW1"} });
	Add("LOADGAMEBUTTON", "LOADGAMEIMAGE1", IMAGE, { {"width", "65"}, {"height", "65"}, {"widthMode", "DIMENSION_ABSOLUTE"}, {"heightMode", "DIMENSION_ABSOLUTE"}, {"imageName", "TIME"}, {"positionX", "25"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "50"}, {"positionModeY", "POSITION_PERCENTAGE_CENTRE"}, {"imageLoadMode", "STRETCH"} });
	Add("LOADGAMEBUTTON", "LOADGAMEIMAGE2", IMAGE, { {"width", "65"}, {"height", "65"}, {"widthMode", "DIMENSION_ABSOLUTE"}, {"heightMode", "DIMENSION_ABSOLUTE"}, {"imageName", "TIME"}, {"positionX", "75"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "50"}, {"positionModeY", "POSITION_PERCENTAGE_CENTRE"}, {"imageLoadMode", "STRETCH"} });
	Add("MENUBOX", "SETTINGSBUTTON", BUTTON, { {"width", "95"}, {"height", "15"}, {"vFunctionID", "Settings"}, {"backgroundColour", "PURPLE2"} });
	Add("SETTINGSBUTTON", "SETTINGSTEXT", TEXT, { {"targetTextHeight", "35"}, {"textDisplayMode", "CONTAINER"}, {"text", "SETTINGS"}, {"alignment", "CENTRE"}, {"baseColour", "YELLOW1"} });
	Add("SETTINGSBUTTON", "SETTINGSIMAGE1", IMAGE, { {"width", "60"}, {"height", "60"}, {"widthMode", "DIMENSION_ABSOLUTE"}, {"heightMode", "DIMENSION_ABSOLUTE"}, {"imageName", "TOOL"}, {"positionX", "25"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "50"}, {"positionModeY", "POSITION_PERCENTAGE_CENTRE"}, {"imageLoadMode", "STRETCH"} });
	Add("SETTINGSBUTTON", "SETTINGSIMAGE2", IMAGE, { {"width", "60"}, {"height", "60"}, {"widthMode", "DIMENSION_ABSOLUTE"}, {"heightMode", "DIMENSION_ABSOLUTE"}, {"imageName", "TOOL"}, {"positionX", "75"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "50"}, {"positionModeY", "POSITION_PERCENTAGE_CENTRE"}, {"imageLoadMode", "STRETCH"} });
	Add("MENUBOX", "CREDITSBUTTON", BUTTON, { {"width", "95"}, {"height", "15"}, {"vFunctionID", "Null"}, {"backgroundColour", "PURPLE2"} });
	Add("CREDITSBUTTON", "CREDITSTEXT", TEXT, { {"targetTextHeight", "35"}, 	{"textDisplayMode", "CONTAINER"}, {"text", "CREDITS"}, {"alignment", "CENTRE"}, {"baseColour", "YELLOW1"} });
	Add("CREDITSBUTTON", "CREDITSIMAGE1", IMAGE, { {"width", "60"}, {"height", "60"}, {"widthMode", "DIMENSION_ABSOLUTE"}, {"heightMode", "DIMENSION_ABSOLUTE"}, {"imageName", "PENCIL"}, {"positionX", "25"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "50"}, {"positionModeY", "POSITION_PERCENTAGE_CENTRE"}, {"imageLoadMode", "STRETCH"} });
	Add("CREDITSBUTTON", "CREDITSIMAGE2", IMAGE, { {"width", "60"}, {"height", "60"}, {"widthMode", "DIMENSION_ABSOLUTE"}, {"heightMode", "DIMENSION_ABSOLUTE"}, {"imageName", "PENCIL"}, {"positionX", "75"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "50"}, {"positionModeY", "POSITION_PERCENTAGE_CENTRE"}, {"imageLoadMode", "STRETCH"} });
	Add("MENUBOX", "QUITBUTTON", BUTTON, { {"width", "95"}, {"height", "15"}, {"vFunctionID", "Quit"}, {"backgroundColour", "PURPLE2"} });
	Add("QUITBUTTON", "QUITTEXT", TEXT, { {"targetTextHeight", "35"}, 	{"textDisplayMode", "CONTAINER"}, {"text", "QUIT"}, {"alignment", "CENTRE"}, {"baseColour", "YELLOW1"} });
	Add("QUITBUTTON", "QUITIMAGE1", IMAGE, { {"width", "65"}, {"height", "65"}, {"widthMode", "DIMENSION_ABSOLUTE"}, {"heightMode", "DIMENSION_ABSOLUTE"}, {"imageName", "FIRE"}, {"positionX", "25"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "50"}, {"positionModeY", "POSITION_PERCENTAGE_CENTRE"}, {"imageLoadMode", "STRETCH"} });
	Add("QUITBUTTON", "QUITIMAGE2", IMAGE, { {"width", "65"}, {"height", "65"}, {"widthMode", "DIMENSION_ABSOLUTE"}, {"heightMode", "DIMENSION_ABSOLUTE"}, {"imageName", "FIRE"}, {"positionX", "75"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "50"}, {"positionModeY", "POSITION_PERCENTAGE_CENTRE"}, {"imageLoadMode", "STRETCH"} });
	widgetList["MENUBOX"]->AutoAdjust();
	}
	//grid
	Add("SCREEN", "EGRID", GRID, { {"width", "25"}, {"alignment", "RIGHT"}, {"backgroundColour", "PURPLE1"}, {"rowCount", "4"} });
	UpdateElementList();

	//pause menu
	{
		Add("SCREEN", "PAUSEMENUPAGE", PAGE, { {"backgroundColour", "YELLOW1"},{"width","50"}, {"positionX", "50"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"} });
		Add("PAUSEMENUPAGE", "PAUSEMENU", PAGE, { {"backgroundColour", "PURPLE2"},{"width","98"},{"height","98"}, {"alignment","CENTRE" } });
		Add("PAUSEMENU", "PAUSESUBPAGE", PAGE, { {"backgroundColour", "PURPLE2"},{"height","20" },{"alignment","TOP"} });

		Add("PAUSESUBPAGE", "PAUSETEXT", TEXT, { {"targetTextHeight", "240"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", " PAUSED "}, {"baseColour", "YELLOW1"}, {"alignment", "CENTRE"} });
		Add("PAUSEMENU", "PAUSELINE3", PAGE, { {"backgroundColour", "YELLOW1"},{"height","2" }, {"positionY", "20"}, {"positionModeY", "POSITION_PERCENTAGE_LEFT"} });

		Add("PAUSEMENU", "PAUSELIST", STATIC_LIST, { {"height", "78"},   {"backgroundColour", "PURPLE1"},{"positionY", "22"}, {"positionModeY", "POSITION_PERCENTAGE_LEFT"} });
		Add("PAUSELIST", "RESUMEGAMEBUTTON", BUTTON, { {"width", "95"}, {"height", "15"},   {"vFunctionID", "Resume"}, {"backgroundColour", "PURPLE2"} });
		Add("RESUMEGAMEBUTTON", "RESUMEGAMEBUTTONTEXT", TEXT, { {"targetTextHeight", "54"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", "RESUME GAME"}, {"alignment", "CENTRE"}, {"baseColour", "YELLOW1"} });
		Add("PAUSELIST", "PAUSESETTINGSBUTTON", BUTTON, { {"width", "95"}, {"height", "15"},   {"vFunctionID", "Settings"}, {"backgroundColour", "PURPLE2"} });
		Add("PAUSESETTINGSBUTTON", "PAUSESETTINGSBUTTONTEXT", TEXT, { {"targetTextHeight", "54"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", "SETTINGS"}, {"alignment", "CENTRE"}, {"baseColour", "YELLOW1"} });
		Add("PAUSELIST", "PAUSEQUITBUTTON", BUTTON, { {"width", "95"}, {"height", "15"},   {"vFunctionID", "Main Menu"}, {"backgroundColour", "PURPLE2"} });
		Add("PAUSEQUITBUTTON", "PAUSEQUITBUTTONTEXT", TEXT, { {"targetTextHeight", "54"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", "EXIT TO MENU"}, {"alignment", "CENTRE"}, {"baseColour", "YELLOW1"} });
		Add("PAUSELIST", "EXITBUTTON", BUTTON, { {"width", "95"}, {"height", "15"},   {"vFunctionID", "Quit"}, {"backgroundColour", "PURPLE2"} });
		Add("EXITBUTTON", "EXITBUTTONTEXT", TEXT, { {"targetTextHeight", "54"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", "EXIT TO DESKTOP"}, {"alignment", "CENTRE"}, {"baseColour", "YELLOW1"} });
		widgetList["PAUSELIST"]->AutoAdjust();
	}

	Add("SCREEN", "LOADPAGE", PAGE, { {"backgroundColour", "YELLOW1"} });
	Add("LOADPAGE", "LOADPAGEINNER", PAGE, { {"backgroundColour", "PURPLE2"},{"width","98"},{"alignment","CENTRE"},{"height","97"} });

	Add("LOADPAGEINNER", "LOADBOX", SCROLLING_LIST, { {"width", "36"}, {"height","85"}, {"backgroundColour", "PURPLE1"}, {"positionX", "13"}, {"positionModeX", "POSITION_PERCENTAGE_LEFT"}, {"positionY", "100"}, {"positionModeY", "POSITION_PERCENTAGE_RIGHT"} });
	for (int i = 0; i < 10; i++)
	{
		Add("LOADBOX", "LOAD " + std::to_string(i + 1) + " BUTTON", BUTTON, { {"width", "95"}, {"height", "15"},   {"sFunctionID", "Set Save"},{"functionMode","STRING"},  {"backgroundColour", "PURPLE2"} });
		Add("LOAD " + std::to_string(i + 1) + " BUTTON", "LOAD " + std::to_string(i + 1) + " TEXT", TEXT, { {"targetTextHeight", "54"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", "SAVE " + std::to_string(i + 1)}, {"alignment", "CENTRE"}, {"baseColour", "YELLOW1"} });
	}

	widgetList["LOADBOX"]->SetAllWidgets();
	Add("LOADPAGEINNER", "SAVETEXT", TEXT, { {"containerID","LOADBOX"},{"targetTextHeight", "120"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", "SAVES"}, {"baseColour", "YELLOW1"}, {"positionX", "50"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "-3"}, {"positionModeY", "POSITION_PERCENTAGE_RIGHT"} });

	Add("LOADPAGEINNER", "LOADLINE1", PAGE, { {"backgroundColour", "YELLOW1"},{"width", "2"}, {"positionX", "11"}, {"positionModeX", "POSITION_PERCENTAGE_LEFT"} });
	Add("LOADPAGEINNER", "LOADLINE2", PAGE, { {"backgroundColour", "YELLOW1"},{"width", "2"}, {"positionX", "49"}, {"positionModeX", "POSITION_PERCENTAGE_LEFT"} });
	Add("LOADPAGEINNER", "LOADLINE3", PAGE, { {"backgroundColour", "YELLOW1"},{"width", "89"}, {"height","2"}, {"positionX", "12"}, {"positionModeX", "POSITION_PERCENTAGE_LEFT"}, {"positionY", "15"}, {"positionModeY", "POSITION_PERCENTAGE_RIGHT"} });

	Add("LOADPAGEINNER", "LOADSUBPAGE1", PAGE, { {"colourMode","TRANSPARENT"},{"backgroundColour", "PURPLE2"},{"width", "49"},{"alignment","RIGHT"} });
	Add("LOADSUBPAGE1", "LOADTEXT", TEXT, { {"targetTextHeight", "120"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", " LOAD GAME"}, {"alignment", "TOP"}, {"baseColour", "YELLOW1"} });

	Add("LOADSUBPAGE1", "LOADSUBPAGE2", PAGE, { {"backgroundColour","PURPLE1"}, {"height", "85"},{"alignment","BOTTOM"} });
	Add("LOADSUBPAGE2", "LOADBUTTON", BUTTON, { {"width", "50"}, {"height", "15"},   {"vFunctionID", "Load Save"}, {"backgroundColour", "YELLOW1"}, {"positionX", "50"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "95"}, {"positionModeY", "POSITION_PERCENTAGE_RIGHT"} });
	Add("LOADBUTTON", "LOADBUTTONTEXT", TEXT, { {"targetTextHeight", "96"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", "LOAD "}, {"alignment", "CENTRE"}, {"baseColour", "PURPLE2"}, {"hoverColour", "PURPLE2"} });

	Add("LOADSUBPAGE2", "SELECTEDSAVETEXT", TEXT, { {"targetTextHeight", "50"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", "SELECTED SAVE: "}, {"positionX", "5"}, {"positionModeX", "POSITION_PERCENTAGE_LEFT"}, {"positionY", "4"}, {"positionModeY", "POSITION_PERCENTAGE_LEFT"}, {"baseColour", "YELLOW1"} });
	Add("LOADSUBPAGE2", "SAVEDATETEXT", TEXT, { {"targetTextHeight", "50"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", "DATE SAVED: "},	{"positionX", "5"}, {"positionModeX", "POSITION_PERCENTAGE_LEFT"}, {"positionY", "14"}, {"positionModeY", "POSITION_PERCENTAGE_LEFT"}, {"baseColour", "YELLOW1"} });
	Add("LOADSUBPAGE2", "EUNLOCKSTEXT", TEXT, { {"targetTextHeight", "50"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", "ELEMENTS UNLOCKED: "}, {"positionX", "5"}, {"positionModeX", "POSITION_PERCENTAGE_LEFT"}, {"positionY", "24"}, {"positionModeY", "POSITION_PERCENTAGE_LEFT"}, {"baseColour", "YELLOW1"} });
	Add("LOADSUBPAGE2", "RUNLOCKSTEXT", TEXT, { {"targetTextHeight", "50"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", "RECIPES UNLOCKED: "}, {"positionX", "5"}, {"positionModeX", "POSITION_PERCENTAGE_LEFT"}, {"positionY", "34"}, {"positionModeY", "POSITION_PERCENTAGE_LEFT"}, {"baseColour", "YELLOW1"} });

	Add("LOADSUBPAGE2", "SELECTEDSAVEDATA", TEXT, { {"targetTextHeight", "50"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", setSaveID},  {"positionX", "95"}, {"positionModeX", "POSITION_PERCENTAGE_RIGHT"}, {"positionY", "4"},  {"positionModeY", "POSITION_PERCENTAGE_LEFT"}, {"baseColour", "YELLOW1"} });
	Add("LOADSUBPAGE2", "SAVEDATEDATA", TEXT, { {"targetTextHeight", "50"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", dateSaved},  {"positionX", "95"}, {"positionModeX", "POSITION_PERCENTAGE_RIGHT"}, {"positionY", "14"}, {"positionModeY", "POSITION_PERCENTAGE_LEFT"}, {"baseColour", "YELLOW1"} });
	Add("LOADSUBPAGE2", "EUNLOCKSDATA", TEXT, { {"targetTextHeight", "50"}, {"textDisplayMode", "DIMENSION_BASED"},	{"text", "0/" + std::to_string(elementManager->GetNumElements())}, {"positionX", "95"}, {"positionModeX", "POSITION_PERCENTAGE_RIGHT"}, {"positionY", "24"}, {"positionModeY", "POSITION_PERCENTAGE_LEFT"}, {"baseColour", "YELLOW1"} });
	Add("LOADSUBPAGE2", "RUNLOCKSDATA", TEXT, { {"targetTextHeight", "50"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", "0/" + std::to_string(elementManager->GetNumRecipes()) },	{"positionX", "95"}, {"positionModeX", "POSITION_PERCENTAGE_RIGHT"}, {"positionY", "34"}, {"positionModeY", "POSITION_PERCENTAGE_LEFT"}, {"baseColour", "YELLOW1"} });

	Add("SCREEN", "BACKBUTTON2", BUTTON, { {"containerID","LOADPAGEINNER"}, {"width", "9"}, {"height", "9"},   {"colourMode","TRANSPARENT"}, {"vFunctionID", "Back"},{"positionX", "1"}, {"positionModeX", "POSITION_PERCENTAGE_LEFT"}, {"positionY", "2"}, {"positionModeY", "POSITION_PERCENTAGE_LEFT"} });
	Add("BACKBUTTON2", "BACKBUTTON2IMAGE", IMAGE, { {"imageName", "BACK"}, {"imageLoadMode", "EXACT"}, {"rectW", "400"}, {"rectH", "225"} });

	Add("SCREEN", "SETTINGSPAGE", PAGE, { {"backgroundColour", "PURPLE2"} });
	Add("SETTINGSPAGE", "SETTINGSTEXT", TEXT, { {"targetTextHeight", "120"}, {"textDisplayMode", "DIMENSION_BASED"}, {"text", "SETTINGS"}, {"alignment", "TOP"}, {"baseColour", "YELLOW1"} });

	Add("SETTINGSPAGE", "SETTINGSBOX", PAGE, { {"width", "80"}, {"height", "80"},   {"backgroundColour", "PURPLE1"}, {"positionX", "50"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "95"}, {"positionModeY", "POSITION_PERCENTAGE_RIGHT"} });
	return true;
}

void Menu::Add(std::string parentID, std::string id, WidgetType widgetType, std::vector<std::pair<std::string, std::string>> valuePairs)
{
	std::string vFunctionID = "Null", sFunctionID = "Null", imageName = "MISSING_TEXTURE", text = "NULL", containerID = "NULL", fontName = "DEFAULT", searchContainerID = "NULL";
	int width = 100, height = 100, fontSize = 10, rowCount = 3, maxLength = 25;
	float positionX = 0.0f, positionY = 0.0f, sourceLeft = 0.0f, sourceTop = 0.0f, sourceWidth = 100.0f, sourceHeight = 100.0f, targetTextHeight = -1.0f;
	DimensionType widthMode = DIMENSION_PERCENTAGE, heightMode = DIMENSION_PERCENTAGE;
	AlignmentType alignment = ALIGNMENT_ABSOLUTE;
	PositionType positionModeX = POSITION_ABSOLUTE, positionModeY = POSITION_ABSOLUTE;
	glm::vec3 backgroundColour = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 baseColour = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 hoverColour = glm::vec3(1.0f, 1.0f, 1.0f);
	FunctionModes functionMode = VOID;
	ColourMode colourMode = FILLED;
	TextRenderModes textRenderMode = SOLID;
	TextDisplayModes textDisplayMode = DIMENSION_BASED;
	ImageLoadModes imageLoadMode = STRETCH;
	ScrollMode scrollMode = LIST;
	RowModes rowMode = WIDGET;

	for (size_t i = 0; i < valuePairs.size(); i++)
	{
		if (valuePairs[i].first == "imageName")
		{
			imageName= valuePairs[i].second;
		}
		else if (valuePairs[i].first == "text")
		{
			text = valuePairs[i].second;
		}
		else if (valuePairs[i].first == "containerID")
		{
			containerID = valuePairs[i].second;
		}
		else if (valuePairs[i].first == "vFunctionID")
		{
			vFunctionID = valuePairs[i].second;
		}
		else if (valuePairs[i].first == "sFunctionID")
		{
			sFunctionID = valuePairs[i].second;
		}
		else if (valuePairs[i].first == "fontName")
		{
			fontName = valuePairs[i].second;
		}
		else if (valuePairs[i].first == "width")
		{
			width = std::stoi(valuePairs[i].second);
		}
		else if (valuePairs[i].first == "height")
		{
			height = std::stoi(valuePairs[i].second);
		}
		else if (valuePairs[i].first == "rectW")
		{
			sourceWidth = stoi(valuePairs[i].second);
		}
		else if (valuePairs[i].first == "rectH")
		{
			sourceHeight = stoi(valuePairs[i].second);
		}
		else if (valuePairs[i].first == "rectX")
		{
			sourceLeft = stoi(valuePairs[i].second);
		}
		else if (valuePairs[i].first == "rectY")
		{
			sourceTop = stoi(valuePairs[i].second);
		}
		else if (valuePairs[i].first == "fontSize")
		{
			fontSize = std::stoi(valuePairs[i].second);
		}
		else if (valuePairs[i].first == "positionX")
		{
			positionX = stof(valuePairs[i].second);
		}
		else if (valuePairs[i].first == "positionY")
		{
			positionY = stof(valuePairs[i].second);
		}
		else if (valuePairs[i].first == "backgroundColour")
		{
			backgroundColour.r = Colours[valuePairs[i].second].x;
			backgroundColour.g = Colours[valuePairs[i].second].y;
			backgroundColour.b = Colours[valuePairs[i].second].z;
		}		
		else if (valuePairs[i].first == "baseColour")
		{
			baseColour.r = Colours[valuePairs[i].second].x;
			baseColour.g = Colours[valuePairs[i].second].y;
			baseColour.b = Colours[valuePairs[i].second].z;
		}		
		else if (valuePairs[i].first == "hoverColour")
		{
			hoverColour.r = Colours[valuePairs[i].second].x;
			hoverColour.g = Colours[valuePairs[i].second].y;
			hoverColour.b = Colours[valuePairs[i].second].z;
		}
		else if (valuePairs[i].first == "colourMode")
		{
			colourMode = ColourModeMap[valuePairs[i].second];
		}
		else if (valuePairs[i].first == "textRenderMode")
		{
			textRenderMode = TextRenderModesMap[valuePairs[i].second];
		}
		else if (valuePairs[i].first == "textDisplayMode")
		{
			textDisplayMode = TextDisplayModesMap[valuePairs[i].second];
		}
		else if (valuePairs[i].first == "widthMode")
		{
			widthMode = DimensionTypeMap[valuePairs[i].second];
		}
		else if (valuePairs[i].first == "heightMode")
		{
			heightMode = DimensionTypeMap[valuePairs[i].second];
		}
		else if (valuePairs[i].first == "rowCount")
		{
			rowCount = std::stoi(valuePairs[i].second);
		}
		else if (valuePairs[i].first == "alignment")
		{
			alignment = AlignmentTypeMap[valuePairs[i].second];
		}
		else if (valuePairs[i].first == "positionModeX")
		{
			positionModeX = PositionTypeMap[valuePairs[i].second];
		}
		else if (valuePairs[i].first == "positionModeY")
		{
			positionModeY = PositionTypeMap[valuePairs[i].second];
		}
		else if (valuePairs[i].first == "imageLoadMode")
		{
			imageLoadMode = ImageLoadModesMap[valuePairs[i].second];
		}
		else if (valuePairs[i].first == "scrollMode")
		{
			scrollMode = ScrollModeMap[valuePairs[i].second];
		}
		else if (valuePairs[i].first == "functionMode")
		{
			functionMode = FunctionModesMap[valuePairs[i].second];
		}
		else if (valuePairs[i].first == "targetTextHeight")
		{
			targetTextHeight = stoi(valuePairs[i].second);
		}
		else if (valuePairs[i].first == "maxLength") {
			maxLength = stoi(valuePairs[i].second);
		}
	}

	if (containerID == "NULL")
	{
		containerID = parentID;
	}
	if (searchContainerID == "NULL")
	{
		searchContainerID = parentID;
	}


	std::function<void()> vFunction = vFunctionMap[vFunctionID];
	std::function<void(std::string)> sFunction = sFunctionMap[sFunctionID];
	Widget* container = widgetList[containerID];
	Widget* searchContainer = widgetList[searchContainerID];
	glm::vec2 position = glm::vec2(positionX,positionY);
	Rect sourceRect = Rect(sourceLeft, sourceTop, sourceWidth, sourceHeight);

	std::string ID = id;

	switch (widgetType)
	{
	case PAGE:
		widgetList[parentID]->AddPage(&widgetList, ID, container, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY, backgroundColour, colourMode); break;	
	case STATIC_LIST:
		widgetList[parentID]->AddList(&widgetList, ID, container, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY, backgroundColour, colourMode); break;
	case SCROLLING_LIST:
		widgetList[parentID]->AddScrollingList(&widgetList, ID, container,scrollMode, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY, backgroundColour, colourMode); break;
	case BUTTON:
		widgetList[parentID]->AddButton(&widgetList, ID,container,width,height, vFunction, sFunction, functionMode, widthMode, heightMode, alignment, position, positionModeX, positionModeY, backgroundColour, colourMode); break;
	case TEXT:
		widgetList[parentID]->AddText(&widgetList, ID,container,text,Fonts[fontName], targetTextHeight,width,height,textDisplayMode, widthMode, heightMode, alignment, position, positionModeX, positionModeY, baseColour,hoverColour,textRenderMode); break;
	case IMAGE:
		widgetList[parentID]->AddImage(&widgetList, ID, container, width, height,sourceRect,Textures[imageName], widthMode, heightMode, alignment, position, positionModeX, positionModeY,imageLoadMode); break;
	case GRID:
		widgetList[parentID]->AddGrid(&widgetList, ID, container, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY, backgroundColour, colourMode, scrollMode, rowCount, rowMode); break;
	case SEARCH:
		widgetList[parentID]->AddSearch(&widgetList, ID, container, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY, maxLength, searchContainer, backgroundColour, colourMode); break;
	}
}

void Menu::Draw(Shader* imageShader, Shader* textShader) {
	if (showMenu)
		widgetList["MAINMENU"]->Draw(imageShader, textShader);
	if (showEGrid)
		widgetList["EGRID"]->Draw(imageShader, textShader);
	if (showLoadMenu) 
		widgetList["LOADPAGE"]->Draw(imageShader, textShader);
	if (showPauseMenu) 
		widgetList["PAUSEMENUPAGE"]->Draw(imageShader, textShader);
	if (showSettings)
		widgetList["SETTINGSPAGE"]->Draw(imageShader, textShader);
}

bool Menu::Input(glm::vec2 mousePos, InputManager* inputManager)
{
	if (!showMenu && !showEGrid && !showSettings) {
		if (inputManager->keyStates["e"] == 2) {
			inputManager->keyStates["e"] = 1;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			showEGrid = true;
			UpdateElementList();
		}
		if (inputManager->keyStates["es"] == 2) {
			inputManager->keyStates["es"] = 1;
			showPauseMenu = true;
			wasPaused = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	if (showMenu) {
		widgetList["MAINMENU"]->Input(true, mousePos, inputManager);
	}
	if (showPauseMenu) {
		if (inputManager->keyStates["e"] == 2) {
			inputManager->keyStates["e"] = 1;
			showPauseMenu = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		widgetList["PAUSEMENUPAGE"]->Input(true, mousePos, inputManager);
	}
	if (showEGrid) {
		if (!widgetList["ESEARCH"]->GetActive()) {
			if (inputManager->keyStates["e"] == 2) {
				inputManager->keyStates["e"] = 1;
				showEGrid = false;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
		widgetList["EGRID"]->Input(true, mousePos, inputManager);
	}

	if (showLoadMenu) {
		if (inputManager->keyStates["es"] == 2) {
			inputManager->keyStates["es"] = 1;
			showMenu = true;
			showLoadMenu = false;
		}
		widgetList["LOADPAGE"]->Input(true, mousePos, inputManager);
	}

	if (showSettings) {
		if (inputManager->keyStates["es"] == 2) {
			inputManager->keyStates["es"] = 1;
			showSettings = false;
			if (wasPaused) {
				showPauseMenu = true;
			}
			else {
				showMenu = true;
			}
			
		}
		widgetList["SETTINGSPAGE"]->Input(true, mousePos, inputManager);
	}
		
		

	return true;

}

void Widget::AddPage(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode)
{
	std::string type = typeid(*this).name();
	type = type.substr(6);
	if (type == "Page" || type == "Widget")
	{
		this->widgets.push_back(new Page(widgetList, ID, container, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY, colour, colourMode));
	}
}

void Widget::AddList(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode)
{
	std::string type = typeid(*this).name();
	type = type.substr(6);
	if (type == "Page" || type == "Widget" || type == "ScrollingList")
	{
		this->widgets.push_back(new List(widgetList, ID, container, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY, colour, colourMode));
	}
}

void Widget::AddScrollingList(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, ScrollMode scrollMode, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode)
{
	std::string type = typeid(*this).name();
	type = type.substr(6);
	if (type == "Page" || type == "Widget")
	{
		this->widgets.push_back(new ScrollingList(widgetList, ID, container,scrollMode, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY, colour, colourMode));
	}
}
 
void Widget::AddText(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, std::string textString, Font* font, float targetHeight, int width, int height, TextDisplayModes displayMode, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 baseColour, glm::vec3 hoverColour, TextRenderModes renderMode)
{
	std::string type = typeid(*this).name();
	type = type.substr(6);
	if (type != "GUIText" && type != "GUIImage")
	{
		this->widgets.push_back(new GUIText(widgetList, ID, container, textString, font, targetHeight, width, height, displayMode, widthMode, heightMode, alignment, position, positionModeX, positionModeY, baseColour,hoverColour, renderMode));
	}
}

void Widget::AddImage(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, Rect sourceRect, GUITexture* texture, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, ImageLoadModes imageLoadMode)
{
	std::string type = typeid(*this).name();
	type = type.substr(6);
	if (type != "GUIText" && type != "GUIImage")
	{
		this->widgets.push_back(new GUIImage(widgetList, ID, container, width, height, sourceRect, texture, widthMode, heightMode, alignment, position, positionModeX, positionModeY,imageLoadMode));
	}
}

void Widget::AddButton(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, std::function<void()> vFunction, std::function<void(std::string)> sFunction, FunctionModes functionMode, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode)
{
	std::string type = typeid(*this).name();
	type = type.substr(6);
	if (type != "Button" && type != "GUIText" && type != "GUIImage")
	{
		this->widgets.push_back(new Button(widgetList, ID, container, width, height, vFunction, sFunction, functionMode, widthMode, heightMode, alignment, position, positionModeX, positionModeY, colour, colourMode));
	}
}

void Widget::AddGrid(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, glm::vec3 colour, ColourMode colourMode, ScrollMode scrollMode, int rowCount, RowModes rowMode)
{
	std::string type = typeid(*this).name();
	type = type.substr(6);
	if (type == "Page" || type == "Widget")
	{
		this->widgets.push_back(new GUIGrid(widgetList, ID, container, scrollMode, width, height, widthMode, heightMode, alignment, position, positionModeX, positionModeY, colour, colourMode, rowCount, rowMode));
	}
}

void Widget::AddSearch(std::unordered_map<std::string, Widget*>* widgetList, std::string ID, Widget* container, int width, int height, DimensionType widthMode, DimensionType heightMode, AlignmentType alignment, glm::vec2 position, PositionType positionModeX, PositionType positionModeY, int maxLength, Widget* searchContainer, glm::vec3 colour, ColourMode colourMode)
{
	std::string type = typeid(*this).name();
	type = type.substr(6);
	if (type != "Button" && type != "GUISlider" && type != "GUISearch" && type != "GUIText" && type != "GUIImage")
	{
		this->widgets.push_back(new GUISearch(widgetList, ID, container, maxLength, width, height, searchContainer, widthMode, heightMode, alignment, position, positionModeX, positionModeY, colour, colourMode));
	}
}

void Menu::Null()
{
	std::cout << "Button Pressed" << std::endl;
}

void Menu::Null(std::string string) {
	std::cout << "Button " << string << " Pressed bitch" << std::endl;
}

void Menu::Quit()
{
	exit(0);
}
void Menu::Settings()
{
	showMenu = false;
	showPauseMenu = false;
	showSettings = true;
}
void Menu::PlayGame()
{
	showMenu = false;
	showEGrid = false;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

void Menu::LoadMenu()
{
	SetSave("SAVE 1 BUTTON");
	showMenu = false;
	showLoadMenu = true;

}

void Menu::UpdateElementList() {
	widgetList["EGRID"]->ClearWidgets();
	Add("EGRID", "ESEARCH", SEARCH, { {"width", "90"},{"heightMode","DIMENSION_ABSOLUTE"},  {"backgroundColour", "PURPLE2"} });
	Add("ESEARCH", "ESEARCHTEXT", TEXT, { {"textDisplayMode", "DIMENSION_BASED"}, {"maxLength", "16"}, {"targetTextHeight", "30"}, {"text", "Search..."}, {"positionModeX", "POSITION_PERCENTAGE_LEFT"}, {"positionX", "5"}, {"positionY", "50"}, {"positionModeY", "POSITION_PERCENTAGE_CENTRE"}, {"baseColour", "YELLOW1"} });
	Add("ESEARCH", "ESEARCHIMAGE", IMAGE, { {"widthMode", "DIMENSION_ABSOLUTE"}, {"heightMode", "DIMENSION_ABSOLUTE"}, {"width", "30"}, {"height", "30"}, {"imageName", "SEARCH"}, {"positionX", "90"}, {"positionModeX", "POSITION_PERCENTAGE_CENTRE"}, {"positionY", "50"}, {"positionModeY", "POSITION_PERCENTAGE_CENTRE"}, {"imageLoadMode", "ORIGINAL"} });
	for (int i = 0; i < elementManager->GetNumElements(); i++)
	{
		if (elementManager->GetElementUnlocks()[i] == 1)
		{
			std::string IDString1 = std::to_string(i);
			std::string IDString2 = std::string(3 - IDString1.length(), '0').append(IDString1);
			Add("EGRID", elementManager->GetElements()[i].GetName() + " BUTTON " + IDString2, BUTTON, { {"width", "22"},{"widthMode","DIMENSION_PERCENTAGE"}, {"heightMode","DIMENSION_ABSOLUTE"},  {"backgroundColour", "PURPLE2"},   {"sFunctionID", "Spawn Element"},{"functionMode","STRING"} });
			Add(elementManager->GetElements()[i].GetName() + " BUTTON " + IDString2, elementManager->GetElements()[i].GetName() + " TEXT", TEXT, { {"textDisplayMode", "CONTAINER"}, {"text",elementManager->GetElements()[i].GetName()}, {"alignment", "CENTRE"}, {"baseColour", "YELLOW1"} });
		}
	}
	widgetList["EGRID"]->SearchWidgets("");
}

void Menu::SpawnElement(std::string ID)
{
	ID = ID.substr(ID.length() - 3);
	itemManager->AddItem(glm::vec3(0.0f, 0.0f, 0.0f), stoi(ID));
	std::cout << "Spawning " + ID << std::endl;
}

void Menu::SetSave(std::string saveID) {
	saveID = saveID.substr(5);
	saveID = saveID.substr(0, saveID.find(" "));
	setSaveID = saveID;
	elementManager->LoadSaveFile("res/data/load/saves/save" + setSaveID + ".txt", true);
	widgetList["SELECTEDSAVEDATA"]->UpdateText(setSaveID);
	widgetList["SAVEDATEDATA"]->UpdateText(dateSaved);
	widgetList["EUNLOCKSDATA"]->UpdateText(std::to_string(elementManager->GetNumElementsUnlocked()) + "/" + std::to_string(elementManager->GetNumElements()));
	widgetList["EUNLOCKSDATA"]->SetColour({ static_cast<int>( (1.0f - (static_cast<float>(elementManager->GetNumElementsUnlocked()) / static_cast<float>(elementManager->GetNumElements())))), static_cast<int>( (static_cast<float>(elementManager->GetNumElementsUnlocked()) / static_cast<float>(elementManager->GetNumElements()))), 0.0f });
	widgetList["RUNLOCKSDATA"]->UpdateText(std::to_string(elementManager->GetNumRecipesUnlocked()) + "/" + std::to_string(elementManager->GetNumRecipes()));
	widgetList["RUNLOCKSDATA"]->SetColour({ static_cast<int>((1.0f - (static_cast<float>(elementManager->GetNumRecipesUnlocked()) / static_cast<float>(elementManager->GetNumRecipes())))), static_cast<int>((static_cast<float>(elementManager->GetNumRecipesUnlocked()) / static_cast<float>(elementManager->GetNumRecipes()))), 0.0f });
}

void Menu::LoadSave()
{
	elementManager->LoadSaveFile("res/data/load/saves/save" + setSaveID + ".txt");
	UpdateElementList();
	showMenu = false;
	showLoadMenu = false;
	showEGrid = true;
}

void Menu::Resume() {
	showPauseMenu = false;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Menu::MainMenu() {
	showPauseMenu = false;
	showMenu = true;
	wasPaused = false;
}