/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */

#define W_WIDTH 500
#define W_HEIGHT 400

#include "Viewport.h"

#define MESSAGERECEIVED virtual void MessageReceived(BMessage *msg);
#define WINDOW_PUBLIC	Viewport *viewport;

#include "TestWinAndApp.h"
#include "ScrollViewWithRulers.h"

#include <MenuItem.h>
#include <MenuBar.h>
#include <MessageFilter.h>
#include <TranslationUtils.h>
#include <Bitmap.h>
#include <AppDefs.h>

filter_result viewport_resize_helper(BMessage *msg, BHandler **h, 
	BMessageFilter *f);

filter_result viewport_ratio_helper(BMessage *msg, BHandler **h, 
	BMessageFilter *f);

class SomeViewport: public Viewport
{
	BBitmap *bitmap;

  public:
	SomeViewport(BRect frame, const char *name, 
		uint32 resizingMode = B_FOLLOW_ALL_SIDES,
		uint32 flags = B_WILL_DRAW | B_FRAME_EVENTS):
		Viewport(frame, name, resizingMode, flags),
		bitmap(0)
	{
		bitmap = 
		BTranslationUtils::GetBitmapFile("../Cat_and_Fly");

	}

	~SomeViewport()
	{
		if (bitmap)
			delete bitmap;
	}
	
	virtual bool CreateAction(BPoint where)
	{
		return true;
	}
	
	virtual void AppendToAction(BPoint where)
	{
	}
	
	virtual void FinishAction(void)
	{
	}
	
	virtual float Width(void) const
	{
		return bitmap? bitmap->Bounds().Width(): 0;
	}
	
	virtual float Height(void) const
	{
		return bitmap? bitmap->Bounds().Height(): 0;
	}
	
	virtual void DrawTarget(BRect updateRect)
	{
		SetHighColor(190, 89, 147);
		StrokeRect(DestinationRect());
		if (SourceRect().IsValid() && DestinationRect().IsValid())
			DrawBitmap(bitmap, SourceRect(), DestinationRect());
	}
};

void Win::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case ' zIn':
			if (viewport)
				viewport->SetRatio(viewport->Ratio() + 0.25);
			break;
		case 'zOut':
			if (viewport)
				viewport->SetRatio(viewport->Ratio() - 0.25);
			break;
		case 'curs':
			if (viewport)
				viewport->SetViewportCursor(B_CURSOR_I_BEAM);
			break;
			
		default:
			BWindow::MessageReceived(msg);
			break;
	}
}

int main(int argc, char *argv[])
{
	App *app = new App();
	
	if (app)
	{
		if (app->win)
		{
			BMenuBar *menuBar = new BMenuBar(BRect(0,0,100,10), "Menu");
			app->win->AddChild(menuBar);
			
			BRect frame;

			if (app->win && app->win->Lock())
			{
				frame = BRect(0, menuBar->Bounds().Height() + 2, app->win->Bounds().Width() - 14, app->win->Bounds().Height() - 14);
				app->win->Unlock();
			}
			
			app->win->viewport = new SomeViewport(frame, "test");
			
			ScrollViewWithRulers *scroll = new ScrollViewWithRulers("scroll", app->win->viewport, B_FOLLOW_ALL, 0, true, true);
						
			app->win->AddChild(scroll);
			
			if (scroll->Window() && scroll->Window()->Lock())
			{
				app->win->viewport->AddFilter(new BMessageFilter(B_VIEW_RESIZED, viewport_resize_helper));
				app->win->viewport->AddFilter(new BMessageFilter(Viewport::RATIO_CHANGED, viewport_ratio_helper));
				scroll->ShowRulers();
				scroll->Window()->Unlock();
			}
			
			Ruler *ruler = 0;
			
			if ((ruler = scroll->GetRuler(B_HORIZONTAL)))
			{
				ruler->SetRange(0, app->win->viewport->Width() + 1.0);
			}

			if ((ruler = scroll->GetRuler(B_VERTICAL)))
			{
				ruler->SetRange(0, app->win->viewport->Height() + 1.0);
			}
			
			// create menu
			
			BMenu *testMenu = new BMenu("Test");
			
			if (testMenu)
			{
				menuBar->AddItem(testMenu);
				
				testMenu->AddItem(new BMenuItem("Zoom In", new BMessage(' zIn'), '+'));
				testMenu->AddItem(new BMenuItem("Zoom Out", new BMessage('zOut'), '-'));
				testMenu->AddItem(new BMenuItem("Set Cursor", new BMessage('curs'), '*'));
			}
		}
	
		app->Run();
		delete app;
	}
	
	return 0;
}