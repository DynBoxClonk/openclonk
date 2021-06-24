/*
 * Overload of the contents menu to accomodate for the building part.
 * 
 * Author: Marky, original by Newton, Maikel
 */
/*
global func CreateContentsMenus()
{
	// Safety checks.
	if (!this) return;
	if (!(GetOCF() & OCF_CrewMember)) return;
	if (!(this->~HasMenuControl())) return;

	// Create the menu controller.
	var controller = CreateObject(GUI_Contents_Controller);
	controller->SetMenuObject(this);
	this->SetMenu(controller);
	
	var index = 0;
	
	// add the clonk itself
	controller->AddContentMenu(this, index, true);
	
	// add all nearby crewmembers
	var teammates = FindObjects(Find_Distance(MENU_Contents_MaxCrewDist), Find_OCF(OCF_CrewMember), Find_Owner(GetOwner()), Find_Exclude(this));
	index = 1;
	for (var t in teammates)
	if (Contained() == t->Contained()) // Marky: only for crew inside the same building, or both outside
		controller->AddContentMenu(t, index++, true);
		
	// Add content menus for all containers at the position of the caller.
	// Marky: structures are only containers if you are inside of them, so added parameter 'this' to IsContainer
	var containers = FindObjects(Find_AtPoint()/*, Find_NoContainer()*//*, Find_OCF(OCF_Fullcon), Find_Func("IsContainer", this), Sort_Func("SortInventoryObjs"));
	for (var c in containers)
	if (Contained() == c->Contained() || Contained() == c) // Marky: the same goes for containers inside buildings.
		controller->AddContentMenu(c, index++, false);
	
	// Show and return content menu.
	controller->Show();
	return controller;
}

global func GetStackedContents()
{
	if (!this) return nil;

	var contents = FindObjects(Find_Container(this), Find_Not(Find_OCF(OCF_CrewMember)));
	var stacked = [];

	// put all contents into stackcontents
	for (var content in contents)
	{
		// check if item of same ID already inside
		var has_stacked = false;
		for (var stackcontent in stacked)
		{
			if (!CanStackObjects(content, stackcontent[0])) continue;
			stackcontent[GetLength(stackcontent)] = content;
			has_stacked = true;
			break;
		}
		// otherwise, put new inside
		if (!has_stacked)
			stacked[GetLength(stacked)] = [content];
	}

	return stacked;
}
*/