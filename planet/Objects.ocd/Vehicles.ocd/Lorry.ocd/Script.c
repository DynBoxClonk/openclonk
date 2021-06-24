/*-- Lorry --*/

#include Library_ElevatorControl

static const LORRY_Effect_DumpContents = "DumpContents";
static const LORRY_Effect_BlockCollection = "BlockCollectionByLorry";

local content_menu;

public func IsLorry() { return true;}
public func IsVehicle() { return true;}
public func IsContainer() { return true;}
public func IsToolProduct() { return true;}

/*-- Knowledge --*/

private func UpdatePlayerHomebaseMaterial(int player)
{
	// The player may buy a lorry, if he could also produce one
	ConfigurePlayerHomebaseMaterial(player, {ClassicLorry = 1}, 0);
}

/*-- Movement --*/

protected func ContactLeft()
{
	if (Stuck() && !Random(5))
		SetRDir(RandomX(-7, +7));
}

protected func ContactRight()
{
	if (Stuck() && !Random(5))
		SetRDir(RandomX(-7, +7));
}

func Hit3()
{
	Sound("Hits::Materials::Metal::DullMetalHit?");
}

/*-- Content Dumping --*/

public func HoldingEnabled() { return true; }

public func ControlUseStart(object clonk, int x, int y)
{
	var direction = DIR_Left;
	if (x > 0)
		direction = DIR_Right;
	if (!GetEffect(LORRY_Effect_DumpContents, this))
		AddEffect(LORRY_Effect_DumpContents, this, 100, 1, this, nil, direction);
	return true;
}

public func ControlUseHolding(object clonk, int x, int y)
{
	var direction = DIR_Left;
	if (x > 0)
		direction = DIR_Right;
	var effect = GetEffect(LORRY_Effect_DumpContents, this);
	if (effect)
		effect.dump_dir = direction;
	return true;
}

public func ControlUseStop(object clonk, int x, int y)
{
	RemoveEffect(LORRY_Effect_DumpContents, this);
	return true;
}

public func ControlUseCancel(object clonk, int x, int y)
{
	RemoveEffect(LORRY_Effect_DumpContents, this);
	return true;
}

public func FxDumpContentsStart(object target, proplist effect, int temp, int direction)
{
	if (temp)
		return FX_OK;
	// The time it takes to dump the contents depends on the mass of the lorry.
	effect.dump_strength = BoundBy(1000 / GetMass(), 3, 8);
	effect.dump_dir = direction;
	// Rotate the lorry into the requested direction.
	var rdir = -effect.dump_strength;
	if (effect.dump_dir == DIR_Right)
		rdir = effect.dump_strength;
	SetRDir(rdir);
	return FX_OK;
}

public func FxDumpContentsTimer(object target, proplist effect, int time)
{
	// Rotate the lorry into the requested direction.
	var rdir = -effect.dump_strength;
	if (effect.dump_dir == DIR_Right)
		rdir = effect.dump_strength;	
	SetRDir(rdir);
	// Dump one item every some frames if the angle is above 45 degrees. Only do this if the effect is at least active 
	// for 10 frames to prevent an accidental click while holding the lorry to dump some of its contents.
	if (time >= 10 && ((effect.dump_dir == DIR_Left && GetR() < -45) || (effect.dump_dir == DIR_Right && GetR() > 45)))
	{
		if (!Random(3))
		{
			var x = RandomX(6, 8) * Sign(GetR());
			var xdir = RandomX(70, 90) * Sign(GetR());
			var random_content = FindObjects(Find_Container(this), Sort_Random());
			if (GetLength(random_content) >= 1)
			{
				random_content = random_content[0];
				random_content->Exit(x, RandomX(2, 3), Random(360), 0, 0, RandomX(-5, 5));
				random_content->SetXDir(xdir, 100);
				// Assume the controller of the lorry is also the one dumping the contents.
				random_content->SetController(GetController());
				AddEffect(LORRY_Effect_BlockCollection, random_content, 100, 8, this);
			}		
		}
	}
	return FX_OK;
}

public func FxDumpContentsStop(object target, proplist effect, int reason, bool temp)
{
	if (temp)
		return FX_OK;
	// Stop rotating the lorry.
	SetRDir(0);
	return FX_OK;
}

