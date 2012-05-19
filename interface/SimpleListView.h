/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */
#ifndef SIMPLE_LIST_VIEW_H
#define SIMPLE_LIST_VIEW_H

#include <SupportDefs.h>
#include <Point.h>
#include <Rect.h>
#include <TextView.h>
#include <Message.h>
#include <MessageFilter.h>
#include <Handler.h>
#include <ListView.h>

template <typename T> class SimpleListView: public T
{
  private:
  	BTextView *editView;
  	BWindow *activeWindow;
  	
  	BRect TextRect(int32 index);

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

#include "SimpleListViewDef.h"

#endif /* SIMPLE_LIST_VIEW_H */
