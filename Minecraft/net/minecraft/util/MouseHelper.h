#pragma once

struct MouseHelper
{
	int deltaX;
	int deltaY;

	void grabMouseCursor();
	void ungrabMouseCursor();
	void mouseXYChange();
};