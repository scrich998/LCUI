/** ******************************************************************************
 * @file	bitmapfont.c
 * @brief	The Bitmap Font operation set.
 * @author	Liu Chao <lc-soft@live.cn>
 * @warning
 * Copyright (C) 2012-2014 by							\n
 * Liu Chao									\n
 * 										\n
 * This file is part of the LCUI project, and may only be used, modified, and	\n
 * distributed under the terms of the GPLv2.					\n
 * 										\n
 * (GPLv2 is abbreviation of GNU General Public License Version 2)		\n
 * 										\n
 * By continuing to use, modify, or distribute this file you indicate that you	\n
 * have read the license and understand and accept it fully.			\n
 *  										\n
 * The LCUI project is distributed in the hope that it will be useful, but 	\n
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 	\n
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.	\n
 * 										\n
 * You should have received a copy of the GPLv2 along with this file. It is 	\n
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.	\n
 * ******************************************************************************/
 
/** ******************************************************************************
 * @file	bitmapfont.c
 * @brief	位图字体的操作集
 * @author	刘超 <lc-soft@live.cn>
 * @warning
 * 版权所有 (C) 2012-2014 归属于						\n
 * 刘超										\n
 * 										\n
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。	\n
 * 										\n
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)				\n
 * 										\n
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。	\n
 * 										\n
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特定\n
 * 用途的隐含担保，详情请参照GPLv2许可协议。					\n
 * 										\n
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果	\n
 * 没有，请查看：<http://www.gnu.org/licenses/>. 				\n
 * ******************************************************************************/

//#define LCUI_FONT_ENGINE_FREETYPE
//#define DEBUG
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/font.h>

//#undef LCUI_FONT_ENGINE_FREETYPE
#ifdef LCUI_FONT_ENGINE_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H

#define LCUI_FONT_RENDER_MODE	FT_RENDER_MODE_NORMAL
#define LCUI_FONT_LOAD_FALGS	(FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT)

/** 获取FT库句柄 */
FT_Library FontLIB_GetLibrary(void);

/** 获取指定字体ID的字体face对象句柄 */
FT_Face FontLIB_GetFontFace( int font_id );

#elif defined LCUI_BUILD_IN_WIN32

#include <wingdi.h>

test()
{
        MAT2 glyph_mat = {{0,1},{0,0},{0,0},{0,1}};
        int buff_len;
        GLYPHMETRICS glyph_metrics;
        unsigned char *buff;
        buff_len = GetGlyphOutline( NULL, L'a', GGO_BITMAP, &glyph_metrics, 0, NULL, &glyph_mat );
        GetGlyphOutline( NULL, L'a', GGO_BITMAP, &glyph_metrics, buff_len, buff, &glyph_mat);
}

#endif

/** 检测位图数据是否有效 */
#define FontBMP_IsValid(fbmp) (fbmp && fbmp->width>0 && fbmp->rows>0)

/** 打印字体位图的信息 */
LCUI_API void FontBMP_PrintInfo( LCUI_FontBMP *bitmap )
{
	printf("address:%p\n",bitmap);
	if( !bitmap ) {
		return;
	}
	printf("top: %d, left: %d, width:%d, rows:%d\n",
	bitmap->top, bitmap->left, bitmap->width, bitmap->rows);
}

/** 初始化字体位图 */
LCUI_API void FontBMP_Init( LCUI_FontBMP *bitmap )
{
	bitmap->rows = 0;
	bitmap->width = 0;
	bitmap->top = 0;
	bitmap->left = 0;
	bitmap->buffer = NULL;
}

/** 释放字体位图占用的资源 */
LCUI_API void FontBMP_Free( LCUI_FontBMP *bitmap )
{
	if( FontBMP_IsValid(bitmap) ) {
		free( bitmap->buffer );
		FontBMP_Init( bitmap );
	}
}

