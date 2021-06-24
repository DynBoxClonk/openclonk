/*-- Castle --*/


#include Library_Structure
#include Library_Ownable
#include Library_HomeBase
#include Library_DoorControl

#include Basement80

static const CASTLE_Arrow_Speed = 80; // very slow, should do less damage
static const CASTLE_Arrow_Count = 5;  // one archer has 5 arrows at most
static const CASTLE_Windows = [[-26, -6], [-16, -6], [-32, 20], [-20, 20]];

local topface;
local defenders;

protected func Definition(def) 
{
	_inherited(def, ...);
	if (def == ClassicCastle)
	{
	    def.PictureTransformation = Trans_Mul(Trans_Rotate(-20, 0, 1, 0), Trans_Rotate(-15, 1, 0, 0), Trans_Rotate(7, 0, 0, 1), Trans_Translate(10000, -5000, 30000), Trans_Scale(900, 900, 450));
	}
}

func NoConstructionFlip(){ return true; } // not supported

protected func Construction()
{
	_inherited(...);
    defenders = [];
}

protected func Initialize()
{
	topface = CreateObject(Dummy, 0, 0, NO_OWNER);
	topface->SetPosition(GetX(), GetY());
	topface->SetGraphics("TopFace", GetID());
	topface.Visibility = VIS_All;
	topface.Plane = 500;
}

func Destruction()
{
	if (topface) topface->RemoveObject();
	_inherited(...);
}


func IsContainer(){ return true; }

  
/* Door control */

private func SoundOpenDoor()
{
	Sound("Structures::DoorOpen");
}
  
private func SoundCloseDoor()
{
	Sound("Structures::DoorClose");
}

private func SoundOpenGate()
{
	Sound("Structures::GateOpen");
}
  
private func SoundCloseGate()
{
	Sound("Structures::GateClose");
}


/*-- Defense script --*/

func Collection2(object crew)
{
	if (crew->~IsClonk())
	{
		AddCastleDefenseAI(crew);		
	}
	
	_inherited(crew);
}

func AddCastleDefenseAI(object crew)
{
	var effect = AddEffect("IntCastleDefense", crew, 1, 20, this);
	if (!effect) return;

	effect.castle = this;

	// save old ai
	if (crew.ai)
	{
		crew.ai_temp = crew.ai;
	}
	crew.ai = effect;
	
	// configure AI
	crew.ExecuteAI = this.DoNothing;
	effect.ai = ClassicCastle;
	effect.castle_window = CASTLE_Windows[Random(GetLength(CASTLE_Windows))];
	effect.ammo_count = CASTLE_Arrow_Count;
	effect.ammo_max = CASTLE_Arrow_Count;
	effect.ammo_gain_rate = 8; // gain ammo every 5 seconds
	
	var guard_range_x = 300;
	var guard_range_y = 200;
	AI->SetGuardRange(crew, GetX()-guard_range_x, GetY()-guard_range_y, guard_range_x*2, guard_range_y*2);
	AI->SetMaxAggroDistance(crew, AI.MaxAggroDistance);
	
}

func RemoveCastleDefenseAI(object crew)
{
	if (crew.ai_temp)
	{
		crew.ai = crew.ai_temp;
		crew.ai_temp = nil;
	}
	else
	{
		crew.ai = nil;
	}
}

func GetDefenderCount()
{
	var count = 0;
	for (var defender in defenders)
	{
		if (defender) ++count;
	}
	return count;
}


func AddDefender(object defender)
{
	// too many defenders make the castle overpowered
	if (GetDefenderCount() > 2) return;

	if (!IsValueInArray(defenders, defender))
	{
		PushBack(defenders, defender);
	}
}


func RemoveDefender(object defender)
{
	if (IsValueInArray(defenders, defender))
	{
		RemoveArrayValue(defenders, defender);
	}
}


func FxIntCastleDefenseTimer(object target, proplist ai, int time)
{
	if (!ai.castle || target->Contained() != ai.castle) return FX_Execute_Kill;

	ai.weapon = target->FindContents(Bow);

	if (ai.weapon)
	{
		ai.projectile_speed = CASTLE_Arrow_Speed;
		
		ai.castle->AddDefender(target);
		ExecuteDefend(target, ai, time);
	}
	else
	{
		ai.castle->RemoveDefender(target);
	}
	
	return FX_OK;
}


func FxIntCastleDefenseStop(object target, proplist ai, int reason)
{
	if (ai.castle) ai.castle->RemoveDefender(target);
}


