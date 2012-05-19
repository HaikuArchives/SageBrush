/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */

#include "TestWinAndApp.h"

int main(int argc, char *argv[])
{
	App *app = new App();
	
	app->Run();
	delete app;
}