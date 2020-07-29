////////////////////////////////
//
//  一且游戏对象的基类
//

#ifndef BASIC_H

#define BASIC_H

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

	enum objType		//对象类型
	{
		role,			//角色
		tnt,			//普通炸弹
		softObstacle,	//软障碍
		hardObstacle,	//硬障碍
		bombArea,		//爆炸区域
		prop			//道具
	}; 

	obj_base(sigPosType x, sigPosType y, bool isMoving, sigPosType moveSpeed) 
		: pos(x, y), isMoving(isMoving), moveSpeed(moveSpeed) 
	{
		CorrectPos(); 
	}

	//获取坐标
	posType GetPos() const { return pos; }

	//设置地图范围——行数和列数以及每个格的边长
	static void SetMapBound(int rows, int cols, int posUnitPerCell);  
	
	//判断是否在移动
	bool IsMoving() const { return isMoving; }

	//设置为移动状态
	void SetMoving() { isMoving = true; }
	//设置为不移动状态
	void SetNoMoving() { isMoving = false; }

	//获取移动速度
	sigPosType GetMoveSpeed() const { return moveSpeed; }

	//获取对象类型虚函数
	virtual objType GetObjType() const = 0;

	virtual ~obj_base() {}

protected:

	enum direction		//方向
	{
		Null, 
		Up, 
		Down, 
		Left, 
		Right
	}; 

	void Move(direction direc); 

	//记录地图左上角和右下角的坐标
	static posType minPos; 
	static posType maxPos; 
	static const sigPosType EPS; //坐标精度
	static int posUnitPerCell;	//每个格子的边长
	void CorrectPos();		//修正坐标，放置越界


	posType pos;			//位置坐标
	bool isMoving;			//是否在移动
	sigPosType moveSpeed;	//移动速度

};

#endif // #ifndef OBJBASE_H
