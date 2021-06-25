/*
 * Additions to the array scripts
 *
 * Author: Marky
 */

#appendto ConstructionSite

local existing_site;

public func Set(id def, int dir, object stick, object existing_object)
{
	_inherited(def, dir, stick);

	existing_site = existing_object;
	
	if (existing_object != nil)
	{
		// create existing contents
		var i, cid, num;
		while (cid = existing_object->GetComponent(nil, i++))
		if ((num = existing_object->GetComponent(cid)) > 0)
			while (num--) CreateContents(cid);
			
		var dummy = CreateObject(Dummy, 0, 0, NO_OWNER);
		existing_object->Enter(dummy);
		//existing_site->SetGraphics();
		
		SetGraphics(nil, nil, 3, GFXOV_MODE_Object, nil, nil, existing_object);
		
		var y_offset = existing_object->GetDefBottom() - existing_object->GetY();
		
		SetObjDrawTransform(1000, 0, 0, 0, 1000, -1000 * y_offset, 3);
	}
	
	SetClrModulation(RGBa(255, 255, 255, 10), 2);
	
	ShowMissingComponents();
}
