#include "stdafx.h"
#include "LevelData.h"

const std::string Pipe::ColourNames[] { "Green", "Yellow", "Blue", "Orange" };

LevelData* LevelData::m_LevelOnePtr = nullptr;

LevelData::LevelData(String platforms, String pipes, String items)
{
	std::string line;

	std::stringstream platformsStream = std::stringstream(platforms.C_str());
	while (std::getline(platformsStream, line))
	{
		int closeBracket = line.find(']');

		DOUBLE2 topLeft = StringToDOUBLE2(line.substr(0, closeBracket + 1));
		DOUBLE2 bottomRight = StringToDOUBLE2(line.substr(closeBracket + 1));

		m_PlatformsPtrArr.push_back(new Platform(topLeft, bottomRight));
	}

	std::stringstream pipesStream = std::stringstream(pipes.C_str());
	while (std::getline(pipesStream, line))
	{
		int closeBracket = line.find(']');

		DOUBLE2 topLeft = StringToDOUBLE2(line.substr(0, closeBracket + 1));
		DOUBLE2 bottomRight = StringToDOUBLE2(line.substr(closeBracket + 1));

		m_PipesPtrArr.push_back(new Pipe(topLeft, bottomRight, Pipe::Colour::YELLOW, false));
	}

	std::stringstream itemsStream = std::stringstream(items.C_str());
	while (std::getline(itemsStream, line))
	{
		int closeBracket = line.find(']');

		DOUBLE2 topLeft = StringToDOUBLE2(line.substr(0, closeBracket + 1));
		int itemType = std::stoi(line.substr(closeBracket+1).c_str());

		if (itemType == int(Item::TYPE::PRIZE_BLOCK))
		{
			m_ItemsPtrArr.push_back(new PrizeBlock(topLeft));
		}
		else if (itemType == int(Item::TYPE::EXCLAMATION_MARK_BLOCK))
		{
			m_ItemsPtrArr.push_back(new ExclamationMarkBlock(topLeft, ExclamationMarkBlock::COLOUR::YELLOW, false));
		}
		else if (itemType == int(Item::TYPE::COIN))
		{
			m_ItemsPtrArr.push_back(new Coin(topLeft, -1));
		}
		else if (itemType == int(Item::TYPE::DRAGON_COIN))
		{
			m_ItemsPtrArr.push_back(new DragonCoin(topLeft));
		}
		else
		{
			OutputDebugString(String("ERROR: Unhandled item in LevelData()"));
		}
	}
}

DOUBLE2 LevelData::StringToDOUBLE2(std::string double2String)
{
	DOUBLE2 result;

	int openBracket = double2String.find('[');
	int closeBracket = double2String.find(']', openBracket);
	int comma = double2String.find(',', openBracket);

	if (openBracket != std::string::npos &&
		closeBracket != std::string::npos &&
		comma != std::string::npos)
	{
		result = DOUBLE2(stod(double2String.substr(openBracket + 1, comma - (openBracket + 1))),
			             stod(double2String.substr(comma + 1, closeBracket - (comma + 1))));
	}
	else
	{
		OutputDebugString(String("\nERROR: Malformed platform data string: \n") + String(double2String.c_str()) + String("\n"));
	}

	return result;
}

LevelData::~LevelData()
{
	for (size_t i = 0; i < m_PlatformsPtrArr.size(); ++i)
	{
		delete m_PlatformsPtrArr[i];
	}
	for (size_t i = 0; i < m_PipesPtrArr.size(); ++i)
	{
		delete m_PipesPtrArr[i];
	}
	for (size_t i = 0; i < m_ItemsPtrArr.size(); ++i)
	{
		delete m_ItemsPtrArr[i];
	}
}

void LevelData::Unload()
{
	delete m_LevelOnePtr;
}

void LevelData::TogglePaused(bool paused)
{
	for (size_t i = 0; i < m_ItemsPtrArr.size(); ++i)
	{
		if (m_ItemsPtrArr[i] != nullptr)
		{
			m_ItemsPtrArr[i]->TogglePaused(paused);
		}
	}
}

