/*
 * Minor changes in the clonk animations:
 * - can dig material chunks on request
 *
 * Author: Marky
 */


#appendto Clonk_Animations

// Removes diggable materials
func FxIntDigTimer(pTarget, effect, iTime)
{
	if (!effect.digMaterialRequest)
	{
		var chunks = FindObjects(Find_Func("IsRemoveableMaterialChunk"), Find_Distance(12));

		for (var obj in chunks) obj->RemoveObject();
	}
	
	return _inherited(pTarget, effect, iTime);
}