func ExecuteDefend(object target, proplist ai, int time)
{
	ai.time = time;
	// Forget an enemy
	if (ai.target) if (!ai.target->GetAlive() || ObjectDistance(ai.target) >= ai.max_aggro_distance) ai.target = nil;
	
	ai.ammo_gain += 1;
	if (ai.ammo_gain > ai.ammo_gain_rate)
	{
		ai.ammo_count = BoundBy(ai.ammo_count + 1, 0, ai.ammo_max);
	}

	// do not do anything if you ran out of arrows
	if (ai.ammo_count <= 0) return;
	
	// Find an enemy
	if (!ai.target)
	{
		if (!(ai.target = FindTarget(ai))) return;
	}

	// delay, to avoid constant shooting
	if (ai.reload > 0)
	{
		ai.reload--;
		return;
	}

	// Attack it!
	return ExecuteRanged(ai);
}


private func FindTarget(ai)
{
	// Consider hostile clonks/animals, or all clonks if the AI does not have an owner.
	var hostile_criteria = Find_Hostile(GetOwner());
	var living = Find_Or(Find_OCF(OCF_CrewMember), Find_Func("IsPredator"));
	
	if (GetOwner() == NO_OWNER)
	{
		hostile_criteria = Find_Not(Find_Owner(GetOwner()));
		living = Find_OCF(OCF_CrewMember);
	}

	for (var target in FindObjects(Find_InRect(ai.guard_range.x-GetX(), ai.guard_range.y-GetY(), ai.guard_range.wdt, ai.guard_range.hgt), 
	                               living, // also finds hostile animals!
	                               hostile_criteria,
	                               Find_NoContainer(),
	                               Sort_Random()))	                             
			return target;

	// Nothing found.
	return;
}

func DoNothing(){}


private func ExecuteRanged(proplist ai)
{
	// Target still in guard range?
	if (!AI->CheckTargetInGuardRange(ai)) return false;
	// Target gone?
	if (ai.target->Contained())
	{
		ai.target = nil;
		return false;
	}

	// Calculate offset to target. Take movement into account
	// Also aim for the head (y-4) so it's harder to evade by jumping
	var x = GetX() + ai.castle_window[0];
	var y = GetY() + ai.castle_window[1];
	var tx = ai.target->GetX();
	var ty = ai.target->GetY()-4;

	var d = Distance(x, y, tx, ty);

	var dt = d * 10 / ai.projectile_speed; // projected travel time of the arrow

	tx += AI->GetTargetXDir(ai.target, dt);
	ty += AI->GetTargetYDir(ai.target, dt);

	if (!ai.target->GetContact(-1)) if (!ai.target->GetCategory() & C4D_StaticBack) ty += GetGravity()*dt*dt/200;

	// Get shooting angle
	var shooting_angle;
	if (ai.ranged_direct)
		shooting_angle = Angle(x, y, tx, ty, 10);
	else
	{
		shooting_angle = AI->GetBallisticAngle(tx-x, ty-y, ai.projectile_speed, 160);
	}

	if (GetType(shooting_angle) != C4V_Nil)
	{
		shooting_angle /= 10;

		// sort of ballistic line
		var half_dist = Distance(x, y, tx, ty) / 2;
		var hx = GetX() + Sin(shooting_angle, half_dist);
		var hy = GetY() - Cos(shooting_angle, half_dist);
	
		var ally = FindObject(Find_OnLine(0, 0, tx-x, ty-y), Find_Exclude(this), Find_OCF(OCF_Alive), Find_Owner(GetOwner()), Find_NoContainer());
		if (ally)
		{
			return false;
		}
		
		// Path to (near) target free? this may waste arrows, but attacks airships for example :)
		if (PathFree(x, y, hx, hy))
		{
				ai.ammo_count -= 1;
				if (ai.ammo_count)
					ai.reload = RandomX(2, 4); // take 3 to 6 seconds for reloading, so that multiple defenders are more attractive
				else
					ai.reload = 0; // no reloading, because ammo replenishes after some time only
	
				// Aim/Shoot there
				FireArrow(x, y, shooting_angle);
				return true;
		}
	}

	// Path not free or out of range. Just wait for enemy to come...
	var new_target;
	if (!Random(3)) if (new_target = FindTarget(ai)) ai.target = new_target;
	// go to another window
	ai.castle_window = CASTLE_Windows[Random(GetLength(CASTLE_Windows))];
	return true;
}


func UpdateDebugDisplay(){}



/*-- Firing arrows --*/

func FireArrow(int x, int y, int angle)
{
	var arrow = CreateObject(Arrow, x - GetX(), y - GetY());
	arrow->SetStackCount(1);
	arrow->Launch(angle, CASTLE_Arrow_Speed, this);
	arrow.Collectible = false; // this is a free arrow after all...
	AddEffect("IntArrowFade", arrow, 1, 1, nil, ClassicCastle);
	Sound("Objects::Weapons::Bow::Shoot?");
}

