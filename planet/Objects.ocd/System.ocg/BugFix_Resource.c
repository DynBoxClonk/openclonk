#appendto Goal_ResourceExtraction

/** Gets the number of objects that can be exploited from a material, based on material count. */
func ExploitableObjectCount(int material_count, int material)
{
	var ratio = GetBlastRatio(material);
	if (ratio == 0)
	{
		return 0;
	}
	else
	{
		return material_count / ratio;
	}
}
