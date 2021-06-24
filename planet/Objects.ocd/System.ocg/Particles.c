
global func Particles_Leaves()
{
	return
	{
		Size = PV_Random(3, 6),
	    Phase = PV_Linear(0, 7*5),
		Alpha = PV_KeyFrames(0, 0, 255, 900, 255, 1000, 0),
		CollisionVertex = 500,
		OnCollision = PC_Stop(),
		ForceX = PV_Wind(100),
		ForceY = PV_Gravity(40),
		DampingX = 975, DampingY = 975,
		Rotation = PV_Direction(PV_Random(750, 1250)),
		Attach = nil
	};
}