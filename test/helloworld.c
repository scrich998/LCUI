﻿#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/widget.h>
#include <LCUI/surface.h>
#include <LCUI/display.h>

#include <io.h>
#include <fcntl.h>

/* 在运行程序时会打开控制台，以查看打印的调试信息 */
static void InitConsoleWindow(void)
{
	int hCrt;
	FILE *hf;
	AllocConsole();
	hCrt=_open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE),_O_TEXT );
	hf=_fdopen( hCrt, "w" );
	*stdout=*hf;
	setvbuf (stdout, NULL, _IONBF, 0);
	// test code
	printf ("InitConsoleWindow OK!\n");
}

void onTimer( void *arg )
{
	static int mode = LDM_SEAMLESS;
	_DEBUG_MSG("tip\n");
	LCUIDisplay_SetMode( mode );
	if( mode == LDM_SEAMLESS ) {
		mode = LDM_WINDOWED;
	} else {
		mode = LDM_SEAMLESS;
	}
}

int main( int argc, char **argv )
{
	LCUI_Widget w;
	LCUI_Graph image;

	InitConsoleWindow();
	LCUI_Init();
	LCUIDisplay_SetMode( LDM_WINDOWED );
	LCUIDisplay_SetSize( 800, 600 );
	w = Widget_New(NULL);
	Widget_Top( w );
	Widget_Show( w );
	Widget_Resize( w, 320, 240 );
	Widget_Move( w, 100, 200 );
	Widget_SetTitleW( w, L"测试" );
	Graph_Init( &image ); 
	_DEBUG_MSG( "load image, result: %d\n", Graph_LoadImage( "background-image.png", &image ) );
	Widget_PullStyle( w, WSS_BACKGROUND | WSS_SHADOW | WSS_BORDER );
	w->style.background.color.value = 0xf6f6f6;
	w->style.background.image = image; 
	w->style.background.size.w.scale = 0.50;
	w->style.background.size.h.px = 200;
	w->style.background.size.w.type = SVT_SCALE;
	w->style.background.size.h.type = SVT_PX;
	w->style.background.size.using_value = FALSE;
	w->style.background.position.using_value = TRUE;
	w->style.background.position.value = SV_BOTTOM_CENTER;
	w->style.shadow.color = ARGB( 50, 0, 0, 0 );
	w->style.shadow.x = 1;
	w->style.shadow.y = 1;
	w->style.shadow.spread = 5;
	w->style.shadow.blur = 5;
	w->style.border.top.width = 4;
	w->style.border.top.color = RGB( 255, 0, 0 );
	w->style.border.right.width = 6;
	w->style.border.right.color = RGB( 0, 255, 0 );
	w->style.border.bottom.width = 8;
	w->style.border.bottom.color = RGB( 0, 0, 255 );
	w->style.border.left.width = 10;
	Widget_PushStyle( w, WSS_BACKGROUND | WSS_SHADOW | WSS_BORDER );
	LCUITimer_Set( 2000, onTimer, NULL, TRUE );
	return LCUI_Main();
}
