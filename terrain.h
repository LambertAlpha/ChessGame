#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>

enum class TerrainType {
    Land,
    Forest,
    Mountain,
    River,
    Desert
};

class Terrain {
public:
    Terrain(int rows, int cols);
    TerrainType getTerrain(int x, int y) const;
    void setupTerrain(); // initialization
    int getRows() const;
    int getCols() const;

private:
    int rows;
    int cols;
    std::vector<std::vector<TerrainType>> boardTerrain;
};

#endif // TERRAIN_H
