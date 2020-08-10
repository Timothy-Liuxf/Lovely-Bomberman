#ifndef TNT_H

#define TNT_H

#include "obj_base.h"

/////////////////////////////////////
//
//  关于炸弹
//

//普通炸弹类
class TNT : public obj_base
{
public: 
	TNT(sigPosType x, sigPosType y, bool isMoving, sigPosType moveSpeed, int ownerID, int distance, int timeLeft) 
		: obj_base(x, y, isMoving, moveSpeed), ownerID(ownerID), distance(distance), timeLeft(timeLeft), direct(direction::Null) {}

	int GetOwnerID() const { return ownerID; }
	int GetDistance() const { return distance; }

	virtual objType GetObjType() const override { return objType::tnt; }
	
	//获取剩余爆炸时间
	int GetTimeLeft() const { return timeLeft; }

	//减少爆炸或存留时间
	void SubTimeLeft(int subTime) { timeLeft -= subTime; }

	//是否将要爆炸
	bool AboutToDisappear() const { return timeLeft <= 0; }

	//被推动
	void SetMoveUp() { SetMove(direction::Up); }
	void SetMoveDown() { SetMove(direction::Down); }
	void SetMoveLeft() { SetMove(direction::Left); }
	void SetMoveRight() { SetMove(direction::Right); }

	//移动
	void Move() { obj_base::Move(direct); }

	//获取移动方向
	direction GetDirect() const { return direct; }

	virtual ~TNT() {}

private: 

	int ownerID;		//主人的ID
	int distance;		//爆炸范围
	int timeLeft;		//剩余爆炸时间
	direction direct;	//移动方向

	//被推动
	void SetMove(direction newDirect) { isMoving = true; direct = newDirect; }

}; 


#endif  //#ifndef BOMB_H
