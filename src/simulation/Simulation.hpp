#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>

namespace simulation {

    struct Vec2 {
        float x, y;

        Vec2& operator+=(const Vec2& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vec2 operator*(float scalar) const {
            return {
                x * scalar,
                y * scalar
            };
        }
    };

	struct CellCoord {
		int x;
		int y;

		bool operator==(const CellCoord& other) const {
			return x == other.x && y == other.y;
		}
	};
    

    struct CellHash {
        std::size_t operator()(const CellCoord& cell) const {
            std::size_t hx = std::hash<int>{}(cell.x);
            std::size_t hy = std::hash<int>{}(cell.y);

            return hx ^ (hy << 1);
        }
    };

    enum class EntityType {
        Circle
    };

    struct Entity {
        Vec2 position{ 0.f, 0.f };
        float radius{ 0.f };
        Vec2 velocity{ 0.f, 0.f };
        CellCoord cell{ 0, 0 };
        EntityType type = EntityType::Circle;

        Entity(Vec2 position, float radius, Vec2 velocity, CellCoord cell);
    };

    class SpatialHashGrid {
    public:
        explicit SpatialHashGrid(float cellSize);

        void insert(Entity* entity);

        void remove(Entity* entity);

        void update(Entity* entity);

        std::vector<Entity*> queryNearby(Vec2 pos);

    private:
        CellCoord getCell(Vec2 pos) const;

        float cellSize_;

        std::vector<Entity*> result_;

        std::unordered_map<CellCoord, std::vector<Entity*>, CellHash> grid_;   
    };

    struct CollisionSystem {
        static bool isColliding(Entity* other, Entity* entity);
        static void resolveCollision(Entity* entity, Entity* other);
        static void narrowPhase(std::vector<Entity*>& candidates, Entity* entity);
        static void limitsCollision(Entity* entity, int limitW, int limitH);
    };

    class Domain {
    public:
        Domain(SpatialHashGrid& grid);

        void populateGrid();
        void updateCells(int deltaTime, int limitW, int limitH);

        const std::vector<std::unique_ptr<Entity>>& entities() const {
            return entities_;
        }

        std::vector<std::unique_ptr<Entity>>& entities() {
            return entities_;
        }

    private:
        SpatialHashGrid& spatialGrid_;
        std::vector<std::unique_ptr<Entity>> entities_;
    };
}
