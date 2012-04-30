#ifndef UNITS_H
#define UNITS_H

#include <String.h>

namespace Units 
{
	enum Type { INCHES = 0, CENTIMETERS = 1, PICAS = 2, PIXELS = 3 };
	
	float ConvertUnitsToPixels(Type units, float value);
	void SetPixelsFromInches(float value);
	void SetUnitsName(Type units, const char *name);
	void GetUnitsName(Type units, BString &name);
	float GetConversion(Type units);
	float GetDivisions(Type units);
}

#endif /* UNITS_H */
