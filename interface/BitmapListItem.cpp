#include <View.h>
#include "BitmapListItem.h"

BitmapListItem::BitmapListItem(const char *text, uint32 outlineLevel = 0, 
	bool expanded = true):
	TitledListItem(text, outlineLevel, expanded),
	bitmap(0), bitmapFrame(BRect()), destroyBitmap(false)
{
}

BitmapListItem::~BitmapListItem()
{
	if (bitmap && destroyBitmap)
		delete bitmap;
}
	
void BitmapListItem::SetBitmap(BBitmap *bitmap, BRect frame, bool destroy)
{
	BitmapListItem::bitmap = bitmap;
	bitmapFrame = frame;
	destroyBitmap = destroy;
	
	if (!bitmapFrame.IsValid())
	{
		bitmapFrame.Set(0, 0, Height() - 5, Height() - 5);
		bitmapFrame.OffsetBy(2, 2);
	}
}
		
void BitmapListItem::DrawItem(BView *view, BRect itemRect, bool drawEverything)
{
	TitledListItem::DrawItem(view, itemRect, drawEverything);

	BRect frame = bitmapFrame;
	frame.OffsetBy(itemRect.left, itemRect.top);
	
	if (bitmap && frame.IsValid())
		view->DrawBitmap(bitmap, frame);
}
