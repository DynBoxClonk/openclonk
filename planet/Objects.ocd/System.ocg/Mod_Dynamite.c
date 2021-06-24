#appendto Dynamite

// the dynamite box creates "Dynamite" sticks
// this is the easiest way for conversion
public func Construction()
{
	_inherited(...);
	if (GetID() == Dynamite)
		ChangeDef(ClassicDynamite);
}
