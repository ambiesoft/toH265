#include "stdafx.h"
#include "CTaskBarProgress.h"

void CTaskBarProgress::Init(HWND h)
{
	m_hWnd = h;
	CoCreateInstance(
		CLSID_TaskbarList, NULL, CLSCTX_ALL,
		IID_ITaskbarList3, (void**)&m_pTaskBarlist);
	if (m_pTaskBarlist)
		m_pTaskBarlist->SetProgressState(m_hWnd, TBPF_INDETERMINATE);
}
CTaskBarProgress::~CTaskBarProgress()
{
	m_pTaskBarlist = nullptr;
}

void CTaskBarProgress::SetProgress(int percent)
{
	if(m_pTaskBarlist)
		m_pTaskBarlist->SetProgressValue(m_hWnd, percent, 100);
}