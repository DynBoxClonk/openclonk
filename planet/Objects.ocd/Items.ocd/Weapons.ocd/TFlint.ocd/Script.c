/*--- T-Flint ---*/

protected func Construction()
{
	var graphic = Random(3);
	if (graphic) SetGraphics(Format("%d", graphic));
}

public func ControlUse(object clonk, int x, int y)
{
	// if already activated, nothing (so, throw)
	if (GetEffect("FuseBurn", this))
	{
		clonk->ControlThrow(this, x, y);
		return true;
	}
	else
	{
		Fuse();
		return true;
	}
}

func Fuse()
{
	if (!GetEffect("FuseBurn"))
	{
		AddEffect("FuseBurn", this, 1, 1, this);
		SetGraphics("Active");
	}
}

func FxFuseBurnTimer(object bomb, proplist effect, int timer)
{

	if (timer == 1)
	{
		Sound("Fire::Fuse");
	}
	if (timer <= 10 && !Random(5))
	{
		CreateParticle("Fire", 0, 0, PV_Random(-5, 5), PV_Random(-15, 5), PV_Random(10, 40), Particles_Glimmer(), 5);
	}
	if (timer >= 60)
	{
		Sound("Fire::FuseLoop", nil, nil, nil,-1);
		DoExplode();
		return FX_Execute_Kill;
	}
}

func DoExplode()
{
	Explode(this->ExplodeSize());
}

func ExplodeSize()
{
	return 20;
}

protected func Hit(int x, int y)
{
	StonyObjectHit(x, y);
	Fuse();
}

protected func RejectEntrance()
{
	return GetAction() == "Fuse" || GetAction() == "Ready";
}

// Drop fusing bomb on death to prevent explosion directly after respawn
public func IsDroppedOnDeath(object clonk)
{
	return !!GetEffect("FuseBurn", this);
}


local Collectible = true;
local Name = "$Name$";
local Description = "$Description$";
local Rebuy = true;
local Plane = 530; // cause it's explosive, players should see it in a pile of stuff
local Components = {Flint = 1, Sulphur = 1};

/* Can be produced in the chemical plant */
public func IsChemicalProduct() { return true; }

func IsAlchemContainer() { return true ; }
func AlchemProcessTime() { return 100; }
