#pragma once
class CTaskBarProgress
{
	ITaskbarList3Ptr m_pTaskBarlist;
	HWND m_hWnd;
public:
	CTaskBarProgress() {}
	virtual ~CTaskBarProgress();
	void Init(HWND h);
	void SetProgress(int percent);
};

