#include "stdafx.h"
#include "gsynth.h"
#include <string.h>
#include <math.h>
extern "C"
{
#include <gui-common/gui-common.h>
}

#include "AddModule.h"

/* Backing pixmap for drawing area */
extern CWnd *childwnd;
extern int mwidth, mheight; /* width and height of the drawing window */
extern CDC *pdc;
extern CPen **penlist;
extern CBrush **brushlist;
extern CBitmap *oldbitmap;
extern CBitmap *pbitmap;

extern "C" void machine_position_abs_to_rel(int x, int y, float *fx, float *fy)
{
  *fx = (float) x/mwidth;
  *fy = (float) y/mheight;
}

extern "C" void machine_position_rel_to_abs(float x, float y, int *ix, int *iy)
{
  x*=mwidth; y*=mheight;
  *ix = x; *iy = y;
}

/* fixed size, regardless of name for now */
extern "C" int machine_hit_test(const char *name, float mx, float my, float x, float y)
{
  mx *= mwidth;  x *= mwidth;
  my *= mheight; y *= mheight;
  if(x > (mx-50) && x < (mx+50) && y > (my-20) && y < (my+20))
    return 1;
  return 0;
}

extern "C" void draw_machine_box(const char *label, float x, float y, int state)
{
	if(!pdc) return;
	int sx, sy, w=100, h=40;
	int labellen = strlen(label);
	sx = (mwidth)*x - (w/2);
	sy = (mheight)*y - (h/2);

	pdc->SelectObject(penlist[state]);
	pdc->SelectObject(brushlist[state]);

	pdc->Rectangle(sx,sy,sx+w,sy+h);

	pdc->SetTextAlign(TA_CENTER | TA_BASELINE);
	sx = (mwidth)*x;// - w/2;
	sy = (mheight)*y;// - h/2;
	pdc->TextOut(sx,sy,label, labellen);
}

/* FIXME: draw a little arrow thingy */
extern "C" void draw_machine_connection(float x1, float y1, float x2, float y2)
{
	float vx, vy, px, py, t;
	float dx, dy;
	if(!pdc) return;

	x1 *= mwidth;
	y1 *= mheight;
	x2 *= mwidth;
	y2 *= mheight;
	pdc->SelectStockObject(BLACK_PEN);
	pdc->SelectObject(brushlist[MACHINE_PRELIGHT]);
	pdc->MoveTo(x1,y1);
	pdc->LineTo(x2,y2);

	vx = x2 - x1; vy = y2 - y1; /* make vector from line */
	t = vx*vx+vy*vy; if(t==0) return; /* normalize, store in dx,dy */
	t = sqrt(t); dx=vx/t; dy=vy/t;

	dx*=8; dy*=8;
	px = -dy; py = dx; /* rotate -90 degrees for px,py */

	x1 += vx/2; y1 += vy/2;
	x1 -= dx; y1 -= dy;
	POINT pts[3];
	pts[0].x = x1+px;
	pts[0].y = y1+py;

	pts[1].x = x1+2*dx;
	pts[1].y = y1+2*dy;

	pts[2].x = x1-px;
	pts[2].y = y1-py;
	pdc->Polygon(pts, 3);
}

extern "C" void clear_machine_window(void)
{
	if(!pdc) return;
	pdc->FillSolidRect(0,0,mwidth,mheight, RGB(192,192,192));
}

extern "C" void flush_machine_window(void)
{
	childwnd->Invalidate(FALSE);
}

