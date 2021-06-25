// Useful function for placing objects on surfaces

/**
  Moves vertically up or down (default) the landscape to find a solid pixel.
  @par x The x coordinate to check, in global coordinates.
  @par y The y coordinate to check, in global coordinates.
  @par step [optional] Moves this many pixels between checks. May not be null;
  @return An y value inside the landscape bounds, or -1 if no valid position could be found.
  @author Tyron, GZE Project
  @version 0.3
 */
global func GetYSolid(int x, int y, int step)
{
	if (x < 0 || x > LandscapeWidth())
	{
		FatalError("The x coordinate is not inside the landscape bounds");
	}
	if (y < 0 || y > LandscapeHeight())
	{
		FatalError("The y coordinate is not inside the landscape bounds");
	}
	if (step == 0)
	{
		FatalError("The step parameter may not be 0");
	}

	if (step == nil)
	{
		step = 1;
	}
	
	while (!GBackSolid(x - GetX(), y - GetY()))
	{
		y += step;
		if (y >= LandscapeHeight() || y <= 0)
		{
			return -1;
		}
	}

	return y;
}

/**
  Moves vertically up (default) or down the landscape to find a free pixel.
  @par x The x coordinate to check, in global coordinates.
  @par y The y coordinate to check, in global coordinates.
  @par step [optional] Moves this many pixels between checks. May not be null;
  @return An y value inside the landscape bounds, or -1 if no valid position could be found.
  @author Tyron, GZE Project
  @version 0.3
 */
global func GetYFree(int x, int y, int step)
{
	if (x < 0 || x > LandscapeWidth())
	{
		FatalError("The x coordinate is not inside the landscape bounds");
	}
	if (y < 0 || y > LandscapeHeight())
	{
		FatalError("The y coordinate is not inside the landscape bounds");
	}
	if (step == 0)
	{
		FatalError("The step parameter may not be 0");
	}

	if (step == nil)
	{
		step = -1;
	}
	
	while (GBackSolid(x - GetX(), y - GetY()))
	{
		y += step;
		if (y >= LandscapeHeight() || y <= 0)
		{
			return -1;
		}
	}
	
	return y;
}


/**
 Moves vertically up or down to find the bordering y-position
 between solid and nonsolid material
 @par x The x coordinate to check, in global coordinates.
 @par y The y coordinate to check, in global coordinates.
 @par step [optional] Moves this many pixels between checks. May not be null;
 @return An y value inside the landscape bounds, or -1 if no valid position could be found.
 @author Tyron, GZE Project
 @version 0.3
 */
global func GetYBorder(int x, int y, int step)
{

	if (GBackSolid(x - GetX(), y - GetY()))
	{
		return GetYFree(x, y, -step);
	}
	else
	{
		return GetYSolid(x, y, step);
	}
}
