// Programmed by Zach Jones and Jesse Hoyt
// 
// Basic OpenGL file structure & functions 
//		are based off of the beginner tutorials
//		at http://nehe.gamedev.net/


// Controls:
//	
//	Move with W,A,S,D
//	look around with arrow keys
//	L toggles lights
//	F toggles fog

#include <windows.h>		// Header File For Windows
#include <iostream>
#include <fstream>
#include <string>
#include "Angel.h"
#include "soil.h"
#include "Camera.h"
#include "mansion.h"
#include <array>
#include "object.h"
#include "floor.h"
#include "wall.h"
#include "room.h"
#include "ghost.h"

#define FLOOR 0
#define BOOKSHELF 1
#define WALL 2
#define GRASS 3
#define GHOST 4
#define CUBE 0

std::ofstream fout ;


HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default
bool    light = FALSE;                      // Lighting ON / OFF
bool	fog = TRUE;						// Fog ON/OFF
bool    lp;                         // L Pressed?
bool    fp;                         // F Pressed?

int n = 10;					// number of cubes per row (n^3 = actual number of cubes)
int velocity = 0;

Camera myCamera = Camera() ; // create camera
Camera oldCamera = Camera() ;

enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 }; // for rotation

extern GLfloat mansionwidth;
extern GLfloat mansiondepth;
extern GLfloat buildscale;

GLfloat xrot;               // X Rotation
GLfloat yrot;               // Y Rotation
GLfloat xspeed;				// X Rotation Speed
GLfloat yspeed;             // Y Rotation Speed
GLfloat z=-5.0f;            // Depth Into The Screen
GLfloat	cubeSpread = 5;		// Distance between cubes	
GLdouble lookx = 0;		// Lookat coordinates
GLdouble looky = 0;
GLdouble lookz = 5;
GLdouble posx = 0;		// camera position coordinates
GLdouble posy = 0;
GLdouble posz = -5;

GLfloat LightAmbient[]= { 0.0f, 0.0f, 0.0f, 1.0f };                 // Ambient Light Values ( NEW )
GLfloat LightDiffuse[]= { 0.0f, 0.0f, 1.0f, 1.0f };              // Diffuse Light Values ( NEW )
GLfloat LightPosition[]= { 3.0f, 0.0f, 1.0f, 1.0f };                 // Light Position ( NEW )

GLfloat LightAmbient2[]= { 0.0f, 0.0f, 0.0f, 1.0f };                 // Ambient Light Values ( NEW )
GLfloat LightDiffuse2[]= { 1.0f, 0.0f, 0.0f, 1.0f };              // Diffuse Light Values ( NEW )
GLfloat LightPosition2[]= { -3.0f, 0.0f, 0.0f, 1.0f };                 // Light Position ( NEW )
GLuint  filter = 2;                                 // Which Filter To Use (set to Mipmap)
GLuint  texture[10];                             // Storage for 10 textures

//Fog Variables
GLuint fogMode=  GL_LINEAR;   // Storage For Three Types Of Fog
GLuint fogfilter= 0;                    // Which Fog To Use
GLfloat fogColor[4]= {0.6f, 0.6f, 0.6f, 1.0f};      // Fog Color

mansion *myMansion ; // create mansion
ghost *myGhost ; // create ghost

GLfloat followspeed = 0.1f ;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLfloat distance(vec4 pt1, vec4 pt2){

	return sqrt(pow(pt1.x - pt2.x, 2) + pow(pt1.y - pt2.y, 2) + pow(pt1.z - pt2.z, 2)) ;

}

bool collision(room theRoom, Camera cam, GLfloat maxDist){

	GLfloat min = 0.0 ;
	GLfloat dist ;
	if(cam.target.x > cam.position.x){//looking left

		for(std::vector<wall>::iterator it = theRoom.walls.begin() ; it < theRoom.walls.end() ; ++it){
			if(it->normal.x == -1){//only need to detect off this wall
				for(std::vector<vec4>::iterator it2 = it->points.begin() ; it2 < it->points.end() ; ++it2){
					if(dist = distance(cam.position, *it2) < min) min = dist ;
				}
				if(dist < maxDist) return true ;
			

			}

		}
	}
	if(cam.target.x < cam.position.x){//looking right
		for(std::vector<wall>::iterator it = theRoom.walls.begin() ; it < theRoom.walls.end() ; ++it){
			if(it->normal.x == 1){//only need to detect off this wall
		
			}
		}

	}
	if(cam.target.z > cam.position.x){//looking up
		for(std::vector<wall>::iterator it = theRoom.walls.begin() ; it < theRoom.walls.end() ; ++it){
			if(it->normal.z == -1){//only need to detect off this wall

			}
		}

	}
	if(cam.target.z > cam.position.x){//looking down
		for(std::vector<wall>::iterator it = theRoom.walls.begin() ; it < theRoom.walls.end() ; ++it){
			if(it->normal.z == 1){//only need to detect off this wall

			}
		}

	}


	return false ;


}


