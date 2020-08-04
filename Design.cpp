
class: UI 
实现：Game* pGameInfo; 

private: 

bool isGaming; 

启动线程开始GameBegin()
GameBegin()
{
	pGameInfo->new
	pGameInfo: ResetGame
	开启数据扫描线程scanData
	开启各个角色线程roleControl * 4
	开启屏幕刷新线程refreshScreen
}

GamePause(); 

GameContinue(); 

GameEnd(); 

refreshScreen()
{
	while (isGaming)
	{
		InvalidateRect; 
		Sleep(画面帧)
	}
}
roleControl_GetKey()
{
	while (isGaming && 角色活着)
	{
		e.g. GetKey('W')
		{
			同步调用函数行走一格: Game::WalkOneUp
		}
		e.g. GetKey('space')
		{
			同步调用放置炸弹Game::LayTnt
		}
		Sleep(数据帧)
	}
}

scanData()
{
	while (isGaming)
	{
		同步调用检查人物：Game::checkRole； 
		同步调用检查炸弹和特殊武器 Game::checkTnt; 
		同步调用检查游戏是否结束 Game::CheckGameEnd; 
		Sleep(数据帧); 
	}
}

class Game: 

public: 

Game(int numOfRole); 		//Construct function

list<const obj_base*> GetMapObj(x, y)		//获取地图上某点的所有物件
{
	for (遍历角色) if (活着) list.push
	for (遍历其它物件) if (没有被人拿起来) list.push
	return list; 
}

WalkOne(int roleID) * 4	//Up, Down, Left, Right
{
	if (canWalk) for (walk one cell);  
	if (能推动炸弹) 同步调用推动炸弹函数
	else return; 
}

bool MoveTnt(); 	//推动炸弹

LayTnt(int roleID)
{
	if (cannotLayTnt: Obstacle、other tnts) return; 
	for (检查角色的道具): if (haveSpecialBomb) 
				{
					erase Role's Special Bomb; 
					lay SpecialBomb at role's pos;  
					return; 
				}
	if (haveNoTnt) return; 
	layTnt: Role's num of Tnt--, lay tnt at role's pos; 
}
checkRole()
{
	for (遍历角色)
	{
		if (die) continue; 
		if (所在格子有道具) 捡起道具，触发道具效果; 
	}
}
checkTnt()
{
	for (遍历地图中的物件) 得到指针，dynamic_cast<>
	if(是道具并且未捡起) continue; 
	switch(指针指向的对象)
	{
	case TNT: 
		if (没有在移动)
		{
			if (即将爆炸但还没有爆炸) 同步执行BombTnt(TNT*); 
			else --timeLeft; 
		}
		else
		{
			检测碰撞，如果碰撞，BombTnt，否则move(); 
		}
		break; 
	case 地雷: 
		if (即将消失) 放入回收站 return; 
		if (有非主人踩在了上面)
		{
			炸了所有人; 放入回收站;
		}
		--timeLeft; break; 
	case 催泪瓦斯: 
		if (即将消失) 放入回收站 return; 
		if (有非主人踩在了上面) 
		{
			所有人减速，放入回收站；
		}
		--timeLeft; break; 
	case 手榴弹: 
		if (即将爆炸) { Bomb手榴弹(); }
		else (不爆炸) move(); 
		break; 
	case 导弹: 
		if (碰撞) { BombMapCell(ownerID, x, y); 放入回收站; }
		else move(); break; 
	case Bombrea: 
		if (即将消失) { 放入回收站; return; } 
		BombMapCell(ownerID, x, y); 
		break; 
	}
}
 
private: 

BombTNT(tnt*)
{
	给主人加子弹、加入爆炸范围、炸掉障碍物、放入回收站等
}

BombFire(fire*)	//火焰弹刚爆炸
{
	录入爆炸范围、炸掉障碍物、放入回收站
}

Bomb手榴弹(手榴弹*)
{录入爆炸范围、炸掉障碍物}

BombMapCell(ownerID, x, y); //爆破地图的一块
