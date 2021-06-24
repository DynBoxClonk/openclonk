/*
 * Helper functions for menus.
 * 
 * Author: Marky
 */

// Because GetMenu() returns nil, even if a menu is open??!
global func IsMenuOpen()
{
	if (!this) return false;
	return GetMenuSelection()!=nil;
}