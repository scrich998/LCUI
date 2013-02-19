/* ***************************************************************************
 * button.c -- LCUI‘s Button widget
 * 
 * Copyright (C) 2012 by
 * Liu Chao
 * 
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 * 
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 * 
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *  
 * The LCUI project is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 * 
 * You should have received a copy of the GPLv2 along with this file. It is 
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/
 
/* ****************************************************************************
 * button.c -- LCUI 的按钮部件
 *
 * 版权所有 (C) 2012 归属于 
 * 刘超
 * 
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 * 
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 * 
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>. 
 * ****************************************************************************/
//#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_LABEL_H
#include LC_BUTTON_H
#include LC_GRAPH_H
#include LC_DRAW_H
#include LC_MISC_H
#include LC_INPUT_H

static void Button_ExecDefalutUpdate( LCUI_Widget *widget )
{
	LCUI_Border border;
	
	border = Border(1, BORDER_STYLE_SOLID, RGB(170,170,170));
	Border_Radius( &border, 2 );
	Widget_SetBorder( widget, border );
	Widget_SetBackgroundTransparent( widget, FALSE );
	
	switch( widget->state ) {
	case WIDGET_STATE_NORMAL:
		Widget_SetBackgroundColor( widget, RGB(225,225,225) );
		break;
	case WIDGET_STATE_OVERLAY :
		Widget_SetBackgroundColor( widget, RGB(170,215,230) );
		break;
	case WIDGET_STATE_ACTIVE :
		Widget_SetBackgroundColor( widget, RGB(255,180,45) );
		break;
	case WIDGET_STATE_DISABLE :
		Widget_SetBackgroundColor( widget, RGB(200,200,200) );
		break;
		default : break;
	}
}

static void Button_ExecCustomUpdate( LCUI_Widget *widget )
{
	LCUI_Button *btn;
	LCUI_Graph *img;
	
	btn = Widget_GetPrivData( widget );
	switch(widget->state) {
	case WIDGET_STATE_NORMAL: img = &btn->btn_normal; break;
	case WIDGET_STATE_OVERLAY: img = &btn->btn_over; break;
	case WIDGET_STATE_ACTIVE: img = &btn->btn_down; break;
	case WIDGET_STATE_DISABLE: img = &btn->btn_disable; break;
	default : img = NULL; break;
	}
	/* 如果图像不可用，则使用默认样式 */
	if( !Graph_Valid(img) ) {
		Button_ExecDefalutUpdate( widget );
	} else {
		Widget_SetBackgroundImage( widget, img );
		Widget_SetBackgroundTransparent( widget, TRUE );
		Widget_SetBackgroundLayout( widget, LAYOUT_STRETCH );
	}
}

static void Button_ExecUpdate( LCUI_Widget *widget )
{
	if(widget->style_id == BUTTON_STYLE_CUSTOM) {
		Button_ExecCustomUpdate( widget );
	} else {
		Button_ExecDefalutUpdate( widget );
	}
	Refresh_Widget( widget );
}

#ifdef use_this_code
static void Exec_Update_Button(LCUI_Widget *widget)
/* 功能：更新按钮的图形数据 */
{
	LCUI_RGB color;
	LCUI_Button *button;
	LCUI_Graph *graph;
	
	DEBUG_MSG("Exec_Update_Button(): enter\n");
	graph = Widget_GetSelfGraph( widget );
	button = (LCUI_Button *)Widget_GetPrivData(widget);
	/* 根据按钮的不同风格来处理 */
	if(Strcmp(&widget->style_name, "custom") == 0) {
		int no_bitmap = 0;
		if( !widget->enabled ) {
			widget->state = WIDGET_STATE_DISABLE;
		}
		/* 判断按钮的状态，以选择相应的背景色 */

	}
	else if(Strcmp(&widget->style_name, "menu_style") == 0){
		/* 菜单默认使用的按钮风格 */ 
		switch(widget->state) {
		case WIDGET_STATE_NORMAL :
			Graph_Free(&widget->background.image);
			Graph_Fill_Alpha(graph, 0);  
			break;
		case WIDGET_STATE_OVERLAY :
			color = RGB(80, 180, 240);
			Graph_Fill_Color(graph, color);
			Graph_Draw_Border(graph, 
			 Border(1, BORDER_STYLE_SOLID, RGB(50,50,255)) );
			Graph_Fill_Alpha(graph, 255);
			break;
		case WIDGET_STATE_ACTIVE :
			color = RGB(80, 170, 255);
			Graph_Fill_Color(graph, color);
			Graph_Draw_Border(graph, 
			 Border(1, BORDER_STYLE_SOLID, RGB(50,50,255)) );
			Graph_Fill_Alpha(graph, 255);
			break;
		case WIDGET_STATE_DISABLE :
			Graph_Free(&widget->background.image);
			Graph_Fill_Alpha(graph, 0); 
			break;
			default :
			break;
		}
	} else {/* 如果按钮的风格为缺省 */ 
		Strcpy(&widget->style_name, "default");
		switch(widget->state) { 
		case WIDGET_STATE_NORMAL :
			color = RGB(30, 145, 255); 
			break;
		case WIDGET_STATE_OVERLAY :
			color = RGB(50, 180, 240);  
			break;
		case WIDGET_STATE_ACTIVE :
			color = RGB(255, 50, 50); 
			break;
		case WIDGET_STATE_DISABLE :
			color = RGB(190, 190, 190); 
			break;
			default : break;
		} 
		Graph_Fill_Alpha(graph, 255);
		Graph_Fill_Image(graph, 
			&widget->background.image, 0, color); 
		Graph_Draw_Border(graph, 
		 Border(1, BORDER_STYLE_SOLID, RGB(0,0,0)) );
	}
	/* 按钮每次更新都需要更新整个按钮区域内的图形 */ 
	Refresh_Widget(widget); 
	DEBUG_MSG("Exec_Update_Button(): quit\n");
}
#endif

