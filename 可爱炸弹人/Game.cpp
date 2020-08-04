#include "Game.h"

const unsigned int Game::numOfLevel = 1; 
const int Game::playerInitialLife = 3;		//玩家初始生命
const int Game::defPosUnitPerCell = 1024;	//玩每格的长度
const int Game::roleInitialMoveSpeed = 32;	//玩家初始移动速度

const std::vector<std::vector<std::vector<int>>> Game::gameMap
{
	{
		{0, 0, 0, 0, 3, 2, 0, 0, 2, 0, 2, 2, 3, 0, 0}, 
		{2, 2, 3, 2, 0, 0, 3, 3, 0, 3, 3, 0, 0, 2, 3}, 
		{3, 2, 3, 0, 3, 3, 2, 3, 2, 0, 3, 0, 3, 2, 2}, 
		{3, 0, 0, 1, 0, 0, 2, 2, 3, 0, 0, 1, 0, 0, 3}, 
		{3, 0, 3, 0, 3, 0, 3, 2, 0, 0, 3, 0, 3, 2, 0}, 
		{2, 0, 2, 0, 3, 2, 3, 3, 3, 0, 2, 2, 3, 3, 2}, 
		{3, 0, 3, 2, 3, 0, 3, 3, 0, 3, 3, 0, 2, 3, 3}, 
		{2, 0, 0, 0, 2, 3, 0, 3, 0, 0, 2, 0, 3, 2, 3}, 
		{3, 0, 3, 0, 3, 3, 2, 2, 2, 3, 3, 0, 3, 0, 2}, 
		{2, 0, 0, 1, 0, 2, 2, 3, 3, 0, 0, 1, 0, 0, 2}, 
		{0, 3, 3, 0, 3, 3, 3, 3, 2, 2, 3, 0, 3, 2, 3}, 
		{0, 2, 3, 0, 3, 2, 0, 2, 3, 0, 2, 2, 0, 2, 2}, 
		{2, 3, 2, 2, 0, 0, 3, 2, 0, 2, 3, 0, 2, 3, 0}
	}
}; 

const std::vector<std::vector<int>>& Game::GetGameMap(unsigned int num) const
{
	return gameMap[num % numOfLevel]; 
}

Game::Game(int numOfPlayer, int id1, int id2) : numOfPlayer(numOfPlayer), id1(id1), id2(id2), nowLevel(0)
{
	int rows = gameMap[0].size(), cols = gameMap[0][0].size(); 
	roles.resize(5, nullptr); 
	obj_base::SetMapBound(rows, cols, defPosUnitPerCell); 
	std::function<int(int, int)> GetRoleID = [rows, cols](int x, int y)
	{
		if (x < rows / 2 && y < cols / 2) return 1; 
		if (x >= rows / 2 && y < cols / 2) return 2; 
		if (x < rows / 2 && y >= cols / 2) return 3; 
		return 4; 
	}; 
	for (int x = 0; x < rows; ++x)
		for (int y = 0; y < cols; ++y)
			if (static_cast<mapItem>(gameMap[0][x][y]) == mapItem::birthPoint)
			{
				int roleID = GetRoleID(x, y); 
				if (roleID == id1 || numOfPlayer == 2 && roleID == id2)
					roles[roleID] = new Role(CellToPos(x), CellToPos(y), roleInitialMoveSpeed, roleID, playerInitialLife);
				else roles[roleID] = new Role(CellToPos(x), CellToPos(y), roleInitialMoveSpeed, roleID, 1);
			}
}

Game::~Game()
{
	for (int i = 1; i <= 4; ++i)
		delete roles[i]; 
	for (auto p : obstacles)
		delete p; 
	for (auto p : otherGameObjs)
		delete p; 
}

void Game::InitNewLevel(int newLevel, bool mergeScore)
{
	nowLevel = newLevel; 
	for (int i = 1; i <= 4; ++i)
	{
		//是玩家
		if (i == id1 || numOfPlayer == 2 && i == id2)
		{
			if (mergeScore) roles[i]->MergeScore();
			roles[i]->Reset(false, false);
		}
		else roles[i]->Reset(true, true); 
	}
	int rows = gameMap[newLevel].size(), cols = gameMap[newLevel][0].size(); 
	for (int x = 0; x < rows; ++x)
		for (int y = 0; y < cols; ++y)
		{
			switch (static_cast<mapItem>(gameMap[newLevel][x][y]))
			{
			case mapItem::hardObstacle: 
				obstacles.push_back(new HardObstacle(CellToPos(x), CellToPos(y))); 
				break; 
			case mapItem::softObstacle:
				obstacles.push_back(new SoftObstacle(CellToPos(x), CellToPos(y)));
				break;
			}
		}
}

