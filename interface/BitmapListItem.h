#ifndef BITMAP_LIST_ITEM_H
#define BITMAP_LIST_ITEM_H

#include <SupportDefs.h>
#include <Rect.h>
#include <Bitmap.h>
#include "TitledListItem.h"

class BitmapListItem: public TitledListItem
{
	BBitmap *bitmap;
	BRect bitmapFrame;
	bool destroyBitmap;
	
  public:

	BitmapListItem(const char *text, uint32 outlineLevel = 0, 
		bool expanded = true);
	~BitmapListItem();
	
	void SetBitmap(BBitmap *bitmap, BRect frame = BRect(), 
		bool destroy = false);
		
	virtual void DrawItem(BView *view, BRect itemRect, bool drawEverything);
};

#endif /* BITMAP_LIST_ITEM_H */