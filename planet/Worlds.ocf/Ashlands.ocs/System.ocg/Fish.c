#appendto ClassicFish

public func Construction()
{
	inherited(...);
	
	ColorGreenish();
}


// ignore old color
func ColorGreyish(){}

// new color
func ColorGreenish()
{
	var r1 = 75, g1 = 165, b1 = 130;
	var r2 = 15, g2 = 100, b2 =  45;

	var amount = Random(100);
	var r, g, b;

	r = (r1 * (100-amount) + r2 * amount)/100;
	g = (g1 * (100-amount) + g2 * amount)/100;
	b = (b1 * (100-amount) + b2 * amount)/100;

	var color = RGB(r, g, b);

	SetColor(color);
}
