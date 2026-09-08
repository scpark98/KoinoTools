
// KoinoTools.h: PROJECT_NAME 애플리케이션에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.
#include "Common/system/CSCSelfPatch/SCSelfPatch.h"


// CKoinoToolsApp:
// 이 클래스의 구현에 대해서는 KoinoTools.cpp을(를) 참조하세요.
//

class CKoinoToolsApp : public CWinApp
{
public:
	CKoinoToolsApp();

	//20260907 by claude. 자체 패치 — 설치 폴더로 옮겨 심기 + 시작 시 버전 검사.
	//원리와 서버 구성은 Common/system/CSCSelfPatch/SCSelfPatch.h 주석 참조.
	CSCSelfPatch	m_self_patch;

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CKoinoToolsApp theApp;
