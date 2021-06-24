/**
 * Helper functions for vegetation placement
 */

// Author: Marky
global func PlaceVegetationForeground(object plant)
{
	if (!plant) return;
	
	// do not put in front of structures!
	if (GetLength(plant->FindObjects(plant->Find_AtPoint(), Find_Or( Find_Func("NoForegroundVegetation", plant), Find_Category(C4D_Structure))))) return;
	
	if (plant->~IsTree())
		plant.Plane = 999;
	else
		plant.Plane = 998;
		
}