func FxIntArrowFadeTimer(object arrow, proplist effect, int time)
{
	if (!GetEffect("InFlight", arrow))
	{
		arrow->FadeOut(30, true);
		return FX_Execute_Kill;
	}
}


/*-- Defense script GUI --*/

public func HasInteractionMenu() { return true; }

public func GetInteractionMenus(object crew)
{
	var menus = _inherited(crew) ?? [];		

	var defense_menu =
	{
		title = "$CastleDefense$",
		entries_callback = this.GetCastleDefenseMenuEntries,
		callback = "OnCastleDefense",
		callback_hover = "OnCastleDefenseHover",
		callback_target = this,
		BackgroundColor = RGB(0, 50, 50),
		Priority = 20
	};

	PushBack(menus, defense_menu);
	return menus;
}


public func GetCastleDefenseMenuEntries(object crew)
{
	var menu_entries = [];

	// default design of a control menu item
	var custom_entry = 
	{
		Right = "100%", Bottom = "2em",
		BackgroundColor = {Std = 0, OnHover = 0x50ff0000},
		image = {Right = "2em"},
		text = {Left = "2em"}
	};
	
	// Add info message about how the defense mechanism works.
	PushBack(menu_entries, {symbol = this, extra_data = "description",
			custom =
			{
				Prototype = custom_entry,
				Bottom = "1.2em",
				Priority = -1,
				BackgroundColor = RGB(25, 100, 100),
				text = {Prototype = custom_entry.text, Text = ""},
				image = {Prototype = custom_entry.image, Symbol = Shield}
			}});
	
	// Add info message for every defender
	for (var defender in defenders)
	{
		if (defender == nil) continue;

		var defender_info = defender->GetName();
		PushBack(menu_entries,
		{
		    symbol = defender,
		    extra_data = Format("Object(%d)", defender->ObjectNumber()),
			custom = 
			{
				Prototype = custom_entry,
				Priority = 1,
				text = {Prototype = custom_entry.text, Text = defender_info},
				image = {Prototype = custom_entry.image, Symbol = defender},
			}
		});
	}

	return menu_entries;
}

public func OnCastleDefenseHover(id symbol, string action, desc_menu_target, menu_id)
{
	var text;
	if (action == "description")
	{
		text = "$DefenseInfo$";
	}
	else
	{
		var defender = eval(action);
		var health_percent = 100000 * defender->GetEnergy() / defender.MaxEnergy;
		var ammo = defender.ai.ammo_count;
		var max = defender.ai.ammo_max;
		
		text = Format("$DefenderInfo$", defender->GetName(), ammo, max, health_percent);
		
	}
	GuiUpdateText(text, menu_id, 1, desc_menu_target);
}

public func OnCastleDefense(id symbol, string action, bool alt)
{
	// do nothing on click, or maybe let the clonk play an idle animation :p
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
local BlastIncinerate = 160;
local ContainBlast = 1;
local Components = {Rock = 12, Wood = 5};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// actions

local ActMap = {
OpenDoor = {
	Prototype = Action,
	Name = "OpenDoor",
	Procedure = DFA_NONE,
	Length = 10,
	Delay = 1,
	NextAction = "DoorOpen",
	StartCall = "SoundOpenGate",
	Animation = "MainOpenDoor",
},

DoorOpen = {
	Prototype = Action,
	Name = "DoorOpen",
	Procedure = DFA_NONE,
	Length = 1,
	Delay = 40,
	NextAction = "CloseDoor",
	StartCall = "OpenEntrance",
	EndCall = "CloseEntrance",
	Animation = "MainDoorOpen",
},

CloseDoor = {
	Prototype = Action,
	Name = "CloseDoor",
	Procedure = DFA_NONE,
	Length = 10,
	Delay = 1,
	NextAction = "Idle",
	StartCall = "SoundCloseGate",
	Animation = "MainCloseDoor",
},

TowerOpenDoor = {
	Prototype = Action,
	Name = "OpenDoor",
	Procedure = DFA_NONE,
	Length = 10,
	Delay = 1,
	NextAction = "TowerDoorOpen",
	StartCall = "SoundOpenDoor",
	Animation = "TowerOpenDoor",
},

TowerDoorOpen = {
	Prototype = Action,
	Name = "Door",
	Procedure = DFA_NONE,
	Length = 1,
	Delay = 40,
	NextAction = "TowerCloseDoor",
	StartCall = "OpenEntrance",
	EndCall = "CloseEntrance",
	Animation = "TowerDoorOpen",
},

TowerCloseDoor = {
	Prototype = Action,
	Name = "CloseDoor",
	Procedure = DFA_NONE,
	Length = 10,
	Delay = 1,
	NextAction = "Idle",
	StartCall = "SoundCloseDoor",
	Animation = "TowerCloseDoor",
},

};
