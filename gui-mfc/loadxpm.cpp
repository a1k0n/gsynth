// this function reads in an xpm.
#include <windows.h>
#include <map>
using namespace std;

typedef struct
{
	char ch;
	COLORREF col;
} coltype;

class Image
{
public:
	int width;
	int height;
	COLORREF *data;
	~Image() {if(data) free(data);}
};

void ReadXPMToImage(char *filename[], Image *img)
{
	int w,h,numcolors,a,b,c,i,j,pixel,k;
	int startofimage=0,newline=0;
	coltype *cols;
	BOOL retval;
	map<char, COLORREF> colormap;

	sscanf(filename[0], "%d %d %d %d", &w,&h,&numcolors,&b);
	startofimage = numcolors+1;
	cols = (coltype*)malloc(sizeof(coltype)*numcolors);
	for(i=1;i<numcolors;i++)
	{
		/* two two two */
		char val[3];
		val[2] = 0;

		val[0] = filename[1+i][5];
		val[1] = filename[1+i][6];
		sscanf(val, "%x", &a);
		val[0] = filename[1+i][7];
		val[1] = filename[1+i][8];
		sscanf(val, "%x", &b);
		val[0] = filename[1+i][9];
		val[1] = filename[1+i][10];
		sscanf(val, "%x", &c);
		/* four four four
		filename[1+i][7] = 0;
		filename[1+i][11] = 0;
		filename[1+i][15] = 0;
		sscanf(filename[1+i]+5, "%x", &a);
		sscanf(filename[1+i]+9, "%x", &b);
		sscanf(filename[1+i]+13, "%x", &c);
		*/
		colormap[filename[1+i][0]] = RGB(a,b,c);
//		cols[i].ch = filename[1+i][0];
//		cols[i].col = RGB(a,b,c);
	}

	img->width = w;
	img->height = h;
    img->data = (COLORREF*)malloc(w*h*sizeof(COLORREF));

	for(i=0,k=0;i<h;i++,startofimage++)
	{
		for(j=0;j<w;j++,k++)
		{
			if(filename[startofimage][j] == ' ')
				img->data[k] = RGB(0,0,0);
			else
			{
				img->data[k] = colormap[filename[startofimage][j]];
			}
		}
	}
}
