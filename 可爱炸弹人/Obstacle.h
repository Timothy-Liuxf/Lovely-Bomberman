#ifndef OBSTACLE_H

#define OBSTACLE_H

#include "obj_base.h"

//障碍基类
class Obstacle : public obj_base
{
public: 
	Obstacle(sigPosType x, sigPosType y, bool style) : obj_base(x, y, false, 0), style(style) {}
	bool GetStyle() const { return style; }
	virtual ~Obstacle() {}

private: 

	bool style;		//障碍的样式，一种障碍有两种样式
};

class SoftObstacle final : public Obstacle
{
public:
	SoftObstacle(sigPosType x, sigPosType y, bool style) : Obstacle(x, y, style) {}
	virtual objType GetObjType() const override { return objType::softObstacle; }
	virtual ~SoftObstacle() {}
};

class HardObstacle final : public Obstacle
{
public:
	HardObstacle(sigPosType x, sigPosType y, bool style) : Obstacle(x, y, style) {}
	virtual objType GetObjType() const override { return objType::hardObstacle; }
	virtual ~HardObstacle() {}
};

#endif // #ifndef OBSTACLE_H
