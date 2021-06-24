/*
 * Bird
 *
 * With improved behavior.
 *
 * Author: Marky, original found in Clonk Rage
 */

#include Library_AnimalControl
#include Library_AnimalFighting
#include Library_AnimalShelter
#include Library_AnimalReproduction

private func AnimalReproductionRate(){    return 2000;}
public func AnimalInitialAction(){    return "Fly";}


/* Initialisierung */

private func FxIntAnimalTurnTimer(object target, proplist ai)
{
    if (target != this) return;

    AvoidHittingLandscape();

    // Flying backwards? Turn around!
    if (GetXDir()>0 && GetDir() == DIR_Left)  return AnimalTurnRight();
    if (GetXDir()<0 && GetDir() == DIR_Right) return AnimalTurnLeft();

}

private func FxIntAnimalActivityStatus( object target, proplist ai)
{
    if (target != this) return;

    ai.ignoreCalls = false;

    // Dead birds don't do much...
    if (!GetAlive())
    {
        ai.ignoreCalls = true;
        return;
    }

    // Make a sound :)
    if (!Random(25)) DoTwitter();

}

private func FxIntAnimalActivityMovement( object target, proplist ai)
{
    if (target != this) return;

    // let bird take further movement commands if he is close to his actual target,
    // but only if he has no second command (such as call to nest, or second move target)
    // this ensures smooth movement :)
    if (GetCommand() != "Wait" && GetCommand() != nil)
    {
        if (GetCommand() == "MoveTo" && GetCommand(0, 1) == nil)
        {
            if (Distance(GetX(), GetY(), GetCommand(2), GetCommand(3)) > 200)
                return;
        }
        else
        {
            return;
        }
    }

    if (GetAction() == "Sit")
    {
        if (Random(2))
            SetDir(DIR_Left);
        else
            SetDir(DIR_Right);

        return;
    }

    // Do nothing
    //if (Random(2) || GetAction() != "Fly") return;
    if (GetAction() != "Fly") return;

    var x = RandomX(20, LandscapeWidth()-20);
    var ref_y = GetFeasableHeight(x);

    if (ref_y == nil)
    {
            // Fly down or up
            if (Random(2))
                SetComDir(GetComDir()|COMD_Down);
            else
                SetComDir(GetComDir()|COMD_Up);

            return;
    }

    var y = RandomX(ref_y[0], ref_y[1]);
    AppendCommand("MoveTo", nil, x, y);

//
//    // Fly down or up
//    if (Random(2))
//        SetComDir(GetComDir()|COMD_Down);
//    else
//        SetComDir(GetComDir()|COMD_Up);
//
//    // Do nothing again
//    if (!Random(4)) return;
//
//    // Turn around
//    if (Random(2))
//        AnimalTurnRight();
//    else
//        AnimalTurnLeft();
}

private func FxIntAnimalActivityFight( object target, proplist ai)
{
    if (target != this) return;

}
private func FxIntAnimalActivityShelter( object target, proplist ai)
{
    if (target != this) return;

    // remove current shelter
    if (ai.shelter)
    {
          // if the nest is not hanging on a tree anymore
          // if the nest is flooded
          // if the nest is burning
          if (ai.shelter->GetAction() == "Decay"
           || GBackSemiSolid(ai.shelter->GetX()-GetX(), ai.shelter->GetY()-GetY())
           || ai.shelter->OnFire())
          {
                  ai.shelter = nil;
          }
    }

    // No nest? Build one!
    if (!ai.shelter)
    {
        var tree = BirdFindTree(ai);

         if (tree)
         {
             if (!GetEffect("IntBirdNestMarker", tree))
                   AddEffect("IntBirdNestMarker", tree, 1, 0, this);

            var nest_x = tree->GetX() + RandomX(tree->GetLeft() + 10, tree->GetRight() - 10);
            var nest_y = tree->GetY() + RandomX(tree->GetTop() + 10, tree->GetBottom() - tree.FireTop - 10);

             nest_x = BoundBy( nest_x, Abs(BirdNest->GetDefOffset()) + 5, LandscapeWidth() - BirdNest->GetDefWidth() - BirdNest->GetDefOffset());

            SetCommand( "Call", this, tree, nil, nil, "BirdStartBuildNest");
            AddCommand( "MoveTo", nil, nest_x, nest_y);
         }

        return;
    }
    else
    {
        if (ai.shelter->GetCon() < 100) BirdFlyToNest(ai); // builds the rest
    }

    if (IsNight()) // go to sleep
    {
        BirdFlyToNest(ai);

        if (Contained() && ai.shelter == Contained() && GetActTime() > 100 && GetAction() != "Sleep")
        {
            SetAction("Sleep");
        }
    }
    else // leave the nest!
    {
        if (Contained() && ai.shelter && Contained() == ai.shelter)
        {
            Exit();
        }
    }
}

