#include "./image.h"
#include <numbers>
#include <cmath>
#include "/public/read.h"
#include "/public/colors.h"
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

ofstream ofs ("console_log.txt", ofstream::out);

ofstream points ("points.txt", ofstream::out);

struct Point
{
	int x;
	int y;

	Point operator - (const Point &rhs)
	{
		Point retval;
		retval.x = x - rhs.x;
		retval.y = y - rhs.y;
		return retval;
	}
	bool operator < (const Point &rhs)
	{
		if(x == rhs.x)
		{
			return y < rhs.y;
		}
		return x < rhs.x;
	}
};

struct Color
{
	double r;
	double g;
	double b;
};

Point convert_to_screen(const int rows, const int cols, const double hms_x, const double hms_y)
{
	ofs << "CONVERTING (" << hms_x << "," << hms_y << ") TO SCREEN SPACE" << endl;

	Point retval;
	
	//FINDS POINT (0,0) IN HMSS BY TAKING THE MAX X AND MAX Y AND DIVIDING BY 2
	int zero_x, zero_y;
	zero_x = cols / 2;
	zero_y = rows / 2;
	
	ofs << "0 FOR X = " << zero_x << endl;
	ofs << "0 FOR Y = " << zero_y << endl;
	//USES THE NEW ZERO POINT TO APPLY THE CONVERSION FACTOR
	int new_row, new_col;
	
	new_col = zero_x + (zero_x * hms_x); 
	new_row = zero_y + (zero_y * hms_y);

	//ASSIGN THE SCREEN SPACE COORDS TO OUR NEW POINT
	retval.x = new_col;
	retval.y = new_row;

	ofs << "POINT CONVERTED TO (" << new_col << "," << new_row << ")" << endl;
	return retval;
}

