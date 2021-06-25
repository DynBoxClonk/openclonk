
/* -- Engine Callbacks -- */

private func Definition(def)
{
	this.PictureTransformation = Trans_Rotate(20, 0, 0, 1);
	_inherited(def, ...);
}


private func Hit(int x, int y)
{
	StonyObjectHit(x,y);
	if (!Random(2))
	{
		Hatch();
	}
}

private func Damage()
{
	Hatch();
}


private func Hatch()
{
	// Already hatched??
	if (GBackSolid(0, -1) || !this.Collectible)
		return;

	Exit();
	this.Collectible = false;
	
	// Release the animal
	var child = Contents() ?? this->~CreateHatchling();
	if (child)
	{
		child->Exit();
		child->~AnimalBirth();
	}

	// Effects
	Sound("Animals::EggBreak");
	FadeOut(200, true);
	SetGraphics("Broken");
	
	var color = SplitRGBaValue(this->~GetEggColor() ?? RGB(255, 255, 255));
	var particles = 
	{
		Size = PV_KeyFrames(0, 0, 0, 100, PV_Random(3, 5), 1000, 3),
		R = color.R,
		G = color.G,
		B = color.B,
		Alpha = PV_Linear(255, 0),
		ForceY = PV_Gravity(100),
		CollisionVertex = 0
	};

	CreateParticle("Hail", 0, 0, PV_Random(-5, 5), PV_Random(-5, 0), PV_Random(10, 60), particles, 20);
	
}


private func Initialize()
{
	AddTimer(this.Breed, 350);
	_inherited(...);
}


private func Breed()
{
	if (!Random(200))
	{
		Hatch();
	}
}
