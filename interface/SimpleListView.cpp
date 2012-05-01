#include <ClassInfo.h>
#include <Font.h>
#include <Window.h>
#include <String.h>
#include <Application.h>
#include "SimpleListView.h"
#include "TitledListItem.h"

SimpleListView::SimpleListView(BRect frame, const char *name, 
		list_view_type type = B_SINGLE_SELECTION_LIST, 
  		uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP,
  		uint32 flags = B_WILL_DRAW | B_NAVIGABLE | B_FRAME_EVENTS):
  		BListView(frame, name, type, resizingMode, flags),
  		editView(NULL),
  		activeWindow(NULL)
{
}
	
SimpleListView::~SimpleListView()
{
}

void SimpleListView::MouseDown(BPoint where)
{
	int32 currentIndex = IndexOf(where);
	TitledListItem *anItem = cast_as(ItemAt(currentIndex), TitledListItem);
	
	if (anItem == NULL) return;

	if (IsBeginEditing())
	{
		if (currentIndex != CurrentSelection() || 
			!(editView->Frame().Contains(where)))
			
			FinishEditing(true);
			BListView::MouseDown(where);
			SelectionChanged();
			return;
		}
		
		BRect titleRect = TitleRect(currentIndex);
		
		if (IsItemSelected(currentIndex) && titleRect.IsValid() && 
			titleRect.Contains(where) && anItem->IsEditable())
		{
			if (!Window()->IsActive())
				Window()->Activate();
			
			BeginEditing(currentIndex);
		}
		else
			BListView::MouseDown(where);
		
		SelectionChanged();
}

void SimpleListView::FrameResized(float width, float height)
{
	BListView::FrameResized(width, height);
	
	BListItem *anItem = 0;
	BFont font;
	GetFont(&font);
	
	for (int32 i = 0; (anItem = ItemAt(i)); i++)
		anItem->Update(this, &font);
	
	if (IsBeginEditing())
		FinishEditing(false);
		
	Invalidate();
}

BRect SimpleListView::TitleRect(int32 index)
{
	TitledListItem *anItem = cast_as(ItemAt(index), TitledListItem);
	
	if (anItem == NULL) return BRect();

	BString title = "";
	font_height height;
	
	BRect titleRect = BRect();
	BRect itemRect = BListView::ItemFrame(index);
	
	BPoint whereTitle = anItem->TitlePosition();
		
	title.SetTo(anItem->Text());
	GetFontHeight(&height);
		
	titleRect.Set(0, 0, StringWidth(title.String()), 
		height.ascent + height.descent + height.leading + 2);
	titleRect.OffsetTo(itemRect.left + whereTitle.x,
		itemRect.top + whereTitle.y);
			
	return titleRect;
}

void SimpleListView::BeginEditing(int32 index)
{
	if (IsBeginEditing()) return;
	
	TitledListItem *anItem = cast_as(ItemAt(index), TitledListItem);
	
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
	
	BRect frame = TitleRect(index);
	frame.right = ItemFrame(index).right - 15;
	
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
		
	BListView::AddChild(editView);
	editView->SelectAll();
	editView->MakeFocus();
}

void SimpleListView::FinishEditing(bool change)
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
			TitledListItem *anItem = 
				cast_as(ItemAt(CurrentSelection()), TitledListItem);
			
			if (anItem)
				anItem->SetText(editView->Text());
		}
		
		RemoveChild(editView);
		delete editView;
		editView = NULL;
	}
}

bool SimpleListView::IsBeginEditing(void) const
{
	return (editView != NULL);
}

filter_result SimpleListView::editing_filter(BMessage *m, BHandler **h, 
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

bool SimpleListView::InitiateDrag(BPoint where, int32 index, bool wasSelected)
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
