/*
 * Everything that has to do with grass
 */

global func GrassForegroundFrequency(){ return 5;} // 1 out of this many grasses is in foreground, on average

// Adjusted from the original method
global func PlaceGrass(int amount, int start, int end, int height, int bottom)
{
	if (!start)
		start = 0;
	if (!end)
		end = LandscapeWidth();
	if (!height)
		height = 0;
	if (!bottom)
		bottom = LandscapeHeight();
		
	var x = start, y; 
	var variance = Random(100);
	while (x < end)
	{
		y = height;
		while (y < bottom)
		{
			if (GetMaterial(AbsX(x), AbsY(y)) == Material("Sky"))
				if (GetMaterial(AbsX(x), AbsY(y + 3)) == Material("Earth"))
					if (Random(100) < amount)
					{
						var grass = CreateObject(Grass, AbsX(x), AbsY(y + 1), NO_OWNER);

						var scale_x = 1000 + 5 * variance;
						var scale_y = 1000 + 10 * variance;
						var lat_x = 0, lat_y = 500 + variance * 30;
						grass->SetObjDrawTransform(scale_x, 0, lat_x, 0, scale_y, -lat_y);

						// put in foreground :D
						if (!Random(GrassForegroundFrequency())) ScheduleCall(grass, "PlaceVegetationForeground", 5, 0, grass);
					}
			y += 3;
		}
		x += 9;

		variance = BoundBy(variance + RandomX(-5, 5), 0, 100);
	}
}
