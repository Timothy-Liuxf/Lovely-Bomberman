#ifndef PROP_H

#define PROP_H

//////////////////////////////////////
//
//  道具
//

#include "obj_base.h"

//道具基类
class Prop : public obj_base
{
public:

	enum class propType
	{
		null = 0,				//不是任何道具

		//buf类道具
		buffbegin = 1,			//buff类道具开始
		glove = 2,				//手套
		shield = 3,				//盾牌
		addtnt = 4,				//增加炮弹数
		addlife = 5,			//增加生命
		shoe = 6,				//跑鞋
		jinKeLa = 7,			//金坷垃
		buffend = 8,			//buff类道具结束

		//特殊炸弹类道具
		specialbombbegin = 9,	//特殊类炸弹开始
		lachrymator = 10,		//催泪瓦斯
		mine = 11,				//地雷
		fire = 12,				//火焰枪
		ice = 13,				//冰枪
		grenade = 14, 			//手榴弹
		missile = 15, 			//导弹
		specialbombend = 16		//特殊类炸弹结束
	};

	Prop(sigPosType x, sigPosType y) : obj_base(x, y, false, 0), state(propState::unpicked) {}
	virtual objType GetObjType() const override { return objType::prop; }
	//获取道具种类
	virtual propType GetPropType() const = 0; 
	virtual bool IsBuff() const = 0; 
	virtual bool IsSpecialBomb() const = 0; 

	//获取道具状态
	bool IsUnpicked() const { return state == propState::unpicked; }
	bool IsPicked() const { return state == propState::picked; }
	bool IsLaid() const { return state == propState::laid; }

	virtual ~Prop() {}

protected: 

	enum class propState		//道具的状态
	{
		unpicked,		//未被捡起
		picked,			//已经被捡起
		laid			//已经被放置
	};

	propState state;	//道具所处的状态
}; 

//buff类道具
class Buff : public Prop
{
public: 

	Buff(sigPosType x, sigPosType y) : Prop(x, y) {}
	virtual bool IsBuff() const { return true; }
	virtual bool IsSpecialBomb() const { return false; }
	virtual ~Buff() {}
};

//道具手套
class Glove : public Buff	//手套类
{
public: 
	Glove(sigPosType x, sigPosType y) : Buff(x, y) {}
	virtual propType GetPropType() const override { return propType::glove; }
	virtual ~Glove() {}
};

//盾牌类
class Shield final : public Buff
{
public: 
	Shield(sigPosType x, sigPosType y) : Buff(x, y) {}
	virtual propType GetPropType() const override { return propType::shield; }
	virtual ~Shield() {}
};

//增加炮弹数
class AddTNT final : public Buff
{
public:
	AddTNT(sigPosType x, sigPosType y) : Buff(x, y) {}
	virtual propType GetPropType() const override { return propType::addtnt; }
	virtual ~AddTNT() {}
};

//增加生命
class AddLife final : public Buff
{
public:
	AddLife(sigPosType x, sigPosType y) : Buff(x, y) {}
	virtual propType GetPropType() const override { return propType::addlife; }
	virtual ~AddLife() {}
};

//跑鞋
class Shoe final : public Buff
{
public:
	Shoe(sigPosType x, sigPosType y) : Buff(x, y) {}
	virtual propType GetPropType() const override { return propType::shoe; }
	virtual ~Shoe() {}
}; 

//金坷垃
class JinKeLa final : public Buff
{
public:
	JinKeLa(sigPosType x, sigPosType y) : Buff(x, y) {}
	virtual propType GetPropType() const override { return propType::jinKeLa; }
	virtual ~JinKeLa() {}
};

//特殊炸弹类道具

class SpecialBomb : public Prop
{
public:

	SpecialBomb(sigPosType x, sigPosType y) : Prop(x, y), ownerID(0), direct(direction::Null) {}

	virtual bool IsBuff() const { return false; }
	virtual bool IsSpecialBomb() const { return true; }

	//获取主人ID
	int GetOwnerID() const { return ownerID; }
	void SetOwnerID(int newOwnerID) { ownerID = newOwnerID; }

	//捡起道具
	void SetPicked(int newOwnerID) { state = propState::picked; ownerID = newOwnerID; }

	//放置炸弹
	void SetLaid(sigPosType x, sigPosType y, direction laidDirect, sigPosType newMoveSpeed);