int LoadGLTextures()                                    // Load Images And Convert To Textures
{
	unsigned int SOIL_flags;
	GLint mag_param;
	GLint min_param;

	SOIL_flags = 16 | 2;   // SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS value
	mag_param = GL_LINEAR;
	min_param = GL_LINEAR_MIPMAP_NEAREST;
	
	// Floor texture ================================
	  texture[FLOOR]=SOIL_load_OGL_texture (
	   "Data/floor.jpg",
	   SOIL_LOAD_AUTO,
	   SOIL_CREATE_NEW_ID,
	   SOIL_flags);
	  
	  if (texture[FLOOR] == 0)
	   return false;

	  // Define Texture Parameters
	  glBindTexture(GL_TEXTURE_2D, texture[FLOOR]);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_param);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_param);

	  // Wall texture ================================
	  texture[WALL]=SOIL_load_OGL_texture (
	   "Data/wall.jpg",
	   SOIL_LOAD_AUTO,
	   SOIL_CREATE_NEW_ID,
	   SOIL_flags);
	  
	  if (texture[WALL] == 0)
	   return false;

	  // Define Texture Parameters
	  glBindTexture(GL_TEXTURE_2D, texture[WALL]);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_param);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_param);

	  // Grass texture ================================
	  texture[GRASS]=SOIL_load_OGL_texture (
	   "Data/grass.jpg",
	   SOIL_LOAD_AUTO,
	   SOIL_CREATE_NEW_ID,
	   SOIL_flags);
	  
	  if (texture[GRASS] == 0)
	   return false;

	  // Define Texture Parameters
	  glBindTexture(GL_TEXTURE_2D, texture[GRASS]);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_param);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_param);

	  // Ghost texture ================================
	  texture[GHOST]=SOIL_load_OGL_texture (
	   "Data/ghost.jpg",
	   SOIL_LOAD_AUTO,
	   SOIL_CREATE_NEW_ID,
	   SOIL_flags);
	  
	  if (texture[GRASS] == 0)
	   return false;

	  // Define Texture Parameters
	  glBindTexture(GL_TEXTURE_2D, texture[GRASS]);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_param);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_param);

    return true;                                        // Return Success
}

void beginBillboard(){		// Billboarding code from http://www.lighthouse3d.com/opengl/billboarding/index.php3?billCheat
	int i, j;				// Objects will be rotated around local origin.
	float modelview[16];

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
	
	// undo all rotations
	// beware all scaling is lost as well 
	for( i=0; i<3; i++ ) 
		for( j=0; j<3; j++ ) {
			if ( i==j )
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}

	// set the modelview with no rotations and scaling
	glLoadMatrixf(modelview);
	return;
}

void endBillboard(){
	glPopMatrix();
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	fout.open("log.txt") ;
	if (!LoadGLTextures())                          // Jump To Texture Loading Routine
    {
        return FALSE;                           // If Texture Didn't Load Return FALSE
    }
	glEnable(GL_TEXTURE_2D);                        // Enable Texture Mapping
    glShadeModel(GL_SMOOTH);                        // Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                   // Black Background
    glClearDepth(1.0f);                         // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective Calculations

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);             // Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);             // Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);            // Position The Light
	glEnable(GL_LIGHT1);										 // Enable Light One
	glLightfv(GL_LIGHT2, GL_AMBIENT, LightAmbient2);             // Setup The Ambient Light
	glLightfv(GL_LIGHT2, GL_DIFFUSE, LightDiffuse2);             // Setup The Diffuse Light
	glLightfv(GL_LIGHT2, GL_POSITION,LightPosition2);            // Position The Light
	glEnable(GL_LIGHT2);                            // Enable Light Two

	//glEnable(GL_LIGHTING);
	//light = !light;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);       // Blending Function For Translucency Based On Source Alpha Value ( NEW )

	//fog stuff  from NeHe tutorials
	glClearColor(0.6f,0.6f,0.6f,1.0f);          // We'll Clear To The Color Of The Fog ( Modified )
 
	glFogi(GL_FOG_MODE, fogMode);        // Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);            // Set Fog Color
	glFogf(GL_FOG_DENSITY, 1.0f);              // How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_NICEST);          // Fog Hint Value
	glFogf(GL_FOG_START, 0.5f);             // Fog Start Depth
	glFogf(GL_FOG_END, 5.0f);               // Fog End Depth
	glEnable(GL_FOG);                   // Enables GL_FOG

	myCamera.position.y = 0.5f;
	//myCamera.position.x = mansionwidth/2;
	//myCamera.position.z = mansiondepth/2;

	myMansion = new mansion(10.0f, 12.5f, 1.0f,texture[WALL], texture[FLOOR], texture[GRASS]);
	myGhost = new ghost(1.0f, 1.0f, texture[GHOST]);
	//myGhost->transform(Translate(0.0, 0.0, -3.0));

	//Music
	PlaySound(L"Data/music.wav", NULL, SND_ASYNC | SND_NOSTOP | SND_LOOP);  // Play Background Music

	return TRUE;										// Initialization Went OK
}

