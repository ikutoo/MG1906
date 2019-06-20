#include "stdafx.h"
#include "director.h"
#include "inputManager.h"
#include "scene.h"

CDirector::CDirector()
{
}

CDirector::~CDirector()
{
}

//***********************************************************************************************
//FUNCTION:
void CDirector::update()
{
	CInputManager::getInstance()->update();
	m_pActiveScene->updateV(0.0);
}

//***********************************************************************************************
//FUNCTION:
bool CDirector::setActiveScene(int vSceneID)
{
	if (m_SceneMap.find(vSceneID) == m_SceneMap.end()) return false;

	if (m_pActiveScene) m_pActiveScene->destroyV();
	m_pActiveScene = m_SceneMap[vSceneID];
	if (!m_pActiveScene->initV()) return false;

	return true;
}