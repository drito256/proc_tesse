#include "../include/headers/noise.h"



float euclideanDistance(float x1, float y1, float x2, float y2) {
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

float manhattanDistance(float x1, float y1, float x2, float y2) {
    return (std::abs(x2 - x1) + std::abs(y2 - y1));
}

float chebyshevDistance(float x1, float y1, float x2, float y2) {
    return fmax(fabs(x2 - x1), fabs(y2 - y1));
}
float minkowskiDistance4(float x1, float y1, float x2, float y2) {
    return pow(pow(fabs(x2 - x1), 4) + pow(fabs(y2 - y1), 4), 1/4.f);
}


// Simple hash function to generate a pseudo-random value
uint32_t hash(int x, int y) {
    uint32_t h = static_cast<uint32_t>(x * 73856093) ^ static_cast<uint32_t>(y * 19349663);
    h = (h >> 13) ^ h;
    return h;
}

// Generate a random float between 0 and 1 based on hashed cell coordinates
float randomFloat(int x, int y) {
    return static_cast<float>(hash(x, y) & 0xFFFFFF) / static_cast<float>(0xFFFFFF);
}

// Generate the feature point for a given cell
std::pair<float, float> getFeaturePoint(int cellX, int cellY) {
    float pointX = cellX + randomFloat(cellX, cellY);
    float pointY = cellY + randomFloat(cellX + 1, cellY + 1); // Slightly offset hash for variability
    return {pointX, pointY};
}

// Worley noise function
float worley_noise(float x, float y) {
    int cellX = static_cast<int>(std::floor(x));
    int cellY = static_cast<int>(std::floor(y));

    float minDistance = std::numeric_limits<float>::max();

    for (int offsetX = -1; offsetX <= 1; ++offsetX) {
        for (int offsetY = -1; offsetY <= 1; ++offsetY) {
            int neighborX = cellX + offsetX;
            int neighborY = cellY + offsetY;

            auto [featureX, featureY] = getFeaturePoint(neighborX, neighborY);

            float distance = euclideanDistance(x, y, featureX, featureY);
            minDistance = std::min(minDistance, distance);
        }
    }

    return minDistance;
}
