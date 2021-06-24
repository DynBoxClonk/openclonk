/**
 *  Functionality for string-based events that objects can react to by subscribing a listener to an event.
 *  
 *  Author: Marky
 */

/**
 * Fires an event and notifies the registered listeners in an object.
 * 
 * Parameters:
 * - event		an event name. The listeners have to be registered to that specific event name.
 * - target		the object that listens to the event. If the object is nil, then only global listeners will listen. 
 */
global func FireEvent(string event, object target)
{
	var i = GetEffectCount("EventListener", target, nil); // get all listeners
	
	var listener;
	while (i--)
		if (listener = GetEffect("EventListener", target, i))
		if (listener.eventName == event)
			EffectCall(target, listener, "OnEvent", this);
}

/**
 * Fires an event and notifies the registered listeners in all objects with event listeners.
 * 
 * Parameters:
 * - event		an event name. The listeners have to be registered to that specific event name.
 */
global func FireEventGlobal(string event)
{
	// fire events in all objects that have a listener
	var objects = FindObjects(Find_Func("HasEventListener"));
	
	var target;
	for (target in objects)
	{
		if (target) FireEvent(event, target);
	}
	
	// now all global effects;
	FireEvent(event, nil);
}

/**
 * Returns whether an object is registered to a certain event.
 * 
 * Parameters:
 * - event		an event name. The listeners have to be registered to that specific event name.
 * - target		the object that listens to the event. If the object is nil, then only global listeners will be checked. 
 * 
 * Returns:
 * true, if there exists a listener in the target.
 */
global func HasEventListener(string event, object target)
{
	if (this != nil && target == nil)
		target = this;
	
	var i = GetEffectCount("EventListener", target, nil); // get all listeners
	
	if (event == nil && i > 0)
		return true;
	
	var listener;
	while (i--)
		if (listener = GetEffect("EventListener", target, i))
			if (listener.eventName == event)
				return true;
	
	return false;
}

/**
 * Registers a listener to an event in an object.
 * 
 * Parameters:
 * - target		the object that listens to the event. If the object is nil, then only global listeners will listen.
 * - events		pass as many events as you like.
 * 				  Format for the parameters is an array of strings: [eventName, functionCall],
 * 				  or a string: eventName
 * 				  
 *              eventName specifies the event to listen to.
 *              functionCall specifies the function that will be called in the object once the event fires with the parameters (source, listener).
 *              if no functionCall is specified, then the event calls the function OnEvent(source, listener) in the target;
 *              
 *              OnEvent(source, listener)
 */
global func AddEventListener(object target, ev1, ev2, ev3, ev4, ev5, ev6, ev7, ev8, ev9)
{
	if (this != nil && target == nil)
		target = this;
	
	var events = [ev1, ev2, ev3, ev4, ev5, ev6, ev7, ev8, ev9];
	for (var event in events)
	{
		var name = nil, functionCall = nil;
		
		if (GetType(event) == C4V_Array)
		{
			name = event[0];
			functionCall = event[1];
		}
		else if (GetType(event) == C4V_String)
		{
			name = event;
		}
		else
		{
			continue;
		}

	    if (HasEventListener(name, target)) continue;

		var listener = AddEffect("EventListener", target, 1, 0, target);
		
		listener.eventName = name;
		listener.functionCall = functionCall;
	}
}

global func FxEventListenerOnEvent(object target, proplist effect, object source)
{
	var function = effect.functionCall;
	
	if (target)
	{
		if (function)
			target->Call(function, source);
		else
			target->Call("OnEvent", source, effect);
	}
	else
	{
		if (function)
			Call(function, source, effect);
		else
			Call("OnEvent", source, effect);
	}
}