GLfloat rot = 1.0 ;
GLfloat camx, camz ;
int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

	myCamera.RotateCamera(Theta[Yaxis], Theta[Xaxis]) ;
	//myCamera.SetCamera(myCamera.position, myCamera.hTheta, myCamera.vTheta) ;
	//myCamera.MoveCamera(velocity) ;
	gluLookAt(myCamera.position.x, myCamera.position.y, myCamera.position.z, myCamera.target.x, myCamera.target.y,myCamera.target.z, myCamera.up.x, myCamera.up.y, myCamera.up.z) ;
		
	myMansion->draw() ;
	
	camx = myCamera.position.x - oldCamera.position.x ;
	camz = myCamera.position.z - oldCamera.position.z ;
	//myGhost->transform(Translate(-camx, 0.0, -camz)) ;
	oldCamera = myCamera;
	
	myGhost->draw(); // billboard the ghost so it's always facing the camera
 

//buildscale *= 1.005 ;
	xrot+=xspeed;                               // Add xspeed To xrot
    yrot+=yspeed;                               // Add yspeed To yrot
    return TRUE;                                // Keep Going										// Keep Going
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,L"Release Of DC And RC Failed.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,L"Release Rendering Context Failed.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,L"Release Device Context Failed.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,L"Could Not Release hWnd.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass(L"OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,L"Could Not Unregister Class.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(LPCWSTR title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= L"OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,L"Failed To Register The Window Class.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",L"NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,L"Program Will Now Close.",L"ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								L"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,L"Window Creation Error.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,L"Can't Create A GL Device Context.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,L"Can't Find A Suitable PixelFormat.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,L"Can't Set The PixelFormat.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,L"Can't Create A GL Rendering Context.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,L"Can't Activate The GL Rendering Context.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,L"Initialization Failed.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,L"Would You Like To Run In Fullscreen Mode?", L"Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow(L"Escape from Reggie's Mansion!",640,480,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
			{
				done=TRUE;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				SwapBuffers(hDC);					// Swap Buffers (Double Buffering)
				if (keys['L'] && !lp)  // Toggle lights
				{
					lp=TRUE;
					light=!light;
					if (!light)
					{
						glDisable(GL_LIGHTING);
					}
					else
					{
						glEnable(GL_LIGHTING);
					}
				}
				if (!keys['L'])
				{
					lp=FALSE;
				}
				if (keys['F'] && !fp)  // toggle fog
				{
					fp=TRUE;
					fog = !fog;	
					if (fog) glEnable(GL_FOG);
					if (!fog) glDisable(GL_FOG);
				}
				if (!keys['F'])
				{
					fp=FALSE;
				}
				if (keys[VK_PRIOR])//strafe up
				{
					posy += 0.2f;
					looky += 0.2f;
				}
				if (keys[VK_NEXT])//strafe down
				{
					posy -= 0.2f;
					looky -= 0.2f;
				}
				if (keys[VK_UP])//look up
				{
					myCamera.RotateCamera(0.0f,0.03f);
					//looky += 0.2f;
					//xspeed-=0.1f;
				}
				if (keys[VK_DOWN])//look down
				{
					myCamera.RotateCamera(0.0f,-0.03f);
					//looky -= 0.2f;
					//xspeed+=0.1f;
				}
				if (keys[VK_RIGHT])//look right
				{
					myCamera.RotateCamera(0.03f,0.0f);
					//lookx -= 0.5f;
					//yspeed+=0.1f;
				}
				if (keys[VK_LEFT])//look left
				{
					myCamera.RotateCamera(-0.03f,0.0f);
					lookx += 0.5f;
					//yspeed-=0.1f;
				}
				if (keys[VK_SPACE]) // stop movement
				{
					velocity = 0;
				}
				if (keys['W'])//forward
				{
					PlaySound(L"Data/footsteps.wav", NULL, SND_ASYNC | SND_NOSTOP);  // Footstep Sound Effects
					myCamera.MoveCamera(10.0f);
					//velocity += 1; //constant motion
				}
				if (keys['S']) //backward
				{
					PlaySound(L"Data/footsteps.wav", NULL, SND_ASYNC | SND_NOSTOP);  // Footstep Sound Effects
					myCamera.MoveCamera(-10.0f);
					//velocity -= 1; //constant motion
				}
				if (keys['A'])//strafe left
				{
					myCamera.SlideCamera(-10.0f,0.0f);
					/*posx +=0.2f;
					lookx += 0.2f;*/
				}
				if (keys['D'])//strafe right
				{
					myCamera.SlideCamera(10.0f,0.0f);
					/*posx -=0.2f;
					lookx -= 0.2f;*/
				}


				if (keys[VK_F1])						// Is F1 Being Pressed?
				{
					keys[VK_F1]=FALSE;					// If So Make Key FALSE
					KillGLWindow();						// Kill Our Current Window
					fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
					// Recreate Our OpenGL Window
					if (!CreateGLWindow(L"NeHe's Textures, Lighting & Keyboard Tutorial",640,480,16,fullscreen))
					{
						return 0;						// Quit If Window Was Not Created
					}
				}
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow(L"NeHe's Rotation Tutorial",640,480,16,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
