#pragma once
#ifndef TEXTADVENTURE_GAMEMANAGER
#define TEXTADVENTURE_GAMEMANAGER

#include "Dungeon.h"
#include "Enums.h"
#include "Player.h"
#include "Random.h"
#include "Spider.h"
#include "Structs.h"

#include <ctime> // time
#include <map>
#include <random> // mt19937
#include <string>
#include <string_view>

class GameManager
{
private:
	Player m_player{};
	Dungeon m_dungeonMap{};
	static std::map<std::string, std::string> m_commandList;

public:

	void constructDungeon() { m_dungeonMap = Dungeon{ Random::getRandomNumberInRange(20, 40) }; };
	void intro();
	GameStatus gameLoop();
	void determineResponseToMovement(int roomId, RoomType roomType);
	CombatStatus combatLoop(Enemy* enemy);
	std::string parseInput(const std::string &input);
	void executeCommand(const std::string_view &command);
	void printCommandList();
	void outro(const GameStatus &gameStatus);
	bool playAgain();
	void reset();

};

#endif