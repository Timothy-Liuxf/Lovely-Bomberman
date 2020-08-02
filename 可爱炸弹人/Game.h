#ifndef GAME_H

#define GAME_H

#include "Role.h"
#include <list>
#include <vector>

class Game
{
private: 

	static const int playerInitialLife;		//玩家初始生命
	static const int defPosUnitPerCell;		//玩每格的长度

public: 
	const std::vector<std::vector<int>>& GetGameMap(unsigned int num) const; 
	Game(int numOfPlayer) : numOfPlayer(numOfPlayer) {}

	int GetNumOfPlayer() const { return numOfPlayer; }

	

	//强制停止游戏
	void StopGame() { isGaming = false; }

private: 
	
	enum class mapItem
	{
		blank = 0,			//空格
		birthPoint = 1,		//出生点
		softObstacle = 2,	//软障碍
		hardObstacle = 3	//硬障碍
	}; 

	//游戏地图
	static const std::vector<std::vector<std::vector<int>>> gameMap; 
	static const unsigned int numOfMap; 
	int numOfPlayer;		//游戏人数
	bool isGaming;			//是否正在游戏

};

#endif	//#ifndef GAME_H
