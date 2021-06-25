/**
 This stuff should be moved to a library or so in OC, so that barriers like the wall kit can be implemented by different items.
 */
 
#appendto WallKit
 
/* Bridge position calculation */

private func Offset2BridgeCoords(object clonk, int x, int y)
{
	// Returns starting and end point offset of bridge to be built as player points to offset x/y
	var dx = clonk->GetDefWidth(), dy = clonk->GetDefHeight(), ox, oy, rx, ry;
	var l = BridgeLength;
	var d = BridgeThickness;
	ox = x * 2 / Abs(y + !y); 
	oy = y * 2 / Abs(x + !x);
	ry = ox /= Abs(ox) + !ox;
	rx = oy /= Abs(oy) + !oy;
	// Offset of the bridge: more for vertical and horizontal bridges.
	ox *= dx / 2 + 3 * !oy;
	oy *= dy / 2 + 3 * !ox;
	// Reduce thickness and length for diagonal bridges.
	l -= l * 3 * Abs(rx * ry) / 10;
	d -= d * 3 * Abs(rx * ry) / 10;
	
	//-----------------------------------------------
	// New code below
	
	// Adjust offset
	if (oy > 0)
	{
		// Diagonal bridge? Move upward
		if (ox != 0)
		{
			oy += d - (dy / 2);	
		}
		// Move bridge below the clonk, so that it starts at his feet
		ox += clonk->GetCalcDir() * l;
		ox -= clonk->GetCalcDir() * (dx / 2);
	}
	else if (oy == 0)
	{
		// Move vertical bridge to clonk feet
		oy += (dy / 2) - l;
	}
	
	// End of new code
	//-----------------------------------------------
	return { dxp = ry * d / 2, dyp = rx * d / 2, dxm = ry * (d + 1) / 2, dym = rx * (d + 1) / 2, x1 = ox + rx * l, y1 = oy - ry * l, x2 = ox - rx * l, y2 = oy + ry * l };
}
