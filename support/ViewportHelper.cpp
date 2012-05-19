/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */

#include <Handler.h>
#include <Message.h>
#include <MessageFilter.h>
#include <ClassInfo.h>
#include <Debug.h>
#include "Viewport.h"
#include "ScrollViewWithRulers.h"

filter_result viewport_resize_helper(BMessage *msg, BHandler **h, 
	BMessageFilter *f)
{
	Viewport *viewport = cast_as(*h, Viewport);
	
	if (!viewport)
		return B_DISPATCH_MESSAGE;
	
	ScrollViewWithRulers *scrollview = cast_as(viewport->Parent(), ScrollViewWithRulers);
	
	if (!scrollview || !scrollview->HasRulers())
		return B_DISPATCH_MESSAGE;
	
	BRect source = viewport->SourceRect();
	BRect dest = viewport->DestinationRect();
	Ruler *ruler = 0;
	
	if ((ruler = scrollview->GetRuler(B_HORIZONTAL)))
	{
		if (source.left == 0.0 && dest.left > 0.0)
			ruler->ScrollTo(-dest.left, 0.0);
		else
			ruler->ScrollTo(source.left, 0.0);
	}
	
	if ((ruler = scrollview->GetRuler(B_VERTICAL)))
	{
		if (source.top == 0.0 && dest.top > 0.0)
			ruler->ScrollTo(0.0, -dest.top);
		else
			ruler->ScrollTo(0.0, source.top);
	}
	
	return B_DISPATCH_MESSAGE;
}

filter_result viewport_ratio_helper(BMessage *msg, BHandler **h, 
	BMessageFilter *f)
{
	Viewport *viewport = cast_as(*h, Viewport);
	
	if (!viewport)
		return B_DISPATCH_MESSAGE;
	
	ScrollViewWithRulers *scrollview = cast_as(viewport->Parent(), ScrollViewWithRulers);
	
	if (!scrollview || !scrollview->HasRulers())
		return B_DISPATCH_MESSAGE;
	
	float ratio = viewport->Ratio();
	BRect source = viewport->SourceRect();
	BRect dest = viewport->DestinationRect();
	Ruler *ruler = 0;
	
	if ((ruler = scrollview->GetRuler(B_HORIZONTAL)))
	{
		if (source.left == 0.0 && dest.left > 0.0)
			ruler->ScrollTo(-dest.left, 0.0);
		else
			ruler->ScrollTo(source.left, 0.0);

		ruler->SetRatio(ratio);
	}
	
	if ((ruler = scrollview->GetRuler(B_VERTICAL)))
	{
		if (source.top == 0.0 && dest.top > 0.0)
			ruler->ScrollTo(0.0, -dest.top);
		else
			ruler->ScrollTo(0.0, source.top);

		ruler->SetRatio(ratio);
	}

	return B_DISPATCH_MESSAGE;
}