/** 创建字体位图 */
LCUI_API int FontBMP_Create( LCUI_FontBMP *bitmap, int width, int rows )
{
	size_t size;
	if(width < 0 || rows < 0) {
		FontBMP_Free(bitmap);
		return -1;
	}
	if(FontBMP_IsValid(bitmap)) {
		FontBMP_Free(bitmap);
	}
	bitmap->width = width;
	bitmap->rows = rows;
	size = width*rows*sizeof(uchar_t);
	bitmap->buffer = (uchar_t*)malloc( size );
	if( bitmap->buffer == NULL ) {
		return -2;
	}
	return 0;
}

/** 根据字符编码，获取内置的字体位图 */
static void GetDefaultFontBMP( unsigned short code, LCUI_FontBMP *out_bitmap )
{
	int i,j, start, m;
	uchar_t const *p;

	p = in_core_font_8x8(); /* 获取指向内置字体位图数组的指针 */
	FontBMP_Create(out_bitmap, 8, 8); /* 为位图分配内存，8x8的尺寸 */
	out_bitmap->left = 0;
	out_bitmap->top = 0;
	if(code < 256) { /* 如果在ASCII编码范围内 */
		if(code == ' ') { /* 空格就直接置0 */
			memset(out_bitmap->buffer, 0, sizeof(uchar_t)*64);
		} else {
			start = code * 8;
			for (i=start;i<start+8;++i) {
				m = (i-start) * 8 + 7;
				/* 将数值转换成一行位图 */
				for (j=0;j<8;++j,--m) {
					out_bitmap->buffer[m]
					 = (p[i]&(1<<j))?255:0;
				}
			}
		}
	} else { /* 否则用“口”表示未知编码字符 */
		static uchar_t null_bmp[] = {
			1,1,1,1,1,1,1,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,1,1,1,1,1,1,1 };
		for (i=0;i<8;i++) {
			m = i*8;
			for (j=0;j<8;j++,++m) {
				out_bitmap->buffer[m] = (null_bmp[m]>0)?255:0;
			}
		}
	}
	/* 修改相关属性 */
	out_bitmap->top = 8;
	out_bitmap->left = 0;
	out_bitmap->rows = 8;
	out_bitmap->width = 8;
	out_bitmap->advance.x = 8;
	out_bitmap->advance.y = 8;
}

