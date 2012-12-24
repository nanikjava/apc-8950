/*++ 
Copyright (c) 2010 WonderMedia Technologies, Inc.

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software 
Foundation, either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details. You
should have received a copy of the GNU General Public License along with this
program. If not, see http://www.gnu.org/licenses/>.

WonderMedia Technologies, Inc.
4F, 531, Chung-Cheng Road, Hsin-Tien, Taipei 231, R.O.C.
--*/

#include <config.h>
#include <common.h>
#include <command.h>
#include <version.h>
#include <stdarg.h>
#include <linux/types.h>
#include <devices.h>
#include <linux/stddef.h>
#include <asm/byteorder.h>
#include <bmp_layout.h>

#include "wmt_display.h"
#include "minivgui.h"

typedef unsigned short GlyphScatter;

#define Memcpy(dest, src, n) memcpy(dest, src, n)


typedef struct {
    unsigned short ucs2Start;
    unsigned short ucs2End;
}GlyphRange;

//
//    字体文件布局
//   ----------------
//   |   文件头     |		// FontFileHead
//   ---------------|
//   | 区间信息数组 |		// 连续区间数据 GlyphRange[]
//   ----------------
//   | 离散信息数组 |		// 离散点数据	GlyphScatter[] 已经排序
//   ---------------|
//   |  missing点阵 |		// missing glyph 点阵，额外的一个点阵用于显示无法找到的字符
//   ----------------
//   |  区间1点阵   |		// 连续区间点阵数据
//   |  区间2点阵   |		// 连续区间点阵数据
//   |  区间...     |		// 连续区间点阵数据
//   ----------------
//   |  离散1点阵   |		// 离散的点阵
//   |  离散2点阵   |		// 离散的点阵
//   |  离散...     |		// 离散的点阵
//   ----------------

