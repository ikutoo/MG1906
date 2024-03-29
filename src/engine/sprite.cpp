#include "sprite.h"
#include <DXLib/DxLib.h>
#include "common.h"
#include "resourceManager.h"

using namespace DxEngine;

CSprite::CSprite(const std::string& vImageFile, const recti& vRect)
{
	setImageFile(vImageFile, vRect);
}

CSprite::~CSprite()
{
	if (!m_ImageFile.empty())
	{
		CResourceManager::getInstance()->deleteImage(m_ImageFile);
	}
}

//*********************************************************************
//FUNCTION:
void CSprite::drawV()
{
	if (!_IsVisible) return;

	if (m_RenderGraph != -1)
	{
		CHECK_RESULT(DxLib::SetDrawScreen(m_RenderGraph));
		if (m_ClearRenderTargetOnDraw) CHECK_RESULT(DxLib::ClearDrawScreen());
	}

	auto SelfDraw = [&]
	{
		if (m_ImageHandle != -1)
		{
			auto Position = getWorldPosition();
			auto Scale = getWorldScale();
			auto Rotation = getWorldRotation();

			if (m_Rect.w != 0)
			{
				CHECK_RESULT(DxLib::DrawRectRotaGraphFast3(Position.x, Position.y, m_Rect.x, m_Rect.y, m_Rect.w, m_Rect.h, m_Anchor.x, m_Anchor.y, Scale.x, Scale.y, Rotation + m_ImageRotation, m_ImageHandle, TRUE, m_IsFliped));
			}
			else
			{
				CHECK_RESULT(DxLib::DrawRotaGraphFast3(Position.x, Position.y, m_Anchor.x, m_Anchor.y, Scale.x, Scale.y, Rotation + m_ImageRotation, m_ImageHandle, TRUE, m_IsFliped));
			}
		}
	};

	if (m_PriorToChilds) { SelfDraw(); CNode::drawV(); }
	else { CNode::drawV(); SelfDraw(); }

	if (m_RenderGraph != -1)
	{
		CHECK_RESULT(DxLib::SetDrawScreen(DX_SCREEN_BACK));
	}
}

//*********************************************************************
//FUNCTION:
void CSprite::setImageFile(const std::string& vImageFile, const recti& vRect)
{
	setRect(vRect);

	if (vImageFile == m_ImageFile) return;

	CResourceManager::getInstance()->deleteImage(m_ImageFile);

	if (!vImageFile.empty())
	{
		m_ImageHandle = CResourceManager::getInstance()->loadImage(vImageFile);
		_ASSERTE(m_ImageHandle != -1);

		if (vRect.w != 0)
			m_Size = { vRect.w, vRect.h };
		else
			CHECK_RESULT(DxLib::GetGraphSize(m_ImageHandle, &m_Size.x, &m_Size.y));
	}
	else
	{
		m_ImageHandle = -1;
	}

	m_Rect = vRect;
	m_ImageFile = vImageFile;
}