public func FxBlockCollectionByLorryTimer() { return FX_Execute_Kill; }

/*-- Rotation behaviour --*/

protected func Initialize()
{
	AddEffect("IntUprightAttach", this, 1, 1, this);
}


protected func FxIntUprightAttachTimer(object target, proplist effect, int timer)
{
	// ignore this while dumping contents
	var is_dumping = GetEffect(LORRY_Effect_DumpContents, target);
	if (is_dumping) return FX_OK;

	// rotate to upright position
	var rot = target->GetR();
	if (Abs(rot) < 30) return FX_OK;
	if (!(GetContact(-1) & CNAT_Bottom)) return FX_OK;
	if (GetContact(2) && GetContact(3)) return FX_OK;

	var rdir_desired = -rot / 5;
	var rdir_current = target->GetRDir();
	target->SetRDir(rdir_current + BoundBy(rdir_desired - rdir_current, -5, 5));
}

/*-- Contents --*/


protected func RejectCollect(id object_id, object obj)
{
	// objects that cannot be collected right now
	if (GetEffect(LORRY_Effect_BlockCollection, obj)) return true;

	// objects can still be collected
	if (ContentsCount() < MaxContentsCount)
	{
		DoClonk();
		return false;
	}
	
	// the object cannot be collected, notify carrier?
	if (obj->Contained())
		Message("$TxtLorryisfull$");
	else
	{
		// if not carried, objects slide over the lorry
		if (Abs(obj->GetXDir()) > 5)
		{
			obj->SetYDir(-2);
			obj->SetRDir(0);
			DoSoftHit();
		}
	}
	// reject collection
	return true;
}

// Automatic unloading in buildings.
protected func Entrance(object container)
{
	// Only in buildings
	if (container->GetCategory() & (C4D_StaticBack | C4D_Structure))
		// Not if the building prohibits this action.
		if (!container->~NoLorryEjection(this))
			// Empty lorry.
			container->GrabContents(this);
}

func DoClonk()
{
	Sound("Hits::Clonk");
}

func DoSoftHit()
{
	Sound("Hits::SoftHit?");
}


/*
func Collection2(object item)
{
	if (item->GetID() == Wipf) DisplayWipfs();
	return _inherited(...);
}


func Ejection(object item)
{
	if (item->GetID() == Wipf) DisplayWipfs();
	return _inherited(...);
}


func DisplayWipfs()
{
	var layers = GetUnusedOverlayID(1);
	for (var i = 0; i < layers; ++i)
	{
		SetGraphics(nil, GetID(), i);
	}
	
	var contents = FindObjects(Find_ID(Wipf), Find_Container(this), Find_OCF(OCF_Alive));
	var points = [[-7, -11], [0, -11], [7, -11], [-3, -14], [+2, -14]];
	var max_wipfs = Min(5, GetLength(contents));
	var wipfs = 0;
	for (; wipfs < max_wipfs; wipfs++)
	{
		var wipf = contents[wipfs];
		var layer = max_wipfs - wipfs;
		if (wipf->GetAction() != "Stand") wipf->SetAction("Stand");
		SetGraphics(nil, nil, layer, GFXOV_MODE_Object, nil, nil, wipf);
		SetObjDrawTransform(1000, 0, 1000 * points[wipfs][0], 0, 1000, 1000 * points[wipfs][1], layer);
	}

	SetGraphics(nil, GetID(), wipfs + 1, GFXOV_MODE_ExtraGraphics);
	SetObjDrawTransform(1000, 0, 0, 0, 1000, 0, wipfs + 1);
}
*/


local ActMap = {
		Drive = {
			Prototype = Action,
			Name = "Drive",
			Procedure = DFA_NONE,
			Directions = 2,
			FlipDir = 1,
			Length = 20,
			Delay = 2,
			X = 0,
			Y = 0,
			Wdt = 22,
			Hgt = 16,
			NextAction = "Drive",
		},
};

func Definition(def)
{
	def.PictureTransformation = Trans_Mul(Trans_Rotate(-25, 1, 0, 0), Trans_Rotate(40, 0, 1, 0));
}

local Name = "$Name$";
local Description = "$Description$";
local Touchable = 1;
local Rebuy = true;
local BorderBound = 1;
local ContactCalls = true;
local MaxContentsCount = 50;
local Components = {Metal = 2};

