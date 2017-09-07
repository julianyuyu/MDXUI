#pragma once

#include "inc.h"
#include "fontrenderer.h"


/*not used below dds as using build-in texture*/
#define MDXUI_TEXTURE_FILENAME		L"mdxuitex_tiny.dds"

///////////
//button
///////////
//
//  +---+-----+---+
//  | 0 |  1  | 2 |              
//  |---+-----+---|
//  | 3 |  4  | 5 |
//  |---+-----+---|
//  | 6 |  7  | 8 |
//  +---+-----+---+
//

struct TEXQUAD
{
	float u1, v1, u2, v2;
	int w, h;
};

struct TEXQUAD0
{
	WXMVECTOR2 uv1;
	WXMVECTOR2 uv2;
	int width;
	int height;
};

// quad location in tex.
struct WINDOWLOCATION
{
	RECT Quad[10];
};

// uv coord of each quad for button
struct WINDOWCOORD
{
	TEXQUAD Quad[10];
};

struct BUTTONLOCATION
{
	RECT Quad[9];
};

// uv coord of each quad for button
struct BUTTONCOORD
{
	TEXQUAD Quad[9];
};

struct CHKBOXLOCATION
{
	RECT Quad;
};

// uv coord of each quad for button
struct CHKBOXCOORD
{
	TEXQUAD Quad;
};


// combobox
//
//  +----+----------+----+
//  | 0  |  1       | 2  |
//  |----+----------+----|
//  |    |    +---+ |    |
//  | 3  |  4 | 9 | | 5  |
//  |    |    +---+ |    |
//  |----+----------+----|
//  | 6  |  7       | 8  |
//  +----+----------+----+
//  +----+----------+----+
//  |    |          |    |
//  | 10 |  11      | 12 |
//  |----+----------+----|
//  | 13 |  14      | 15 |
//  +----+----------+----+
//
// uv coord of each quad for combobox
struct CMBBOXLOCATION
{
	RECT Quad[11];
};

struct CMBBOXCOORD
{
	TEXQUAD Quad[11];
};

// uv coord of each quad for slider
struct SLIDERLOCATION
{
	RECT Quad[5];
};

struct SLIDERCOORD
{
	TEXQUAD Quad[5];
};

class MDXUITexLayouter
{
public:
	MDXUITexLayouter() {}
	~MDXUITexLayouter() {}
	virtual void CalcLayout()
	{
		// first pixel in tex is transparent, which can used for transparent widget.
		TransparentUV.u1 = 0.f;
		TransparentUV.v1 = 0.f;
		TransparentUV.u2 = 1 / gTexWidth;
		TransparentUV.v2 = 1 / gTexHeight;

		// button
		for (int state = 0; state < 3; state++)
		{
			for (int i = 0; i < 9; i++)
			{
				// calculate the UV coordinates of each of the 9 images that
				// make up a button.  Do this for the active, pressed, and disabled states.
				ButtonUV[state].Quad[i].u1 = ButtonLocation[state].Quad[i].left   / gTexWidth;
				ButtonUV[state].Quad[i].v1 = ButtonLocation[state].Quad[i].top    / gTexHeight;
				ButtonUV[state].Quad[i].u2 = ButtonLocation[state].Quad[i].right  / gTexWidth;
				ButtonUV[state].Quad[i].v2 = ButtonLocation[state].Quad[i].bottom / gTexHeight;

				// calculate the width/height in pixels of each of the 9 image slices
				// that makes up the button images
				ButtonUV[state].Quad[i].w = ButtonLocation[state].Quad[i].right - ButtonLocation[state].Quad[i].left;
				ButtonUV[state].Quad[i].h = ButtonLocation[state].Quad[i].bottom - ButtonLocation[state].Quad[i].top;
			}
			ChkboxUV[state].Quad.u1 = ChkboxLocation[state].Quad.left   / gTexWidth;
			ChkboxUV[state].Quad.v1 = ChkboxLocation[state].Quad.top    / gTexHeight;
			ChkboxUV[state].Quad.u2 = ChkboxLocation[state].Quad.right  / gTexWidth;
			ChkboxUV[state].Quad.v2 = ChkboxLocation[state].Quad.bottom / gTexHeight;
			ChkboxUV[state].Quad.w = ChkboxLocation[state].Quad.right - ChkboxLocation[state].Quad.left;
			ChkboxUV[state].Quad.h = ChkboxLocation[state].Quad.bottom - ChkboxLocation[state].Quad.top;

			if (state < 2)
			{
				// for now, only 2 states (normal, pressed) are difined for combobox
				for (int i = 0; i < 11; i++)
				{
					CmbboxUV[state].Quad[i].u1 = CmbboxLocation[state].Quad[i].left / gTexWidth;
					CmbboxUV[state].Quad[i].v1 = CmbboxLocation[state].Quad[i].top / gTexHeight;
					CmbboxUV[state].Quad[i].u2 = CmbboxLocation[state].Quad[i].right / gTexWidth;
					CmbboxUV[state].Quad[i].v2 = CmbboxLocation[state].Quad[i].bottom / gTexHeight;
					CmbboxUV[state].Quad[i].w = CmbboxLocation[state].Quad[i].right - CmbboxLocation[state].Quad[i].left;
					CmbboxUV[state].Quad[i].h = CmbboxLocation[state].Quad[i].bottom - CmbboxLocation[state].Quad[i].top;
				}
			}

			for (int i = 0; i < 5; i++)
			{
				SliderUV[state].Quad[i].u1 = SliderLocation[state].Quad[i].left / gTexWidth;
				SliderUV[state].Quad[i].v1 = SliderLocation[state].Quad[i].top / gTexHeight;
				SliderUV[state].Quad[i].u2 = SliderLocation[state].Quad[i].right / gTexWidth;
				SliderUV[state].Quad[i].v2 = SliderLocation[state].Quad[i].bottom / gTexHeight;
				SliderUV[state].Quad[i].w = SliderLocation[state].Quad[i].right - SliderLocation[state].Quad[i].left;
				SliderUV[state].Quad[i].h = SliderLocation[state].Quad[i].bottom - SliderLocation[state].Quad[i].top;
			}

			for (int i = 0; i < 10; i++)
			{
				WindowUV[state].Quad[i].u1 = WindowLocation[state].Quad[i].left / gTexWidth;
				WindowUV[state].Quad[i].v1 = WindowLocation[state].Quad[i].top / gTexHeight;
				WindowUV[state].Quad[i].u2 = WindowLocation[state].Quad[i].right / gTexWidth;
				WindowUV[state].Quad[i].v2 = WindowLocation[state].Quad[i].bottom / gTexHeight;
				WindowUV[state].Quad[i].w = WindowLocation[state].Quad[i].right - WindowLocation[state].Quad[i].left;
				WindowUV[state].Quad[i].h = WindowLocation[state].Quad[i].bottom - WindowLocation[state].Quad[i].top;
			}

		}
	}

protected:
	// texture information
	static float gTexWidth;
	static float gTexHeight;
	static WINDOWLOCATION WindowLocation[3];
	static BUTTONLOCATION ButtonLocation[3]; // for normal/ pressed/ disabled state.
	static CHKBOXLOCATION ChkboxLocation[3];
	static CMBBOXLOCATION CmbboxLocation[2];
	static SLIDERLOCATION SliderLocation[3];
public:
	TEXCOORD TransparentUV;
	WINDOWCOORD WindowUV[3];
	BUTTONCOORD ButtonUV[3];
	CHKBOXCOORD ChkboxUV[3];
	CMBBOXCOORD CmbboxUV[2];
	SLIDERCOORD SliderUV[3];
};