static void Button_Init(LCUI_Widget *widget)
/* 功能：初始化按钮部件的数据 */
{
	int valid_state;
	LCUI_Button *button;
	
	button = WidgetPrivData_New(widget, sizeof(LCUI_Button));
	/* 初始化图像数据 */ 
	Graph_Init(&button->btn_disable);
	Graph_Init(&button->btn_normal);
	Graph_Init(&button->btn_focus);
	Graph_Init(&button->btn_down);
	Graph_Init(&button->btn_over);
	
	valid_state = (WIDGET_STATE_NORMAL | WIDGET_STATE_ACTIVE);
	valid_state |= (WIDGET_STATE_DISABLE | WIDGET_STATE_OVERLAY);
	Widget_SetValidState( widget, valid_state );
	button->label = Widget_New("label");/* 创建label部件 */ 
	/* 将按钮部件作为label部件的容器 */
	Widget_Container_Add(widget, button->label);
	/* label部件居中显示 */
	Widget_SetAlign(button->label, ALIGN_MIDDLE_CENTER, Pos(0,0));
	Widget_Show(button->label); /* 显示label部件 */
	/* 启用自动尺寸调整，以适应内容 */
	Widget_SetAutoSize( widget, TRUE, AUTOSIZE_MODE_GROW_AND_SHRINK);
	Widget_SetStyleID( widget, BUTTON_STYLE_DEFAULT );
}

LCUI_Widget *Get_Button_Label(LCUI_Widget *widget)
/* 功能：获取嵌套在按钮部件里的label部件 */
{
	LCUI_Button *button = (LCUI_Button*)Widget_GetPrivData(widget);
	return button->label;
}

/* 自定义按钮在各种状态下显示的位图 */
void Button_CustomStyle(	LCUI_Widget *widget, LCUI_Graph *normal, 
				LCUI_Graph *over, LCUI_Graph *down, 
				LCUI_Graph *focus, LCUI_Graph *disable)
{
	LCUI_Button *btn_data;
	btn_data = Widget_GetPrivData(widget);
	if( Graph_Valid(normal) ) {
		btn_data->btn_normal = *normal;
	} else {
		Graph_Init( &btn_data->btn_normal );
	}
	if( Graph_Valid(over) ) {
		btn_data->btn_over = *over;
	} else {
		Graph_Init( &btn_data->btn_over );
	}
	if( Graph_Valid(down) ) {
		btn_data->btn_down = *down;
	} else {
		Graph_Init( &btn_data->btn_down );
	}
	if( Graph_Valid(focus) ) {
		btn_data->btn_focus = *focus;
	} else {
		Graph_Init( &btn_data->btn_focus );
	}
	if( Graph_Valid(disable) ) {
		btn_data->btn_disable = *disable;
	} else {
		Graph_Init( &btn_data->btn_disable );
	}
	/* 设定为自定义风格 */
	Widget_SetStyleID(widget, BUTTON_STYLE_CUSTOM);
	Widget_Draw(widget); /* 重新绘制部件 */
}

void Set_Button_Text(LCUI_Widget *widget, const char *fmt, ...)
/* 功能：设定按钮部件显示的文本内容 */
{
	char text[LABEL_TEXT_MAX_SIZE];
	LCUI_Button *button = (LCUI_Button*)Widget_GetPrivData(widget);
	LCUI_Widget *label = button->label;  
	
	memset(text, 0, sizeof(text));
	/* 由于是可变参数，让vsnprintf函数根据参数将字符串保存至text中 */
	va_list ap; 
	va_start(ap, fmt);
	vsnprintf(text, LABEL_TEXT_MAX_SIZE-1, fmt, ap);
	va_end(ap);
	/* 设定部件显示的文本 */
	Label_Text(label, text);
}


LCUI_Widget *Create_Button_With_Text(const char *fmt, ...)
/* 功能：创建一个带文本内容的按钮 */
{
	char text[LABEL_TEXT_MAX_SIZE];
	LCUI_Widget *widget = Widget_New("button");
	
	memset(text, 0, sizeof(text)); 
    
	va_list ap; 
	va_start(ap, fmt);
	vsnprintf(text, LABEL_TEXT_MAX_SIZE-1, fmt, ap);
	va_end(ap); 
	
	Set_Button_Text(widget, text);
	return widget;
}


void Register_Button()
/*功能：将按钮部件类型注册至部件库 */
{
	/* 添加部件类型 */
	WidgetType_Add("button");
	
	/* 为部件类型关联相关函数 */
	WidgetFunc_Add("button", Button_Init,		FUNC_TYPE_INIT);
	WidgetFunc_Add("button", Button_ExecUpdate,	FUNC_TYPE_UPDATE);
}
