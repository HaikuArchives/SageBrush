#include <Application.h>
#include <Window.h>
#include <Debug.h>

#ifndef W_WIDTH
#define W_WIDTH 320
#endif

#ifndef W_HEIGHT
#define W_HEIGHT 200
#endif

#ifndef MESSAGERECEIVED
#define MESSAGERECEIVED virtual void MessageReceived(BMessage *msg) { BWindow::MessageReceived(msg); }
#endif

#ifndef WINDOW_PUBLIC
#define WINDOW_PUBLIC
#endif

class Win: public BWindow
{
  public:
  	WINDOW_PUBLIC
  
	Win(): BWindow(BRect(0, 0, W_WIDTH, W_HEIGHT).OffsetBySelf(100, 100),
		"Test", B_DOCUMENT_WINDOW, B_QUIT_ON_WINDOW_CLOSE)
	{
	}
	
	~Win()
	{
	}
	
	MESSAGERECEIVED
};

class App: public BApplication
{
  public:
  	Win *win;
  
	App(): BApplication("application/x-vnd.Test"), win(0)
	{
		win = new Win();
		
		if (win)
			win->Show();
	}
	
	~App()
	{
	}

	virtual void MessageReceived(BMessage *msg) 
	{
		msg->PrintToStream();
	}	
};