	//按不同方向放置炸弹
	void SetLaidUp(sigPosType x, sigPosType y, sigPosType moveSpeed) { SetLaid(x, y, direction::Up, moveSpeed); }
	void SetLaidDown(sigPosType x, sigPosType y, sigPosType moveSpeed) { SetLaid(x, y, direction::Down, moveSpeed); }
	void SetLaidLeft(sigPosType x, sigPosType y, sigPosType moveSpeed) { SetLaid(x, y, direction::Left, moveSpeed); }
	void SetLaidRight(sigPosType x, sigPosType y, sigPosType moveSpeed) { SetLaid(x, y, direction::Right, moveSpeed); }

	//获取方向
	direction GetDirection() const { return direct; }

	//检查是否要消失了
	virtual bool AboutToDisappear() const = 0; 

	virtual ~SpecialBomb() {}

protected:

	int ownerID;		//主人ID
	direction direct;	//发射朝向
}; 


//催泪瓦斯
class Lachrymator final : public SpecialBomb
{
public:
	Lachrymator(sigPosType x, sigPosType y, int timeLeft) : SpecialBomb(x, y), timeLeft(timeLeft), visible(true) {}
	virtual propType GetPropType() const override { return propType::lachrymator; }

	int GetTimeLeft() const { return timeLeft; }
	void SubTimeLeft(int subTime) { timeLeft -= subTime; }

	bool Visible() const { return visible; }
	//设为不可见
	void SetInvisible() { visible = false; }

	//检查催泪瓦斯是否要消失了
	virtual bool AboutToDisappear() const override { return timeLeft <= 0; }

	virtual ~Lachrymator() {}

private:

	int timeLeft; 		//距离催泪瓦斯消失的时间
	bool visible;		//是否可见
};

//地雷
class Mine final : public SpecialBomb
{
public: 
	Mine(sigPosType x, sigPosType y, int timeLeft) : SpecialBomb(x, y), timeLeft(timeLeft), visible(true) {}
	virtual propType GetPropType() const override { return propType::mine; }

	int GetTimeLeft() const { return timeLeft; }
	void SubTimeLeft(int subTime) { timeLeft -= subTime; }

	bool Visible() const { return visible; }
	//设为不可见
	void SetInvisible() { visible = false; }

	//检查地雷是否要消失了
	virtual bool AboutToDisappear() const override { return timeLeft <= 0; }

	virtual ~Mine() {}

private: 

	int timeLeft; 		//距离地雷消失的时间
	bool visible;		//是否可见
}; 

//火焰枪
class Fire final : public SpecialBomb
{
public:
	Fire(sigPosType x, sigPosType y) : SpecialBomb(x, y) {}
	virtual propType GetPropType() const override { return propType::fire; }

	//检查火焰是否要消失了
	virtual bool AboutToDisappear() const override { return false; }

	virtual ~Fire() {}

};

//寒冰枪
class Ice final : public SpecialBomb
{
public:
	Ice(sigPosType x, sigPosType y) : SpecialBomb(x, y) {}
	virtual propType GetPropType() const override { return propType::ice; }

	//检查冰是否要消失了
	virtual bool AboutToDisappear() const override { return false; }

	virtual ~Ice() {}

};

//手榴弹
class Grenade final : public SpecialBomb
{
public: 
	Grenade(sigPosType x, sigPosType y, sigPosType maxDistance) : SpecialBomb(x, y), leftDistance(maxDistance) {}
	virtual propType GetPropType() const override { return propType::grenade; }
	
	sigPosType GetLeftDistance() const { return leftDistance; }

	//检查手榴弹是否要爆炸了
	virtual bool AboutToDisappear() const override { return leftDistance <= 0; }

	//手榴弹移动
	void Move(); 

	virtual ~Grenade() {}

private: 

	sigPosType leftDistance;		//剩余可以行进的距离

}; 

//导弹
class Missile final : public SpecialBomb
{
public: 
	Missile(sigPosType x, sigPosType y) : SpecialBomb(x, y) {}
	virtual propType GetPropType() const override { return propType::missile; }
	//导弹移动
	void Move() { obj_base::Move(direct); }

	virtual bool AboutToDisappear() const override { return false; }

	virtual ~Missile() {}
};

#endif // #ifndef PROP_H
