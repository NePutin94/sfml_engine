#pragma once
#include "Entity.h"
#include "AnimationManager.h"
#include "Inventory.h"
#include <MetaStuff/Meta.h>
#include "JsonLoader.h"
#include "InventoryMenu.h"
#include "Ray.h"
using namespace meta;
namespace Engine
{
	class Actor : public Entity
	{
	protected:
		AnimationManager  animManager;
		sf::Clock         Pressclock;
		sf::Clock         actionClock;
		sf::Vector2f      originOffset;
		DirectionX        direction;
		DirectionY        directionY = DirectionY::Up;
		sf::RenderWindow* window;

		float scale = 0.5;
		float time_actor;

		void updateSprite();
	public:
		float Radian;
		sf::Vector2f ray;
		sf::Vector2f ray2;
		Ray left;
		Ray right;
		Ray middle;
		Ray govno;
		Actor(Level& lvl);
		Actor(sf::Vector2f position, std::string name, sf::RenderWindow& window, Level& lvl, std::string_view animation);
		~Actor() {}

		virtual void handleEvent(sf::Event& e) = 0;
		virtual void isKeyPressed() = 0;
		virtual void checkClashes(float time) = 0;
		virtual void update(float time) = 0;

		sf::Vector2f getOrigin() { return originOffset * sf::Vector2f(scale, scale); }
		float getScale() { return scale; }
		void CollisionUpdate(Entity* entity) override;

		friend class DebugWindow;
		friend auto meta::registerMembers<Engine::Actor>();
	};
}
namespace meta
{
	template <>
	inline auto registerMembers<Engine::Actor>()
	{
		return members(
			member("originOffset", &Engine::Actor::originOffset)
		);
	}
}