#ifndef SIMPLE_LIST_VIEW_DEF_H
#define SIMPLE_LIST_VIEW_DEF_H

#include <ClassInfo.h>
#include <Font.h>
#include <Window.h>
#include <String.h>
#include <Application.h>
#include <ListView.h>
#include <OutlineListView.h>
#include <View.h>
#include "SimpleListView.h"
#include "EditableListItem.h"

template <typename T> SimpleListView<T>::SimpleListView(BRect frame, const char *name, 
		list_view_type type, // = B_SINGLE_SELECTION_LIST, 
  		uint32 resizingMode, // = ((B_FOLLOW_LEFT) | (B_FOLLOW_TOP)),
  		uint32 flags): // = B_WILL_DRAW | B_NAVIGABLE | B_FRAME_EVENTS):
  		T(frame, name, type, resizingMode, flags),
  		editView(NULL),
  		activeWindow(NULL)
{
}
	
template <typename T> SimpleListView<T>::~SimpleListView()
{
}

template <typename T> void SimpleListView<T>::MouseDown(BPoint where)
{
	int32 currentIndex = IndexOf(where);
	EditableListItem *anItem = cast_as(ItemAt(currentIndex), EditableListItem);
	
	if (anItem == NULL) return;

	if (IsBeginEditing())
	{
		if (currentIndex != CurrentSelection() || 
			!(editView->Frame().Contains(where)))
			
			FinishEditing(true);
			T::MouseDown(where);
			SelectionChanged();
			return;
		}
		
		BRect textRect = TextRect(currentIndex);
		
		if (IsItemSelected(currentIndex) && textRect.IsValid() && 
			textRect.Contains(where) && anItem->IsEditable())
		{
			if (!Window()->IsActive())
				Window()->Activate();
			
			BeginEditing(currentIndex);
		}
		else
			T::MouseDown(where);
		
		SelectionChanged();
}

template <typename T> void SimpleListView<T>::FrameResized(float width, float height)
{
	T::FrameResized(width, height);
	
	BListItem *anItem = 0;
	BFont font;
	GetFont(&font);
	
	for (int32 i = 0; (anItem = ItemAt(i)); i++)
		anItem->Update(this, &font);
	
	if (IsBeginEditing())
		FinishEditing(false);
		
	Invalidate();
}

template <typename T> BRect SimpleListView<T>::TextRect(int32 index)
{
	int outline_d = (cast_as(this, BOutlineListView))? 15: 0;

	EditableListItem *anItem = cast_as(ItemAt(index), EditableListItem);
	
	if (anItem == NULL) return BRect();

	BString text = "";
	font_height height;
	
	BRect textRect = BRect();
	BRect itemRect = T::ItemFrame(index);
	
	BPoint whereText = anItem->TextPosition();
		
	text.SetTo(anItem->Text());
	GetFontHeight(&height);
		
	textRect.Set(0, 0, StringWidth(text.String()), 
		height.ascent + height.descent + height.leading + 2);
		
	textRect.OffsetTo(itemRect.left + whereText.x + 
		anItem->OutlineLevel()*10 + outline_d,
		itemRect.top + whereText.y);
			
	return textRect;
}

template <typename T> void SimpleListView<T>::BeginEditing(int32 index)
{
	if (IsBeginEditing()) return;
	
	EditableListItem *anItem = cast_as(ItemAt(index), EditableListItem);
	
	if (anItem == NULL) return;
	
	if (Window() && Window()->Flags() & B_AVOID_FOCUS)
	{
		if (be_app)
			for (int32 i = 0; (activeWindow = be_app->WindowAt(i)); i++)
				if (activeWindow->IsActive())
					break;
		
		Window()->SetFlags(Window()->Flags() ^ B_AVOID_FOCUS);
		Window()->Activate(true);
	}
	
	BFont font;
	GetFont(&font);
	font.SetFace(B_BOLD_FACE);
	
	BRect frame = TextRect(index);
	frame.right = ItemFrame(index).right - 1; //15;
		
	BRect textRect = frame;
	textRect.OffsetTo(B_ORIGIN);
	textRect.left += 1;
	textRect.top += 1;
	
	editView = new BTextView(frame, "editView", textRect, &font, 0,
		B_FOLLOW_NONE, B_WILL_DRAW);
	
	editView->SetText(anItem->Text());
	editView->SetWordWrap(false);
	editView->DisallowChar('\n');
	
	editView->AddFilter(new BMessageFilter(B_KEY_DOWN, editing_filter));
		
	//BListView::AddChild(editView);
	T::AddChild(editView);
	editView->SelectAll();
	editView->MakeFocus();
}

template <typename T> void SimpleListView<T>::FinishEditing(bool change)
{
	if (IsBeginEditing())
	{
		if (Window() && activeWindow)
		{
			Window()->SetFlags(Window()->Flags() | B_AVOID_FOCUS);
			activeWindow->Activate(true);
			activeWindow = NULL;
		}
		
		if (change && editView->TextLength() > 0)
		{
			EditableListItem *anItem = 
				cast_as(ItemAt(CurrentSelection()), EditableListItem);
			
			if (anItem)
				anItem->SetText(editView->Text());
		}
		
		RemoveChild(editView);
		delete editView;
		editView = NULL;
	}
}

template <typename T> bool SimpleListView<T>::IsBeginEditing(void) const
{
	return (editView != NULL);
}

template <typename T> filter_result SimpleListView<T>::editing_filter(BMessage *m, BHandler **h, 
	BMessageFilter *f)
{
	BTextView *textView = cast_as(*h, BTextView);
	SimpleListView *listView = cast_as(textView->Parent(), SimpleListView);
	
	if (!textView || !listView)
		return B_DISPATCH_MESSAGE;
		
	BRect textRect = textView->TextRect();
	
	textRect.right = textView->LineWidth() + 20;
	textView->SetTextRect(textRect);
	textView->ScrollToSelection();
		
	uchar key;
	
	if (m->FindInt8("byte", (int8 *)&key) != B_OK)
		return B_DISPATCH_MESSAGE;
	
	if (key == B_RETURN || key == B_ESCAPE)
	{
		listView->FinishEditing(key == B_RETURN);
		return B_SKIP_MESSAGE;
	}
		
	return B_DISPATCH_MESSAGE;
}

template <typename T> bool SimpleListView<T>::InitiateDrag(BPoint where, int32 index, bool wasSelected)
{
	BPoint point;
	uint32 buttons;
	int32 n = -1;
	
	if (Window() == NULL) return false;
	
	while (1)
	{
		if (Window()->Lock())
		{
			GetMouse(&point, &buttons);
			Window()->Unlock();
		}
		else
			break;
		
		if (buttons != B_PRIMARY_MOUSE_BUTTON)
			break;
		
		n = IndexOf(point);
		
		if (n > -1 && n < CountItems() && CurrentSelection() != n)
		{
			MoveItem(CurrentSelection(), n);
			Select(n);
			SelectionChanged();
			ScrollToSelection();
		}
		
		snooze(10000);
	}
	
	return true;
}

#endif /* SIMPLE_LIST_VIEW_DEF_H */