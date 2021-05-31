#pragma once
#include "../singleton.h"
#include "../common.h"
class math : public singleton<math>
{
public:
	 float normalizefloat(float angle);
	 void vectortransform(const Vector & in1, const matrix3x4_t & in2, Vector & out);

	void vector_angles(const Vector& forward, QAngle& angles);
	QAngle calcangle(Vector src, Vector dst);
	void sincos(float radians, float * sine, float * cosine);
	void angle_vectors(const QAngle & angles, Vector * forward, Vector * right, Vector * up);
	Vector normalizevector(Vector angle);

	void rotate_triangle(std::array<Vector2D, 3>& points, float rotation);
	template<class T, class U>
	T clamp(T in, U low, U high)
	{
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}
};