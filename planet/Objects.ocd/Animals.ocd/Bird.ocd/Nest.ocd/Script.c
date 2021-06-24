/*
 * Nest
 *
 * Author: Marky, original found in Clonk Rage
 */

local tree;

public func IsNest(){ return true;}

/* -- Engine callbacks -- */

public func Initialize()
{
	SetAction("Be");
	CreateEffect(CheckTree, 1, 10);
}


local CheckTree = new Effect
{
	Timer = func()
	{
		if (Target.tree && (Target.tree->OnFire() || !Target.tree->IsStanding()))
		{
			Target->DestroyNest();
			return FX_Execute_Kill;
		}
	},
};


public func Incineration()
{
	DestroyNest();
}


private func Destruction()
{
	DropContents();
}



public func Collection2(object pObj)
{
	UpdateGraphics(pObj);
}


public func Ejection(object pObj)
{
	UpdateGraphics(pObj);
}


public func Hit()
{
	if (IsDecaying())
	{
		BurstNest();
	}
	else
	{
		DestroyNest();
	}
}


// Burst on damage > 10, or immediately if already decaying
// Remove object after con < 30 from fire damage.
public func Damage(int change, int type)
{
	if (type == FX_Call_DmgFire)
	{
		if (GetCon() < 30)
		{
			CastPXS("Ashes", 20, 15);
			RemoveObject();
		}
	}
	else if (change > 5 || IsDecaying())
	{
		BurstNest();
	}
	else if (GetDamage() > 10)
	{
		DestroyNest();
	}
}

/* -- Internals -- */

private func DestroyNest()
{
	  SetAction("Decay");
	  SetCategory(C4D_Vehicle);
	  DropContents();
}


// Create straw particles and remove object when burst.
private func BurstNest()
{
	var radius = GetCon() * GetDefWidth() / 200;
	CreateParticle("Straw", 0, 0, PV_Random(-radius, radius), PV_Random(-radius, radius), PV_Random(30, 120), {Prototype = Particles_Straw(), R = 255, G = 206, B = 110}, GetCon());
	return RemoveObject();
}



public func UpdateGraphics(object pObj)
{
	if (GetType(this.contentList) != C4V_Array)
		this.contentList = CreateArray();

	if (pObj != nil)
	{
		var info = {object = nil, posX = nil, posY = nil,};
		info.object = pObj;
		info.posX = RandomX(GetDefOffset()-pObj->GetDefOffset(), GetID()->GetDefWidth()+GetDefOffset()-(pObj->GetDefWidth()+ pObj->GetDefOffset()));
		info.posY = GetID()->GetDefHeight()+GetDefOffset(1)-(pObj->GetDefHeight()+ pObj->GetDefOffset(1)) -3 + RandomX(-1, 1);

		var addInfo = true;
		for (var savedInfo in this.contentList)
		{
			if (GetType(savedInfo) != C4V_PropList) continue;
			if (savedInfo.object == pObj) addInfo = false;
		}

		if (addInfo) PushBack(this.contentList, info);
	}

	var layer = GetUnusedOverlayID(1);

	for (var i = 1; i < layer; i++)
	{
		SetGraphics(nil, nil, i, GFXOV_MODE_Base);
	}

	for (var savedInfo in this.contentList)
	{
		if (savedInfo.object == nil) continue;
		if (savedInfo.object->Contained() != this) continue;

		layer = GetUnusedOverlayID(1);
		SetGraphics(nil, nil, layer, GFXOV_MODE_Object, nil, 0, savedInfo.object);

		SetObjDrawTransform(1000, 0, 1000*savedInfo.posX, 0, 1000, 1000 * savedInfo.posY, layer);
	}

	SetGraphics(nil, GetID(), layer+1, GFXOV_MODE_ExtraGraphics);
}


private func Remove()
{
	RemoveObject();
}

private func DropContents()
{
	if (this.contentList)
	for (var savedInfo in this.contentList)
	{
		if (savedInfo.object->Contained() != this) continue;
		savedInfo.object->Exit(savedInfo.posX, savedInfo.posY);
	}
}

private func IsDecaying()
{
	return GetAction() == "Decay";
}

local ActMap = {

Be = {
		Prototype = "Action",
		Name = "Be",
		Procedure = DFA_FLOAT,
		Directions = 1,
		Length = 1,
		Delay = 10,
		X = 0, Y = 0, Wdt = 15, Hgt = 9,
		NextAction = "Be",
		InLiquidAction = "Decay",
},

Decay = {
		Prototype = "Action",
		Name = "Decay",
		Procedure = DFA_NONE,
		Directions = 1,
		Length = 5,
		Delay = 350,
		X = 0, Y = 0, Wdt = 15, Hgt = 9,
		NextAction = "Decay",
		EndCall = "Remove",
},
};

local Name = "$Name$";
local Description = "$Description$";
local ContactIncinerate = 1;
local BlastIncinerate = 10;
local Components = {Wood = 1};
