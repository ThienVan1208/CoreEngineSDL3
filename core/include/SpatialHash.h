#pragma once

#ifndef SPATIALHASH_H
#define SPATIALHASH_H

#include <unordered_map>
#include <vector>
#include <cmath>
#include "components/Collider.h"

class SpatialHash {
private:
    int cellSize;
    // Map of (cellIndex -> List of Colliders)
    std::unordered_map<long long, std::vector<Collider*>> grid;

    // Generates a 64-bit key from two 32-bit integers
    long long getHashKey(int x, int y) const {
        return ((long long)x << 32) | (unsigned int)y;
    }

public:
    SpatialHash(int size = 100) : cellSize(size) {}

    void Insert(Collider* col) {
        if (!col) return;
        
        Bound b = col->GetBound();
        
        // Quantize coordinates to grid space
        int minX = static_cast<int>(std::floor(b.left / cellSize));
        int maxX = static_cast<int>(std::floor(b.right / cellSize));
        int minY = static_cast<int>(std::floor(std::min(b.top, b.bottom) / cellSize));
        int maxY = static_cast<int>(std::floor(std::max(b.top, b.bottom) / cellSize));

        // Insert collider into all cells it overlaps
        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                grid[getHashKey(x, y)].push_back(col);
            }
        }
    }

    // Queries all colliders in the cells occupied by the given collider
    std::vector<Collider*> Query(Collider* col) {
        std::vector<Collider*> neighbors;
        if (!col) return neighbors;

        Bound b = col->GetBound();
        int minX = static_cast<int>(std::floor(b.left / cellSize));
        int maxX = static_cast<int>(std::floor(b.right / cellSize));
        int minY = static_cast<int>(std::floor(std::min(b.top, b.bottom) / cellSize));
        int maxY = static_cast<int>(std::floor(std::max(b.top, b.bottom) / cellSize));

        // Use a local set or check to avoid duplicates if needed, 
        // but for simple collision pairs, we'll handle duplicates in the CheckCollisions loop
        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                long long key = getHashKey(x, y);
                if (grid.count(key)) {
                    auto& cells = grid[key];
                    neighbors.insert(neighbors.end(), cells.begin(), cells.end());
                }
            }
        }
        return neighbors;
    }

    void Clear() {
        grid.clear();
    }
};

#endif // SPATIALHASH_H
