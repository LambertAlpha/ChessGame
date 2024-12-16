#include "terrain.h"


// construct
Terrain::Terrain(int rows, int cols)
    : rows(rows), cols(cols), boardTerrain(rows, std::vector<TerrainType>(cols, TerrainType::Land)) {}

void Terrain::setupTerrain() {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            boardTerrain[row][col] = TerrainType::Land;
        }
    }

    int riverRow = rows / 2;
    for (int col = 0; col < 3; ++col) {
        boardTerrain[riverRow][col] = TerrainType::River;
    }
    for (int col = 4; col < 7; ++col) {
        boardTerrain[riverRow][col] = TerrainType::River;
    }
    for (int col = 8; col < cols; ++col) {
        boardTerrain[riverRow][col] = TerrainType::River;
    }

    std::vector<std::pair<int, int>> mountainPositions = {{2, 1}, {2, 2}, {3, 5}, {3, 6}};
    for (auto [x, y] : mountainPositions) {
        if (x < riverRow) {
            boardTerrain[x][y] = TerrainType::Mountain;
            boardTerrain[rows - 1 - x][y] = TerrainType::Mountain;
        }
    }

    std::vector<std::pair<int, int>> desertPositions = {{1, 5}, {2, 6}, {3, 7}};
    for (auto [x, y] : desertPositions) {
        if (x < riverRow) {
            boardTerrain[x][y] = TerrainType::Desert;
            boardTerrain[rows - 1 - x][y] = TerrainType::Desert;
        }
    }


    std::vector<std::pair<int, int>> forestPositions = {{4, 8}, {4,1},{4,2},{3, 9}, {4,9},{1, 10}};
    for (auto [x, y] : forestPositions) {
        if (x < riverRow) {
            boardTerrain[x][y] = TerrainType::Forest;
            boardTerrain[rows - 1 - x][y] = TerrainType::Forest; // 对称位置
        }
    }
}

// get the terrain at x y
TerrainType Terrain::getTerrain(int x, int y) const {
    if (x >= 0 && x < rows && y >= 0 && y < cols) {
        return boardTerrain[x][y];
    }
    return TerrainType::Land; // 默认返回 Land
}

int Terrain::getRows() const {
    return rows;
}

int Terrain::getCols() const {
    return cols;
}

