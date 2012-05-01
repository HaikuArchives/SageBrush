#include "TestWinAndApp.h"
#include <View.h>
#include "ScrollViewWithRulers.h"

class testView: public BView
{
  public:
  	testView(BRect frame, const char *name, uint32 resizeMask, uint32 flags):
  		BView(frame, name, resizeMask, flags)
  	{
  	}
  
	virtual void MessageReceived(BMessage *msg)
	{
		msg->PrintToStream();
		BView::MessageReceived(msg);
	}
	
	virtual void Draw(BRect updateRect)
	{
		SetHighColor(0, 0, 0);
		StrokeLine(BPoint(0, 0), BPoint(100, 100));
	}
};

int main(int argc, char *argv[])
{
	App *app = new App();
	
	if (app)
	{
		if (app->win)
		{
			BRect frame = BRect(0, 0, app->win->Bounds().Width() - 14, app->win->Bounds().Height() - 14);
			
			testView *some = new testView(frame, "some", B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS);
			some->SetViewColor(156, 29, 145);
			
			ScrollViewWithRulers *scroll = 
				new ScrollViewWithRulers("test", some, B_FOLLOW_ALL, 0, true, true);
			
			app->win->AddChild(scroll);
			
			if (scroll->Window() && scroll->Window()->Lock())
			{
				scroll->ShowRulers();
				scroll->Window()->Unlock();
			}
		}
	
		app->Run();
		delete app;
	}
	
	return 0;
}