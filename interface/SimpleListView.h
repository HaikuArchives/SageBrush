#ifndef SIMPLE_LIST_VIEW_H
#define SIMPLE_LIST_VIEW_H

#include <SupportDefs.h>
#include <Point.h>
#include <Rect.h>
#include <ListView.h>
#include <TextView.h>
#include <Message.h>
#include <MessageFilter.h>
#include <Handler.h>
#include <Debug.h>

class SimpleListView: public BListView
{
  private:
  	BTextView *editView;
  	BWindow *activeWindow;
  	
  	BRect TitleRect(int32 index);

	static filter_result editing_filter(BMessage *m, BHandler **h, BMessageFilter *f);
  	
  protected:
  	void BeginEditing(int32 index);
  	
  	void FinishEditing(bool change);
  	
  	bool IsBeginEditing(void) const;
  	
  public:
	SimpleListView(BRect frame, const char *name, 
		list_view_type type = B_SINGLE_SELECTION_LIST, 
  		uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP,
  		uint32 flags = B_WILL_DRAW | B_NAVIGABLE | B_FRAME_EVENTS);
	
	~SimpleListView();
	
	virtual void MouseDown(BPoint where);
	virtual void FrameResized(float width, float height);
	
	virtual bool InitiateDrag(BPoint where, int32 index, bool wasSelected);
};

#endif /* SIMPLE_LIST_VIEW_H */