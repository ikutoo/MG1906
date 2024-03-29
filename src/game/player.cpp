#include "stdafx.h"
#include "player.h"
#include <DXLib/DxLib.h>
#include "engine/jsonUtil.h"
#include "engine/resourceManager.h"
#include "common.h"
#include "barragePattern.h"
#include "barrageManager.h"

using namespace DxEngine;

const float INITIAL_POS_X = GRAPH_SIZE_X * 0.5;
const float INITIAL_POS_Y = 960;
const int	DEATH_PROTECTION_MAX_COUNT = 240;

//*********************************************************************
//FUNCTION:
CPlayer::CPlayer(const std::string& vConfigFile)
{
	__init(vConfigFile);
}

//*********************************************************************
//FUNCTION:
CPlayer::~CPlayer()
{
	__destroy();
}

//*********************************************************************
//FUNCTION:
void CPlayer::updateV(double vDeltaTime)
{
	CSprite::updateV(vDeltaTime);
	if (_IsPaused) return;

	__updatePlayerState();
	__updatePlayerPosition();
	__updateAnimation();
	__updateBarrage();
	__updateDeathProtection();

	if (m_State & PLAYER_STATE_GRAZE) { CHECK_RESULT(DxLib::PlaySoundMem(m_SoundHandleGraze, DX_PLAYTYPE_LOOP)); }
	else { CHECK_RESULT(DxLib::StopSoundMem(m_SoundHandleGraze)); }
}

//*********************************************************************
//FUNCTION:
void CPlayer::freeze(int vTime)
{
	//TODO
}

//*********************************************************************
//FUNCTION:
void CPlayer::dead()
{
	if (m_DeathProtectionCounter > 0) { return; }
	m_DeathProtectionCounter = DEATH_PROTECTION_MAX_COUNT;

	this->setPosition(INITIAL_POS_X, INITIAL_POS_Y);

	CHECK_RESULT(DxLib::PlaySoundMem(m_SoundHandleDead, DX_PLAYTYPE_BACK));
}

//*********************************************************************
//FUNCTION:
void CPlayer::graze(bool IsGrazed)
{
	m_IsGrazed = IsGrazed;

	if (m_DeathProtectionCounter > 0) { return; }

	m_GrazeScore++;
}

//*********************************************************************
//FUNCTION:
void CPlayer::__init(const std::string& vConfigFile)
{
	_ASSERTE(!vConfigFile.empty());

	this->setPosition(INITIAL_POS_X, INITIAL_POS_Y);

	CJsonReader JsonReader(vConfigFile);
	SPEED_HIGH = JsonReader.readFloat("speed_high");
	SPEED_LOW = JsonReader.readFloat("speed_low");
	_ASSERTE(SPEED_HIGH > 0 && SPEED_LOW > 0);

	m_pPlayer = new CSprite;
	this->addChild(m_pPlayer, -1);

	m_pPlayerBg1 = JsonReader.readSprite("effect_sprite_1");
	m_pPlayerBg1->setAnchor(m_pPlayerBg1->getSize() / 2);
	this->addChild(m_pPlayerBg1, 0);
	m_pPlayerBg2 = JsonReader.readSprite("effect_sprite_2");
	m_pPlayerBg2->setAnchor(m_pPlayerBg2->getSize() / 2);
	this->addChild(m_pPlayerBg2, 0);

	m_PlayerAnm1 = JsonReader.readAnimation("player_anm_1");
	m_PlayerAnm2 = JsonReader.readAnimation("player_anm_2");

	m_SoundHandleGraze = DxLib::LoadSoundMem(LOCATE_FILE("se_graze.wav"));
	ChangeVolumeSoundMem(255 * 6 / 10, m_SoundHandleGraze);
	m_SoundHandleDead = DxLib::LoadSoundMem(LOCATE_FILE("se_pldead_00.wav"));

	m_DeathProtectionCounter = DEATH_PROTECTION_MAX_COUNT;
}

//*********************************************************************
//FUNCTION:
void CPlayer::__destroy()
{
	for (auto pBarrage : m_Barrages) SAFE_DELETE(pBarrage);
	CHECK_RESULT(DxLib::DeleteSoundMem(m_SoundHandleGraze));
	CHECK_RESULT(DxLib::DeleteSoundMem(m_SoundHandleDead));
}

