#include "../include/headers/noise.h"


float randomFloat() {
    float r =  static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
//    std::cout << r << std::endl;
    return r;
}

// Helper function to calculate the Euclidean distance between two points
float euclideanDistance(float x1, float y1, float x2, float y2) {
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float manhattanDistance(float x1, float y1, float x2, float y2) {
    return std::abs(x2 - x1) + std::abs(y2 - y1);
}

// Worley noise function
float worley_noise(float x, float y) {
    float minDistance = std::numeric_limits<float>::max();
    // Store the two smallest distances for more variation
    float minDist1 = std::numeric_limits<float>::max();
    float minDist2 = std::numeric_limits<float>::max();

    for (float offsetX = -0.05; offsetX <= 0.05; offsetX += 0.02) {
        for (float offsetY = -0.05; offsetY <= 0.05; offsetY += 0.02) {
            float neighborX = x + offsetX;
            float neighborY = y + offsetY;
            
            float randomPointX = neighborX + std::clamp(randomFloat(),-0.2f, 0.2f);
            float randomPointY = neighborY + std::clamp(randomFloat(),-0.2f, 0.2f);

            float distance = manhattanDistance(x, y, randomPointX, randomPointY);
            if (distance < minDist1) {
                minDist2 = minDist1;
                minDist1 = distance;
            } else if (distance < minDist2) {
                minDist2 = distance;
            }

            minDistance = std::min(minDistance, distance);
        }
    }
    return minDistance * 5;
    // Combine the distances for smoother and more varied output
    float noiseValue = minDist1 + 0.5f * minDist2;

    // Normalize to [0, 1] range
//     return noiseValue;
    return std::clamp(noiseValue, 0.0f, 5.0f);
}
