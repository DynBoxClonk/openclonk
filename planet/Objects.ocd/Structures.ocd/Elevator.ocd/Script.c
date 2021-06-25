/*-- Elevator --*/

#include Library_Structure
#include Library_Ownable
#include BasementElevator


func NoConstructionFlip(){ return true; } // not supported

// used in the elevator case
//static const Elevator_needed_power = 50;

local case, rope, dummy;

// Frees a rectangle for the case
func CreateShaft(int length)
{
	// Move the case out of the way
	case->SetPosition(case->GetX(), GetY()-10);
	ClearFreeRect(GetX()-12, GetY()+25, 24, length + 13);
	// Move the case back
	case->SetPosition(case->GetX(), GetY()+20);
}

func SetCasePosition(int y)
{
	// Move case to specified absolute y position
	if (case) return case->SetPosition(case->GetX(), y);
	return false;
}

/* Initialization */

func Construction(object creator)
{
	SetAction("Default");
	return _inherited(creator, ...);
}

func Initialize()
{
	SetCategory(C4D_StaticBack);
	CreateCase();
	CreateRope();
	CreateDummy();

	return _inherited(...);
}

func CreateCase()
{
	case = CreateObject(ClassicElevatorCase, 0, 10, GetOwner());
	case->Connect(this);
	case.Plane = 450;
}

func CreateRope()
{
	rope = CreateObject(ElevatorRope, 0, -18, GetOwner());
	rope->SetAction("Be", case);
}

func CreateDummy()
{
	dummy = CreateObject(Dummy, 0, 0, NO_OWNER);
	dummy->SetPosition(GetX(), GetY());
	dummy->SetGraphics("TopFace", GetID());
	dummy.Visibility = VIS_All;
	dummy.Plane = 500;
}

/* Scenario saving */

func SaveScenarioObject(props)
{
	if (!inherited(props, ...)) return false;
	props->Remove("Category");
//	if (partner && slave)
//	{
//		props->AddCall("Friends", partner, "LetsBecomeFriends", this);
//	}
	if (case && case->GetY() > GetY() + 20)
	{
		props->AddCall("Shaft", this, "CreateShaft", case->GetY() - GetY() - 20);
		props->AddCall("Shaft", this, "SetCasePosition", case->GetY());
	}
	return true;
}

/* Destruction */

func Destruction()
{
	if (rope) rope->RemoveObject();
	if (case) case->LostElevator();
//	if (partner) partner->LoseCombination();
	if (dummy) dummy->RemoveObject();
}

func LostCase()
{
//	if (partner) partner->LoseCombination();
	if (rope) rope->RemoveObject();

	// for now: the elevator dies, too
	Incinerate();
}

/* Effects */

func StartEngine()
{
	Sound("Structures::ElevatorStart", nil, nil, nil, nil, 100);
	ScheduleCall(this, "EngineLoop", 34);
	//Sound("ElevatorMoving", nil, nil, nil, 1);
}
func EngineLoop()
{
	Sound("Structures::ElevatorMoving", nil, nil, nil, 1, 100);
}
func StopEngine()
{
	Sound("Structures::ElevatorMoving", nil, nil, nil, -1);
	ClearScheduleCall(this, "EngineLoop");
	Sound("Structures::ElevatorStop", nil, nil, nil, nil, 100);
}

/* Construction */

// Forward config to case
func SetNoPowerNeed(bool to_val)
{
	if (case) return case->SetNoPowerNeed(to_val);
	return false;
}

local ActMap = {
		Default = {
			Prototype = Action,
			Name = "Default",
			Procedure = DFA_NONE,
			Directions = 2,
			FlipDir = 1,
			Length = 1,
			Delay = 0,
			FacetBase = 1,
			NextAction = "Default",
		},
};

func Definition(def) {
	SetProperty("PictureTransformation", Trans_Mul(Trans_Rotate(-20, 1, 0), Trans_Rotate(-20, 0, 1, 0)));
}

public func IsHammerBuildable() { return true; }

local Name = "$Name$";
local Description = "$Description$";
local BlastIncinerate = 100;
local HitPoints = 70;
local Plane = 249;
local Components = {Wood = 3, Metal = 1};