private func AnimalCanTurn(){ return !Stuck() && GetAction() == "Fly"; }
private func AnimalOnTurnLeft()
{
    SetAction("Turn");
}
private func AnimalOnTurnRight()
{
    SetAction("Turn");
}

private func AnimalReproductionCondition()
{
    var ai = AnimalGetActivityEffect();
    if (!ai) return false;
    if (!ai.shelter) return false;
    if (Contained() != ai.shelter) return false;
    return true;
}

private func AnimalReproductionCustom()
{
    if (Random(AnimalReproductionRate())) return true;
    var ai = AnimalGetActivityEffect();
    if (!ai) return true;

    var egg = ai.shelter->CreateContents(BirdEgg);
    egg->Fertilize(GetID(), 2000);

    return true;
}

private func FlyTryAttack()
{
      // Only while flying
      if (GetAction() != "Fly") return;

      // Find a blimp or balloon
      if (!this.pokeTarget)
      {
          this.pokeTarget = FindObject(Find_Func("IsPokeableByBird", this));
      }
      if (!this.pokeTarget) return;

      // Poke it!
      this.pokeTarget->~GetPoked(this);
      SetAction("Attack");
      DoTwitter();
}

private func FlyAvoidDanger()
{
    // Avoid liquids and burning objects
    if (InLiquid() || GBackLiquid(GetXDir()*2, GetYDir()*2) || GBackLiquid(GetXDir()*3, GetYDir()*3) || FindObject(Find_InRect(-20 + GetXDir() * 2, -20 + GetYDir() * 2, 40, 40), Find_OCF(OCF_OnFire)))
        SetComDir(COMD_Up);
}


/* Hitting the landscape... */

protected func ContactLeft()
{
    return AnimalTurnRight();
}

protected func ContactRight()
{
    return AnimalTurnLeft();
}

protected func ContactTop()
{
    return SetComDir(COMD_Down);
}

protected func ContactBottom()
{
    return SetComDir(COMD_Up);
}

protected func Entrance()
{
    if (GetAlive()) SetAction("Sit");
}

protected func Departure()
{
    if (GetAlive()) SetAction("Fly");
}

protected func Death()
{
    ChangeDef(DeadBird);
    _inherited(...);
}

func DoTwitter()
{
    Sound("Animals::Bird::Raven?");
}

/* Animations */

local ActMap = {
Fly = {
    Prototype = Action,
    Name = "Fly",
    Procedure = DFA_FLOAT,
    Speed = 200,
    Accel = 16,
    Decel = 16,
    Directions = 2,
    FlipDir = 1,
    Length = 20,
    Delay = 1,
    X = 0,
    Y = 0,
    Wdt = 14,
    Hgt = 12,
    NextAction = "Fly",
    TurnAction = "Turn",
    StartCall = "FlyTryAttack",
    PhaseCall = "FlyAvoidDanger",
},

Turn = {
    Prototype = Action,
    Name = "Turn",
    Procedure = DFA_FLOAT,
    Speed = 100,
    Accel = 16,
    Decel = 32,
    Directions = 2,
    FlipDir = 1,
    Length = 20,
    Delay = 1,
    X = 0,
    Y = 12,
    Wdt = 14,
    Hgt = 12,
    NextAction = "Fly",
},

Attack = {
    Prototype = Action,
    Name = "Attack",
    Procedure = DFA_FLOAT,
    Directions = 2,
    FlipDir = 1,
    Length = 20,
    Delay = 1,
    X = 0,
    Y = 24,
    Wdt = 14,
    Hgt = 12,
    NextAction = "Fly",
},

Tumble = {
    Prototype = Action,
    Name = "Tumble",
    Procedure = DFA_NONE,
    Directions = 2,
    FlipDir = 1,
    Length = 20,
    Delay = 1,
    X = 0,
    Y = 36,
    Wdt = 14,
    Hgt = 12,
    NextAction = "Fly",
    ObjectDisabled = 1,
    InLiquidAction = "Fly",
},

Eat = {
    Prototype = Action,
    Name = "Eat",
    Procedure = DFA_FLOAT,
    Directions = 2,
    FlipDir = 1,
    Length = 20,
    Delay = 2,
    X = 0,
    Y = 24,
    Wdt = 14,
    Hgt = 12,
    NextAction = "Fly",
    //EndCall = "Eating",
},

BuildNest = {
    Prototype = Action,
    Name = "BuildNest",
    Procedure = DFA_FLOAT,
    Directions = 2,
    FlipDir = 1,
    Length = 20,
    Delay = 2,
    X = 0,
    Y = 24,
    Wdt = 14,
    Hgt = 12,
    NextAction = "BuildNest",
    PhaseCall = "BirdDoBuildNest",
},

Sit = {
    Prototype = Action,
    Name = "Sit",
    Procedure = DFA_FLOAT,
    Directions = 2,
    FlipDir = 1,
    Length = 1,
    Delay = 40,
    X = 96,
    Y = 48,
    Wdt = 14,
    Hgt = 12,
    OffY = 2,
    NextAction = "Sit",
},

Sleep = {
    Prototype = Action,
    Name = "Sleep",
    Procedure = DFA_FLOAT,
    Directions = 2,
    FlipDir = 1,
    Length = 1,
    Delay = 40,
    X = 96,
    Y = 48,
    Wdt = 14,
    Hgt = 12,
    OffY = 3,
    NextAction = "Sleep",
},
};

