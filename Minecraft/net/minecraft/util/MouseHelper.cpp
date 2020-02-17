#include "MouseHelper.h"

void MouseHelper::grabMouseCursor()
{
	Mouse.setGrabbed(true);
	deltaX = 0;
	deltaY = 0;
}

void MouseHelper::ungrabMouseCursor()
{
	Mouse.setCursorPosition(Display.getWidth() / 2, Display.getHeight() / 2);
	Mouse.setGrabbed(false);
}

void MouseHelper::mouseXYChange()
{
	deltaX = Mouse.getDX();
	deltaY = Mouse.getDY();
}
