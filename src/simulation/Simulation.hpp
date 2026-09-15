#pragma once
#include <vector>
#include <unordered_map>

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
        Esphere
    };

    struct Entity {
        Vec2 position;
        Vec2 size;
        Vec2 velocity = {0, 1}; 
        CellCoord cell;
        EntityType type = EntityType::Esphere;
    };

    class SpatialHashGrid {
    public:
        explicit SpatialHashGrid(float cellSize)
            : cellSize_(cellSize) {
        }

        void clear() {
            grid_.clear();
        }

        void insert(Entity* entity, CellCoord newCellCoord);

        void remove(Entity* entity);

        void update(Entity* entity);

        std::vector<Entity*> queryNearby(Vec2 pos);

        std::unordered_map<CellCoord, std::vector<Entity*>, CellHash>& grid() {
            return grid_;
        }

        const std::unordered_map<CellCoord, std::vector<Entity*>, CellHash>& grid() const {
            return grid_;
        }

        std::vector<Entity*>& candidates() {
            return candidates_;
        }

        const std::vector<Entity*>& candidates() const {
            return candidates_;
        }

    private:
        CellCoord getCell(Vec2 pos) const;

        float cellSize_;

        std::vector<Entity*> candidates_{};

        std::unordered_map<CellCoord, std::vector<Entity*>, CellHash> grid_;   
    };

    struct CollisionSystem {
        bool isColliding(Entity* other, Entity* entity);
        void resolveCollision(Entity* entity, Entity* other);
        void narrowPhase(std::vector<Entity*> candidates, Entity* entity);
    };

    class Domain {
    public:
        Domain(SpatialHashGrid& grid) : spatialGrid_(grid) {}

        void updateCells();
    private:
        const float deltaTime_ = 0.0016f;
        SpatialHashGrid& spatialGrid_;
    };
}
