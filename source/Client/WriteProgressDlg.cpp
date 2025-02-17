/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Wait dialog.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "StdAfx.h"
#include "resource.h"
#include "WriteProgressDlg.h"

#include "BugTrapUtils.h"
#include "ResManager.h"
#include "Globals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 * @addtogroup BugTrapUI BugTrap Graphical User Interface
 * @{
 */

/// Flag indicating window de-initialization.
BOOL CWriteProgressDialog::m_bDone;
/// Progress control.
CAnimProgressBar CWriteProgressDialog::m_pbAnim;

void CWriteProgressDialog::InitVars(void)
{
	m_hThread = NULL;
	m_hwndParent = NULL;
	m_hwndWait = NULL;
	m_bDone = FALSE;
}

/**
 * @param hwnd - window handle.
 * @param hwndFocus - system-defined focus window.
 * @param lParam - user-defined parameter.
 * @return true to setup focus to system-defined control.
 */
BOOL CWriteProgressDialog::WriteProgressDlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	lParam; hwndFocus;

	HWND hwndCtl = GetDlgItem(hwnd, IDC_PROGRESS_BAR);
	m_pbAnim.Attach(hwndCtl);
	m_pbAnim.SetNumChunks(3);
	m_pbAnim.Play();

	return FALSE;
}

/**
 * @param hwnd - window handle.
 * @param id - control ID.
 * @param hwndCtl - control handle.
 * @param codeNotify - notification code.
 */
void CWriteProgressDialog::WriteProgressDlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	codeNotify; hwndCtl;
	switch (id)
	{
	case IDOK:
	case IDCANCEL:
		if (m_bDone)
		{
			// User may press Alt-F4 and close
			// this window before we are ready.
			DestroyWindow(hwnd);
		}
		break;
	}
}

/**
 * @param hwnd - window handle.
 */
void CWriteProgressDialog::WriteProgressDlg_OnDestroy(HWND hwnd)
{
	hwnd;
	m_pbAnim.Detach();
	PostQuitMessage(0);
}

/**
 * @param hwnd - window handle.
 * @param x - X coordinate.
 * @param y - Y coordinate.
 */
UINT CWriteProgressDialog::WriteProgressDlg_OnNCHitTest(HWND hwnd, int x, int y)
{
	hwnd; x; y;
	SetWindowLongPtr(hwnd, DWLP_MSGRESULT, HTCAPTION);
	return TRUE;
}

/**
 * @param hwndDlg - window handle.
 * @param uMsg - message identifier.
 * @param wParam - 1st message parameter.
 * @param lParam - 2nd message parameter.
 * @return message result.
 */
INT_PTR CALLBACK CWriteProgressDialog::WriteProgressDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hwndDlg, WM_INITDIALOG, WriteProgressDlg_OnInitDialog);
		HANDLE_MSG(hwndDlg, WM_COMMAND, WriteProgressDlg_OnCommand);
		HANDLE_MSG(hwndDlg, WM_DESTROY, WriteProgressDlg_OnDestroy);
		HANDLE_MSG(hwndDlg, WM_NCHITTEST, WriteProgressDlg_OnNCHitTest);
	default: return FALSE;
	}
}

/**
 * @param hwndParent - parent window handle.
 */
CWriteProgressDialog::CWriteProgressDialog(HWND hwndParent)
{
	InitVars();
	BeginWait(hwndParent);
}

/**
 * @param pParam - thread parameter (pointer to CWriteProgressDialog).
 * @return error code.
 */
UINT CALLBACK CWriteProgressDialog::WaitThreadProc(PVOID pParam)
{
	CWriteProgressDialog* _this = (CWriteProgressDialog*)pParam;
	_ASSERT(_this != NULL);

	HCURSOR hOldCursor = SetCursor(g_pResManager->m_hWaitCursor);

	_this->m_hwndWait = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_WRITEPROGRESS_DLG), _this->m_hwndParent, WriteProgressDlgProc);
	if (_this->m_hwndWait != NULL)
	{
		CenterWindow(_this->m_hwndWait, _this->m_hwndParent);
		ShowWindow(_this->m_hwndWait, SW_SHOW);
		UpdateWindow(_this->m_hwndWait);

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
			DispatchMessage(&msg);
	}

	SetCursor(hOldCursor);

	return 0;
}

/**
 * @param hwndParent - parent window handle.
 * @return true if UI thread was successfully launched.
 */
BOOL CWriteProgressDialog::BeginWait(HWND hwndParent)
{
	_ASSERT(m_hThread == NULL);
	m_hwndParent = hwndParent;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, WaitThreadProc, this, 0, NULL);
	return (m_hThread != NULL);
}

void CWriteProgressDialog::EndWait(void)
{
	if (m_hThread != NULL)
	{
		m_bDone = TRUE;
		PostMessage(m_hwndWait, WM_CLOSE, 0, 0);
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}
	InitVars();
}

/** @} */
