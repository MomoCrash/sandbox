#pragma once

#include <SFML/Graphics.hpp>

namespace US {

	class RenderState : public sf::RectangleShape {
		
		sf::Vector2u mViewPortSize;
		sf::Vector2f mPosition;
		float mZoom;

		void UpdateShape();

	public:
		RenderState(sf::Vector2u viewportSize, sf::Texture* mpTexture);
		~RenderState() = default;

		void SetCameraPosition(float x, float y);
		void SetCameraPosition(sf::Vector2f const& pos);
		void MoveCamera(float dx, float dy);
		sf::Vector2f const& GetCameraPosition() const;

		void SetCameraZoom(float zoom);
		void IncrementZoom(float dz);
		float GetCameraZoom() const;

		sf::Vector2u WorldSpaceToCellSpace(const sf::Vector2i& pos);
	};

}