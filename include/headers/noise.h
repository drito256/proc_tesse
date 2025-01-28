#ifndef NOISE_H
#define NOISE_H

#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>

float random_float();
float euclidian_dist(float x1, float y1, float x2, float y2);

float worley_noise(float x, float y, int dist_function);


#endif
