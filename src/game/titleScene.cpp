#include "stdafx.h"
#include "titleScene.h"
#include <thread>
#include "common.h"
#include "levelScene.h"
#include "helpScene.h"
#include "engine/utility.h"
#include "engine/inputManager.h"
#include "engine/engine.h"
#include "engine/resourceManager.h"

using namespace DxEngine;

namespace
{
	const vec3i MENU_COLOR_NORMAL = { 100, 100, 100 };
	const vec3i MENU_COLOR_SELECTED = { 255, 255, 255 };
}

//*********************************************************************
//FUNCTION:
bool CTitleScene::initV()
{
	if (!CScene::initV()) return false;

	CHECK_RESULT(DxLib::SetBackgroundColor(0, 0, 0));
	CHECK_RESULT(DxLib::ChangeFont("simkai"));

	auto pBgLabel = new CImageLabel("bg_00.png");
	pBgLabel->setPosition(1100, 100);
	pBgLabel->setSize(800, 1200);
	pBgLabel->setBrightness(vec3i{ 50, 50, 50 });
	this->addChild(pBgLabel);

	m_pParticles = new CParticle01(recti{ 0, 0, GRAPH_SIZE_X, GRAPH_SIZE_Y }, 2.0);
	this->addChild(m_pParticles);

	auto pTitleLabel = new CImageLabel("title.png");
	pTitleLabel->setPosition((GRAPH_SIZE_X - pTitleLabel->getSize().x) / 2, 100);
	this->addChild(pTitleLabel);

	auto pPlayLabel = new CTextLabel("开始游戏", 50, DX_FONTTYPE_ANTIALIASING_EDGE_4X4, 0xffffff, 0xffff00);
	pPlayLabel->setPosition(800, 600);
	m_MenuLabels.emplace_back(pPlayLabel);

	auto pHelpLabel = new CTextLabel("操作说明", 50, DX_FONTTYPE_ANTIALIASING_EDGE_4X4, 0xffffff, 0xffff00);
	pHelpLabel->setPosition(800, 700);
	m_MenuLabels.emplace_back(pHelpLabel);

	auto pExitLabel = new CTextLabel("退出游戏", 50, DX_FONTTYPE_ANTIALIASING_EDGE_4X4, 0xffffff, 0xffff00);
	pExitLabel->setPosition(800, 800);
	m_MenuLabels.emplace_back(pExitLabel);

	for (auto pLabel : m_MenuLabels) this->addChild(pLabel);

	m_pFlagLabel = new CImageLabel("flag.png");
	this->addChild(m_pFlagLabel);

	CHECK_RESULT(DxLib::PlayMusic(LOCATE_FILE("bgm_01.mp3"), DX_PLAYTYPE_BACK));

	return true;
}

//*********************************************************************
//FUNCTION:
void CTitleScene::updateV(double vDeltaTime)
{
	CScene::updateV(vDeltaTime);

	m_pParticles->update();

	m_pFlagLabel->setPosition(m_MenuLabels[m_SelectedLabelIndex]->getPosition().x - 100, m_MenuLabels[m_SelectedLabelIndex]->getPosition().y);

	bool IndexChanged = false;
	if (CHECK_HIT_KEY(KEY_INPUT_DOWN)) { m_SelectedLabelIndex++; IndexChanged = true; }
	else if (CHECK_HIT_KEY(KEY_INPUT_UP)) { m_SelectedLabelIndex--; IndexChanged = true; }

	if (IndexChanged) CHECK_RESULT(DxLib::PlaySoundFile(LOCATE_FILE("se_select_00.wav"), DX_PLAYTYPE_BACK));

	if (m_SelectedLabelIndex < 0) m_SelectedLabelIndex = m_MenuLabels.size() - 1;
	else if (m_SelectedLabelIndex >= m_MenuLabels.size()) m_SelectedLabelIndex = 0;

	for (int i = 0; i < m_MenuLabels.size(); ++i)
	{
		if (i == m_SelectedLabelIndex)
			m_MenuLabels[i]->setBrightness(MENU_COLOR_SELECTED);
		else
			m_MenuLabels[i]->setBrightness(MENU_COLOR_NORMAL);
	}

	if (CHECK_HIT_KEY(KEY_INPUT_Z))
	{
		switch (m_SelectedLabelIndex)
		{
		case 0:
			CEngine::getInstance()->setActiveScene(new CLevelScene);
			break;
		case 1:
			CEngine::getInstance()->setActiveScene(new CHelpScene);
			break;
		case 2:
			CEngine::getInstance()->stop();
			break;
		default:
			_ASSERT(false);
			break;
		}

		CEngine::getInstance()->pushScene(this);

		CHECK_RESULT(DxLib::PlaySoundFile(LOCATE_FILE("se_ok_00.wav"), DX_PLAYTYPE_BACK));
	}

	if (CHECK_HIT_KEY(KEY_INPUT_X))
	{
		m_SelectedLabelIndex = m_MenuLabels.size() - 1;
		CHECK_RESULT(DxLib::PlaySoundFile(LOCATE_FILE("se_cancel_00.wav"), DX_PLAYTYPE_BACK));
	}
}

//***********************************************************************************************
//FUNCTION:
void CTitleScene::destroyV()
{
	CHECK_RESULT(DxLib::StopMusic());

	CScene::destroyV();
}