std::list<obj_base*> Game::GetMapObj(int x, int y) const
{
	std::list<obj_base*> res; 
	for (int i = 1; i <= 4; ++i)
	{
		obj_base::posType pos = roles[i]->GetPos(); 
		if (roles[i]->IsLiving() && PosToCell(pos.x) == x && PosToCell(pos.y) == y) 
			res.push_back(static_cast<obj_base*>(roles[i])); 
	}
	for (auto p : obstacles)
	{
		obj_base::posType pos = p->GetPos();
		if (PosToCell(pos.x) == x && PosToCell(pos.y) == y)
			res.push_back(static_cast<obj_base*>(p)); 
	}
	for (auto p : otherGameObjs)
	{
		//被捡起来的道具是看不见的
		if (p->GetObjType() == obj_base::objType::prop && dynamic_cast<Prop*>(p)->IsPicked()) continue; 
		obj_base::posType pos = p->GetPos();
		if (PosToCell(pos.x) == x && PosToCell(pos.y) == y)
			res.push_back(static_cast<obj_base*>(p));
	}
	return res; 
}

void Game::WalkOneCell(int roleID, direction direct)
{
	Role* pRole = roles[roleID]; 
	if (!pRole->IsLiving()) return;											//人死了不能行走
	auto x0 = roles[roleID]->GetPos().x, y0 = roles[roleID]->GetPos().y;	//获取角色位置
	auto x = x0, y = y0; 
	int xc = PosToCell(x0), yc = PosToCell(y0); 
	std::function<int(void)> leftDistance = nullptr;						//行走剩余距离
	std::function<void(void)> walk = nullptr, walkHalf = nullptr;			//行走
	std::list<obj_base*> mapObjList; 
	switch (direct)
	{
	case direction::Up:
	{
		//设置朝向
		pRole->GetMutex().lock(); pRole->SetDirectUp(); pRole->GetMutex().unlock();
		if (xc <= 0) return;			//已经到达边界
		mapObjList = GetMapObj(xc - 1, yc);
		leftDistance = [&x0, &y0, &x, &y]() { return x - (x0 - defPosUnitPerCell); };
		walk = [pRole]() { pRole->MoveUp(); };
		walkHalf = [pRole, &leftDistance] { pRole->MoveUpLessThanOneStep(leftDistance()); };
		break;
	}
	case direction::Down: 
	{
		//设置朝向
		pRole->GetMutex().lock(); pRole->SetDirectDown(); pRole->GetMutex().unlock();
		if (xc >= (obj_base::sigPosType)gameMap[nowLevel].size() - 1) return;			//已经到达边界
		mapObjList = GetMapObj(xc + 1, yc);
		leftDistance = [&x0, &y0, &x, &y]() { return x0 + defPosUnitPerCell - x; };
		walk = [pRole]() { pRole->MoveDown(); };
		walkHalf = [pRole, &leftDistance] { pRole->MoveDownLessThanOneStep(leftDistance()); };
		break;
	}
	case direction::Left:
	{
		//设置朝向
		pRole->GetMutex().lock(); pRole->SetDirectLeft(); pRole->GetMutex().unlock();
		if (yc <= 0) return;			//已经到达边界
		mapObjList = GetMapObj(xc, yc - 1);
		leftDistance = [&x0, &y0, &x, &y]() { return y - (y0 - defPosUnitPerCell); };
		walk = [pRole]() { pRole->MoveLeft(); };
		walkHalf = [pRole, &leftDistance] { pRole->MoveLeftLessThanOneStep(leftDistance()); };
		break;
	}
	case direction::Right:
	{
		//设置朝向
		pRole->GetMutex().lock(); pRole->SetDirectRight(); pRole->GetMutex().unlock();
		if (yc >= (obj_base::sigPosType)gameMap[nowLevel][0].size() - 1) return;			//已经到达边界
		mapObjList = GetMapObj(xc, yc + 1);
		leftDistance = [&x0, &y0, &x, &y]() { return y0 + defPosUnitPerCell - y; };
		walk = [pRole]() { pRole->MoveRight(); };
		walkHalf = [pRole, &leftDistance] { pRole->MoveRightLessThanOneStep(leftDistance()); };
		break;
	}
	}

	//检查是否有障碍
	for (auto pObj : mapObjList)
	{
		switch (pObj->GetObjType())
		{
		case obj_base::objType::hardObstacle:
		case obj_base::objType::softObstacle:
			return;							//碰到障碍不能走
		case obj_base::objType::tnt:		//碰到炸弹
		{
			TNT* pTnt = dynamic_cast<TNT*>(pObj);
			if (pRole->CanPushTnt() && !pTnt->IsMoving())
			{
				//是否能推动炸弹
				if (!MoveTnt(pTnt, direct)) return;	//不能推动炸弹，不能行走
			}
			break;
		}
		}
	}

	//行走一格
	while (leftDistance() > 0)
	{
		pRole->GetMutex().lock();
		if (leftDistance() >= pRole->GetMoveSpeed()) walk();
		else walkHalf();
		pRole->GetMutex().unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / GAME_FPS));
		x = pRole->GetPos().x; y = pRole->GetPos().y; 
	}

}

