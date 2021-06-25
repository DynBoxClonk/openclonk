
/**
 Sets the Rotation using a custom rotation center.
 @par angle The angle to rotate, in degrees.
 @par center_x The x coordinate of the center. Offset.
 @par center_y The y coordinate of the center. Offset.
 @return The object itself.
 @version 0.3
 @author Clonk Infinity Project
  */
global func RelSetR(int angle, int center_x, int center_y)
{
	if (!this)
	{
		FatalError("This function needs object context");
	}

	// angle relative to the new coordinates
	var angle1 = Angle(0, 0, center_x, center_y);
	var angle2 = angle1 + GetR();
	var radius = Distance(0, 0, center_x, center_y);
	// angle2 + Radius => calculated pivot
	var rotX = (Sin(angle2, 100) * radius) / 100;
	var rotY = (-Cos(angle2, 100) * radius) / 100;
	// rotation + angle1 + Radius + calculated pivot => final position
	angle1 += angle;
	var targetX = (-Sin(angle1, 100) * radius) / 100 + rotX;
	var targetY = (Cos(angle1, 100) * radius) / 100 + rotY;
	// adjust position
	MovePosition(targetX, targetY);
	SetR(angle);
	return this;
}
