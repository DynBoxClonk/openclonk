/*
 * Material chunks can be dug out, but only on request!
 * 
 * Emulates the old CR behaviour :)
 *
 * Author: Marky
 */

#appendto Earth
#appendto Sand
#appendto Snow

protected func Construction()
{
	ScheduleCall(this, this.IgnoreDigRemove, 5);
	_inherited(...);
}

protected func IgnoreDigRemove()
{
	this.ignoreRemoveByDigging = true;
}

public func IsRemoveableMaterialChunk()
{
	return !this.ignoreRemoveByDigging; // nil also...
}
