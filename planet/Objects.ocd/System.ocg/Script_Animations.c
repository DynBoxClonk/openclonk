/**
 Play actions as animations.
 
 @author Marky
 */
 
static const ANIMACTION_Effect_Name = "IntAnimAction";
 
/**
 Plays an animation with certains specifics of an action that is defined in
 the ActMap. This is useful if you want to play several "actions" at the same
 time.
 */
global func SetAnimAction(string name, int anim_slot)
{
	if (!this) FatalError("This function has to be called from object context");
	if (!name) FatalError("Expected an animation name, got nil");
	
	var previous = GetAnimAction(anim_slot);
	
	if (name == "Idle")
	{
		RemoveEffect(nil, nil, previous);
	}
	else
	{
		var action = GetActionFromActMap(name);
		RemoveEffect(nil, nil, previous);
		var effect = AddEffect(ANIMACTION_Effect_Name, this, 1, Min(action.Delay, 1), nil, nil, action, anim_slot);
		return effect;
	}
}


/**
 Gets the animation that is currently active in that animation slot.
 @par anim_slot the animation slot to search in
 @return proplist the effect that plays the animation, or nil
         if no animation is active.
 */
global func GetAnimAction(int anim_slot)
{
	if (!this) FatalError("This function has to be called from object context");
	
	for (var i = 0; i < GetEffectCount(ANIMACTION_Effect_Name, this); i++)
	{
		var effect = GetEffect(ANIMACTION_Effect_Name, this, i);
		
		if (effect && effect.anim_slot == anim_slot) return effect;
	}
	
	return nil;
}

/**
 Gets the proplist for an action with the specified name from an object.
 @par object the object, is optional in local calls.
 @return the action property, or nil if nothing was found.
 */
global func GetActionFromActMap(string name, proplist object)
{
	if (!object) object = this;
	if (!object) FatalError("Expected an object that is not nil");
	if (!object.ActMap) FatalError("The object does not have an ActMap");
	
	for (var prop in GetProperties(object.ActMap))
	{
		var action = object.ActMap[prop];
		
		if (action.Name == name) return action;
	}
	
	return nil;
}

global func FxIntAnimActionStart(object target, proplist effect, temp, proplist action, int anim_slot)
{
	if (temp) return;
	
	var animation = action.Animation;
	if (!animation) FatalError(Format("The specificied action %s is not based on an animation", action.Name));

	var duration = Max(1, action.Length * action.Delay);
	
	//Log("Playing animation %s in slot %d with duration %d", animation, anim_slot, duration);

	effect.action = action;
	effect.anim_slot = anim_slot;
	effect.anim_number = target->PlayAnimation(animation, anim_slot, Anim_Linear(0, 0, target->GetAnimationLength(animation), duration, ANIM_Remove), Anim_Const(1000));

	if (effect.action.StartCall)
	{
		target->Call(effect.action.StartCall);
	}
}

global func FxIntAnimActionTimer(object target, proplist effect, int time)
{
	if (effect.action.PhaseCall)
	{
		target->Call(effect.action.PhaseCall);
	}	

	if (time >= Max(effect.action.Length, 1) * Max(effect.action.Delay, 1)) return FX_Execute_Kill;
}

global func FxIntAnimActionStop(object target, proplist effect, int reason, temp)
{
	if (temp) return;
	
	if (target)
	{
		target->StopAnimation(effect.anim_number);
		
		if (effect.action.EndCall)
		{
			target->Call(effect.action.EndCall);
		}
		if (effect.action.NextAction && effect.action.NextAction != "Idle")
		{
			target->SetAnimAction(effect.action.NextAction, effect.anim_slot);
		}
	}
}