std::vector<Platform*> LevelData::GetPlatforms()
{
	return m_PlatformsPtrArr;
}

std::vector<Pipe*> LevelData::GetPipes()
{
	return m_PipesPtrArr;
}

std::vector<Item*> LevelData::GetItems()
{
	return m_ItemsPtrArr;
}

LevelData* LevelData::GetLevel(int levelIndex)
{
	// LATER: Add more levels in here
	// TODO: Make m_LevelOnePtr an array with all the levels?
	if (m_LevelOnePtr == nullptr)
	{
		m_LevelOnePtr = GenerateLevel(levelIndex);
	}

	return m_LevelOnePtr;
}

void LevelData::RegenerateLevel(int levelIndex)
{
	if (levelIndex == 1)
	{
		delete m_LevelOnePtr;
		m_LevelOnePtr = GenerateLevel(levelIndex);
		return;
	}
	else
	{
		OutputDebugString(String("ERROR: Invalid level index passed to LevelData::RegenerateLevel: ") + String(levelIndex) + String("\n"));
		return;
	}
}

LevelData* LevelData::GenerateLevel(int levelIndex)
{
	std::stringstream platformsStream;	// [ DOUBLE2(TOP LEFT, BTM RIGHT) ]
	std::stringstream pipesStream;		// [ DOUBLE2(TOP LEFT, BTM RIGHT), Color?(color) ]
	std::stringstream itemsStream;		// [ DOUBLE2(TOP LEFT, BTM RIGHT), ItemName, ItemProperties... ]

	switch (levelIndex)
	{
	case 1:
	{
		// TODO: Make this cleaner/more efficient
		// e.g. Move scale to the constructor
		double scale = 2.0;
		platformsStream << "[" << 305 * scale << "," << 337 * scale << "]";
		platformsStream << "[" << 559 * scale << "," << 342 * scale << "]";
		platformsStream << "\n";
		platformsStream << "[" << 705 * scale << "," << 321 * scale << "]";
		platformsStream << "[" << 815 * scale << "," << 326 * scale << "]";
		platformsStream << "\n";
		platformsStream << "[" << 2993 * scale << "," << 337 * scale << "]";
		platformsStream << "[" << 3070 * scale << "," << 342 * scale << "]";
		platformsStream << "\n";
		platformsStream << "[" << 3025 * scale << "," << 289 * scale << "]";
		platformsStream << "[" << 3150 * scale << "," << 294 * scale << "]";
		platformsStream << "\n";
		platformsStream << "[" << 3121 * scale << "," << 337 * scale << "]";
		platformsStream << "[" << 3214 * scale << "," << 342 * scale << "]";
		platformsStream << "\n";
		platformsStream << "[" << 3393 * scale << "," << 305 * scale << "]";
		platformsStream << "[" << 3550 * scale << "," << 310 * scale << "]";

		pipesStream << "[" << 2705 * scale << "," << 337 * scale << "]";
		pipesStream << "[" << 2733 * scale << "," << 384 * scale << "]";
		pipesStream << std::string(Pipe::ColourNames[int(Pipe::Colour::YELLOW)]);
		pipesStream << "\n";
		pipesStream << "[" << 2737 * scale << "," << 321 * scale << "]";
		pipesStream << "[" << 2767 * scale << "," << 384 * scale << "]";
		pipesStream << std::string(Pipe::ColourNames[int(Pipe::Colour::YELLOW)]);
		pipesStream << "\n";
		pipesStream << "[" << 3873 * scale << "," << 353 * scale << "]";
		pipesStream << "[" << 3902 * scale << "," << 383 * scale << "]";
		pipesStream << std::string(Pipe::ColourNames[int(Pipe::Colour::BLUE)]);
		pipesStream << "\n";
		pipesStream << "[" << 4049 * scale << "," << 337 * scale << "]";
		pipesStream << "[" << 4078 * scale << "," << 383 * scale << "]";
		pipesStream << std::string(Pipe::ColourNames[int(Pipe::Colour::BLUE)]);
		pipesStream << "\n";
		pipesStream << "[" << 4321 * scale << "," << 353 * scale << "]";
		pipesStream << "[" << 4350 * scale << "," << 383 * scale << "]";
		pipesStream << std::string(Pipe::ColourNames[int(Pipe::Colour::GREY)]);
		pipesStream << "\n";
		pipesStream << "[" << 4369 * scale << "," << 337 * scale << "]";
		pipesStream << "[" << 4398 * scale << "," << 383 * scale << "]";
		pipesStream << std::string(Pipe::ColourNames[int(Pipe::Colour::GREEN)]);

		itemsStream << "[" << 609 * scale << "," << 320 * scale << "]";
		itemsStream << int(Item::TYPE::PRIZE_BLOCK);
		itemsStream << "\n";
		itemsStream << "[" << 625 * scale << "," << 320 * scale << "]";
		itemsStream << int(Item::TYPE::EXCLAMATION_MARK_BLOCK);
		itemsStream << "\n";
		itemsStream << "[" << 641 * scale << "," << 320 * scale << "]";
		itemsStream << int(Item::TYPE::PRIZE_BLOCK);
		itemsStream << "\n";
		itemsStream << "[" << 1240 * scale << "," << 320 * scale << "]";
		itemsStream << int(Item::TYPE::COIN);
		itemsStream << "\n";
		itemsStream << "[" << 1250 * scale << "," << 300 * scale << "]";
		itemsStream << int(Item::TYPE::COIN);
		itemsStream << "\n";
		itemsStream << "[" << 1266 * scale << "," << 280 * scale << "]";
		itemsStream << int(Item::TYPE::DRAGON_COIN);
		itemsStream << "\n";
		itemsStream << "[" << 1288 * scale << "," << 300 * scale << "]";
		itemsStream << int(Item::TYPE::COIN);
		itemsStream << "\n";
		itemsStream << "[" << 1302 * scale << "," << 320 * scale << "]";
		itemsStream << int(Item::TYPE::COIN);
		itemsStream << "\n";
	} break;
	default:
	{
		OutputDebugString(String("ERROR: Unhandled level index in LevelData::GenerateLevel: ") + String(levelIndex) + String("\n"));
		return nullptr;
	} break;
	}

	String platformsString = String(platformsStream.str().c_str());
	String pipesString = String(pipesStream.str().c_str());
	String itemsString = String(itemsStream.str().c_str());

	return new LevelData(platformsString, pipesString, itemsString);
}