local Name = "$Name$";
local Description = "$Description$";
local MaxEnergy = 40000;
local MaxBreath = 720; // Bird can breathe for 20 seconds under water.
local NoBurnDecay = 1;
local ContactIncinerate = 1;
local ContactCalls = true;
local Collectible = true;
local BorderBound = 7;



private func BirdFindTree(proplist ai)
{
    if (ai.tree)
    {
        if (!(ai.tree->IsStanding()))
            ai.tree = nil;
        else
            return ai.tree;
    }

    var goodTrees = CreateArray();
    var distance = 500;

    var obj, tree;
    // Find a suitable tree
    for (obj in FindObjects(Find_Func("IsTree"), Find_Distance(distance),
               Find_Not(Find_OCF(OCF_OnFire)), Find_Func("IsStanding")))
    {
        if (obj->GetCon() < 100) continue;
        // not flooded
        if (GBackSemiSolid(obj->GetX()-GetX(), obj->GetY()-GetY())) continue;
        if (FindObject(Find_ID(BirdNest), Find_ActionTarget(obj))) continue;
        if (GetEffect("IntBirdNestMarker", obj)) continue;

        PushBack(goodTrees, obj);
    }

    var target;
    for (target in goodTrees)
    {
        var dist = ObjectDistance(target);
        if (dist < distance)
        {
            tree = target;
            distance = dist;
        }
    }
    // save for future use
    ai.tree = target;
    return target;
}


public func BirdStartBuildNest(self, tree)
{
      var ai = AnimalGetActivityEffect();
      if (!tree || ai == nil) return;
      if (tree->GetX()<GetX())
          SetDir(DIR_Left);
      else
          SetDir(DIR_Right);

      ai.shelter = CreateConstruction(BirdNest, (-GetID()->GetDefWidth()+2*GetID()->GetDefWidth()*GetDir())/2, +5, -1, 30);

      ai.shelter.tree = tree;
      ai.shelter->SetAction("Be", tree);

      if (GetEffect("IntBirdNestMarker", tree))
          RemoveEffect("IntBirdNestMarker", tree);
}

protected func BirdPrepareBuildNest()
{
      SetAction("BuildNest");
      SetComDir(COMD_Stop);
      SetXDir(0);
      SetYDir(0);
}

protected func BirdDoBuildNest()
{
      var ai = AnimalGetActivityEffect();
      if (!ai || !ai.shelter || ai.shelter->OnFire() || ObjectDistance(ai.shelter) > GetID()->GetDefWidth())
      { SetAction("Fly");  return; }

      if (GetPhase()%2) return;

      ai.shelter->DoCon(1);
      if (ai.shelter->GetCon()>=100)
      {
               SetAction("Fly");
               BirdFlyToNest(ai);
      }
}



private func BirdFlyToNest(proplist ai)
{
    if (!ai.shelter) return;

    if (ai.shelter->GetCon() < 100)
    {
        // continue building the nest
        SetCommand("Call", this, 0, 0, nil, "BirdPrepareBuildNest");
        AddCommand("MoveTo", ai.shelter);
    }
    else
    {
        // Fly to the nest and enter it
        SetCommand("Call", this, 0, 0, nil, "BirdEnterNest");
        AddCommand("MoveTo", ai.shelter);
    }
}


protected func BirdEnterNest()
{
      var ai = AnimalGetActivityEffect();
      if (!ai || !ai.shelter) return;

      ForceEnter(ai.shelter);
}


