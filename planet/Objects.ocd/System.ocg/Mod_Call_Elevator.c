#appendto Clonk

/** Returns additional interactions the clonk possesses as an array of function pointers.
	Returned Proplist contains:
		Fn			= Name of the function to call
		Object		= Object to call the function in. Will also be displayed on the interaction-button
		Description	= A description of what the interaction does
		IconID		= ID of the definition that contains the icon (like GetInteractionMetaInfo)
		IconName	= Name of the graphic for the icon (like GetInteractionMetaInfo)
		Priority	= Where to sort in in the interaction-list. 0=front, 10=after script, 20=after vehicles, 30=after structures, nil means no preverence
*/
public func GetExtraInteractions()
{
	var functions = _inherited(...) ?? [];
	
	// call elevator cases
	var elevators = FindObjects(Find_ID(ClassicElevatorCase), Find_InRect(-ELEVATOR_CALL_DISTANCE, AbsY(0), ELEVATOR_CALL_DISTANCE * 2, GetY() + AbsY(LandscapeHeight())), Find_Func("Ready", this));
	for (var elevator in elevators)
		PushBack(functions, { Fn = "CallCase", Object=elevator, Description=elevator->GetCallDescription(), Priority=0 });
	return functions;
}
