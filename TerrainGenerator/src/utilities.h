#ifndef UTILITIES_H
#define UTILITIES_H




inline float normalize_coordinate(float a, float max, float min){
	return (1) / (max - min)*(a - max) + 1;
}
#endif