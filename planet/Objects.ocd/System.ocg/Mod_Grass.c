#appendto Grass


private func Randomize(plane, int min_growth, int max_growth)
{
	if (GetType(plane) == C4V_Int)
	{
		this.Plane = plane;
	}
	else if (GetType(plane) == C4V_Array)
	{
		this.Plane = plane[Random(GetLength(plane))];
	}
	
	SetCon(RandomX(min_growth ?? 30, max_growth ?? 130));
	Unstick(30);

	if (this.Plane > 0)
	{
		// Adjust to landscape slope
		var x = 18 * GetCon() / 100;
		var y = 15 * GetCon() / 100;
		var slope = GetSolidOffset(-x, y) - GetSolidOffset(x, y);
		SetR(slope);
	}
}

private func GetSolidOffset(int x, int y)
{
	var off;
	for (off = -15; GBackSolid(x, y - off) && (off < 15); ++off);
	return off;
}
