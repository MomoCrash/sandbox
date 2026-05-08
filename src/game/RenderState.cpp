#include "RenderState.h"
#pragma once 

namespace US
{
	void RenderState::UpdateShape()
	{
		// Set Origine to the middle of the rectangle
		//setOrigin({ 0.5f * mZoom * mViewPortSize.x, 0.5f * mZoom * mViewPortSize.y });
		setOrigin({ 0, 0 });
		setSize({ static_cast<float>(mViewPortSize.x) * mZoom, static_cast<float>(mViewPortSize.y) * mZoom });
		setPosition({ (1 - mZoom) * mViewPortSize.x * 0.5f - mPosition.x, (1 - mZoom) * mViewPortSize.y * 0.5f - mPosition.y });
	}

	US::RenderState::RenderState(sf::Vector2u viewportSize, sf::Texture* mpTexture)
		: mViewPortSize(viewportSize), mPosition(0, 0), mZoom(1) {
		
		SetCameraZoom(1);
		SetCameraPosition(0, 0);

		setTexture(mpTexture);
		setFillColor(sf::Color::White);
	}

	void RenderState::SetCameraPosition(float x, float y)
	{
		mPosition.x = x;
		mPosition.y = y;
		UpdateShape();
	}

	void RenderState::SetCameraPosition(sf::Vector2f const& pos)
	{
		SetCameraPosition(pos.x, pos.y);
	}

	void RenderState::MoveCamera(float dx, float dy)
	{
		SetCameraPosition(mPosition.x + dx, mPosition.y + dy);
	}

	void RenderState::SetCameraZoom(float zoom)
	{
		mZoom = zoom;
		if (mZoom < 1)
			mZoom = 1;
		UpdateShape();
	}

	void RenderState::IncrementZoom(float dz)
	{
		SetCameraZoom(mZoom + dz);
	}

	sf::Vector2f const& RenderState::GetCameraPosition() const
	{
		return mPosition;
	}

	float RenderState::GetCameraZoom() const
	{
		return mZoom;
	}

	sf::Vector2u RenderState::WorldSpaceToCellSpace(const sf::Vector2i& pos)
	{
		return sf::Vector2u(
			pos.x / mZoom + mPosition.x,
			pos.y / mZoom + mPosition.y
		);
	}

}
