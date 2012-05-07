#ifndef NAME_OF_TYPE_H
#define NAME_OF_TYPE_H

#include <SupportDefs.h>
#include <TypeConstants.h>

struct NameOfType
{
	const char *name;
	type_code type;
};

const NameOfType name_of_type[] =
{
	{ "BAlignment", 	B_ALIGNMENT_TYPE},
	{ "BRect", 			B_RECT_TYPE},
	{ "BPoint", 		B_POINT_TYPE},
	{ "BSize", 			B_SIZE_TYPE},
	{ "BString", 		B_STRING_TYPE},
	{ "int8", 			B_INT8_TYPE},
	{ "uint8", 			B_UINT8_TYPE},
	{ "int16", 			B_INT16_TYPE},
	{ "uint16", 		B_UINT16_TYPE},
	{ "int32", 			B_INT32_TYPE},
	{ "uint32", 		B_UINT32_TYPE},
	{ "int64", 			B_INT64_TYPE},
	{ "uint64", 		B_UINT64_TYPE},
	{ "bool", 			B_BOOL_TYPE},
	{ "float", 			B_FLOAT_TYPE},
	{ "double", 		B_DOUBLE_TYPE},
	{ "void *", 		B_POINTER_TYPE},
	{ "BMessenger", 	B_MESSENGER_TYPE},
	{ "entry_ref", 		B_REF_TYPE},
	{ "BMessage", 		B_MESSAGE_TYPE},
	{ "BFlattenable",	'none'},			// ???????????
	{ "data",			B_RAW_TYPE}
};

#endif /* NAME_OF_TYPE_H */