public func AvoidHittingLandscape()
{
    if (Contained()) return;

    var dist = GetID()->GetDefHeight()*2;

    var d_top = 0;
    var d_down = 0;
    var d_top_dir = 0;
    var d_down_dir = 0;

    var tolerance_y = 20;
    //var ydir_max = 10;
    var ydir_change = 1;

    var dx = GetXDir()*15;
    var dist2 = dist + 10*BoundBy(Abs(GetXDir())-3, 0, 10);
    var d_min = 40;

    // calculate distance to landscape right below the bird
    for (d_top = Abs(GetDefOffset(1)); d_top < dist; d_top++)
    {
        if (GBackSemiSolid(0,-d_top)) break;
    }

    for (d_down = GetDefOffset(1) + GetID()->GetDefHeight(); d_down < dist; d_down++)
    {
        if (GBackSemiSolid(0, d_down)) break;
    }

    // calculate distance to landscape in movement direction
    for (d_top_dir = Abs(GetDefOffset(1)); d_top_dir < dist2; d_top_dir++)
    {
        if (GBackSemiSolid(dx,-d_top_dir)) break;
    }

    for (d_down_dir = GetDefOffset(1) + GetID()->GetDefHeight(); d_down_dir < dist2; d_down_dir++)
    {
        if (GBackSemiSolid(dx, d_down_dir)) break;
    }

    // adjust height

    var target_y = GetY()+(d_down-d_top)/2 +BoundBy(+d_min-d_top, 0, d_min)-BoundBy(+d_min-d_down, 0, d_min);
    var target_y_dir = GetY()+(d_down_dir-d_top_dir)/2 +BoundBy(+d_min-d_top_dir, 0, d_min)-BoundBy(+d_min-d_down_dir, 0, d_min);

    var w1, w2;

    if (GetYDir() <= 3)
    {
        w1 = 2;
        w2 = 1;
    }
    else
    {
        w1 = 1;
        w2 = 2;
    }
    target_y = (w1*target_y + w2*target_y_dir)/(w1+w2);

    if (GetY() < target_y - tolerance_y)
    {
        SetYDir(BoundBy(GetYDir()+ydir_change, -7, 7));
    }

    if (GetY() > target_y + tolerance_y)
    {
        SetYDir(BoundBy(GetYDir()-ydir_change, -7, 7));
    }
}

private func GetFeasableHeight(int x)
{
    var height = GetY();

    var safe_distance = GetID()->GetDefHeight() * 2;

    var found_starting_point = false;

    var top_range, bottom_range;

    // first look for a good point upwards!
    while ( height > safe_distance && !found_starting_point)
    {
        height -= 10;

        if (GBackSemiSolid(x-GetX(), height-GetY())) continue; // actually free where we move
        if (GBackSemiSolid(x-GetX(), height-GetY()+safe_distance)) continue; // keep safe distance to bottom

        if (GetPathLength(GetX(), GetY(), x, height) != nil) // and we can plan to this point!!
            found_starting_point = true;
    }

    if (found_starting_point)
    {
        bottom_range = height;

        while ( height > 0 && !GBackSemiSolid(x-GetX(), height-GetY()))
        {
            height -= 10;
        }

        top_range = height + safe_distance;

        return [top_range, bottom_range];
    }


    // now look for a good point downwards!
    height = GetY();
    while ( height < LandscapeHeight()-safe_distance && !found_starting_point)
    {
        height += 10;

        if (GBackSemiSolid(x-GetX(), height-GetY())) continue; // actually free where we move
        if (GBackSemiSolid(x-GetX(), height-GetY()-safe_distance)) continue; // keep safe distance to top

        if (GetPathLength(GetX(), GetY(), x, height) != nil) // and we can plan to this point!!
            found_starting_point = true;
    }

    if (found_starting_point)
    {
        top_range = height;

        while ( height < LandscapeHeight() && !GBackSemiSolid(x-GetX(), height-GetY()))
        {
            height += 10;
        }

        bottom_range = height - safe_distance;

        return [top_range, bottom_range];
    }

    return nil;
}


private func Place(int amount, proplist rectangle, proplist settings)
{
	var max_tries = 4 * amount;
	var loc_area = nil;
	if (rectangle) 
		loc_area = Loc_InArea(rectangle);
	var birds = [];
	var loc_bkg = Loc_Or(Loc_Tunnel(), Loc_Sky());

	while ((amount > 0) && (--max_tries > 0))
	{
		var spot = FindLocation(loc_bkg, Loc_Space(30), loc_area);
		if (!spot)
			continue;
		
		var bird = CreateObject(this, spot.x, spot.y, NO_OWNER);
		if (!bird) 
			continue;
		
		if (bird->Stuck())
		{
			bird->RemoveObject();
			continue;
		}
		PushBack(birds, bird);
		--amount;
	}
	// Return a list of all created birds.
	return birds;
}
