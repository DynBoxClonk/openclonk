/*
 * Author: Tyron, GZE Project
 */


global func IntToBytes(int value) {
	return [value >> (3*8) & 255, value >> (2*8) & 255, value >> (1*8) & 255, value & 255];
}

global func BytesToInt(array bytes) {
	return 
		(bytes[3] << (0*8)) |
		(bytes[2] << (1*8)) |
		(bytes[1] << (2*8)) |
		(bytes[0] << (3*8));
}

// Extracts a single byte from a variable (useful mostly just for ints)
global func GetByte(value, position) {
	if (GetType(value) == C4V_Int) {
		return value >> ((3-position)*8) & 255;
	}
	if (GetType(value) == C4V_String) {
		return GetChar(value, position);
	}
	return -1;
}

global func SetByte(value, newvalue, position) {
	if (GetType(value) == C4V_Int) {
		// Delete old color
		value = value & ~(255 << ((3-position)*8));
		// Add new color
		return (value|newvalue << ((3-position)*8));
	}
}
