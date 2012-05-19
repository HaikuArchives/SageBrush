/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */

#include <TypeConstants.h>
#include <String.h>
#include <map>
#include <regex.h>
#include <ClassInfo.h>
#include <TextView.h>
#include <Clipboard.h>
#include <cctype>
#include "SimpleTextFilter.h"

#include <cstdio>

typedef std::map<int, std::pair<BString, pair<BString, regex_t> > > Definition;
static Definition definition;
static regex_t name_cere;

namespace _private
{
	class module_init
	{
		public:
			module_init();
			~module_init() {}
	};
		
	module_init InitModule = module_init();
}

bool setDefinitionType(int typeConstant, const char *name, const char *defs)
{
	std::pair<BString, pair<BString, regex_t> > anItem;
	
	anItem.first = BString().SetTo(name);
	anItem.second.first = BString().SetTo(defs);
	anItem.second.second = regex_t();
//	regcomp(&(anItem.second.second), defs, REG_EXTENDED);
	
	definition[typeConstant] = anItem;
	
	return true;
}

BString getTypeName(int typeConstant)
{
	Definition::const_iterator pos = definition.find(typeConstant);
	
	if (pos == definition.end())
		return BString();

	return (pos->second).first;
}

BString getTypeDefs(int typeConstant)
{
	Definition::const_iterator pos = definition.find(typeConstant);
	
	if (pos == definition.end())
		return BString();

	return (pos->second).second.first;
}

regex_t getTypeCere(int typeConstant)
{
	Definition::const_iterator pos = definition.find(typeConstant);
	
	if (pos == definition.end())
		return regex_t();

	return (pos->second).second.second;
}


void textHelper(BMessage *m, BString &text, int32 start, int32 end)
{
	int8 byte;
	BString bytes;
	int32 modifiers = 0;

	if (m->FindInt8("byte", &byte) != B_OK)
		return ; //B_DISPATCH_MESSAGE;

	if (m->FindString("bytes", &bytes) != B_OK)
		return ; //B_DISPATCH_MESSAGE;
	
	if (byte == B_BACKSPACE || byte == B_DELETE)
	{
		if (start == end)
		{
			int32 remove = start - 1 + ((byte == B_BACKSPACE)? 0: 1);
		
			if (remove >= 0 && remove < text.Length())
				text.Remove(remove, 1);
		}
		else
			text.Remove(start, end - start);
	}
	else
	
	if (m->FindInt32("modifiers", &modifiers) == B_OK &&
		(modifiers & B_COMMAND_KEY) && tolower(byte) == 'v')
	{
		const char *clipText;
		int32 clipTextLength;
		BMessage *data = NULL;
		BString clipString;
		
		if (be_clipboard->Lock())
		{
			data = be_clipboard->Data();
			
			if (data)
			{
				data->FindData("text/plain", B_MIME_TYPE, (const void**) &clipText, &clipTextLength);
				clipString.SetTo(clipText, clipTextLength);
			}
			
			be_clipboard->Unlock();
		}
		
		if (start != end)
			text.Remove(start, end - start);
		
		text.Insert(clipString, start);
	}
	else
		if (isprint(byte))
		{
			if (start != end)
				text.Remove(start, end - start);
			
			text.Insert(bytes, start);
		}
}

filter_result filterName(BMessage *m, BHandler **h, BMessageFilter *f)
{
	regmatch_t pm;
	BTextView *textView = cast_as(*h, BTextView);
	
	if (!textView)
		return B_DISPATCH_MESSAGE;

	BString text = BString();
	text.SetTo(textView->Text());
	
	int32 start = 0, end = 0;
	textView->GetSelection(&start, &end);
	
	textHelper(m, text, start, end);

	if (regexec(&name_cere, text.String(), 1, &pm, 0) == 0)
	{
		printf("cool - %s\n", text.String());
	}
	else
	{
		printf("bad - %s\n", text.String());
	}

	return B_DISPATCH_MESSAGE;
}

filter_result filterTypes(BMessage *m, BHandler **h, BMessageFilter *f)
{
//	return B_SKIP_MESSAGE;
	return B_DISPATCH_MESSAGE;
}

_private::module_init::module_init()
{
	setDefinitionType(B_ALIGNMENT_TYPE, "BAlignment", "");
	setDefinitionType(B_RECT_TYPE, "BRect", "");
	setDefinitionType(B_POINT_TYPE, "BPoint", "");
	setDefinitionType(B_SIZE_TYPE, "BSize", "");
	setDefinitionType(B_STRING_TYPE, "BString", "");
	setDefinitionType(B_INT8_TYPE, "int8", "");
	setDefinitionType(B_UINT8_TYPE, "uint8", "");
	setDefinitionType(B_INT16_TYPE, "int16", "");
	setDefinitionType(B_UINT16_TYPE, "uint16", "");
	setDefinitionType(B_INT32_TYPE, "int32", "");
	setDefinitionType(B_UINT32_TYPE, "uint32", "");
	setDefinitionType(B_INT64_TYPE, "int64", "");
	setDefinitionType(B_UINT64_TYPE, "uint64", "");
	setDefinitionType(B_BOOL_TYPE, "bool", "");
	setDefinitionType(B_FLOAT_TYPE, "float", "");
	setDefinitionType(B_DOUBLE_TYPE, "double", "");
	setDefinitionType(B_POINTER_TYPE, "void *", "");
	setDefinitionType(B_MESSENGER_TYPE, "BMessenger", "");
	setDefinitionType(B_REF_TYPE, "entry_ref", "");
	setDefinitionType(B_MESSAGE_TYPE, "BMessage", "");
	setDefinitionType(B_RAW_TYPE, "raw", "");
	
	regcomp(&name_cere, "^[A-Za-z][A-Za-z0-9]{0,31}$", REG_EXTENDED);
}

