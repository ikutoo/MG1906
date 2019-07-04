#include "engine.h"
#include <chrono>
#include <DXLib/DxLib.h>
#include "inputManager.h"
#include "scene.h"

using namespace DxEngine;

//*********************************************************************
//FUNCTION:
int CEngine::run()
{
	if (!__init()) return EXIT_FAILURE;

	while (0 == DxLib::ProcessMessage() && !m_IsMainLoopDone)
	{
		__update();
		__render();
	}

	__destroy();

	return EXIT_SUCCESS;
}

//***********************************************************************************************
//FUNCTION:
bool CEngine::setActiveScene(CScene* vScene)
{
	_ASSERT(vScene);

	if (m_pActiveScene)
	{
		m_pActiveScene->destroyV();
		SAFE_DELETE(m_pActiveScene);
	}

	m_pActiveScene = vScene;

	if (!m_IsInitialized) return true;
	if (!m_pActiveScene->initV()) return false;

	return true;
}

//*********************************************************************
//FUNCTION:
bool CEngine::__init()
{
	for (auto InitFunc : m_InitFuncs) InitFunc();

	__initWindowInfo();

	CHECK_RESULT(DxLib::DxLib_Init());
	CHECK_RESULT(DxLib::SetDrawScreen(DX_SCREEN_BACK));

	if (!m_pActiveScene->initV()) return false;

	m_IsInitialized = true;
	return m_IsInitialized;
}

//***********************************************************************************************
//FUNCTION:
void CEngine::__update()
{
	CInputManager::getInstance()->update();

	int DeltaTime = GetNowCount() - m_TimeCounter;
	m_FPS = 1000.0f / DeltaTime;
	m_TimeCounter = GetNowCount();

	updateStatus("FPS", m_FPS);

	m_pActiveScene->updateV(DeltaTime);
	for (auto UpdateFunc : m_UpdateFuncs) UpdateFunc();
}

//***********************************************************************************************
//FUNCTION:
void CEngine::__render()
{
	CHECK_RESULT(ClearDrawScreen());

	m_pActiveScene->drawV();
	if (m_DisplayStatus) __drawStatus();

	CHECK_RESULT(DxLib::ScreenFlip());
}

//*********************************************************************
//FUNCTION:
void CEngine::__destroy()
{
	m_pActiveScene->destroyV();
	SAFE_DELETE(m_pActiveScene);
	CHECK_RESULT(DxLib::DxLib_End());
}

//*********************************************************************
//FUNCTION:
void CEngine::__drawStatus()
{
	unsigned int Color = 0x00ff00;
	CHECK_RESULT(DxLib::SetFontSize(20));
	int y = 10;

	for (auto & iter : m_AppStatusMap)
	{
		char Buf[0xff];
		sprintf(Buf, " %s: %6.1f \n", iter.first.c_str(), iter.second);
		CHECK_RESULT(DxLib::DrawString(10, y, Buf, Color));
		y += 20;
	}
}

//***********************************************************************************************
//FUNCTION:
bool CEngine::__initWindowInfo()
{
	CHECK_RESULT(DxLib::SetWindowSize(m_DisplayInfo.WindowSize.x, m_DisplayInfo.WindowSize.y));
	CHECK_RESULT(DxLib::SetGraphMode(m_GraphSize.x, m_GraphSize.y, 32));

	m_DisplayInfo.ScreenSize = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	m_DisplayInfo.WindowPosition.x = (m_DisplayInfo.ScreenSize.x - m_DisplayInfo.WindowSize.x) / 2;
	m_DisplayInfo.WindowPosition.y = (m_DisplayInfo.ScreenSize.y - m_DisplayInfo.WindowSize.y) / 2;

	CHECK_RESULT(DxLib::ChangeWindowMode(!m_DisplayInfo.IsFullscreen));
	CHECK_RESULT(DxLib::SetWindowPosition(m_DisplayInfo.WindowPosition.x, m_DisplayInfo.WindowPosition.y));
	CHECK_RESULT(DxLib::SetWindowText(m_DisplayInfo.WindowTitle.c_str()));

	return true;
}