void LevelData::AddItem(Item* newItemPtr)
{
	for (size_t i = 0; i < m_ItemsPtrArr.size(); ++i)
	{
		if (m_ItemsPtrArr[i] == nullptr)
		{
			m_ItemsPtrArr[i] = newItemPtr;
			return;
		}
	}

	m_ItemsPtrArr.push_back(newItemPtr);
}

void LevelData::RemoveItem(Item* itemPtr)
{
	for (size_t i = 0; i < m_ItemsPtrArr.size(); ++i)
	{
		if (m_ItemsPtrArr[i] == itemPtr)
		{
			delete m_ItemsPtrArr[i];
			m_ItemsPtrArr[i] = nullptr;
			return;
		}
	}

	OutputDebugString(String("ERROR: Could not delete item in LevelData::RemoveItem\n"));
}

void LevelData::RemoveItem(int itemIndex)
{
	assert(itemIndex >= 0 && itemIndex < int(m_ItemsPtrArr.size()));

	delete m_ItemsPtrArr[itemIndex];
	m_ItemsPtrArr[itemIndex] = nullptr;
}

void LevelData::TickItems(double deltaTime, Level* levelPtr)
{
	for (size_t i = 0; i < m_ItemsPtrArr.size(); ++i)
	{
		if (m_ItemsPtrArr[i] != nullptr)
		{
			if (m_ItemsPtrArr[i]->Tick(deltaTime, levelPtr))
			{
				// This item needs to be removed
				RemoveItem(i);
			}
		}
	}
}

void LevelData::PaintItems()
{
	for (size_t i = 0; i < m_ItemsPtrArr.size(); ++i)
	{
		if (m_ItemsPtrArr[i] != nullptr)
		{
			m_ItemsPtrArr[i]->Paint();
		}
	}
}