/** 在屏幕打印以0和1表示字体位图 */
LCUI_API int FontBMP_Print( LCUI_FontBMP *fontbmp )
{
	int x,y,m;
	for(y = 0;y < fontbmp->rows; ++y){
		m = y*fontbmp->width;
		for(x = 0; x < fontbmp->width; ++x,++m){
			if(fontbmp->buffer[m] > 128) {
				printf("#");
			} else if(fontbmp->buffer[m] > 64) {
				printf("-");
			} else {
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

/** 将字体位图绘制到目标图像上 */
int FontBMP_Mix( LCUI_Graph *graph, LCUI_Pos pos, 
		 LCUI_FontBMP *bmp, LCUI_Color color )
{
	int val, x, y;
	LCUI_Graph *des;
	LCUI_Rect des_rect, cut;
	LCUI_ARGB *px_des, *px_row_des;
	uchar_t *bmp_src, *bmp_row_src, *byte_row_des, *byte_des;

	/* 数据有效性检测 */
	if( !FontBMP_IsValid( bmp ) || !Graph_IsValid( graph ) ) {
		return -1;
	}
	/* 获取背景图形的有效区域 */
	Graph_GetValidRect( graph, &des_rect );
	/* 获取背景图引用的源图形 */
	des = Graph_GetQuote( graph );
	/* 起点位置的有效性检测 */
	if(pos.x > des->w || pos.y > des->h) {
		return -2;
	}
	/* 获取需要裁剪的区域 */
	LCUIRect_GetCutArea( Size( des_rect.width, des_rect.height ),
			Rect( pos.x, pos.y, bmp->width, bmp->rows ), &cut );
	pos.x += cut.x;
	pos.y += cut.y;
	if( graph->color_type == COLOR_TYPE_ARGB ) {
		bmp_row_src = bmp->buffer + cut.y*bmp->width + cut.x;
		px_row_des = des->argb + (pos.y + des_rect.y) * des->w;
		px_row_des += pos.x + des_rect.x;
		for( y=0; y<cut.h; ++y ) {
			bmp_src = bmp_row_src;
			px_des = px_row_des;
			for( x=0; x<cut.w; ++x ) {
				ALPHA_BLEND( px_des->r, color.r, *bmp_src );
				ALPHA_BLEND( px_des->g, color.g, *bmp_src );
				ALPHA_BLEND( px_des->b, color.b, *bmp_src );
				if( px_des->alpha == 255 || *bmp_src == 255 ) {
					px_des->alpha = 255;
				} else {
					val = (255 - *bmp_src)*(255 - px_des->alpha);
					px_des->alpha = (uchar_t)(255 - val / 65025);
				}
				++bmp_src;
				++px_des;
			}
			px_row_des += des->w;
			bmp_row_src += bmp->width;
		}
		return 0;
	}
	
	bmp_row_src = bmp->buffer + cut.y*bmp->width + cut.x;
	byte_row_des = des->bytes + (pos.y + des_rect.y) * des->bytes_per_row;
	byte_row_des += (pos.x + des_rect.x)*des->bytes_per_pixel;
	for( y=0; y<cut.h; ++y ) { 
		bmp_src = bmp_row_src;
		byte_des = byte_row_des;
		for( x=0; x<cut.w; ++x ) {
			ALPHA_BLEND( *byte_des, color.b, *bmp_src );
			byte_des++;
			ALPHA_BLEND( *byte_des, color.g, *bmp_src );
			byte_des++;
			ALPHA_BLEND( *byte_des, color.r, *bmp_src );
			byte_des++;
			++bmp_src;
		}
		byte_row_des += des->w*3;
		bmp_row_src += bmp->width;
	}
	return 0;
}

/* 如果定义了LCUI_FONT_ENGINE_FREETYPE宏定义，则使用FreeType字体引擎处理字体数据 */
#ifdef LCUI_FONT_ENGINE_FREETYPE

/** 转换FT_GlyphSlot类型数据为LCUI_FontBMP */
static int Convert_FTGlyph( LCUI_FontBMP *bmp, FT_GlyphSlot slot, int mode )
{
	int error;
	size_t size;
	FT_BitmapGlyph bitmap_glyph;
	FT_Glyph  glyph;

	/* 从字形槽中提取一个字形图像
	 * 请注意，创建的FT_Glyph对象必须与FT_Done_Glyph成对使用 */
	error = FT_Get_Glyph( slot, &glyph );
	if(error) {
		return -1;
	}
	/*---------------------- 打印字体信息 --------------------------
	printf(" width= %ld,  met->height= %ld\n"
	"horiBearingX = %ld, horiBearingY = %ld, horiAdvance = %ld\n"
	"vertBearingX = %ld, vertBearingY = %ld,  vertAdvance = %ld\n",
	slot->metrics.width>>6, slot->metrics.height>>6,
	slot->metrics.horiBearingX>>6, slot->metrics.horiBearingY>>6,
	slot->metrics.horiAdvance>>6, slot->metrics.vertBearingX>>6,
	slot->metrics.vertBearingY>>6, slot->metrics.vertAdvance>>6 );
	------------------------------------------------------------*/
	if ( glyph->format != FT_GLYPH_FORMAT_BITMAP ) {
		error = FT_Glyph_To_Bitmap(&glyph, mode, 0 ,1);
		if(error) {
			return -1;
		}
	}
	bitmap_glyph = (FT_BitmapGlyph)glyph;
	/*
	 * FT_Glyph_Metrics结构体中保存字形度量，通过face->glyph->metrics结
	 * 构访问，可得到字形的宽、高、左边界距、上边界距、水平跨距等等。
	 * 注意：因为不是所有的字体都包含垂直度量，当FT_HAS_VERTICAL为假时，
	 * vertBearingX，vertBearingY和vertAdvance的值是不可靠的，目前暂不考虑
	 * 此情况的处理。
	 * */
	bmp->top = bitmap_glyph->top;
	bmp->left = slot->metrics.horiBearingX>>6;
	bmp->rows = bitmap_glyph->bitmap.rows;
	bmp->width = bitmap_glyph->bitmap.width;
	bmp->advance.x = slot->metrics.horiAdvance>>6;	/* 水平跨距 */
	bmp->advance.y = slot->metrics.vertAdvance>>6;	/* 垂直跨距 */
	/* 分配内存，用于保存字体位图 */
	size = bmp->rows * bmp->width * sizeof(uchar_t);
	bmp->buffer = (uchar_t*)malloc( size );
	if( !bmp->buffer ) {
		FT_Done_Glyph(glyph);
		return -1;
	}

	switch( bitmap_glyph->bitmap.pixel_mode ) {
	    /* 8位灰度位图，直接拷贝 */
	    case FT_PIXEL_MODE_GRAY:
		memcpy( bmp->buffer, bitmap_glyph->bitmap.buffer, size );
		break;
	    /* 单色点阵图，需要转换 */
	    case FT_PIXEL_MODE_MONO: {
		FT_Bitmap bitmap;
		FT_Library lib;
		FT_Int x, y;
		uchar_t *t, *s;

		lib = FontLIB_GetLibrary();
		FT_Bitmap_New( &bitmap );
		/* 转换位图bitmap_glyph->bitmap至bitmap，1个像素占1个字节 */
		FT_Bitmap_Convert( lib, &bitmap_glyph->bitmap, &bitmap, 1);
		s = bitmap.buffer;
		t = bmp->buffer;
		for( y=0; y<bmp->rows; ++y ) {
			for( x=0; x<bmp->width; ++x ) {
				*t = *s?255:0;
				++t,++s;
			}
		}
		FT_Bitmap_Done( lib, &bitmap );
		break;
	    }
	    /* 其它像素模式的位图，暂时先直接填充255，等需要时再完善 */
	    default:
		memset( bmp->buffer, 255, size );
		break;
	}
	FT_Done_Glyph(glyph);
	return size;
}

#endif

/** 载入字体位图 */
LCUI_API int FontBMP_Load( LCUI_FontBMP *buff, int font_id, 
				wchar_t ch, int pixel_size )
{
#ifdef LCUI_FONT_ENGINE_FREETYPE
	size_t size;
	LCUI_BOOL have_space = FALSE;

	FT_Face face;
	int error;

	if( font_id > 0 ) {
		face = FontLIB_GetFontFace( font_id );
		if( !face ) {
			GetDefaultFontBMP( ch, buff );
			return -1;
		}
	} else {
		GetDefaultFontBMP( ch, buff );
		return -1;
	}

	/* 设定字体尺寸 */
	FT_Set_Pixel_Sizes( face, 0, pixel_size );
	/* 如果是空格 */
	if( ch == ' ' ) {
		ch = 'a';
		have_space = TRUE;
	}
	/* 这个函数只是简单地调用FT_Get_Char_Index和FT_Load_Glyph */
	error = FT_Load_Char( face, ch, LCUI_FONT_LOAD_FALGS );
	if(error) {
		return error;
	}
	size = Convert_FTGlyph( buff, face->glyph, LCUI_FONT_RENDER_MODE );
	/* 如果是空格则将位图内容清空 */
	if( have_space ) {
		memset( buff->buffer, 0, size );
	}
	return 0;
#else
	GetDefaultFontBMP( ch, buff );
	return -1;
#endif
}

