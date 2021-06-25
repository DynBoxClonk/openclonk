/*-- Zap --*/

local target, origin_x, origin_y;

private func Flying()
{
	var xdir, ydir;

	if (GetXDir() < 0)
		SetDir(DIR_Left);
	else
		SetDir(DIR_Right);

	var fleeFromObj = FindObject(Find_Distance(50), Find_Category(C4D_Object), Find_OCF(OCF_HitSpeed1), Find_NoContainer());
	if (fleeFromObj != nil)
	{
		xdir = BoundBy(GetX() - fleeFromObj->GetX(), -1, 1);
		ydir = BoundBy(GetY() - fleeFromObj->GetY(), -1, 1);
		if (xdir == 0) xdir = Random(2) * 2 - 1;
		if (ydir == 0) ydir = Random(2) * 2 - 1;
		xdir = RandomX(5 * xdir, 10 * xdir);
		ydir = RandomX(5 * ydir, 10 * ydir);

		SetSpeed(xdir, ydir);

		return;
	}


	// do nothing
	if (Random(4)) return;

	// random velocity
	xdir = Random(13)-6;
	ydir = Random(9)-4;

	// Follow someone? Adjust velocity...

	if (target)
	{
		xdir += BoundBy((target->GetX()-GetX()),-11, 11);
		ydir += BoundBy((target->GetY()-GetY())/3,-7, 7);
	}
	else if (Distance(GetX(), GetY(), origin_x, origin_y) > 50) // return to the swarm
	{
		xdir = BoundBy(origin_x - GetX(), -1, 1);
		ydir = BoundBy(origin_y - GetY(), -1, 1);
		xdir = RandomX(xdir, 6 * xdir);
		ydir = RandomX(ydir, 6 * ydir);
	}

	if (GBackLiquid(xdir, ydir))
		SetSpeed();
	else
		SetSpeed(xdir, ydir);
}

protected func Check()
{
	var obj;

	// Stuck or drowning? Die!
	if (GBackSemiSolid()) return Death();
	// Die randomly
	if (!Random(160)) return Death();
	// Buzz
	if (!Random(50)) DoBuzz();

	// Get another target...
	if (!Random(10)) target = FindTarget(200);

	// Randomly abandon target
	// Target is safe in a building
	// Target is safe under water
	// Abandon target because you cannot reach him
	if (target)
	if (!Random(30)
	|| target->Contained()
	|| GBackSemiSolid(target->GetX()-GetX(), target->GetY()-GetY())
    || (!Random(5) && !PathFree(GetX(), GetY(), target->GetX(), target->GetY())))
		target = nil;

	// sting
	if (!Random(2))
	 Sting(FindTarget());
}
  
private func Sting(obj)
{
	if (obj == nil) return;
	Punch(obj, 3+Random(2));
	DoSting();
	if (!Random(3)) Death();
}

func DoBuzz()
{
	Sound("Animals::Zap::Bsss");
}

func DoSting()
{
	Sound("Animals::Zap::Sting");
}

public func FindTarget(maxdist)
{
	if (!maxdist)
	return	FindObject(Find_AtPoint(),
					   Find_OCF(OCF_CrewMember),
					   Find_OCF(OCF_InFree),
					   Find_NoContainer(),
					   Find_Not(Find_OCF(OCF_InLiquid)));
	else
	return FindObject(Find_Distance(maxdist),
			          Find_OCF(OCF_CrewMember),
			          Find_OCF(OCF_InFree),
			          Find_NoContainer(),
			          Find_Not(Find_OCF(OCF_InLiquid)));
}

protected func Initialize()
{
	SetAction("Fly");
	SetPhase(Random(6));
	origin_x = GetX();
	origin_y = GetY();
}

local ActMap = {
Fly = {
		Prototype=Action,
		Name="Fly",
		Procedure=DFA_FLOAT,
		Directions=2,
		FlipDir=1,
		Speed=100,
		Length=10,
		Delay=2,
		X=0, Y=0, Wdt=4, Hgt=4, OffX=-1, OffY=-1,
		NextAction="Fly",
		PhaseCall="Flying",
		EndCall="Check",
},
};

public func CatchBlow()	{ RemoveObject(); }
public func Damage()	{ RemoveObject(); }
protected func Death()	{ RemoveObject(); }

local Name="$Name$";
local Description="$Description$";
local ContactIncinerate=1;
local BlastIncinerate=1;
