#include <simulation/Simulation.hpp>

simulation::SpatialHashGrid::SpatialHashGrid(float cellSize)
	: cellSize_(cellSize){}

void simulation::SpatialHashGrid::insert(Entity* entity, CellCoord newCellCoord) {
	grid_[newCellCoord].push_back(entity);
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
		insert(entity, newCell);
		entity->cell = newCell;
	}
}

std::vector<simulation::Entity*> simulation::SpatialHashGrid::queryNearby(Vec2 pos) {
	CellCoord center = getCell(pos);
	std::vector<Entity*> result;
	for (auto dx = -1; dx < 1; ++dx) {
		for (auto dy = -1; dy < 1; ++dy) {
			CellCoord cell{center.x + dx, center.y + dy};

			auto it = grid_.find(cell);

			if (it == grid_.end())
				continue;

			for (Entity* entity : it->second) {
				result.push_back(entity);
			}
		}
	}
	candidates_ = result;
}

simulation::CellCoord simulation::SpatialHashGrid::getCell(Vec2 pos) const {
	return {
		static_cast<int>(std::floor(pos.x / cellSize_)),
		static_cast<int>(std::floor(pos.y / cellSize_))
	};
}


bool simulation::CollisionSystem::isColliding(Entity* entity, Entity* other) {
	if (entity->position.x == other->position.x && entity->position.y == other->position.y)
		return true;
	return false;
}

void simulation::CollisionSystem::resolveCollision(Entity* entity, Entity* other) {
	auto newPos = entity->position += other->position;
}

void simulation::CollisionSystem::narrowPhase(std::vector<Entity*> candidates, Entity* entity) {
	for (Entity* other : candidates) {
		if (isColliding(entity, other)) {
			resolveCollision(entity, other);
		}
	}
	return;
}

simulation::Domain::Domain(simulation::SpatialHashGrid& grid) : spatialGrid_(grid){}

void simulation::Domain::updateCells() {

}