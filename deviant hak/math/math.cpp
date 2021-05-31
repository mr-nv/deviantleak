#include "math.h"
 float math::normalizefloat(float angle) {
	auto revolutions = angle / 360.f;
	if (angle > 180.f || angle < -180.f) {
		revolutions = round(abs(revolutions));
		if (angle < 0.f)
			angle = (angle + 360.f * revolutions);
		else
			angle = (angle - 360.f * revolutions);
		return angle;
	}
	return angle;
}
 void math::vectortransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
 {
	 out[0] = DotProduct(in1, Vector(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
	 out[1] = DotProduct(in1, Vector(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
	 out[2] = DotProduct(in1, Vector(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
 }

QAngle math::calcangle(Vector src, Vector dst)
{
	QAngle angles;
	Vector delta = src - dst;
	vector_angles(delta, angles);
	delta.Normalized();
	return angles;
}
void math::sincos(float radians, float *sine, float *cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);

}
void math::vector_angles(const Vector& forward, QAngle& angles)
{
	if (forward.x == 0.f && forward.y == 0.f)
	{
		angles.pitch = forward.z > 0.f ? -90.f : 90.f;
		angles.yaw = 0.f;
	}
	else
	{
		angles.pitch = RAD2DEG(atan2(-forward.z, forward.Length2D()));
		angles.yaw = RAD2DEG(atan2(forward.y, forward.x));
	}

	angles.roll = 0.f;
}

void math::angle_vectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up) 
{
	float sp, sy, sr, cp, cy, cr;

	sincos(DEG2RAD(angles.pitch), &sp, &cp);
	sincos(DEG2RAD(angles.yaw), &sy, &cy);
	sincos(DEG2RAD(angles.roll), &sr, &cr);

	if (forward != nullptr)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right != nullptr)
	{
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if (up != nullptr)
	{
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}
Vector  math::normalizevector(Vector angle)
{
	while (angle.x > 89.f)
	{
		angle.x -= 180.f;
	}
	while (angle.x < -89.f)
	{
		angle.x += 180.f;
	}
	if (angle.y > 180)
	{
		angle.y -= (round(angle.y / 360) * 360.f);
	}
	else if (angle.y < -180)
	{
		angle.y += (round(angle.y / 360) * -360.f);
	}
	if ((angle.z > 50) || (angle.z < 50))
	{
		angle.z = 0;
	}
	return angle;
}
//

void math::rotate_triangle(std::array<Vector2D, 3>& points, float rotation)
{
	const auto points_center = (points.at(0) + points.at(1) + points.at(2)) / 3;
	for (auto& point : points)
	{
		point -= points_center;

		const auto temp_x = point.x;
		const auto temp_y = point.y;

		const auto theta = DEG2RAD(rotation);
		const auto c = cosf(theta);
		const auto s = sinf(theta);

		point.x = temp_x * c - temp_y * s;
		point.y = temp_x * s + temp_y * c;

		point += points_center;
	}
}