//*********************************************************************
//FUNCTION:
void CPlayer::__updatePlayerState()
{
	m_State = PLAYER_STATE_IDLE;

	if (isPressed(GAME_INPUT_UP)) m_State |= PLAYER_STATE_MOVE_UP;
	else if (isPressed(GAME_INPUT_DOWN)) m_State |= PLAYER_STATE_MOVE_DOWN;

	if (isPressed(GAME_INPUT_LEFT)) m_State |= PLAYER_STATE_MOVE_LEFT;
	else if (isPressed(GAME_INPUT_RIGHT)) m_State |= PLAYER_STATE_MOVE_RIGHT;

	if (isPressed(GAME_INPUT_LSHIFT)) m_State |= PLAYER_STATE_LOW_SPEED;

	if (isPressed(GAME_INPUT_Z)) m_State |= PLAYER_STATE_SHOOTING;

	if (m_IsGrazed) m_State |= PLAYER_STATE_GRAZE;
}

//*********************************************************************
//FUNCTION:
void CPlayer::__updatePlayerPosition()
{
	m_Speed = {};

	if (m_State & PLAYER_STATE_MOVE_UP) m_Speed.y = -1;
	if (m_State & PLAYER_STATE_MOVE_DOWN) m_Speed.y = 1;
	if (m_State & PLAYER_STATE_MOVE_LEFT) m_Speed.x = -1;
	if (m_State & PLAYER_STATE_MOVE_RIGHT) m_Speed.x = 1;

	m_Speed.normalize();
	m_Speed = m_State & PLAYER_STATE_LOW_SPEED ? m_Speed * SPEED_LOW : m_Speed * SPEED_HIGH;

	_Position += m_Speed;

	int Offset = 20;
	_Position.x = clip<float>(_Position.x, BORDER_L + Offset, BORDER_R - Offset);
	_Position.y = clip<float>(_Position.y, BORDER_U + Offset, BORDER_D - Offset);
}

//*********************************************************************
//FUNCTION:
void CPlayer::__updateAnimation()
{
	if ((m_State & PLAYER_STATE_MOVE_LEFT) || (m_State & PLAYER_STATE_MOVE_RIGHT))
	{
		m_pPlayer->setImageFile(m_PlayerAnm2.ImageFile, m_PlayerAnm2.forward());
		m_PlayerAnm1.reset();
	}
	else
	{
		m_pPlayer->setImageFile(m_PlayerAnm1.ImageFile, m_PlayerAnm1.forward());
		m_PlayerAnm2.reset();
	}

	m_pPlayer->setAnchor(m_pPlayer->getSize() / 2);
	m_pPlayer->setFlip(m_State & PLAYER_STATE_MOVE_RIGHT);

	m_pPlayerBg1->setRotation(m_pPlayerBg1->getRotation() + 0.02);
	m_pPlayerBg1->setVisible(m_State & PLAYER_STATE_LOW_SPEED);
	m_pPlayerBg2->setRotation(m_pPlayerBg2->getRotation() + 0.04);
}

//*********************************************************************
//FUNCTION:
void CPlayer::__updateBarrage()
{
	if (m_State & PLAYER_STATE_SHOOTING)
	{
		for (auto pBarrage : m_Barrages)
			CBarrageManager::getInstance()->startBarrage(pBarrage, true);
	}
	else
	{
		for (auto pBarrage : m_Barrages)
		{
			CBarrageManager::getInstance()->stopBarrage(pBarrage);
			pBarrage->resetCounter();
		}
	}
}

//*********************************************************************
//FUNCTION:
void CPlayer::__updateDeathProtection()
{
	m_DeathProtectionCounter--;
	if (m_DeathProtectionCounter > 1)
	{
		if (m_DeathProtectionCounter % 12 == 0) m_pPlayer->setBrightness(vec3i{ 255, 0, 0 });
		else if (m_DeathProtectionCounter % 12 == 6) m_pPlayer->setBrightness(vec3i{ 255, 255, 255 });
	}
	else if (m_DeathProtectionCounter == 1)
	{
		m_pPlayer->setBrightness(vec3i{ 255, 255, 255 });
	}
}