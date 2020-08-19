#ifndef BASIC_DIALOG_H

#define BASIC_DIALOG_H

#include "Win32Basic.h"

//模态对话框基类
class BasicModalDialog
{
protected:

	HWND m_hDlg = NULL;				//对话框窗口句柄
	HINSTANCE m_hInst = NULL;		//当前实例句柄
	BOOL Init(HINSTANCE hInstance, LPCTSTR c_lpszTemplateName, HWND hWndParent); 
	virtual void MessageProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) = 0;		//消息处理函数

	bool IsButtonCheck(int buttonID) const;
	void EnableButton(int buttonID);
	void DisableButton(int buttonID);
	void SetUnChecked(int buttonID);
	void SetChecked(int buttonID);
	
private: 

	static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam); 
};

#endif	//#ifndef BASIC_DIALOG_H
