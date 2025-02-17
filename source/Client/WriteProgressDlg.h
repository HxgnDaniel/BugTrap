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

#pragma once
#include "AnimProgressBar.h"

/**
 * @addtogroup BugTrapUI BugTrap Graphical User Interface
 * @{
 */

/// Wait dialog class.
class CWriteProgressDialog
{
public:
	/// Initialize wait dialog.
	CWriteProgressDialog(void);
	/// Initialize wait dialog and wait message.
	explicit CWriteProgressDialog(HWND hwndParent);
	/// Destroy the object.
	~CWriteProgressDialog(void);
	/// Shows wait dialog.
	BOOL BeginWait(HWND hwndParent);
	/// Closes wait dialog.
	void EndWait(void);

private:
	/// Protect the class from being accidentally copied.
	CWriteProgressDialog(const CWriteProgressDialog& rWaitDialog);
	/// Protect the class from being accidentally copied.
	CWriteProgressDialog& operator=(const CWriteProgressDialog& rWaitDialog);
	/// Initialize member variables.
	void InitVars(void);
	/// Thread procedure that displays wait dialog.
	static UINT CALLBACK WaitThreadProc(PVOID pParam);
	/// Dialog procedure of wait dialog.
	static INT_PTR CALLBACK WriteProgressDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	/// WM_INITDIALOG handler of wait dialog.
	static BOOL WriteProgressDlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	/// WM_NCHITTEST handler of wait dialog.
	static UINT WriteProgressDlg_OnNCHitTest(HWND hwnd, int x, int y);
	/// WM_COMMAND handler of wait dialog.
	static void WriteProgressDlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	/// WM_DESTROY handler of wait dialog.
	static void WriteProgressDlg_OnDestroy(HWND hwnd);

	/// Flag indicating window de-initialization.
	static BOOL m_bDone;
	/// Progress control.
	static CAnimProgressBar m_pbAnim;
	/// Thread handle where wait dialog is displayed.
	HANDLE m_hThread;
	/// Wait dialog handle.
	HWND m_hwndWait;
	/// Parent window handle.
	HWND m_hwndParent;
};

inline CWriteProgressDialog::CWriteProgressDialog(void)
{
	InitVars();
}

inline CWriteProgressDialog::~CWriteProgressDialog(void)
{
	EndWait();
}

/** @} */
