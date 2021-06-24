#appendto LargeCaveMushroom

// for the placement
private func GoodSpot(int x, int y)
{
	return ObjectCount(Find_Distance(100, x, y), Find_ID(LargeCaveMushroom)) < 3;
}
