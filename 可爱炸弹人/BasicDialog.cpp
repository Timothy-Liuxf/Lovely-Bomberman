#include "BasicDialog.h"
#include <iostream>
BOOL BasicModalDialog::Init(HINSTANCE hInstance, LPCTSTR c_lpszTemplateName, HWND hWndParent)
{
	m_hInst = hInstance; 
	return (BOOL)DialogBoxParam(hInstance, c_lpszTemplateName, hWndParent, DlgProc, (LPARAM)this); 
	m_hInst = NULL; 
	m_hDlg = NULL; 
}

INT_PTR CALLBACK BasicModalDialog::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	BasicModalDialog* thisDlg = NULL; 
	if (message == WM_INITDIALOG)
	{
		thisDlg = (BasicModalDialog*)lParam; 
		thisDlg->m_hDlg = hDlg; 
		SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR)thisDlg); 
	}
	thisDlg = (BasicModalDialog*)GetWindowLongPtr(hDlg, GWLP_USERDATA); 
	if (thisDlg) thisDlg->MessageProc(hDlg, message, wParam, lParam); 
	return 0; 
}


bool BasicModalDialog::IsButtonCheck(int buttonID) const
{
	return SendMessage(GetDlgItem(m_hDlg, buttonID), BM_GETCHECK, 0, 0) == BST_CHECKED;
}

void BasicModalDialog::EnableButton(int buttonID)
{
	EnableWindow(GetDlgItem(m_hDlg, buttonID), TRUE);
}

void BasicModalDialog::DisableButton(int buttonID)
{
	EnableWindow(GetDlgItem(m_hDlg, buttonID), FALSE);
}

void BasicModalDialog::SetUnChecked(int buttonID)
{
	SendMessage(GetDlgItem(m_hDlg, buttonID), BM_SETCHECK, BST_UNCHECKED, 0); 
}

void BasicModalDialog::SetChecked(int buttonID)
{
	SendMessage(GetDlgItem(m_hDlg, buttonID), BM_SETCHECK, BST_CHECKED, 0);
}

