/* -- Monster -- */

#include Library_AnimalControl
#include Library_AnimalReproduction

static const ANIM_SLOT_Monster_Attack = 2;

/* -- Reproduction -- */

private func AnimalReproductionRate(){    return 2000;}
private func AnimalReproductionMaxCount()       { return 4; }
public func AnimalInitialAction(){    return "Walk";}


// Special behaviour, e.g. monster eggs
private func AnimalReproductionCustom()
{
    if (!Random(AnimalReproductionRate()))
    {
		CreateContents(MonsterEgg)->Exit(0, 10);
	}
	return true;
}


/* -- Movement -- */

private func ContactLeft()
{
	AnimalTurnRight();
}

private func ContactRight()
{
	AnimalTurnLeft();
}

private func AnimalCanTurn(){ return !Stuck() && (GetAction() == "Walk" || GetAction() == "Swim"); }


public func DoJump()
{
	if (GetAction() == "Walk")
	{
		if (Random(2)) PlaySoundGrowl();
		Jump();
	}
}

public func Jump(int xdir, int ydir)
{
	SetAction("Jump");
	
	ydir = ydir ?? -30;
	xdir = xdir ?? 5;
	if (GetDir() == DIR_Left)
		xdir *= -1;

	SetSpeed(GetXDir() + xdir, ydir - GetXDir() / 2);
	
	if (IsDusty())
	{
		var range = GetDefWidth() * GetCon() / 200;
		var dir = Sign(GetXDir());
		var particles = Particles_GroundDust();
		CreateParticle("Dust", PV_Random(range * dir * -2, range * dir * -1), GetVertex(2, 1), PV_Random(dir * 2, dir * 1), PV_Random(-2, -5), PV_Random(36, 2 * 36), particles, GetCon() / 10);
	}
	return;
}

/* -- Engine callbacks -- */

private func Death()
{
	PlaySoundDeath();
	SetDir(DIR_Left);
	SetAction(Format("Dead%d", RandomX(1, 2)));
	SetName(Format("$DeadDesc$ %s", GetName()));
}

/* -- Activity -- */

private func FxIntAnimalActivityMovement(object target, proplist ai)
{
    if (target != this) return;
    
	// Do nothing
	if (Random(2) && (GetAction() == "Walk" || GetAction() == "Swim"))
	{
		if (GetAction() == "Walk")
		{
			// Jump
			if (!Random(3)) return DoJump();
			
			if (this->~SpecialActivity()) return;
			
			// Look around
			if (!Random(8)) return SetAction("Look");
		}
		
		if (Random(2))
		{
			AnimalTurnRight();
		}
		else
		{
			AnimalTurnLeft();
		}
	}
}

/* -- Attacking things -- */


// Hit targets while walking
private func HitCheck()
{
	var strength = GetAttackStrength();
	DoAttack(strength, false);
	if (!Random(3))
	{
		ShakeViewport(strength, 0, 0, Max(1, GetCon()));
	}

}

// Hit targets by jumping on them
private func Hit2()
{
	var full_range = GetYDir() >= 20;
	var factor;
	if (full_range)
	{
		factor = 2;
	}
	else
	{
		factor = 1;
	}

	var strength = GetAttackStrength(factor);
	DoAttack(strength, full_range);
	ShakeViewport(strength, 0, 0, Max(1, GetCon()));
	if (IsDusty())
	{
		var particles = Particles_GroundDust();
		var range = GetDefWidth() * GetCon() / 300;
		CreateParticle("Dust", PV_Random(-range, range), GetVertex(2, 1), PV_Random(-3, 3), PV_Random(-2, -7), PV_Random(36, 2 * 36), particles, GetCon() / 5);
	}
}


private func DoAttack(int strength, bool full_range)
{
	for (var target in GetAttackTargets(full_range))
	{
		if (!GetEffect("FxMonsterHitCooldown", target))
		{
			target->CreateEffect(FxMonsterHitCooldown, 1, 40);
			Punch(target, strength);
		}
	}
}


private func GetAttackTargets(bool full_range)
{
	var range_x = GetDefWidth() * GetCon() / 200;
	var range_y = GetDefWidth() * GetCon() / 200;
	var area;
	if (full_range)
	{
		area = Find_InRect(-range_x, 0, 2 * range_x, range_y);
	}
	else
	{
		area = Find_InRect((-1 + GetDir()) * range_x, 0, range_x, range_y);
	}
	return FindObjects(Find_Func("IsPrey"), area, Find_NoContainer());
}


private func GetAttackStrength(int factor)
{
	return Max(1, (factor ?? 1) * GetCon() / 10);
}


local FxMonsterHitCooldown = new Effect
{
	Timer = func()
	{
		return FX_Execute_Kill;
	},
};


/* -- Effects -- */


