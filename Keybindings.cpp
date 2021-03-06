#include "stdafx.h"

#include "Keybindings.h"
#include "Enumerations.h"
#include "FileIO.h"

int Keybindings::Y_BUTTON;
int Keybindings::X_BUTTON;
int Keybindings::A_BUTTON;
int Keybindings::B_BUTTON;

int Keybindings::D_PAD_RIGHT;
int Keybindings::D_PAD_LEFT;
int Keybindings::D_PAD_UP;
int Keybindings::D_PAD_DOWN;

int Keybindings::START_BUTTON;
int Keybindings::SELECT_BUTTON;

int Keybindings::TOGGLE_INFO_OVERLAY;
int Keybindings::SHOW_NEXT_INFO_SESSION;
int Keybindings::SHOW_PREVIOUS_INFO_SESSION;
int Keybindings::SCROLL_THROUGH_SESSIONS;
int Keybindings::TOGGLE_MUTED;
int Keybindings::GENERATE_BAR_GRAPH;

int Keybindings::DEBUG_TELEPORT_PLAYER;
int Keybindings::DEBUG_QUICK_RESET;
int Keybindings::DEBUG_TOGGLE_PHYSICS_RENDERING;
int Keybindings::DEBUG_FRAME_BY_FRAME_ADVANCE;
int Keybindings::DEBUG_TOGGLE_CAMERA_DEBUG_OVERLAY;
int Keybindings::DEBUG_TOGGLE_PLAYER_INFO;
int Keybindings::DEBUG_TOGGLE_ENEMY_AI_OVERLAY;

int Keybindings::LEFT_SHOULDER;
int Keybindings::RIGHT_SHOULDER;

void Keybindings::ReadBindingsFromFile()
{
	const std::string filePath = "Resources/Keybindings.xml";
	std::ifstream fileInStream;
	std::stringstream stringStream;

	fileInStream.open(filePath);
	if (!fileInStream) // The file hasn't yet been created
	{
		std::ofstream fileOutStream;
		fileOutStream.open(filePath); // Create the file
		fileOutStream.close();

		fileInStream.open(filePath);
	}

	if (fileInStream.fail() == false)
	{
		std::string line;
		while (fileInStream.eof() == false)
		{
			std::getline(fileInStream, line);
			stringStream << line;
		}
	}
	fileInStream.close();

	StoreStringData(stringStream.str());
}

void Keybindings::StoreStringData(const std::string& fileContents)
{
	Y_BUTTON = RegisterKeycode(fileContents, "Y_Button", 'A');
	X_BUTTON = RegisterKeycode(fileContents, "X_Button", 'S');
	A_BUTTON = RegisterKeycode(fileContents, "A_Button", 'X');
	B_BUTTON = RegisterKeycode(fileContents, "B_Button", 'Z');
	
	D_PAD_RIGHT = RegisterKeycode(fileContents, "DPadRight", VK_RIGHT);
	D_PAD_LEFT = RegisterKeycode(fileContents, "DPadLeft", VK_LEFT);
	D_PAD_UP = RegisterKeycode(fileContents, "DPadUp", VK_UP);
	D_PAD_DOWN = RegisterKeycode(fileContents, "DPadDown", VK_DOWN);

	START_BUTTON = RegisterKeycode(fileContents, "StartButton", VK_SPACE);
	SELECT_BUTTON = RegisterKeycode(fileContents, "SelectButton", VK_RETURN);

	TOGGLE_INFO_OVERLAY = RegisterKeycode(fileContents, "ToggleInfoOverlay", 'I');
	SHOW_NEXT_INFO_SESSION = RegisterKeycode(fileContents, "ShowNextInfoSession", VK_NEXT); // (PgDn)
	SHOW_PREVIOUS_INFO_SESSION = RegisterKeycode(fileContents, "ShowPreviousInfoSession", VK_PRIOR); // (PgUp)
	SCROLL_THROUGH_SESSIONS = RegisterKeycode(fileContents, "ScrollThroughSessions", VK_CONTROL);
	TOGGLE_MUTED = RegisterKeycode(fileContents, "ToggleMuted", 'M');

	DEBUG_TELEPORT_PLAYER = RegisterKeycode(fileContents, "DEBUGTeleportPlayer", 'O');
	DEBUG_QUICK_RESET = RegisterKeycode(fileContents, "DEBUGQuickReset", 'R');
	DEBUG_TOGGLE_PHYSICS_RENDERING = RegisterKeycode(fileContents, "DEBUGTogglePhysicsRendering", 'P');
	DEBUG_FRAME_BY_FRAME_ADVANCE = RegisterKeycode(fileContents, "DEBUGFrameByFrameAdvance", VK_OEM_5); // (Period)
	GENERATE_BAR_GRAPH = RegisterKeycode(fileContents, "GenerateBarGraph", VK_F2);
	DEBUG_TOGGLE_CAMERA_DEBUG_OVERLAY = RegisterKeycode(fileContents, "DEBUGToggleCameraDebugOverlay", VK_F9);
	DEBUG_TOGGLE_PLAYER_INFO = RegisterKeycode(fileContents, "DEBUGTogglePlayerInfo", VK_F10);
	DEBUG_TOGGLE_ENEMY_AI_OVERLAY = RegisterKeycode(fileContents, "DEBUGToggleEnemyAIInfo", VK_F11);

	LEFT_SHOULDER = RegisterKeycode(fileContents, "LeftShoulder", 'Q');
	RIGHT_SHOULDER = RegisterKeycode(fileContents, "RightShoulder", 'E');
}

int Keybindings::RegisterKeycode(const std::string& fileContents, const std::string& tagName, int defaultValue)
{
	int result = std::stoi(FileIO::GetTagContent(fileContents, tagName));
	if (result == -1) 
	{
		OutputDebugString(String("ERROR: Invalid keybinding: ") + String(result) + 
			String(", used default value instead ") + String(defaultValue) + String("\n"));
		result = defaultValue;
	}
	return result;
}
