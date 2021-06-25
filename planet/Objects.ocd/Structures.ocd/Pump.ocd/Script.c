/*
 * Pump
 *
 * Author: Marky
 */

#include Pump
#include Basement26

protected func Definition(def) 
{
	if (def == ClassicPump)
	{
		def.PictureTransformation = Trans_Mul(Trans_Rotate(-20, 0, 1, 0), Trans_Rotate(10, 0, 0, 1), Trans_Rotate(-10, 1, 0, 0));
		def.MeshTransformation = Trans_Translate(1000, 4000, 0);
	}
}

public func IsHammerBuildable() { return true; }
func NoConstructionFlip(){ return true; } // not supported

func Initialize()
{
	this.MeshTransformation = ClassicPump.MeshTransformation; // somehow it always took that of the pump
	_inherited(...);
}


/*
// interface for the extraction logic
func ExtractMaterialFromSource(object source_obj, int amount)
{
	var extracted = 0;
	var barrels = GetBarrelsIn(source_obj);
	var material_name = nil;
	
	for (var barrel in barrels)
	{
		if (!material_name) material_name = barrel->GetBarrelMaterial();

		var diff = barrel->GetLiquid(material_name, amount, this);
		amount -= diff;
		extracted += diff;

		if (!amount) break;
	}

	if (material_name)
	{
		return [material_name, extracted];
	}
	else
	{
		return source_obj->ExtractLiquidAmount(source_obj.ApertureOffsetX, source_obj.ApertureOffsetY, amount, true);
	}
}

// interface for the insertion logic
func InsertMaterialAtDrain(object drain_obj, int material_index, int amount)
{
	var material_name = MaterialName(material_index);
	var barrels = GetBarrelsIn(drain_obj, material_name);
	var inserted = 0;
	
	for (var barrel in barrels)
	{
		var diff = barrel->PutLiquid(material_name, amount, this);
		amount -= diff;
		inserted += diff;
	}

	if (amount)
	{
		inserted += drain_obj->InsertMaterial(material_index, drain_obj.ApertureOffsetX, drain_obj.ApertureOffsetY);
	}
	
	return inserted;
}

func GetBarrelsIn(object target, material_name)
{
	var barrels;
	var container;

	if (target->~IsContainer()) // the drain object is a container and contains barrels? this is usually the case if the pump is the drain
	{
		container = target;
	}
	else if (target->Contained()) // the drain object is contained, this is the case if the pipe is in a building
	{
		container = target->Contained();
	}
	// missing case: the drain object can store liquids without barrels
	
	if (container == nil || container->~QueryPumpMaterial(material_name))
	{
		barrels = [];
	}
	else
	{
		barrels = FindObjects(Find_Func("IsBarrel"), Find_Container(container));
	}
	return barrels;
}
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
local BlastIncinerate = 50;
local LineAttach = [-10, 10];
local Components = {Wooed = 3, Metal = 1};