static const unsigned char s_fontData[]=
{
0x46,0x54,0x02,0x10,0x46,0x6F,0x6E,0x74,0x20,0x38,0x78,0x31,0x36,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x08,0x10,0x10,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x7F,0x00,
0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,
0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x10,0x10,0x10,0x10,0x10,0x10,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x38,0x7C,0x7C,0x7C,0x38,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xFE,0xFE,0xFE,0xFE,0xC6,0x82,0x82,0x82,0xC6,0xFE,0xFE,0xFE,0xFE,0xFE,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x1E,0x06,0x0A,0x0A,0x10,0x10,0x30,0x48,0x48,0x30,0x00,0x00,0x00,
0x00,0x00,0x00,0x38,0x44,0x44,0x38,0x10,0x7C,0x10,0x10,0x10,0x10,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x06,0x3A,0x26,0x3A,0x22,0x22,0x22,0x26,0x66,0x60,0x00,0x00,0x00,
0x00,0x00,0x00,0x54,0x54,0x28,0x28,0x6C,0x28,0x28,0x54,0x54,0x00,0x00,0x00,0x00,
0x00,0x10,0x10,0x10,0x10,0x10,0x10,0xFE,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,
0x00,0x00,0x02,0x06,0x0E,0x1E,0x3E,0x7E,0x3E,0x1E,0x0E,0x06,0x02,0x00,0x00,0x00,
0x00,0x10,0x38,0x54,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x54,0x38,0x10,0x00,0x00,
0x00,0x00,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x00,0x00,0x24,0x24,0x00,0x00,
0x00,0x00,0x34,0x54,0x54,0x54,0x54,0x54,0x34,0x14,0x14,0x14,0x14,0x14,0x00,0x00,
0x00,0x10,0x10,0x10,0x10,0x10,0x10,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,
0x00,0x10,0x10,0x10,0x10,0x10,0x10,0xF0,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,
0x00,0x00,0x10,0x38,0x54,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,
0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x1E,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x04,0x7E,0x04,0x08,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x20,0x7E,0x20,0x10,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x18,0x18,0x00,0x00,
0x00,0x12,0x36,0x24,0x48,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x24,0x24,0x24,0xFE,0x48,0x48,0x48,0xFE,0x48,0x48,0x48,0x00,0x00,
0x00,0x00,0x10,0x38,0x54,0x54,0x50,0x30,0x18,0x14,0x14,0x54,0x54,0x38,0x10,0x10,
0x00,0x00,0x00,0x44,0xA4,0xA8,0xA8,0xA8,0x54,0x1A,0x2A,0x2A,0x2A,0x44,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x60,0x60,0x20,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x02,0x04,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x08,0x08,0x04,0x02,0x00,
0x00,0x40,0x20,0x10,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x10,0x10,0x20,0x40,0x00,
0x00,0x00,0x00,0x00,0x10,0x10,0xD6,0x38,0x38,0xD6,0x10,0x10,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x10,0x10,0x10,0x10,0xFE,0x10,0x10,0x10,0x10,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x20,0xC0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00,0x00,
0x00,0x00,0x01,0x02,0x02,0x04,0x04,0x08,0x08,0x10,0x10,0x20,0x20,0x40,0x40,0x00,
0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00,
0x00,0x00,0x00,0x10,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,
0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x04,0x04,0x08,0x10,0x20,0x42,0x7E,0x00,0x00,
0x00,0x00,0x00,0x3C,0x42,0x42,0x04,0x18,0x04,0x02,0x02,0x42,0x44,0x38,0x00,0x00,
0x00,0x00,0x00,0x04,0x0C,0x14,0x24,0x24,0x44,0x44,0x7E,0x04,0x04,0x1E,0x00,0x00,
0x00,0x00,0x00,0x7E,0x40,0x40,0x40,0x58,0x64,0x02,0x02,0x42,0x44,0x38,0x00,0x00,
0x00,0x00,0x00,0x1C,0x24,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x24,0x18,0x00,0x00,
0x00,0x00,0x00,0x7E,0x44,0x44,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,
0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x3C,0x00,0x00,
0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x26,0x1A,0x02,0x02,0x24,0x38,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x20,
0x00,0x00,0x00,0x02,0x04,0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x04,0x02,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x40,0x20,0x10,0x08,0x04,0x02,0x04,0x08,0x10,0x20,0x40,0x00,0x00,
0x00,0x00,0x00,0x3C,0x42,0x42,0x62,0x02,0x04,0x08,0x08,0x00,0x18,0x18,0x00,0x00,
0x00,0x00,0x00,0x38,0x44,0x5A,0xAA,0xAA,0xAA,0xAA,0xB4,0x42,0x44,0x38,0x00,0x00,
0x00,0x00,0x00,0x10,0x10,0x18,0x28,0x28,0x24,0x3C,0x44,0x42,0x42,0xE7,0x00,0x00,
0x00,0x00,0x00,0xF8,0x44,0x44,0x44,0x78,0x44,0x42,0x42,0x42,0x44,0xF8,0x00,0x00,
0x00,0x00,0x00,0x3E,0x42,0x42,0x80,0x80,0x80,0x80,0x80,0x42,0x44,0x38,0x00,0x00,
0x00,0x00,0x00,0xF8,0x44,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x44,0xF8,0x00,0x00,
0x00,0x00,0x00,0xFC,0x42,0x48,0x48,0x78,0x48,0x48,0x40,0x42,0x42,0xFC,0x00,0x00,
0x00,0x00,0x00,0xFC,0x42,0x48,0x48,0x78,0x48,0x48,0x40,0x40,0x40,0xE0,0x00,0x00,
0x00,0x00,0x00,0x3C,0x44,0x44,0x80,0x80,0x80,0x8E,0x84,0x44,0x44,0x38,0x00,0x00,
0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x7E,0x42,0x42,0x42,0x42,0xE7,0x00,0x00,
0x00,0x00,0x00,0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,
0x00,0x00,0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x88,0xF0,
0x00,0x00,0x00,0xEE,0x44,0x48,0x50,0x70,0x50,0x48,0x48,0x44,0x44,0xEE,0x00,0x00,
0x00,0x00,0x00,0xE0,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x42,0xFE,0x00,0x00,
0x00,0x00,0x00,0xEE,0x6C,0x6C,0x6C,0x6C,0x54,0x54,0x54,0x54,0x54,0xD6,0x00,0x00,
0x00,0x00,0x00,0xC7,0x62,0x62,0x52,0x52,0x4A,0x4A,0x4A,0x46,0x46,0xE2,0x00,0x00,
0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00,
0x00,0x00,0x00,0xFC,0x42,0x42,0x42,0x42,0x7C,0x40,0x40,0x40,0x40,0xE0,0x00,0x00,
0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0xB2,0xCA,0x4C,0x38,0x06,0x00,
0x00,0x00,0x00,0xFC,0x42,0x42,0x42,0x7C,0x48,0x48,0x44,0x44,0x42,0xE3,0x00,0x00,
0x00,0x00,0x00,0x3E,0x42,0x42,0x40,0x20,0x18,0x04,0x02,0x42,0x42,0x7C,0x00,0x00,
0x00,0x00,0x00,0xFE,0x92,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00,
0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00,
0x00,0x00,0x00,0xE7,0x42,0x42,0x44,0x24,0x24,0x28,0x28,0x18,0x10,0x10,0x00,0x00,
0x00,0x00,0x00,0xD6,0x92,0x92,0x92,0x92,0xAA,0xAA,0x6C,0x44,0x44,0x44,0x00,0x00,
0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x18,0x18,0x18,0x24,0x24,0x42,0xE7,0x00,0x00,
0x00,0x00,0x00,0xEE,0x44,0x44,0x28,0x28,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00,
0x00,0x00,0x00,0x7E,0x84,0x04,0x08,0x08,0x10,0x20,0x20,0x42,0x42,0xFC,0x00,0x00,
0x00,0x1E,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1E,0x00,
0x00,0x00,0x40,0x40,0x20,0x20,0x10,0x10,0x10,0x08,0x08,0x04,0x04,0x04,0x02,0x02,
0x00,0x78,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x78,0x00,
0x00,0x1C,0x22,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0x00,0x60,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x1E,0x22,0x42,0x42,0x3F,0x00,0x00,
0x00,0x00,0x00,0xC0,0x40,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x64,0x58,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x22,0x40,0x40,0x40,0x22,0x1C,0x00,0x00,
0x00,0x00,0x00,0x06,0x02,0x02,0x02,0x1E,0x22,0x42,0x42,0x42,0x26,0x1B,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x7E,0x40,0x40,0x42,0x3C,0x00,0x00,
0x00,0x00,0x00,0x0F,0x11,0x10,0x10,0x7E,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x44,0x44,0x38,0x40,0x3C,0x42,0x42,0x3C,
0x00,0x00,0x00,0xC0,0x40,0x40,0x40,0x5C,0x62,0x42,0x42,0x42,0x42,0xE7,0x00,0x00,
0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x70,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,
0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x1C,0x04,0x04,0x04,0x04,0x04,0x04,0x44,0x78,
0x00,0x00,0x00,0xC0,0x40,0x40,0x40,0x4E,0x48,0x50,0x68,0x48,0x44,0xEE,0x00,0x00,
0x00,0x00,0x00,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x49,0x49,0x49,0x49,0x49,0xED,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDC,0x62,0x42,0x42,0x42,0x42,0xE7,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8,0x64,0x42,0x42,0x42,0x44,0x78,0x40,0xE0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x22,0x42,0x42,0x42,0x22,0x1E,0x02,0x07,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEE,0x32,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x42,0x40,0x3C,0x02,0x42,0x7C,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x7C,0x10,0x10,0x10,0x10,0x10,0x0C,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6,0x42,0x42,0x42,0x42,0x46,0x3B,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x28,0x10,0x10,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD7,0x92,0x92,0xAA,0xAA,0x44,0x44,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6E,0x24,0x18,0x18,0x18,0x24,0x76,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x28,0x18,0x10,0x10,0xE0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x44,0x08,0x10,0x10,0x22,0x7E,0x00,0x00,
0x00,0x03,0x04,0x04,0x04,0x04,0x04,0x08,0x04,0x04,0x04,0x04,0x04,0x04,0x03,0x00,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x00,0x60,0x10,0x10,0x10,0x10,0x10,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x60,0x00,
0x30,0x4C,0x43,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
/**
 * the VGUI private Font file version 1.0 head struct, internally used only.
 */
typedef struct {
    char      magic[2];     // "FT"
    unsigned short  version;        // 0x1001	(1.01)

    char      name[32];     // description

    unsigned char charWidth;
    unsigned char charHeight;
    unsigned short glyphBytes;

    unsigned short rangeCount;
    unsigned short scatterCount;
}FontFileHeadV1;


static mv_surface s_primary;

unsigned int (*rgb2color)(unsigned char r, unsigned char g, unsigned char b);
void (*putcolor2fb)(const mv_surface * s, int x, int y, unsigned int color);

static unsigned char  * m_fileStart;
static unsigned char  * m_glyphStart;
static FontFileHeadV1 *  m_fh;

static int setFontFileDat(const unsigned char * dat);

static unsigned int rgb2color565(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned int ret = 0;
	ret = (unsigned int)(((r) >> 3) << 11 | ((g) >> 2) << 5 | ((b) >> 3));
	return ret;
}
/*
static unsigned int rgb2color555(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned int ret = 0;
	ret = (unsigned int)((((r) >> 3) << 10 | ((g) >> 3) << 5 | ((b) >> 3)) & 0x7fff);
	return ret;
}
*/
static unsigned int rgb2color888(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned int ret = 0;
	ret = (unsigned int)(((r) << 16 | (g) << 8 | (b)) & 0x00FFFFFF);
	return ret;
}
/*
static unsigned int rgb2color666(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned int ret = 0;
	ret = (unsigned int)(((r) >> 2) << 12 | ((g) >> 2) << 6 | ((b) >> 2));
	return ret;
}
*/
static void putcolor2fb16(const mv_surface * s, int x, int y, unsigned int color)
{
	*(unsigned short *)(s->startAddr + (x * 2) + (s->lineBytes * y)) = (unsigned short)color;
}

static void putcolor2fb32(const mv_surface * s, int x, int y, unsigned int color)
{
	*(unsigned int *)(s->startAddr + (x * 4) + (s->lineBytes * y)) = (unsigned int)color;
}


void mv_initPrimary(const mv_surface * s)
{
    //s_primary = *s;
    Memcpy(&s_primary, s, sizeof(s_primary));
    setFontFileDat(s_fontData); //defined in "fontdata.h"
    if (s->bits_per_pixel == 16) {
    	rgb2color = rgb2color565;
    	putcolor2fb = putcolor2fb16;
    } else {
		rgb2color = rgb2color888;
    	putcolor2fb = putcolor2fb32;
    }
}

void mv_drawLine(int x1, int y1, int x2, int y2, unsigned int color)
{
    int i;
	//mv_color* p;

    if ( y1 == y2 ) {
        if (x1 > x2) {
            int tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
#if 0
        // horz line
        p = getXYData(&s_primary, x1, y1);
        for (i = x1; i < x2; ++i)
            *p++ = color;
#else
		for (i = x1; i < x2; ++i)
			putcolor2fb(&s_primary, i, y1, color);
#endif
    }
    else if ( x1 == x2 ) {
        if (y1 > y2) {
            int tmp = y1;
            y1 = y2;
            y2 = tmp;
        }
#if 0
        // vert line
        p = getXYData(&s_primary, x1,y1);
        for (i = y1; i < y2; ++i) {
            *p = color;
            p = p + s_primary.lineBytes;        
        }
#else
		for (i = y1; i < y2; ++i)
			putcolor2fb(&s_primary, x1, i, color);
#endif
    }
}

void mv_drawRect(const mv_Rect* rect, unsigned int color)
{
    mv_drawLine(rect->left, rect->top, rect->right, rect->top, color);
    mv_drawLine(rect->left, rect->bottom, rect->right, rect->bottom, color);

    mv_drawLine(rect->left, rect->top, rect->left, rect->bottom, color);
    mv_drawLine(rect->right, rect->top, rect->right, rect->bottom, color);
}

void mv_fillRect(const mv_Rect* rect, unsigned char r, unsigned char g, unsigned char b)
{
    int y;
	unsigned int color = 0;

	color = rgb2color(r, g, b);
    for (y = rect->top; y < rect->bottom ; y++)
        mv_drawLine(rect->left, y, rect->right, y, color);
}

int mv_le_to_cpu(char *buf,int size)
{
	int i;
	int val = 0;

	for(i=size-1;i>=0;i--){
		val = val << 8;
		val |= buf[i];
	}
	return val;
}

int mv_loadBmp(unsigned char* fileBuffer)
{
    unsigned char r,g,b;
    int i, j, bytes;
    int width, height, offset;
    short colorBits;
	unsigned char *colorMap;
	mv_surface img;
	bmp_header_t *header;
	unsigned int dcolor = 0;

    if (*fileBuffer != 'B') {
    	printf("fileBuffer : 0x%x = 0x%x \n",fileBuffer,*fileBuffer);
    	printf("Error : Not BMP Format\r\n");
        return -1;
	}
#if 0 // mark & move to uboot_vpp.c by sam 20120803
// clear display buffer jakie 20120312
	for(i=0;i<s_primary.height;i++) 
	      for(j=0;j<s_primary.width;j++)
		putcolor2fb(&s_primary, j, i, 0 );
#endif
	header = (bmp_header_t *)fileBuffer;
	offset = mv_le_to_cpu((char *)&header->data_offset,4);
	width = mv_le_to_cpu((char *)&header->width,4);
	height = mv_le_to_cpu((char *)&header->height,4);
	colorBits = mv_le_to_cpu((char *)&header->bit_count,2);

    colorMap   = (unsigned char*)(fileBuffer + 54);  //颜色表地址(8bit时使用)

    img.width = width;
    img.height = height;
    img.lineBytes = s_primary.lineBytes;

	i = 0;
	j = 0;
	if (width != s_primary.width) {
		if (width > s_primary.width)
			printf("Warning : BMP > LCD\r\n");
		else {
			i = (s_primary.width-width)/2;
		}
	}
	if (height != s_primary.height) {
		if (height > s_primary.height)
			printf("Warning : BMP > LCD\r\n");
		else {
			j = (s_primary.height-height)/2;
		}
	}
  //--> added by bobor on 2010202
    if ((g_logo_x >= 0) && (g_logo_x < s_primary.width)) i = g_logo_x;
    if ((g_logo_y >= 0) && (g_logo_y < s_primary.height)) j = g_logo_y;
    //<-- end add

	img.startAddr = (s_primary.startAddr + (i * (s_primary.bits_per_pixel>>3)) + (s_primary.lineBytes * j));

    switch (colorBits) {
    //  AimarMa  -- Add 32bit BMP support
    case 32:    // r,g,b,a
        for (i = height - 1; i >= 0; i--) {
            for (j = 0; j < width; j++) {
                b = *(fileBuffer + offset);
                g = *(fileBuffer + offset + 1);
                r = *(fileBuffer + offset + 2);
//				a = *(unsigned char*)(fileBuffer + offset + 3);
                //*getXYData(&img, j, i) = mv_RGB2Color(r,g,b);
                dcolor = rgb2color(r, g, b);
                putcolor2fb(&img, j, i, dcolor);
                offset += 4;
            }
        }
        break;

    case 24:     // r,g,b
        bytes = 4 - (width * 3) % 4;
        if (bytes == 4)
            bytes = 0;
        for (i = height - 1; i >= 0; i--) {
            for (j = 0; j < width; j++) {
                b = *(fileBuffer + offset);
                g = *(fileBuffer + offset + 1);
                r = *(fileBuffer + offset + 2);
                //*getXYData(&img, j, i) = mv_RGB2Color(r,g,b);
                dcolor = rgb2color(r, g, b);
                putcolor2fb(&img, j, i, dcolor);
                offset += 3;
            }
            offset  += bytes;
        }
        break;
    case 16:      //16bit 555
        {
            unsigned short color16;

            bytes = 4 - (width * 2) % 4;
            if (bytes == 4)
                bytes = 0;
            for (i = height - 1; i >= 0; i--) {
                for (j = 0; j < width; j++) {
                    color16 = *(unsigned short *)(fileBuffer+offset);
#if ( GUI_RGB_FORMAT == RGB_FORMAT_565)
                    //*getXYData(&img, j, i) = (color16<<1) & 0xFFC0 | (color16  & 0x1F);
                    dcolor = (unsigned int)(((color16<<1) & 0xFFC0) | (color16  & 0x1F));
                	putcolor2fb(&img, j, i, dcolor);
#else
                    //*getXYData(&img, j, i) = mv_RGB2Color((color16 & 0x7C00) >> 7, (color16 & 0x03E0) >> 2, (color16 & 0x001F) << 3);
					dcolor = rgb2color((color16 & 0x7C00) >> 7, (color16 & 0x03E0) >> 2, (color16 & 0x001F) << 3);
					putcolor2fb(&img, j, i, dcolor);
#endif
                    offset += 2;
                }
                offset  += bytes;
            }
        }
        break;
    case 8:     //256 colormap
        bytes = 4 - width % 4;
        if (bytes == 4)
            bytes = 0;
        for (i = height - 1; i >= 0; i--) {
            for (j = 0; j < width; j++) {
                int offSet = *(fileBuffer + offset) * 4;
                b = colorMap[offSet];
                g = colorMap[offSet + 1];
                r = colorMap[offSet + 2];
                //*getXYData(&img, j, i) = mv_RGB2Color(r,g,b);
                dcolor = rgb2color(r, g, b);
                putcolor2fb(&img, j, i, dcolor);
                offset ++;
            }
            offset  += bytes;
        }
        break;
    default:
        break;
    }
    return 0;
}

static int getGlyphIndex(unsigned short unicode)
{
    int i;
    GlyphRange * const range = (GlyphRange*)(m_fh+1);
	GlyphScatter * start;
	GlyphScatter * scatter;
    int low;
    int high;
    int mid;
    int glyphIndex = 1;     // == 1 to skip missing lattice

    for (i = 0 ; i < m_fh->rangeCount; i++) {
        if ( unicode >= range[i].ucs2Start && unicode <= range[i].ucs2End) {
            glyphIndex += (unicode - range[i].ucs2Start);
            return glyphIndex;
        }
        glyphIndex += range[i].ucs2End - range[i].ucs2Start + 1;
    }

    start = (GlyphScatter*)(range + m_fh->rangeCount);
    low = 0;
    high = m_fh->scatterCount - 1;
    
    while (low <= high) {
        mid = (low + high) / 2;
        scatter = start + mid;
        if ( *scatter < unicode)
            low = mid + 1;
        else if ( *scatter > unicode)
            high = mid - 1;
        else
            return(glyphIndex + mid);
    }
    return 0;       // the missing glyph
}


static unsigned char * getLattice(int glyphIndex)
{
    return m_glyphStart + glyphIndex * m_fh->glyphBytes;
}

static int setFontFileDat(const unsigned char * dat)
{
    FontFileHeadV1 * fh = (FontFileHeadV1*)dat;
    if ( fh->magic[0] != 'F' || fh->magic[1] != 'T') {
        return -1;
    }

    m_fileStart = (unsigned char * )dat;
    m_fh = (FontFileHeadV1*)m_fileStart;

    m_glyphStart = m_fileStart + sizeof(FontFileHeadV1)
                   + m_fh->rangeCount * sizeof(GlyphRange)
                   + m_fh->scatterCount * sizeof(GlyphScatter);
    return 0;
}

static void loadFontImage(int code, mv_surface* img)
{
    int idx = getGlyphIndex((unsigned short)code);

    img->width = m_fh->charWidth;
    img->height = m_fh->charHeight;
    img->startAddr =  (char *)getLattice(idx);
}

void mv_textOut(int x, int y, const char * string, unsigned char r, unsigned char g, unsigned char b)
{
    mv_surface img;
    const char * p;
    const unsigned char * lattice;
    int h,w;
    unsigned int textColor = 0;

    for (p = string; *p != '\0'; p++) {
        loadFontImage(*p, &img);
        lattice = (const unsigned char * )img.startAddr;
        if (!lattice)
            continue;

        for ( h = 0 ; h < img.height;  ++h) {
            unsigned char mask = 0x80;
            for ( w = 0 ; w < img.width;  ++w) {
                if (*lattice & mask) {
                    //*(getXYData(&s_primary, x + w, y + h)) = textColor;
					textColor = rgb2color(r, g, b);
					putcolor2fb(&s_primary, x + w, y + h, textColor);
                }
                mask >>= 1;
                if ( mask == 0 ) {
                    mask = 0x80;
                    ++lattice;      // next byte
                }
            }
            if (mask != 0x80) {
                ++lattice;          // next byte
            }
        }

        x += img.width;
    }
}


