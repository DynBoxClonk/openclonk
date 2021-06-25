
/**
 Does a function call with the given parameters on each
 value in an array.
 */
global func CallForEach(array every, function_name)
{
	var results = [];
	RemoveHoles(every);
	for (var each in every)
	{
		PushBack(results, each->Call(function_name, ...));
	}
	return results;
}


/**
 Gets a random number starting at 'initial' with 
 values differing by +/- 'range'
 */
global func RandomRange(int initial, int range)
{
	return RandomX(initial - range, initial + range);
}


/**
 Create contents if necessary.
 */
global func EnsureHasContents(id type, int amount)
{
	AssertObjectContext();
	amount = amount ?? 1;
	amount = Max(0, amount - ContentsCount(type));
	if (amount > 0)
	{
		CreateContents(type, amount);
	}
}
