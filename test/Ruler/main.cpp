/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */
#include "TestWinAndApp.h"

#include <View.h>
#include "Ruler.h"
#include <Debug.h>

int main(int argc, char *argv[])
{
	App *app = new App();

	if (app)
	{
		if (app->win)
		{
			BView *view = new BView(app->win->Bounds(), "t", B_FOLLOW_ALL, B_WILL_DRAW);			
			view->SetViewColor(145, 89, 200);			
			app->win->AddChild(view);
			
			if (view)
			{
				Ruler *ruler = new Ruler(B_HORIZONTAL, B_ALIGN_BOTTOM, 20, 20);
				
				if (ruler)
					view->AddChild(ruler);
				
				ruler = new Ruler(B_VERTICAL, B_ALIGN_RIGHT, 20, 20);

				if (ruler)
				{
					ruler->SetRange(0, 200);
					ruler->SetUnits(Units::PIXELS);					
					view->AddChild(ruler);
				}
				
				ruler = new Ruler(B_HORIZONTAL, B_ALIGN_TOP, 20, 20);
				
				if (ruler)
				{
					ruler->SetRange(0, 200);
					ruler->SetUnits(Units::INCHES);
					view->AddChild(ruler);
				}

				ruler = new Ruler(B_VERTICAL, B_ALIGN_LEFT, 20, 20);
				
				if (ruler)
				{
					ruler->SetUnits(Units::PICAS);
					view->AddChild(ruler);
				}
			}
		}
	
		app->Run();
		delete app;
	}

	return 0;
}