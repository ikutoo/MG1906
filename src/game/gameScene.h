#pragma once
#include <vector>
#include "engine/luaGlue.h"
#include "engine/scene.h"
#include "engine/sprite.h"
#include "engine/label.h"
#include "particles.h"

using namespace DxEngine;

enum class EGameState : char
{
	UNDEFINED = 0,
	NORMAL,
	PAUSED,
	IN_DIALOGUE
};

class CPlayer;
class CCollisionDetector;
class CBackground3d;

namespace DxEngine
{
	class CRenderTarget;
}

class CGameScene : public CScene
{
public:
	CGameScene(const char* vScriptFile);
	~CGameScene() = default;

	virtual bool _initV() override;
	virtual void _updateV(double vDeltaTime) override;
	virtual void _destroyV() override;

private:
	bool __initUI();
	void __updateBarrage();
	void __updateUI();

	void __detectCollision();

	void __initLuaEnv();
	void __registerLuaGlue();
	void __performLuaFunc(const char* vFuncName);
	void __performLuaSource(const char* vSource);
	void __closeLuaEvn();

	void __setDialogueScript(const char* vFilePath);

	LUAGLUE __setBackgroundImage(lua_State* vioLuaState);
	LUAGLUE __setLCharaterImage(lua_State* vioLuaState);
	LUAGLUE __setRCharaterImage(lua_State* vioLuaState);
	LUAGLUE __beginDialogue(lua_State* vioLuaState);
	LUAGLUE __endDialogue(lua_State* vioLuaState);
	LUAGLUE __setDialogue(lua_State* vioLuaState);
	LUAGLUE __setCharacterName(lua_State* vioLuaState);
	LUAGLUE __startBgAnimation(lua_State *vioLuaState);
	LUAGLUE __getCounter(lua_State *vioLuaState);
	LUAGLUE __displayStageInfo(lua_State *vioLuaState);
	LUAGLUE __createEnemy(lua_State* vioLuaState);

	CSprite* __findUISprite(const std::string& vName);
	CLabel*  __findUILabel(const std::string& vName);

	EGameState m_GameState = EGameState::NORMAL;

	std::string m_ScriptFile = {};

	lua_State* m_pLuaState = nullptr;
	std::string m_DialogueSource = {};
	std::vector<std::string> m_DialogueActions;
	unsigned m_ActionIndex = 0;

	CNode* m_pUIRootNode = nullptr;
	CPlayer* m_pPlayer = nullptr;

	CParticle01* m_pLParticles = nullptr;
	CParticle01* m_pRParticles = nullptr;

	int m_Counter = 0;

	CSprite* m_pPlayerBarrageContainer = nullptr;
	CSprite* m_pEnemyBarrageContainer = nullptr;
	CRenderTarget* m_pBarrageRenderTarget = nullptr;

	CCollisionDetector* m_pCollisionDetector = nullptr;
};