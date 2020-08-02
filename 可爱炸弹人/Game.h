#ifndef GAME_H

#define GAME_H

#include "Role.h"
#include "Obstacle.h"
#include <list>
#include <vector>
#include <functional>

class Game
{
private: 

	static const int playerInitialLife;		//玩家初始生命
	static const int defPosUnitPerCell;		//玩每格的长度
	static const int roleInitialMoveSpeed;	//玩家初始移动速度

public: 
	Game(int numOfPlayer, int id1, int id2); 

	int PosToCell(obj_base::sigPosType x) const { return x / defPosUnitPerCell; }
	obj_base::sigPosType CellToPos(int x) const { return x * defPosUnitPerCell + defPosUnitPerCell / 2; }

	const std::vector<std::vector<int>>& GetGameMap(unsigned int num) const; 
	std::list<obj_base*> GetMapObj(int x, int y) const; 
	const std::vector<Role*>& GetRoles() const { return roles; }
	const std::list<Obstacle*>& GetObstacles() const { return obstacles; }
	const std::list<obj_base*>& GetOtherGameObjs() const { return otherGameObjs; }

	int GetNumOfPlayer() const { return numOfPlayer; }
	int GetNumOfLevel() const { return numOfLevel; }

	//开始新的一关
	void InitNewLevel(int newLevel, bool mergeScore); 

	~Game(); 

private: 
	
	enum class mapItem
	{
		blank = 0,			//空格
		birthPoint = 1,		//出生点
		softObstacle = 2,	//软障碍
		hardObstacle = 3	//硬障碍
	}; 

	//角色，roles[0]是未使用的
	std::vector<Role*> roles; 
	//障碍
	std::list<Obstacle*> obstacles; 
	//其他
	std::list<obj_base*> otherGameObjs; 

	int numOfPlayer;		//游戏人数
	int id1;				//1P的ID
	int id2;				//2P的ID（如果有的话）
	
	//游戏地图
	static const std::vector<std::vector<std::vector<int>>> gameMap; 
	static const unsigned int numOfLevel; 
};

#endif	//#ifndef GAME_H
