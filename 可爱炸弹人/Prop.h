#ifndef PROP_H

#define PROP_H

//////////////////////////////////////
//
//  道具
//

#include "basic.h"

//道具基类
class Prop : public obj_base
{
public:

	enum propType
	{
		//buf类道具
		glove,		//手套
		shield,		//盾牌
		addtnt,		//增加炮弹数
		addlife,	//增加生命
		shoe,		//跑鞋
		jinKeLa,	//金坷垃
		
		//特殊炸弹类道具
		lachrymator,//催泪瓦斯
		mine,		//地雷
		fire,		//火焰枪
		ice,		//冰枪
		grenade, 	//手榴弹
		missile		//导弹
	};

	Prop(sigPosType x, sigPosType y) : obj_base(x, y, false, 0), state(propState::unpicked) {}
	virtual objType GetObjType() const override { return objType::prop; }
	//获取道具种类
	virtual propType GetPropType() const = 0; 

	//获取道具状态
	bool isUnpicked() const { return state == propState::unpicked; }
	bool ispicked() const { return state == propState::picked; }
	bool isLaid() const { return state == propState::laid; }

	//捡起道具
	virtual void SetPicked() { state = propState::picked; }
	
	virtual ~Prop() {}

protected: 

	enum propState		//道具的状态
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

	//获取主人ID
	int GetOwnerID() const { return ownerID; }
	void SetOwnerID(int newOwnerID) { ownerID = newOwnerID; }

	//按不同方向放置炸弹
	void SetLaidUp(sigPosType x, sigPosType y, int moveSpeed) { SetLaid(x, y, direction::Up, moveSpeed); }
	void SetLaidDown(sigPosType x, sigPosType y, int moveSpeed) { SetLaid(x, y, direction::Down, moveSpeed); }
	void SetLaidLeft(sigPosType x, sigPosType y, int moveSpeed) { SetLaid(x, y, direction::Left, moveSpeed); }
	void SetLaidRight(sigPosType x, sigPosType y, int moveSpeed) { SetLaid(x, y, direction::Right, moveSpeed); }

	//道具移动
	void Move() { obj_base::Move(direct); }

	//检查是否要消失/爆炸了
	virtual bool AboutToDisappear() const = 0; 

	virtual ~SpecialBomb() {}

protected:

	//放置炸弹
	virtual void SetLaid(sigPosType x, sigPosType y, direction laidDirect, int newMoveSpeed); 

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
	Fire(sigPosType x, sigPosType y, int timeLeft) : SpecialBomb(x, y), timeLeft(timeLeft) {}
	virtual propType GetPropType() const override { return propType::fire; }

	int GetTimeLeft() const { return timeLeft; }
	void SubTimeLeft(int subTime) { timeLeft -= subTime; }

	//检查火焰是否要消失了
	virtual bool AboutToDisappear() const override { return timeLeft <= 0; }

	virtual ~Fire() {}
private:
	int timeLeft;	//火焰剩余的存在时间
};

//寒冰枪
class Ice final : public SpecialBomb
{
public:
	Ice(sigPosType x, sigPosType y, int timeLeft) : SpecialBomb(x, y), timeLeft(timeLeft) {}
	virtual propType GetPropType() const override { return propType::mine; }

	int GetTimeLeft() const { return timeLeft; }
	void SubTimeLeft(int subTime) { timeLeft -= subTime; }

	//检查冰是否要消失了
	virtual bool AboutToDisappear() const override { return timeLeft <= 0; }

	virtual ~Ice() {}

private:

	int timeLeft; 		//距离寒冰消失的时间
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

	virtual ~Grenade() {}
private: 
	sigPosType leftDistance;		//剩余可以行进的距离
}; 

//导弹
class Missil final : public SpecialBomb
{
public: 
	Missil(sigPosType x, sigPosType y) : SpecialBomb(x, y) {}

	virtual ~Missil() {}
};

#endif // #ifndef PROP_H
