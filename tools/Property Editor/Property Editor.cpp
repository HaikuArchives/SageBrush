#include <Application.h>
#include "PropertyWindow.h"

class App: public BApplication
{
  public:
	App(): BApplication("application/x-vnd.Property_Editor")
	{
		PropertyWindow *win = new PropertyWindow();
		
		win->Show();
	}
	
	~App()
	{
	}
	
	virtual	void AboutRequested()
	{
	}
	
	virtual bool QuitRequested()
	{
		return true;
	}
};

int main(int argc, char *argv[])
{
	App *app = new App();
	
	app->Run();
	
	delete app;

	return 0;
}