/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */

#include "Units.h"

struct
{
	float conversion; // from inches
	float divisions;  // default divisions
	BString name;
} units_data[] = {
	{1.00, 8, "Inches"}, 
	{2.54, 2, "Centimeters"}, 
	{6.00, 0.5, "Picas"}, 
	{72.00,  0.02, "Pixels"}
};

float Units::ConvertUnitsToPixels(Units::Type units, float value)
{
	return value*units_data[PIXELS].conversion/units_data[units].conversion;
}

void Units::SetPixelsFromInches(float value)
{
	if (value > 1)
		units_data[PIXELS].conversion = value;
}

void Units::SetUnitsName(Units::Type units, const char *name)
{
	units_data[units].name.SetTo(name);
}

void Units::GetUnitsName(Units::Type units, BString &name)
{
	name.SetTo(units_data[units].name.String());
}

float Units::GetConversion(Units::Type units)
{
	return units_data[units].conversion;
}

float Units::GetDivisions(Units::Type units)
{
	return units_data[units].divisions;
}
