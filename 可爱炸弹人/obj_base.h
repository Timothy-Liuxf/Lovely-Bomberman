////////////////////////////////
//
//  一且游戏对象的基类
//

#ifndef OBJ_BASE_H

#define OBJ_BASE_H

#include <mutex>

class obj_base
{
public:

	typedef int sigPosType;
	typedef struct tagPosType
	{
		sigPosType x;
		sigPosType y;
		tagPosType(sigPosType x, sigPosType y) : x(x), y(y) {}
	} posType;

	enum class objType		//对象类型
	{
		null,			//不是任何类型
		role,			//角色
		tnt,			//普通炸弹
		softObstacle,	//软障碍
		hardObstacle,	//硬障碍
		bombArea,		//爆炸区域
		prop			//道具
	}; 

	enum class direction		//方向
	{
		Null,
		Up,
		Down,
		Left,
		Right
	};

	obj_base(sigPosType x, sigPosType y, bool isMoving, sigPosType moveSpeed) 
		: pos(x, y), isMoving(isMoving), moveSpeed(moveSpeed) 
	{
		CorrectPos(); 
	}

	
	//设置地图范围——行数和列数以及每个格的边长
	static void SetMapBound(int rows, int cols, obj_base::sigPosType posUnitPerCell);
	
	//获取坐标
	posType GetPos() const { return pos; }

	//获取范围
	posType GetMinPos() const { return minPos; }
	posType GetMaxPos() const { return maxPos; }

	//判断是否在移动
	bool IsMoving() const { return isMoving; }

	//获取移动速度
	sigPosType GetMoveSpeed() const { return moveSpeed; }

	//获取互斥锁
	std::mutex& GetMutex() { return objMutex; }
	
	//获取对象类型虚函数
	virtual objType GetObjType() const = 0; 

	virtual ~obj_base() {}

protected:

	void Move(direction direc);										//移动一步
	void MoveLessThanOneStep(direction direc, sigPosType moveDistance);	//移动少于1步

	//记录地图左上角和右下角的坐标
	static posType minPos; 
	static posType maxPos; 
	static const sigPosType EPS;				//坐标精度
	static obj_base::sigPosType posUnitPerCell;	//每个格子的边长
	void CorrectPos();						//修正坐标，放置越界


	posType pos;			//位置坐标
	bool isMoving;			//是否在移动
	sigPosType moveSpeed;	//移动速度
	std::mutex objMutex;	//互斥锁，供多线程使用
};

#endif // #ifndef OBJBASE_H
