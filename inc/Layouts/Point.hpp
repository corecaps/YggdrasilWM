#ifndef POINT_HPP
#define POINT_HPP
/**
 * @struct Point
 * @brief Point struct
 * This struct represents a 2D point
 */
struct Point {
	unsigned int	x;
	unsigned int	y;

	Point(unsigned int x, unsigned int y) : x(x), y(y) {}
	Point() : x(0), y(0) {}
};
#endif // POINT_HPP