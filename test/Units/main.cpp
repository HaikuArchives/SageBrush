/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */
#include "Units.h"

#include <cstdio>

int main(int argc, char *argv[])
{
	BString s;
	
	Units::GetUnitsName(Units::CENTIMETERS, s);
	printf("default: %s\n", s.String());	

	Units::SetUnitsName(Units::CENTIMETERS, "Сантиметры");
	
	Units::GetUnitsName(Units::CENTIMETERS, s);
	printf("%s\n", s.String());	

	return 0;
}