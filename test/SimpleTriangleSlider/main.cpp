/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */
#include <View.h>
#include <Box.h>
#include <InterfaceDefs.h>
#include <Slider.h>
#include "TestWinAndApp.h"
#include "SimpleTriangleSlider.h"

int main(int argc, char *argv[])
{
	App *app = new App();
	
	if (app)
	{
		if (app->win)
		{
			BView *view = new BView(app->win->Bounds(), "test_view", B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
			
			if (view)
			{
				view->SetViewColor(ui_color(B_TOOL_TIP_BACKGROUND_COLOR));
				app->win->AddChild(view);
				
				SimpleTriangleSlider *slider = 
					new SimpleTriangleSlider(BRect(50, 50, 250, 68), 
					"test_slider", NULL, 0, 255);
				
				if (slider)
				{
					slider->MoveTo(B_ORIGIN + BPoint(50, 50));
					view->AddChild(slider);
				}
				
			}
		}		
		
		app->Run();
		delete app;
	}

	return 0;
}