private func FootStep()
{
	if (IsDusty())
	{
		var range = GetDefWidth() * GetCon() / 300;
		var dir = Sign(GetXDir());
		var particles = Particles_GroundDust();
		CreateParticle("Dust", PV_Random(dir * -2 * range, dir * range), 8, PV_Random(dir * 2, dir * 1), PV_Random(-2, -3), PV_Random(36, 2 * 36), particles);
	}
}


private func IsDusty()
{
	return GetMaterialVal("DigFree", "Material", GetMaterial(GetVertex(2, 0), GetVertex(2, 1) + 2)) != 0;
}


private func Particles_GroundDust()
{
	var clr = SplitRGBaValue(GetAverageTextureColor(GetTexture(GetVertex(2, 0), GetVertex(2, 1) + 2)));
	var particles =
	{
		Prototype = Particles_Dust(),
		R = clr.R,
		G = clr.G,
		B = clr.B,
		Size = PV_KeyFrames(0, 0, 5, 100, 20, 1000, 10), // slightly larger dust than usually
	};
	return particles;
}

/* -- Sounds -- */

private func PlaySoundGrowl()
{
	Sound("Animals::Monster::MonsterGrowl?", {pitch = 100 - GetCon()});
}

private func PlaySoundDeath()
{
	Sound("Animals::Monster::MonsterDeathGrowl", {pitch = 100 - GetCon()});
}

/* -- Animal hunt goal -- */

private func IsAnimalType(type)
{
	return type == GOAL_AnimalType_Monster;
}


/* -- Turning -- */

private func Construction()
{
	_inherited(...);
	var fx = CreateEffect(FxTurn, 1, 1);
	fx.dir = GetDir();
}


local FxTurn = new Effect
{
	Timer = func ()
	{
		// Switch directions?
		if (this.dir != this.Target->GetDir())
		{
			this.dir = this.Target->GetDir();
			this.progress = 180 - this.progress; // invert the turn progress
		}
		// Update the graphics
		if (this.progress)
		{
			this.progress = Max(0, this.progress - 10);
			this.Target.MeshTransformation = Trans_Rotate(this.progress, 0, 1, 0);
		}
	}
};


/* -- Properties -- */

local Name = "$Name$";
local Description = "$Description$";

local MaxEnergy = 250000;
local MaxBreath = 720; // Monster can breathe for 20 seconds under water.
local NoBurnDecay = 1;
local ContactCalls = true;
local Collectible = false;
local BorderBound = C4D_Border_Sides;

/* -- Actions -- */

local ActMap = {
Walk = {
	Prototype = Action,
	Name = "Walk",
	Procedure = DFA_WALK,
	Directions = 2,
	FlipDir = 1,
	Length = 16,
	Delay = 5,
	FacetBase = 1,
	Accel = 8,
	Decel = 48,
	Speed = 200,
	NextAction = "Walk",
	StartCall = "HitCheck",
	InLiquidAction = "Swim",
	Animation = "Walk",
	PhaseCall = "FootStep",
},

Turn = {
	Prototype = Action,
	Name = "Turn",
	Procedure = DFA_NONE,
	Directions = 2,
	FlipDir = 1,
	Length = 7,
	Delay = 2,
	FacetBase = 1,
	NextAction = "Walk",
},

Jump = {
	Prototype = Action,
	Name = "Jump",
	Procedure = DFA_FLIGHT,
	Directions = 2,
	FlipDir = 1,
	Length = 17,
	Delay = 1,
	FacetBase = 1,
	Speed = 100,
	Accel = 8,
	NextAction = "Hold",
	InLiquidAction = "Swim",
	Animation = "Jump",
},

Swim = {
	Prototype = Action,
	Name = "Swim",
	Procedure = DFA_SWIM,
	Directions = 2,
	FlipDir = 1,
	Length = 16,
	Delay = 5,
	FacetBase = 1,
	Speed = 96,
	Accel = 7,
	Decel = 9,
	NextAction = "Swim",
	Animation = "Walk",
},

Look = {
	Prototype = Action,
	Name = "Look",
	Procedure = DFA_NONE,
	Directions = 2,
	FlipDir = 1,
	Delay = 40,
	Attach = 8,
	FacetBase = 1,
	NextAction = "Walk",
	InLiquidAction = "Swim",
	Animation = "Look",
},

Dead1 = {
	Prototype = Action,
	Name = "Dead1",
	Animation = "Death1",
	Procedure = DFA_NONE,
	Length = 30,
	Delay = 1,
	FacetBase = 1,
	Directions = 2,
	FlipDir = 1,
	NextAction = "Hold",
	NoOtherAction = 1,
	ObjectDisabled = 1,
},

Dead2 = {
	Prototype = Action,
	Name = "Dead2",
	Animation = "Death2",
	Procedure = DFA_NONE,
	Length = 50,
	Delay = 1,
	FacetBase = 1,
	Directions = 2,
	FlipDir = 1,
	NextAction = "Hold",
	NoOtherAction = 1,
	ObjectDisabled = 1,
},


};
