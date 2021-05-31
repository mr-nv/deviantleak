#pragma once
#include "../singleton.h"
#include "../common.h"
class movement : public singleton<movement>
{
public:

	void bunnyhop(c_usercmd * cmd);

	void strafe(c_usercmd * cmd);

	void duckinair(c_usercmd * cmd);

	Vector fix_movement(c_usercmd * cmd, c_usercmd original_command) const;

private:
	float get_move_angle(float speed);

	bool get_closest_plane(Vector * plane);

	bool will_hit_obstacle_in_future(float predict_time, float step);

	void circle_strafe(c_usercmd * cmd, float * circle_yaw);
};