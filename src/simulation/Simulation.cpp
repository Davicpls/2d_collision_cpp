#include <simulation/Simulation.hpp>
#include <ctime>
#include <cmath>

simulation::SpatialHashGrid::SpatialHashGrid(float cellSize)
	: cellSize_(cellSize){}

void simulation::SpatialHashGrid::insert(Entity* entity) {
	auto cell = getCell(entity->position);
	grid_[cell].push_back(entity);
	entity->cell = cell;
}

void simulation::SpatialHashGrid::remove(Entity* entity) {
	auto cellIt = grid_.find(entity->cell);

	if (cellIt == grid_.end())
		return;

	auto& entities = cellIt->second;

	std::erase(entities, entity);

	if (entities.empty()) {
		grid_.erase(cellIt);
	}
}

void simulation::SpatialHashGrid::update(Entity* entity) {
	CellCoord newCell = getCell(entity->position);
	if (newCell != entity->cell) {
		remove(entity);
		insert(entity);
		entity->cell = newCell;
	}
}

std::vector<simulation::Entity*> simulation::SpatialHashGrid::queryNearby(Vec2 pos) {
	CellCoord center = getCell(pos);
	result_.clear();
	for (auto dx = -1; dx <= 1; ++dx) {
		for (auto dy = -1; dy <= 1; ++dy) {
			CellCoord cell{center.x + dx, center.y + dy};

			auto it = grid_.find(cell);

			if (it != grid_.end())
				result_.insert(result_.end(), it->second.begin(), it->second.end());
		}
	}
	return result_;
}

simulation::CellCoord simulation::SpatialHashGrid::getCell(Vec2 pos) const {
	return {
		static_cast<int>(std::floor(pos.x / cellSize_)),
		static_cast<int>(std::floor(pos.y / cellSize_))
	};
}

bool simulation::CollisionSystem::isColliding(Entity* entity, Entity* other) {
	if (entity == other)
		return false;

	const float dx = entity->position.x - other->position.x;
	const float dy = entity->position.y - other->position.y;

	const float radiusSum = entity->radius + other->radius;

	return dx * dx + dy * dy <= radiusSum * radiusSum;
}

void simulation::CollisionSystem::resolveCollision(Entity* entity, Entity* other) {

	Vec2 delta{
		other->position.x - entity->position.x,
		other->position.y - entity->position.y
	};

	float distance = std::sqrt(
		delta.x * delta.x +
		delta.y * delta.y
	);

	Vec2 normal{
		delta.x / distance,
		delta.y / distance
	};

	float penetration =
		entity->radius + other->radius - distance;

	Vec2 relativeVelocity{
		other->velocity.x - entity->velocity.x,
		other->velocity.y - entity->velocity.y
	};

	float velocityAlongNormal =
		relativeVelocity.x * normal.x +
		relativeVelocity.y * normal.y;

	if (velocityAlongNormal > 0.0f)
		return;

	entity->position.x -= normal.x * penetration * 0.5f;
	entity->position.y -= normal.y * penetration * 0.5f;

	other->position.x += normal.x * penetration * 0.5f;
	other->position.y += normal.y * penetration * 0.5f;

	entity->velocity.x *= -1.0f;
	entity->velocity.y *= -1.0f;

	other->velocity.x *= -1.0f;
	other->velocity.y *= -1.0f;
}

void simulation::CollisionSystem::narrowPhase(std::vector<Entity*>& candidates, Entity* entity) {
	for (Entity* other : candidates) {

		if (isColliding(entity, other)) {
			resolveCollision(entity, other);
		}
	}
	return;
}

void simulation::CollisionSystem::limitsCollision(Entity* entity, int limitW, int limitH) {

	if (entity->position.x - entity->radius <= 0 && entity->position.x + entity->radius >= limitW) {
		entity->velocity.x *= -1;
		entity->velocity.x *= -1;
	}
	else if (entity->position.x - entity->radius <= 0) {
		entity->velocity.x *= -1;
	} 
	else if (entity->position.x + entity->radius >= limitW) {
		entity->velocity.x *= -1;
	}
	if (entity->position.y - entity->radius <= 0 && entity->position.y + entity->radius >= limitW) {
		entity->velocity.y *= -1;
		entity->velocity.y *= -1;
	}
	else if (entity->position.y - entity->radius < 0) {
		entity->velocity.y *= -1;
	}
	else if (entity->position.y + entity->radius >= limitH) {
		entity->velocity.y *= -1;
	}
	return;
}

simulation::Domain::Domain(simulation::SpatialHashGrid& grid) : spatialGrid_(grid){}

simulation::Entity::Entity(Vec2 position, float radius, Vec2 velocity, CellCoord cell) 
	: position(position), radius(radius), velocity(velocity), cell(cell){}

void simulation::Domain::populateGrid(int limitW, int limitH) {

	for (int balls = 1; balls < 151; balls += 3) {
		std::srand(balls);
		auto entity = std::make_unique<Entity>(
			Vec2 { static_cast<float>(balls) * 0.5f, static_cast<float>(balls) * 0.5f },
			2.0f, 
			Vec2 { 1.0f, 20.0f },
			CellCoord { balls, balls });
		Entity* ptr = entity.get();
		entities_.emplace_back(std::move(entity));
		spatialGrid_.insert(ptr);
	}

	return;
}

void simulation::Domain::updateCells(int deltaTime, int limitW, int limitH) {
	for (const auto& entity : entities_) {
		entity->position += entity->velocity * (static_cast<float>(deltaTime) / 1000);

		simulation::CollisionSystem::limitsCollision(entity.get(), limitW, limitH);

		spatialGrid_.update(entity.get());

		auto candidates = spatialGrid_.queryNearby(entity->position);

		for (Entity* candidate : candidates) {
			if (entity.get() <= candidate)
				continue;
			simulation::CollisionSystem::narrowPhase(candidates, entity.get());
		}
	}

	return;
}