#include "inc.h"

#include "mdxuitex.h"

// texture information
float MDXUITexLayouter::gTexWidth = 128.0f;
float MDXUITexLayouter::gTexHeight = 64.0f;

// use mdxuitex_tiny.dds
WINDOWLOCATION MDXUITexLayouter::WindowLocation[3] =
{
	{	{
			{ 4,     2,      4 + 3,  2 + 29 },
			{ 4 + 4, 2,      4 + 8,  2 + 29 },
			{ 4 + 9, 2,      4 + 12, 2 + 29 },
			{ 4,     2 + 30, 4 + 3,  2 + 35 },
			{ 4 + 4, 2 + 30, 4 + 8,  2 + 35 },
			{ 4 + 9, 2 + 30, 4 + 12, 2 + 35 },
			{ 4,     2 + 35, 4 + 3,  2 + 38 },
			{ 4 + 4, 2 + 35, 4 + 8,  2 + 38 },
			{ 4 + 9, 2 + 35, 4 + 12, 2 + 38 },
			// btn
			{ 4,2 + 38,4 + 20,2 + 58 }
		}
	},
	{	{
		// pressed state not used for now
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			// btn
			{ 4,2 + 38,4 + 20,2 + 58 }
		}
	},
	{	{
		// disabled state not used for now
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			// btn
			{ 4,2 + 38,4 + 20,2 + 58 }
		}
	}
};

BUTTONLOCATION MDXUITexLayouter::ButtonLocation[3] =
{
	// 20, 10
	{	{
		{ 20,  10, 23, 13 }, // lt
		{ 23,  10, 25, 13 }, // mt
		{ 25,  10, 28, 13 }, // rt
		{ 20, 13, 23, 15 }, // lm
		{ 23, 13, 25, 15 }, // mm
		{ 25, 13, 28, 15 }, // rm
		{ 20, 15, 23, 18 }, // lb
		{ 23, 15, 25, 18 }, // mb
		{ 25, 15, 28, 18 } } // rb
	},
	{	{
		{ 30,  10, 33, 13 }, // lt
		{ 33,  10, 35, 13 }, // mt
		{ 35,  10, 38, 13 }, // rt
		{ 30, 13, 33, 15 }, // lm
		{ 33, 13, 35, 15 }, // mm
		{ 35, 13, 38, 15 }, // rm
		{ 30, 15, 33, 18 }, // lb
		{ 33, 15, 35, 18 }, // mb
		{ 35, 15, 38, 18 } } // rb
	},
	{	{
		// disabled state not used for now
		{ 36,  8, 43, 31 }, // lt
		{ 43,  8, 45, 31 }, // mt
		{ 45,  8, 55, 31 }, // rt
		{ 36, 31, 43, 32 }, // lm
		{ 43, 31, 45, 32 }, // mm
		{ 45, 31, 55, 32 }, // rm
		{ 36, 32, 43, 40 }, // lb
		{ 43, 32, 45, 40 }, // mb
		{ 45, 32, 55, 40 } } // rb
	}
};

CHKBOXLOCATION MDXUITexLayouter::ChkboxLocation[3] =
{
	{ 100,  5, 123, 28 },
	{ 100, 35, 123, 58 },
	{ 100, 35, 123, 58 }, // disabled state not used for now
};

CMBBOXLOCATION MDXUITexLayouter::CmbboxLocation[2] =
{
	{ {
		// normal state
		{ 20,20, 23,23 }, // lt
		{ 25,20, 26,23 }, // mt
		{ 28,20, 31,23 }, // rt

		{ 20,25, 23,26 }, // lm
		{ 25,25, 26,26 }, // mm
		{ 28,25, 31,26 }, // rm

		{ 20,28, 23,31 }, // lb
		{ 25,28, 26,31 }, // mb
		{ 28,28, 31,31 }, // rb

		{ 50,10, 72,32 }, // button
		{ 40,30, 42,32 }}
	},
	{ {
		// pressed state not used for now
		{ 20,30, 23,33 }, // lt
		{ 25,30, 26,33 }, // mt
		{ 28,30, 31,33 }, // rt

		{ 20,35, 23,36 }, // lm
		{ 25,35, 26,36 }, // mm
		{ 28,35, 31,36 }, // rm
		{ 20,38, 23,41 }, // lb

		{ 25,38, 26,41 }, // mb
		{ 28,38, 31,41 }, // rb

		{ 186,21, 206,41 }, // button
		{ 145, 48, 147, 50 }}
	}

	// no disabled state for now.
};

SLIDERLOCATION MDXUITexLayouter::SliderLocation[3] =
{
	{{
			{ 75, 20, 80, 41 }, // tray left
			{ 81, 20, 89, 41 }, // tray middle
			{ 90, 20, 95, 41 }, // tray right

			{ 29, 37, 50, 60 }, //thumb
			{ 0,0,0,0 },   // tick not used for now
		}
	},
	{{
			// pressed state not used for now.
			{ 75, 10, 80, 21 }, // tray left
			{ 81, 10, 89, 21 }, // tray middle
			{ 90, 10, 95, 21 }, // tray right

			{ 29, 37, 50, 60 }, //thumb
			{ 0,0,0,0 },   // tick not used for now
		}
	},
	{{
			// disabled state not used for now.
			{ 60, 60, 65, 81 }, // tray left
			{ 66, 60, 74, 81 }, // tray middle
			{ 75, 60, 80, 81 }, // tray right

			{ 29, 37, 50, 60 }, //thumb
			{ 0,0,0,0 },   // tick not used for now
		}
	}
};
