#pragma once
class Vec2f
{
	static Vec2f ZERO;
	static Vec2f ONE;
	static Vec2f UNIT_X;
	static Vec2f NEGATIVE_UNIT_X;
	static Vec2f UNIT_Y;
	static Vec2f NEGATIVE_UNIT_Y;
	static Vec2f MAX;
	static Vec2f MIN;

	Vec2f(float xIn, float yIn);

public:
	float x;
	float y;
};
