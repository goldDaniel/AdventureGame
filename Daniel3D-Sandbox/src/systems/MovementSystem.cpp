#include "systems/MovementSystem.h"

#include "Components.h"

using namespace dg3d;
using namespace dg3d::game;

enum class CollisionSide
{
	INVALID,

	Left,
	Right,
	Top,
	Bottom
};

static CollisionSide getCollisionSide(const glm::vec4& r0, const glm::vec4& r1);
static void DoTilemapCollisions(entt::registry& registry, bool horizontal);
static bool CheckForTilemapCollisions(const entt::entity& entity, PositionComponent& entityPos, VelocityComponent& vel, const TilemapColliderComponent& collider, entt::registry& registry, bool horizontal);
static bool ResolveCollision(const entt::entity& entity, PositionComponent& entityPos, VelocityComponent& vel, const TilemapColliderComponent& collider, const glm::vec2& tileMin, const glm::vec2& tileMax, entt::registry& registry, bool horizontal);


MovementSystem::MovementSystem(entt::registry& registry)
	: core::GameSystem(registry)
{
}

void MovementSystem::Update(float dt)
{
	mRegistry.view<PositionComponent, const VelocityComponent>().each([dt](auto& pos, const auto& vel)
		{
			pos.pos.x += vel.velocity.x * dt;
		});
	DoTilemapCollisions(mRegistry, true);

	mRegistry.view<PositionComponent, const VelocityComponent>().each([dt](auto& pos, const auto& vel)
		{
			pos.pos.y += vel.velocity.y * dt;

		});
	DoTilemapCollisions(mRegistry, false);
}


void DoTilemapCollisions(entt::registry& registry, bool horizontal)
{
	registry.view<PositionComponent, VelocityComponent, const TilemapColliderComponent>().each([&registry, horizontal](auto entity, auto& pos, auto& vel, const auto& collider)
	{
		if (!CheckForTilemapCollisions(entity, pos, vel, collider, registry, horizontal))
		{
			if (!horizontal)
			{
				if (auto jump = registry.try_get<JumpComponent>(entity))
				{
					jump->canJump = false;
				}
			}
		}
	});
}

bool CheckForTilemapCollisions(const entt::entity& entity, PositionComponent& entityPos, VelocityComponent& vel, const TilemapColliderComponent& collider, entt::registry& registry, bool horizontal)
{
	//gather tiles in tilemap
	std::vector<entt::entity> collisionTiles;
	registry.view<TilemapComponent>().each([&entityPos, &collisionTiles](auto& tilemap)
	{
		glm::ivec2 keyBase{ glm::round(entityPos.pos.x), glm::round(entityPos.pos.y) };
		for (int y = -1; y <= 1; ++y)
		{
			for (int x = -1; x <= 1; ++x)
			{
				glm::ivec2 key = keyBase + glm::ivec2(x, y);
				if (tilemap.tileEntities.find(key) != tilemap.tileEntities.end())
				{
					collisionTiles.push_back(tilemap.tileEntities[key]);
				}
			}
		}	
	});

	//resolve
	for (auto tile : collisionTiles)
	{
		auto& position = registry.get<PositionComponent>(tile);
		auto& tileComp = registry.get<TilemapTileComponent>(tile);
		if (tileComp.isSolid)
		{
			glm::vec2 tileMin = { position.pos.x - 0.5f, position.pos.y - 0.5f };
			glm::vec2 tileMax = { position.pos.x + 0.5f, position.pos.y + 0.5f };

			glm::vec2 playerMin = { entityPos.pos.x - 0.5f, entityPos.pos.y - 0.5f };
			glm::vec2 playerMax = { entityPos.pos.x + 0.5f, entityPos.pos.y + 0.5f };

			if ((playerMin.x >= tileMin.x && playerMin.x <= tileMax.x ||
				 playerMax.x >= tileMin.x && playerMax.x <= tileMax.x)
				&&
				(playerMin.y >= tileMin.y && playerMin.y <= tileMax.y ||
				 playerMax.y >= tileMin.y && playerMax.y <= tileMax.y))
			{
				if (ResolveCollision(entity, entityPos, vel, collider, tileMin, tileMax, registry, horizontal))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool ResolveCollision(const entt::entity& entity, PositionComponent& entityPos, VelocityComponent& vel, const TilemapColliderComponent& collider, const glm::vec2& tileMin, const glm::vec2& tileMax, entt::registry& registry, bool horizontal)
{
	glm::vec2 diff = tileMax - tileMin;
	glm::vec4 tileRect = { tileMin + diff / 2.0f, diff };
	glm::vec4 playerRect = { entityPos.pos, collider.width, collider.height };

	CollisionSide side = getCollisionSide(tileRect, playerRect);

	if (horizontal)
	{
		if (side == CollisionSide::Right)
		{
			entityPos.pos.x = tileMin.x - collider.width / 2;
			if (registry.any_of<SideToSideAIComponent>(entity))
			{
				vel.velocity.x = -glm::abs(vel.velocity.x);
			}

			return true;
		}
		if (side == CollisionSide::Left)
		{
			entityPos.pos.x = tileMax.x + collider.width / 2;
			if (registry.any_of<SideToSideAIComponent>(entity))
			{
				vel.velocity.x = glm::abs(vel.velocity.x);
			}

			return true;
		}
	}
	else
	{
		if (side == CollisionSide::Top)
		{
			entityPos.pos.y = tileMin.y - collider.height / 2;
			vel.velocity.y = 0;
			return true;
		}
		if (side == CollisionSide::Bottom)
		{
			entityPos.pos.y = tileMax.y + collider.height / 2;
			vel.velocity.y = 0;
			if (auto jump = registry.try_get<JumpComponent>(entity))
			{
				jump->canJump = true;
			}
			return true;
		}
	}

	return false;
}

CollisionSide getCollisionSide(const glm::vec4& r0, const glm::vec4& r1)
{
	CollisionSide result = CollisionSide::INVALID;
	//horizontal side
	bool left = r0.x + r0.z / 2 < r1.x + r1.z / 2;
	//vertical side
	bool above = r0.y + r0.w / 2 > r1.y + r1.w / 2;

	//holds how deep the r1ect is inside the tile on each axis
	float horizontalDif = 0;
	float verticalDif = 0;

	//determine the differences for depth
	if (left)
	{
		horizontalDif = r0.x + r0.z - r1.x;
	}
	else
	{
		horizontalDif = r1.x + r1.z - r0.x;
	}

	if (above)
	{
		verticalDif = r1.y + r1.w - r0.y;
	}
	else
	{
		verticalDif = r0.y + r0.w - r1.y;
	}

	if (horizontalDif < verticalDif)
	{
		if (left)
		{
			result = CollisionSide::Left;
		}
		else
		{
			result = CollisionSide::Right;
		}
	}
	else if (above)
	{
		result = CollisionSide::Top;
	}
	else
	{
		result = CollisionSide::Bottom;
	}

	return result;
}