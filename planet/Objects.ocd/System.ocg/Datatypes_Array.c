/*
 * Additions to the array scripts
 *
 * Author: Marky
 */


global func IsArray(parameter)
{
	return (GetType(parameter) == C4V_Array);
}
