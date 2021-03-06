#pragma once

class Level;
class Player;
class Item;
class Enemy;
class Platform;
class Pipe;

// Holds/updates/paints all entities in the game
class LevelData
{
public:
	virtual ~LevelData();

	LevelData(const LevelData&) = delete;
	LevelData& operator=(const LevelData&) = delete;

	static LevelData* GetLevelData(int levelIndex, Level* levelPtr);
	static void UnloadAllLevelData();
	static void UnloadLevelData(int levelIndex);

	void AddItem(Item* newItemPtr);
	void RemoveItem(Item* itemPtr);
	void RemoveItem(int itemIndex);

	void AddEnemy(Enemy* newEnemyPtr);
	void RemoveEnemy(Enemy* enemyPtr);
	void RemoveEnemy(int enemyIndex);

	Pipe* GetPipeWithIndex(int index) const;

	void PaintEnemiesInBackground(); // These enemies are drawn behind the level image (piranha plants)
	void PaintItemsAndEnemies();
	void PaintItemsInForeground(); // These items are drawn in front of the player (Goal gates/ midway gates)
	void TickItemsAndEnemies(double deltaTime, Level* levelPtr);

	void SetItemsAndEnemiesPaused(bool paused);

	std::vector<Platform*>& GetPlatforms();
	std::vector<Pipe*>& GetPipes();
	std::vector<Item*>& GetItems();
	std::vector<Enemy*>& GetEnemies();

private:
	LevelData(const std::string& platforms, const std::string& pipes, const std::string& items, const std::string& enemies, Level* levelPtr);

	static LevelData* CreateLevelData(int levelIndex, Level* levelPtr);

	static std::vector<LevelData*> m_LevelDataPtrArr;

	// Everything is stored in terms of tile col/row, we need to multiply by
	// this scale to get actual world coordinates
	static const int TILE_SIZE = 16;

	Level* m_LevelPtr = nullptr;

	DOUBLE2 StringToDOUBLE2(const std::string& double2String) const;

	std::vector<Platform*> m_PlatformsPtrArr;
	std::vector<Pipe*> m_PipesPtrArr;
	std::vector<Item*> m_ItemsPtrArr;
	std::vector<Enemy*> m_EnemiesPtrArr;
};
