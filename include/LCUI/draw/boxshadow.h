﻿/* ****************************************************************************
* boxshadow.h -- graph box shadow draw support.
* 
* Copyright (C) 2014 by Liu Chao <lc-soft@live.cn>
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
* ***************************************************************************/

/* ****************************************************************************
* boxshadow.h -- 矩形阴影绘制支持
*
* 版权所有 (C) 2014-2015 归属于 刘超 <lc-soft@live.cn>
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
* ***************************************************************************/

#ifndef __LCUI_DRAW_BOXSHADOW_H__
#define __LCUI_DRAW_BOXSHADOW_H__

#define BLUR_N			1.5
#define SHADOW_WIDTH(sd)	(int)(sd->blur*(BLUR_N-1.0) + shadow->spread)
#define BLUR_WIDTH(sd)		(int)(sd->blur*BLUR_N)
#define INNER_SHADOW_WIDTH(sd)	(SHADOW_WIDTH(sd)-BLUR_WIDTH(sd))

static inline LCUI_BoxShadow BoxShadow( int x, int y, int blur, LCUI_Color color )
{
	LCUI_BoxShadow shadow;
	shadow.x = x;
	shadow.y = y;
	shadow.blur = blur;
	shadow.spread = 0;
	shadow.color = color;
	return shadow;
}

static inline int BoxShadow_GetBoxWidth( LCUI_BoxShadow *shadow, int w )
{
	/* 如果水平向右的偏移距离大于阴影宽度，说明内容区左上角与容器一致 */
	if( shadow->x >= SHADOW_WIDTH(shadow) ) {
		return w - SHADOW_WIDTH(shadow)*2
			 - (shadow->x - SHADOW_WIDTH(shadow));
	}
	/* 如果水平向左的偏移距离大于阴影宽度，说明阴影区左上角与容器一致 */
	else if( shadow->x <= -SHADOW_WIDTH(shadow) ) {
		return w - SHADOW_WIDTH(shadow)*2
			 + shadow->x + SHADOW_WIDTH(shadow);
	}
	/* 水平偏移距离没有超出阴影宽度，容器大小会是固定的，所以直接减
	 * 去两边的阴影宽度即可得到内容区宽度  
	 */
	return w - SHADOW_WIDTH(shadow)*2;
}

static inline int BoxShadow_GetBoxHeight( LCUI_BoxShadow *shadow, int h )
{
	if( shadow->y >= SHADOW_WIDTH(shadow) ) {
		return h - SHADOW_WIDTH(shadow)*2
			 - (shadow->y - SHADOW_WIDTH(shadow));
	}
	else if( shadow->y <= -SHADOW_WIDTH(shadow) ) {
		return h - SHADOW_WIDTH(shadow)*2
			 + shadow->y + SHADOW_WIDTH(shadow);
	}
	return h - SHADOW_WIDTH(shadow)*2 + shadow->y;
}

/** 计算Box在添加阴影后的宽度 */
static inline int BoxShadow_GetWidth( LCUI_BoxShadow *shadow, int box_w )
{
	if( shadow->x >= SHADOW_WIDTH(shadow) ) {
		return box_w + SHADOW_WIDTH(shadow) + shadow->x;
	}
	else if( shadow->x <= -SHADOW_WIDTH(shadow) ) {
		return box_w + SHADOW_WIDTH(shadow) - shadow->x;
	}
	return box_w + SHADOW_WIDTH(shadow)*2;
}

/** 计算Box在添加阴影后的高度 */
static inline int BoxShadow_GetHeight( LCUI_BoxShadow *shadow, int box_h )
{
	if( shadow->y >= SHADOW_WIDTH(shadow) ) {
		return box_h + SHADOW_WIDTH(shadow) + shadow->y;
	}
	else if( shadow->y <= -SHADOW_WIDTH(shadow) ) {
		return box_h + SHADOW_WIDTH(shadow) - shadow->y;
	}
	return box_h + SHADOW_WIDTH(shadow)*2;
}

static inline int BoxShadow_GetBoxX( LCUI_BoxShadow *shadow )
{
	if( shadow->x >= SHADOW_WIDTH(shadow) ) {
		return 0;
	}
	return SHADOW_WIDTH(shadow) - shadow->x;
}

static inline int BoxShadow_GetBoxY( LCUI_BoxShadow *shadow )
{
	if( shadow->y >= SHADOW_WIDTH(shadow) ) {
		return 0;
	}
	return SHADOW_WIDTH(shadow) - shadow->y;
}

static inline int BoxShadow_GetY( LCUI_BoxShadow *shadow )
{
	if( shadow->y <= SHADOW_WIDTH(shadow) ) {
		return 0;
	}
	return shadow->y - SHADOW_WIDTH(shadow);
}

static inline int BoxShadow_GetX( LCUI_BoxShadow *shadow )
{
	if( shadow->x <= SHADOW_WIDTH(shadow) ) {
		return 0;
	}
	return shadow->x - SHADOW_WIDTH(shadow);
}

LCUI_API void BoxShadow_Init( LCUI_BoxShadow *shadow );

LCUI_API void Graph_ClearShadowArea( LCUI_PaintContext paint, LCUI_Rect *box,
				     LCUI_BoxShadow *shadow );

LCUI_API int Graph_DrawBoxShadow( LCUI_PaintContext paint, LCUI_Rect *box,
				  LCUI_BoxShadow *shadow );

#endif
