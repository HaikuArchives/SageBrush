/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */

#ifndef PROPERTY_LIST_ITEM_H
#define PROPERTY_LIST_ITEM_H

#include <Rect.h>
#include <View.h>
#include <Font.h>
#include <String.h>
#include "EditableListItem.h"

class PropertyListItem: public EditableListItem
{
	BString name;
  public:
	PropertyListItem(const char *text, uint32 outlineLevel = 0, 
		bool expanded = true);
	
	~PropertyListItem();
	
	virtual	void DrawItem(BView *owner, BRect itemRect, 
		bool drawEverything = false);

	virtual void Update(BView *owner, const BFont *font);
	
	virtual void SetName(const char *name);
	
	virtual const char *Name(void) const;
};

#endif /* PROPERTY_LIST_ITEM_H */