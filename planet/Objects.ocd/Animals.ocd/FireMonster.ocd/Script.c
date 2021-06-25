/* -- Fire monster -- */

#include Monster

private func Construction()
{
	_inherited(...);
	MakeNonFlammable();
}

/* -- Fire attack -- */

private func SpecialActivity()
{
	if (Random(8))
		return false;
	
	return StartFlame();
}

public func StartFlame()
{
	if (GetAction() != "Walk" || GetEffect("FxFireAttack", this) || GetCon() < 50)
		return false;
		
	CreateEffect(FxFireAttack, 1, 1);
	Sound("Fire::Inflame");
	PlaySoundGrowl();
	PlayAnimation("Breath", ANIM_SLOT_Monster_Attack, Anim_Linear(0, 0, GetAnimationLength("Breath"), 40, ANIM_Remove));
	SetAction("FireBreath"); // So that it does not walk
}


private func Flaming(int target_angle)
{
	var pos = GetAttackPos();
	
	var angle = Normalize(target_angle ?? (90 * GetCalcDir()), -180);
	angle -= GetCalcDir() * 10;
	angle += RandomX(-7, 7);
	var range = RandomX(40, 60);

	var flame = CreateObject(Flame, pos.x, pos.y, GetOwner());
	flame->SetController(GetController());
	flame->SetXDir(+Sin(angle, range));
	flame->SetYDir(-Cos(angle, range));
	
	// Fiat lux!
	flame->SetLightRange(60, 40);
	flame->SetLightColor(FIRE_LIGHT_COLOR);
}


private func GetAttackPos()
{
	return { x = 22 * GetCalcDir() * GetCon() / 100, y = (-2) * GetCon() / 100, };
}


local FxFireAttack = new Effect
{

	Timer = func(int time)
	{
		// Timeout - should exist long after the attack, for cooldown
		if (time >= 140)
		{
			return FX_Execute_Kill;
		}
		// Actual flame attack every 2 frames
		if (Inside(time, 15, 35) && time%2)
		{
			// Find a victim
			if (!this.Prey)
			{
				var x_range = 2 * this.Target->GetCon();
				var y_range = this.Target->GetCon();
				var dir = this.Target->GetDir();
				this.Prey = this.Target->FindObject(Find_Func("IsPrey"), Find_OCF(OCF_Alive), 
				                                    Find_InRect((-1 + dir) * x_range, -y_range, x_range, 2 * y_range));
			}
			// Spray it!!!!
			if (this.Prey)
			{
				var pos = this.Target->GetAttackPos();
				var angle = Angle(this.Target->GetX() + pos.x, this.Target->GetY() + pos.y, this.Prey->GetX(), this.Prey->GetY()); 
				this.Target->Flaming(angle);
			}
			else
			{
				this.Target->Flaming();
			}
		}
		return FX_OK;
	},	
};


/* -- Properties -- */

local Name = "$Name$";
local Description = "$Description$";

local MaxEnergy = 250000;
local MaxBreath = 720; // Monster can breathe for 20 seconds under water.
local NoBurnDecay = 1;
local ContactCalls = true;
local Collectible = true;
local BorderBound = C4D_Border_Sides;

local ActMap = {

FireBreath = {
	Prototype = Action,
	Name = "FireBreath",
	Procedure = DFA_NONE,
	Directions = 2,
	FlipDir = 1,
	Delay = 40,
	Attach = 8,
	FacetBase = 1,
	NextAction = "Walk",
	InLiquidAction = "Swim",
},

};
