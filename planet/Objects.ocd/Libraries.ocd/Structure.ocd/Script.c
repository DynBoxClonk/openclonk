/**
	Structure Library
	Basic library for structures, handles:
	* Damage
	* Energy bar if rule active
	
	@author Marky
*/

local IsDeytroyed = false;
local IsDisabled = false;

static const STRUCTURE_Destruction_Con = 30;


public func Damage(int change, int cause, int cause_player)
{
	// Only do stuff if the object has the HitPoints property.
	if (this)
	{
		if (this.HitPoints != nil && GetDamage() >= this.HitPoints)
		{		
			StructureDestroyed();
		}
		
		if (this.BlastIncinerate != nil && GetDamage() >= this.BlastIncinerate
		&& (cause & FX_Call_DmgBlast || cause & FX_Call_EngBlast))
		{
			StructureIncinerate();
		}
	}
	return _inherited(change, cause, cause_player);
}

// This object is a structure.
public func IsStructure() { return true; }

protected func StructureIncinerate()
{
	if (!OnFire()) Incinerate();
}

protected func StructureDestroyed()
{
	if (!this.IsDestroyed)
	{
		this.IsDestroyed = true;
		this->~OnStructureDestroyed();
		this->~OnStructureDestroyedFx();
	}
}

protected func StructureDisable()
{
	this.IsDisabled = true;
	SetObjectLayer(this);
}

protected func StructureEnable()
{
	this.IsDisabled = false;
	SetObjectLayer(nil);
}

protected func OnStructureDestroyed()
{
	if (GetMeshMaterial() == nil)
	{
		SetGraphics("Destroyed");
	}
	else
	{
		var mat;
		for (var i = 0; (mat = GetMeshMaterial(i)) != nil; i++)
		{
			SetMeshMaterial(Format("%s_Destroyed", mat), i);
		}
	}
	
	if (!GetEffect("IntDestructionDecay", this))
	{
		AddEffect("IntDestructionDecay", this, 1, 1, this);
	}

	if (!GetEffect("IntCheckReconstruct", this))
	{
		AddEffect("IntCheckReconstruct", this, 1, 1, this);
	}
}

protected func Incineration(int by_player)
{
	StructureDestroyed();
	
	if (!GetEffect("IntStopBurnDecay", this))
	{
		AddEffect("IntStopBurnDecay", this, 1, 1, this);
	}
	
	return _inherited(by_player, ...);
}

protected func FxIntStopBurnDecayTimer(object target, proplist effect, int timer)
{
	if (!(target->OnFire()))
	{
		return FX_Execute_Kill;
	}
	
	if (timer < 76)
	{
		var rgba = target->GetClrModulation();
		rgba = ColorSetLightness(rgba, BoundBy(ColorGetLightness(rgba) - 1, 0, 255));
		target->SetClrModulation(rgba);
	}

	if (target->GetCon() <= STRUCTURE_Destruction_Con)
	{
		target->Extinguish();
		return FX_Execute_Kill;
	}
	
	return FX_OK;
}

protected func FxIntCheckReconstructTimer(object target, proplist effect, int timer)
{
	if (target->OnFire() || GetEffect("IntDestructionDecay", target) != nil)
	{
		return FX_OK;
	}
	else
	{
		target->StructureReconstruct();
		return FX_Execute_Kill;
	}
}

protected func FxIntDestructionDecayTimer(object target, proplist effect, int timer)
{
	if (target->OnFire())
	{
		return FX_Execute_Kill;
	}

	for (var i = 0; i < 5; i++)
	{
		if (target->GetCon() <= STRUCTURE_Destruction_Con)
		{
			return FX_Execute_Kill;
		}
		
		target->DoCon(-1);
	}
	
	return FX_OK;
}

protected func StructureReconstruct()
{
	if (Contained() != nil) return;
	
	var site;
	site = CreateObjectAbove(ConstructionSite, 0, GetDefBottom()-GetY(), GetOwner());
	
	// Randomize sign rotation
	site->SetProperty("MeshTransformation", Trans_Mul(Trans_Rotate(RandomX(-30, 30), 0, 1, 0), Trans_Rotate(RandomX(-10, 10), 1, 0, 0)));
	site->PlayAnimation("LeftToRight", 1, Anim_Const(RandomX(0, GetAnimationLength("LeftToRight"))), Anim_Const(500));
	
	site->Set(GetID(), 0, nil, this);
	site->SetPosition(GetX(), GetDefBottom() - (site->GetDefBottom() - site->GetY()) - 2);
}