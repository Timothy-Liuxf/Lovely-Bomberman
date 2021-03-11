

#ifndef UI_H

#define UI_H

#include "Game.h"
#include "framework.h"
#include "resource.h"
#include "str.h"
#include "BasicWindow.h"
#include "BasicDialog.h"
#include <queue>
#include <future>
#include <exception>
#include "..\Images\Images.h"
#include "..\Images\resource.h"

//定时器ID

#define TIMER_ID_START 9999					//开始刷新窗口计时器

class UI final : public BasicWindow
{
public: 

	UI();

	//开始创建窗口执行
	int Begin(HINSTANCE hInstance, int nCmdShow); 
	

	~UI();

	class FailToLoadGameImage : std::exception
	{
	public:
		virtual const char* what() const { return "Fail to load images!"; }
	};

	class FailToExecuteGameSound : std::exception
	{
	public:
		virtual const char* what() const { return "Fail to execute the application for playing the sound of the game!"; }
	};

protected: 

	//消息处理函数
	virtual bool MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override; 

private: 

	enum class programstate	//程序状态
	{
		starting = 0,		//开始界面
		gaming = 1,			//正在游戏
		gamePulsing = 2,	//游戏暂停
		changeLevel = 3		//换关
	}; 
	programstate programState;			//程序状态

	static const int objSize;
	static const int propSize;
	static const POINT mainWndPos;
	static const POINT mainWndSize;
	static const int dataFps;			//数据帧
	static const int paintFps;			//画面帧

	Game* pGame = nullptr;				//游戏内部逻辑
	bool IsPlayer(int roleID) const		//判断是否是玩家
	{
		return pGame ? (roleID == pGame->GetID1() || (pGame->GetNumOfPlayer() == 2 && roleID == pGame->GetID2())) : false; 
	}
	bool IsComputer(int roleID) const { return pGame ? !IsPlayer(roleID) : false; }

	void ScanData();					//扫描游戏数据
	void RoleControl(int roleID);		//角色控制
	void RefreshScreen();				//刷新屏幕
	void EndGame(int result);			//结束游戏
	void AI(int roleID);				//电脑AI

	//读取高分
	std::vector<std::pair<std::_tstring, int>> ReadHighScore() const;

	//写入高分
	void SaveHighScore(const std::vector<std::pair<std::_tstring, int>>& scoreList) const;
	
	//新分数
	void newScore(int numOfPlayer, Game::Difficulty difficulty, int score); 

	std::future<void>* pScanDataTask = nullptr; 
	std::future<void>* pRefreshScreenTask = nullptr;
	std::vector<std::future<void>*> pRoleControlTasks;

	//记录角色按键状态
	std::vector<bool> playerLay{ false, false }; 
	
	//位图句柄，缺省值均为NULL（C++11及以上）
	HBITMAP hBmMem = NULL;				//用于缓冲
	HBITMAP hBmMain = NULL;				//开始界面位图背景
	HBITMAP hBmBkgnd = NULL;			//背景位图句柄
	HBITMAP hBmRole = NULL;				//角色位图句柄
	HBITMAP hBmTnt = NULL;				//炸弹位图句柄
	HBITMAP hBmObstacle = NULL;			//障碍物位图句柄
	HBITMAP hBmGlove = NULL;			//手套位图句柄
	HBITMAP hBmShield = NULL;			//盾牌位图句柄
	HBITMAP hBmAddTnt = NULL;			//加炮弹位图句柄
	HBITMAP hBmAddLife = NULL;			//加生命位图句柄
	HBITMAP hBmShoe = NULL;				//跑鞋位图句柄
	HBITMAP hBmJinKeLa = NULL;			//金坷垃位图句柄
	HBITMAP hBmLachrymator = NULL;		//催泪瓦斯位图句柄
	HBITMAP hBmMine = NULL;				//地雷位图句柄
	HBITMAP hBmFire = NULL;				//火焰枪位图句柄
	HBITMAP hBmIce = NULL;				//冰枪位图句柄
	HBITMAP hBmGrenade = NULL;			//手榴弹位图句柄
	HBITMAP hBmMissile = NULL;			//导弹位图句柄

	//（位图信息）
	BITMAP bmBkgnd;						//背景位图信息
	BITMAP bmMain;						//开始界面位图
	BITMAP bmRole;						//角色位图信息
	BITMAP bmTnt;						//炸弹位图信息
	BITMAP bmObstacle;					//障碍物位图信息
	BITMAP bmGlove;						//手套位图信息
	BITMAP bmShield;					//盾牌位图信息
	BITMAP bmAddTnt;					//加炮弹位图信息
	BITMAP bmAddLife;					//加生命位图信息
	BITMAP bmShoe;						//跑鞋位图信息
	BITMAP bmJinKeLa;					//金坷垃位图信息
	BITMAP bmLachrymator;				//催泪瓦斯位图信息
	BITMAP bmMine;						//地雷位图信息
	BITMAP bmFire;						//火焰枪位图句柄信息
	BITMAP bmIce;						//冰枪位图信息
	BITMAP bmGrenade;					//手榴弹位图信息
	BITMAP bmMissile;					//导弹位图信息

	LONG capMenuAppendCy;				//标题栏和菜单栏占用的高度

	bool LoadGameImg();					//加载位图
	void CreateBuffer(HWND hWnd);		//创建缓冲位图
	bool TryExecutingGameSound();		//尝试加载音乐播放文件
	const SoundPlay soundPlay;			//音乐/音效播放者

	//播放背景音乐

	class PlayMainMusic
	{
	public:
		void operator()() const;
	};

	//开始画图
	void Paint(HWND hWnd, BOOL calledByPaintMessage); 

	//人物坐标转图像坐标
	int PosToPaint(int p) { return (int)(((double)p / Game::GetPosUnitPerCell() - 0.5) * objSize); }

	////////////////////////////////////////////////////
	//
	// 子对话框
	//

	//开始游戏对话框类
	class StartGameDlg : public BasicModalDialog
	{
	private: using Difficulty = Game::Difficulty; 

	protected: 
		virtual void MessageProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) override; 

	public: 
		StartGameDlg() : choose(false), difficulty(Difficulty::easy), numOfPlayer(0), player1ID(0), player2ID(0) {}
		bool Begin(HINSTANCE hInstance, HWND hWndParent);
		bool Choose() const { return choose; }
		int NumOfPlayer() const { return numOfPlayer; }
		int Player1ID() const { return player1ID; }
		int Player2ID() const { return player2ID; }
		Difficulty GetDifficulty() const { return difficulty; }

	private: 
		bool choose;					//是否选择成功
		Difficulty difficulty;			//难度
		int numOfPlayer;				//玩家数
		int player1ID;					//1P的id
		int player2ID;					//2P的ID
	};

	//输入高手姓名对话框
	class InputNameDlg : public BasicModalDialog
	{
	protected: 
		virtual void MessageProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) override;
	public: 
		void Begin(HINSTANCE hInstance, HWND hWndParent);
		std::_tstring GetName() const { return name; }
	private: 
		std::_tstring name; 
	};

	StartGameDlg startGameDlg; 
	InputNameDlg inputNameDlg; 
};

#endif	// #ifndef GLOBALS_H
