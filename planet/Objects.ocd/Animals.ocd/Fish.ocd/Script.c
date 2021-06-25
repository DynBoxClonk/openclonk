/*
 * Fish
 *
 * Author: Marky, original from Clonk Rage
 */

#include Fish

func Definition(def)
{
	def.PictureTransformation = Trans_Mul(Trans_Rotate(-10, 0, 0, 1), Trans_Rotate(20, 1, 0, 0), Trans_Rotate(20, 0, 1, 0));
}

public func Construction()
{
	inherited(...);
	
	ColorGreyish();
}

func ColorGreyish()
{
	var r1 = 255, g1 = 255, b1 = 255;
	var r2 =  75, g2 =  85, b2 = 108;
	var r3 = 139, g3 = 142, b3 = 149;

	var amount = Random(100);
	var r, g, b;

	if (!Random(2))
	{
		r = (r1 * (100-amount) + r2 * amount)/100;
		g = (g1 * (100-amount) + g2 * amount)/100;
		b = (b1 * (100-amount) + b2 * amount)/100;
	}
	else
	{
		r = (r1 * (100-amount) + r3 * amount)/100;
		g = (g1 * (100-amount) + g3 * amount)/100;
		b = (b1 * (100-amount) + b3 * amount)/100;
	}

	var color = RGB(r, g, b);

	SetColor(color);
}

func ColorRainbow()
{
	SetColor(HSL(Random(256), 255, RandomX(100, 160)));
}


func DoEat(object obj)
{
	// a little effect
	var animation = "Bite";
	var duration = 30;
	var slot = 1;
	PlayAnimation(animation, slot, Anim_Linear(0, 0, GetAnimationLength(animation), duration, ANIM_Remove), Anim_Const(1000));
	
	// the usual
	_inherited(obj);
}

local ActMap = {

Swim = {
	Prototype = Action,
	Name = "Swim",
	Procedure = DFA_SWIM,
	Speed = 100,
	Accel = 16,
	Decel = 16,
	Length = 1,
	Delay = 0,
	FacetBase=1,
	NextAction = "Swim",
	StartCall = "StartSwim"
},
Walk = {
	Prototype = Action,
	Name = "Walk",
	Procedure = DFA_WALK,
	Speed = 30,
	Accel = 16,
	Decel = 16,
	Length = 1,
	Delay = 0,
	FacetBase=1,
	Directions = 2,
	FlipDir = 1,
	NextAction = "Walk",
	StartCall = "StartWalk",
	InLiquidAction = "Swim",
},
Jump = {
	Prototype = Action,
	Name = "Jump",
	Procedure = DFA_FLIGHT,
	Speed = 15, //was: 30, this fish is not so fast 
	Accel = 16,
	Decel = 16,
	Length = 1,
	Delay = 0,
	FacetBase=1,
	Directions = 2,
	FlipDir = 1,
	NextAction = "Jump",
	StartCall = "StartJump",
	InLiquidAction = "Swim",
},
Dead = {
	Prototype = Action,
	Name = "Dead",
	Procedure = DFA_NONE,
	Speed = 10,
	Length = 1,
	Delay = 0,
	FacetBase=1,
	Directions = 2,
	FlipDir = 1,
	NextAction = "Hold",
	NoOtherAction = 1,
	ObjectDisabled = 1,
}
};

local Name="$Name$";
local MaxEnergy=30000;
local MaxBreath = 180;
local Description="$Description$";
local Collectible = true;
local NoBurnDecay=1;
local BreatheWater=1;
local BorderBound = C4D_Border_Sides | C4D_Border_Top | C4D_Border_Bottom;
local ContactCalls = true;