int cross_product(const Point &a, const Point &b)
{
	int retval;
	retval = (a.x * b.y) - (b.x * a.y);
	return retval;
}
int main() {
	int err = system("/public/aseity ./heart.png");

	/* NOTE: C++17 and later only - this is called a "structured binding" and makes two
	   variables, rows and cols, that are set equal to the rows and cols of the
	   screen */
	const auto [SCREEN_ROWS,SCREEN_COLS] = get_terminal_size();
	cout << "The screen has " << SCREEN_ROWS << " rows and " << SCREEN_COLS << " columns\n";

	cout << "Loading ./heart.png - change this to a different texture.\n";
	vector<vector<vector<int>>> vec = load_image("./heart.png");
	const int TEXTURE_ROWS = vec.size();
	cout << "Texture has: " << TEXTURE_ROWS << " rows\n";
	const int TEXTURE_COLS = vec.at(0).size();
	cout << "Texture has: " << TEXTURE_COLS << " cols\n";
	const int TEXTURE_COLORS = vec.at(0).at(0).size();
	cout << "Texture has: " << TEXTURE_COLORS << " colors\n";


//------------------------------------------------------------------------------------------------	
	ofs << "SCREEN DIMENSIONS:\nX: " << SCREEN_COLS << "\nY: " << SCREEN_ROWS << endl;
	
	//USER ENTRY PHASE
	double hms_x, hms_y,hms2_x,hms2_y,hms3_x,hms3_y, angle_deg, angle_rad, light_percent;
	Point a, b, c;
	Point uva, uvb, uvc;
	int xMin, xMax, yMin, yMax;

	cout << "Please enter the coordinates in HMS for vertice A: (x,y)" << endl;
	
	//GRAB THE POINTS IN HMS FORM AND CONVERT THEM
	//A
	cin >> hms_x;
	cin >> hms_y;
	a = convert_to_screen(SCREEN_ROWS, SCREEN_COLS, hms_x, hms_y);
	
	xMin = a.x;
	xMax = a.x;
	yMin = a.y;
	yMax = a.y;

	cout << "Please enter the U,V values you want to attach A to:" << endl;
	cin >> uva.x;//THIS IS A'S U VALUE
	cin >> uva.y;//THIS IS A's V VALUE

	cout << "Please enter the coordinates in HMS for vertice B: (x,y)" << endl;
	
	//B
	cin >> hms2_x;
	cin >> hms2_y;
	b = convert_to_screen(SCREEN_ROWS, SCREEN_COLS, hms2_x, hms2_y);
	
	if(b.x < xMin) xMin = b.x;
	if(b.x > xMax) xMax = b.x;
	if(b.y < yMin) yMin = b.y;
	if(b.y > yMax) yMax = b.y;
	

	cout << "Please enter the U,V values you want to attach B to:" << endl;
	cin >> uvb.x;//THIS IS B'S U VALUE
	cin >> uvb.y;//THIS IS B's V VALUE

	cout << "Please enter the coordinates in HMS for vertice C: (x,y)" << endl;
	
	//C
	cin >> hms3_x;
	cin >> hms3_y;
	c = convert_to_screen(SCREEN_ROWS, SCREEN_COLS, hms3_x, hms3_y);
	
	if(c.x < xMin) xMin = c.x;
	if(c.x > xMax) xMax = c.x;
	if(c.y < yMin) yMin = c.y;
	if(c.y > yMax) yMax = c.y;

	cout << "Please enter the U,V values you want to attach C to:" << endl;
	cin >> uvc.x;//THIS IS C'S U VALUE
	cin >> uvc.y;//THIS IS C's V VALUE

	//NOW LETS GRAB THE ANGLE (IN DEGREES) OF THE LIGHT THEY WANT SHINING ON THE TRIANGLE
	cout << "Please enter the angle in degrees of the light shining on our triangle:" << endl;
	cin >> angle_deg;
	
	//NOW THAT OUR INPUT IS ALL FINISHED WE CLEAR THE SCREEN BEFORE WE DRAW THE POINTS
	clearscreen();

	//WE ARE GOING TO CONVERT IT TO RADIANS AND CALCULATE THE LIGHTING RATIO IMMEDIATELY
	angle_rad = angle_deg * (std::numbers::pi/180);
	
	ofs << "LIGHTING ANGLE IN DEGREES ENTERED BY USER: " << angle_deg << endl;
	ofs << "LIGHTING ANGLE IN RADIANS CALCULATED: " << angle_rad << endl;
	
	//DO THE LIGHTING CALCULATION
	light_percent = cos(angle_rad);
	//IF IT IS LESS THAN 0 KEEP IT ZERO
	if(light_percent < 0) light_percent = 0;

	ofs << "LIGHT PERCENTAGE CALCULATED: " << light_percent * 100 << "%" << endl;
	
	//TIME TO GET OUR BOUNDING BOX AND START ITERATING THROUGH IT, FILLING THE COLOR AND DOING THE LIGHTING AS WE GO.
	ofs << "STARTING THE PROCESS OF FILLING THE TRIANGLE" << endl;

	ofs << "OUR BOUNDING BOX IN SCREEN SPACE IS FROM THE CORNER " << xMin << "," << yMin << " TO THE CORNER " << xMax << "," << yMax << endl;

	Point p, AB, BC, CA; //OUR TEMP POINT, AND THE DISPLACEMENT VECTORS THAT WILL NEVER CHANGE

	AB = b - a;
	ofs << "AB = " << AB.x << "," << AB.y << endl;
	BC = c - b;
	ofs << "BC = " << BC.x << "," << BC.y << endl;
	CA = a - c;
	ofs << "CA = " << CA.x << "," << CA.y << endl;

	//CALCULATE THE TOTAL AREA OF THE TRIANGLE ONCE
	double area = abs(cross_product(AB,BC));
	ofs << "THE TOTAL AREA OF OUR TRIANGLE IN SCREEN SPACE IS " << area << endl;

	//ITERATES HORIZONTALLY GOING THROUGH EVERY ROW, COLUMN BY COLUMN FIRST
	for (int i = yMin; i <= yMax; i++)
	{
		p.y = i;
		for (int j = xMin; j <= xMax; j++)
		{
			//NOW WE ARE DOING BARYCENTRIC COORDINATES
			p.x = j;
			
			bool neg, pos;
			Point AP, BP, CP, uvp; 
			double a_weight, b_weight, c_weight, zero;

			AP = p - a;
			BP = p - b;
			CP = p - c;

			ofs << "AP = " << AP.x << "," << AP.y << endl;
			ofs << "BP = " << BP.x << "," << BP.y << endl;
			ofs << "CP = " << CP.x << "," << CP.y << endl;

			a_weight = cross_product(BP,BC);
			b_weight = cross_product(CP,CA);
			c_weight = cross_product(AP,AB);
			
			zero = a_weight == 0 && b_weight == 0 && c_weight == 0; //HANDLES OUR TRIANGLE BEING ONLY A POINT?
			
			neg = a_weight <= 0 && b_weight <= 0 && c_weight <=0;
			pos = a_weight >= 0 && b_weight >= 0 && c_weight >=0;
			
			ofs << "A's CONTRIBUTION: " << a_weight << endl;
			ofs << "B's CONTRIBUTION: " << b_weight << endl;
			ofs << "C's CONTRIBUTION: " << c_weight << endl;

			if(zero)
			{
				ofs << "THIS TRIANGLE IS ONE POINT, SO JUST DRAW THE VERTICE" << endl;
			}
			else if(neg || pos) //ALL POINTS CROSS PRODUCTS ARE CONSISTENTLY A SINGLE SIGN, IT'S IN THE TRIANGLE (THIS WILL CHANGE WHEN WE DO BACKFACE CULLING)
			{
				ofs << "THE POINT " << p.x << "," << p.y << " IS INSIDE OUR TRIANGLE" << endl;
				//NOW WE ARE GOING TO ABSOLUTE VALUE THE WEIGHTS BECAUSE WE KNOW THAT IT IS INSIDE OUR TRIANGLE
				//AND WE NEED TO HAVE THEM BE POSITIVE TO USE AS RATIOS WITH THE U,V COORDINATES
				abs(a_weight);
				abs(b_weight);
				abs(c_weight);
				
				//CALCULATE THE RATIOS NEEDED TO USE IN THE U,V CALCULATIONS
				double a_ratio = abs((a_weight / area));
				//HOPEFULLY THIS SHOWS THE PERCENTAGES CORRECTLY
				ofs << "A's RATIO ON THIS POINT: " << a_ratio*100 << "%" << endl; 
				double b_ratio = abs((b_weight / area));
				ofs << "B's RATIO ON THIS POINT: " << b_ratio*100 << "%" << endl; 
				double c_ratio = abs((c_weight / area));
				ofs << "C's RATIO ON THIS POINT: " << c_ratio*100 << "%" << endl; 
				
				//NOW WE MULTIPLY THE U,V COORDINATES OF EACH OF THE POINTS BY THE RELATIVE RATIO TO FIND OUR POINT IN TEXTURE SPACE
				
				double total_u = 0, total_v = 0;

				//UVA
				total_u += uva.x * a_ratio;
				total_v += uva.y * a_ratio;
				
				ofs << "A's UV COORDINATES ARE (" << uva.x << "," << uva.y <<"), WHEN A'S RATIO OF " << a_ratio << " IS APPLIED, IT CHANGES THE VALUE TO " << uva.x * a_ratio << " TO ADD TO THE U TOTAL AND " << uva.y * a_ratio << " TO ADD TO THE V TOTAL" << endl;

				ofs << "CURRENT U TOTAL: " << total_u << "\nCURRENT V TOTAL: " << total_v << endl;

				//UVB
				total_u += uvb.x * b_ratio;
				total_v += uvb.y * b_ratio;

				ofs << "B's UV COORDINATES ARE (" << uvb.x << "," << uvb.y <<"), WHEN B'S RATIO OF " << b_ratio << " IS APPLIED, IT CHANGES THE VALUE TO " << uvb.x * b_ratio << " TO ADD TO THE U TOTAL AND " << uvb.y * b_ratio << " TO ADD TO THE V TOTAL" << endl;
				
				ofs << "CURRENT U TOTAL: " << total_u << "\nCURRENT V TOTAL: " << total_v << endl;

				//UVC
				total_u += uvc.x * c_ratio;
				total_v += uvc.y * c_ratio;
				
				ofs << "C's UV COORDINATES ARE (" << uvc.x << "," << uvc.y <<"), WHEN C'S RATIO OF " << c_ratio << " IS APPLIED, IT CHANGES THE VALUE TO " << uvc.x * c_ratio << " TO ADD TO THE U TOTAL AND " << uvc.y * c_ratio << " TO ADD TO THE V TOTAL" << endl;

				ofs << "CURRENT U TOTAL: " << total_u << "\nCURRENT V TOTAL: " << total_v << endl;

				uvp.x = total_u;
				uvp.y = total_v;

				ofs << "THE POINT (" << p.x << "," << p.y << ") IN OUR SCREEN SPACE HAS BEEN CALCULATED TO BE THE POINT (" << uvp.x << "," << uvp.y << ")" << endl;
				
				//NOW IT'S TIME TO INDEX TO THAT SPACE IN THE TRIANGLE, AND GRAB THE COLOR
				//I'M ASSUMING THAT WE INDEX IN THIS FORMAT
				//RED:		VEC.AT(UVP.Y).AT(UVP.X).AT(0)
				//GREEN:	VEC.AT(UVP.Y).AT(UVP.X).AT(1)
				//BLUE: 	VEC.AT(UVP.Y).AT(UVP.X).AT(2)

				Color colop;
				colop.r = vec.at(uvp.y).at(uvp.x).at(0);
				colop.g = vec.at(uvp.y).at(uvp.x).at(1);
				colop.b = vec.at(uvp.y).at(uvp.x).at(2);
				
				ofs << "THE COLOR AT THE POINT (" << uvp.x << "," << uvp.y <<") IN OUR TEXTURE, (" << p.x << "," << p.y << ") IN OUR SCREEN SPACE IS THE COLOR \nR: " << colop.r << "\nG: " << colop.g << "\nB: " << colop.b << endl;
				//RESET TOTAL_U
				total_u = 0;
				total_v = 0;
				
				points << "SCREEN COORDS: (" << p.x << "," << p.y << ")\nU,V COORDS: (" << uvp.x << "," << uvp.y << ")\nCOLOR: " << colop.r << ", " << colop.g << ", " << colop.b << "\n\n";

				//TODO: NOW THAT WE HAVE OUR COLOR, WE SHOULD APPLY THE LIGHTING REQUESTED
				Color lighted;

				lighted.r = floor(colop.r * light_percent);
				lighted.g = floor(colop.g * light_percent);
				lighted.b = floor(colop.b * light_percent);
				
				ofs << "THE ORIGNINAL COLOR, " << colop.r << "," << colop.g << "," << colop.b << ", WHEN THE LIGHTING PERCENTAGE OF " << light_percent * 100 << "% IS APPLIED TO IT, IT BECOMES " << lighted.r << "," << lighted.g << "," << lighted.b << endl;
				//DRAW THE POINT WITH THE COLOR
				movecursor(p.y,p.x);
				setbgcolor(lighted.r,lighted.g,lighted.b);
				cout << " ";

			}
			else
			{
				ofs << "THE POINT " << p.x << "," << p.y << " IS NOT INSIDE OUR TRIANGLE" << endl;
			}
		}
	}
	movecursor(yMax + 10, 0);
	resetcolor();
}
