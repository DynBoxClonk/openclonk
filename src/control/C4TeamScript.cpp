/*
 * OpenClonk, http://www.openclonk.org
 *
 * Copyright (c) 2018, The OpenClonk Team and contributors
 *
 * Distributed under the terms of the ISC license; see accompanying file
 * "COPYING" for details.
 *
 * "Clonk" is a registered trademark of Matthes Bender, used with permission.
 * See accompanying file "TRADEMARK" for details.
 *
 * To redistribute this file separately, substitute the full license texts
 * for the above references.
 */

#include "C4Include.h"

#include "control/C4Teams.h"
#include "control/C4TeamScript.h"
#include "script/C4Aul.h"
#include "script/C4AulDefFunc.h"


void C4TeamScript::RegisterWithEngine(C4AulScriptEngine *engine)
{
    C4PropListStatic* prototype = new C4PropListStatic(nullptr, nullptr, ::Strings.RegString(PROTOTYPE_NAME_ENGINE));
	engine->RegisterGlobalConstant(PROTOTYPE_NAME_ENGINE, C4VPropList(prototype));
	#define F(f) ::AddFunc(prototype, #f, Fn##f)
	#undef F
	prototype->Freeze();
}
