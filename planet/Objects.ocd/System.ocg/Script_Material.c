
/**
 Conversion method for placing objects in earth, as in old Clonk Rage
 scenarios. This can work as a general indicator for the size of the map,
 though, and can be used for animals, vegetation, etc., too.
 */
global func ConvertInEarthAmount(int amount, int divisor)
{
	var materialAmount = GetMaterialCount(Material("Earth"));
	var factor = Max(1, materialAmount / 40000); // creates a factor of 10 most of the time
	return amount * factor / Max(divisor, 1);
}


/**
 Conversion method for placing objects in materials, as in old Clonk Rage
 scenarios.
 
 @par material This material will be converted to an amount. Defaults to "Earth".
 @par level This level will be used. Defaults to 100.
 */
global func ConvertInMatAmount(string material, int level)
{
	material = material ?? "Earth";
	level = BoundBy(level ?? 100, 1, 100);
	var total = GetMaterialCount(Material(material)) / 600;
	return total * level / 100;
}



/**
 Conversion method for placing objects in materials, as in old Clonk Rage
 scenarios.
 
 @par material This material will be converted to an amount. Defaults to "Earth".
 @par level This level will be used. Defaults to 100.
 */
global func ConvertInMapAmount(int level)
{
	level = BoundBy(level ?? 100, 1, 100);
	var total = (LandscapeWidth()/10) * (LandscapeHeight()/10) / 50; // map area / 5000
	return total * level / 100;
}
