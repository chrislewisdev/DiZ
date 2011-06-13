/********************************************
*Diz-window.cpp by Chris Lewis
Defines the DIZ_WINDOW class in diz-graphics.h
********************************************/

//Include our graphics header
#include "../Include/diz-graphics.h"

//Define our Constructor- basically initalises all our properties to safe values
DIZ_WINDOW::DIZ_WINDOW() {
	//We don't need to worry about our DC/RC etc. handles so we just initialise our INFO structure
	info.title		= "Default DiZ Window";
	info.width		= 640;
	info.height		= 480;
	info.bits		= 16;
	info.fullscreen	= false;
	info.menu		= false;
	info.showCursor	= true;
	info.wndProc	= NULL;

	//Initialise our OpenGL settings
	info.gl.mode	= DIZ_3D;
	info.gl.zNear	= 0.1f;
	info.gl.zFar	= 100.0f;
}

//Define our Deconstructor
DIZ_WINDOW::~DIZ_WINDOW() {

}

//This function sets our OpenGL viewport to the right size
void DIZ_WINDOW::resizeGL() {
	//First we make sure our height value isn't zero since that would cause a divide-by-zero (bad)
	if (info.height == 0) {
		info.height = 1;
	}

	//Set our OpenGL viewport to the correct size
	glViewport(0, 0, info.width, info.height);

	//Select the PROJECTION matrix and reset it
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Check if we want 3D or 2D projection
	if (info.gl.mode == DIZ_3D) {
		//If we want 3D, set up our perspective for the scene
		gluPerspective(45.0f, (GLfloat)info.width / (GLfloat)info.height, info.gl.zNear, info.gl.zFar);
	}else if (info.gl.mode == DIZ_2D) {
		//Otherwise, set up our screen dimensions for 2D projection
		glOrtho(0, info.width, info.height, 0, 0, 1);
	}

	//Select the MODELVIEW matrix and reset it
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//This function intiates all our OpenGL settings and so on
bool DIZ_WINDOW::initGL() {
	//First we enable smooth shading for colours and so on
	glShadeModel(GL_SMOOTH);

	//Set our colour to clear the screen to
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Set our depth value to clear to
	glClearDepth(1.0f);
	//Check if we want 3D projection again
	if (info.gl.mode == DIZ_3D) {
		//If we do, then we enable Depth Testing of course
		glEnable(GL_DEPTH_TEST);
	}
	//Set our type of Depth Testing to do (Less than or Equal)
	glDepthFunc(GL_LEQUAL);

	//Enable the use of 2D textures
	glEnable(GL_TEXTURE_2D);

	//Enable nicest Perspective correction
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return true;
}

//This function kills our window
void DIZ_WINDOW::killWindow() {
	//Check if we need to re-show the cursor
	if (!info.showCursor) {
		//And, uh, do so if necessary
		ShowCursor(true);
	}

	//Perform our kill routine
	killGL();

	//Finally, destroy the actual window and make sure nothing went wrong
	if (hWnd && !DestroyWindow(hWnd)) {
		MessageBox(NULL, "Failed to destroy window.", "DiZ Shutdown Error", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;
	}

	//Now try to Unregister our Window Class
	if (!UnregisterClass("DiZ Window", hInstance)) {
		MessageBox(NULL, "Could not unregister window class.", "DiZ Shutdown Error", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;
	}
}

//This function destroys our OpenGL scene
void DIZ_WINDOW::killGL() {
	//Check if we're in fullscreen mode
	if (info.fullscreen) {
		//If we are, change our display settings back to normal
		ChangeDisplaySettings(0, NULL);
	}

	//Double check if we have a rendering context
	if (hRC) {
		//Try and release our rendering context and notify of any errors
		if (!wglMakeCurrent(NULL, NULL)) {
			MessageBox(NULL, "Failed to release RC.", "DiZ Shutdown Error", MB_OK | MB_ICONINFORMATION);
		}
		//Then try to delete the context
		if (!wglDeleteContext(hRC)) {
			MessageBox(NULL, "Failed to delete RC.", "DiZ Shutdown Error", MB_OK | MB_ICONINFORMATION);
		}
		//And finally just set it to NULL
		hRC = NULL;
	}

	//Check if we have a Device Context and try to release it at the same time
	if (hDC && !ReleaseDC(hWnd, hDC)) {
		//If anything went wrong, give a message and set DC to NULL
		MessageBox(NULL, "Failed to release DC.", "DiZ Shutdown Error", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;
	}
}

//This function creates the window according to our info
bool DIZ_WINDOW::createWindow() {
	//Declare our window Style holders
	DWORD dwStyle, dwExStyle;

	//Declare and set up our window Rect structure (used for adjusting our full window size)
	RECT wndRect;
	//Do the left-right values
	wndRect.left = 0; wndRect.right = info.width;
	//And of course the top-bottom values
	wndRect.top = 0; wndRect.bottom = info.height;

	//Declare our Window Class structure
	WNDCLASS wc;

	hInstance			= GetModuleHandle(NULL);				//Grab an Instance Handle
	wc.hInstance		= hInstance;							//Use our instance Handle
	wc.cbClsExtra		= 0;									//Set it for no extra data
	wc.cbWndExtra		= 0;									//Again, no extra data
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			//Use the default cursor
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			//Use the default icon
	wc.hbrBackground	= NULL;									//Set it to no background
	wc.lpfnWndProc		= (WNDPROC)info.wndProc;				//Point it to our Window Procedure
	wc.lpszMenuName		= NULL;									//Don't bother with a menu name
	wc.lpszClassName	= "DiZ Window";							//Set the name of our class
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	//Specify our basic styles to use

	//Attempt to Register our Window Class
	if (!RegisterClass(&wc)) {
		MessageBox(NULL, "Could not register class.", "DiZ Startup Error", MB_OK | MB_ICONINFORMATION);
		return false;
	}

	//And check if we want to go fullscreen
	if (info.fullscreen) {
		//Declare our DEVMODE structure for screen settings
		DEVMODE dwSettings;
		//Clear it out
		memset(&dwSettings, 0, sizeof(dwSettings));

		//Set our screen settings properties
		//Indicate the size of our structure
		dwSettings.dmSize			= sizeof(dwSettings);
		//Specify our Bits property
		dwSettings.dmBitsPerPel		= info.bits;
		//Set the width
		dwSettings.dmPelsWidth		= info.width;
		//Set the height
		dwSettings.dmPelsHeight		= info.height;
		//Specify what fields we are actually using
		dwSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//Now attempt to set our display to Fullscreen
		if (ChangeDisplaySettings(&dwSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			//If it didn't work, then ask the user if we want to use windowed mode instead
			if (MessageBox(	NULL, 
							"Fullscreen failed. Try Windowed instead?", "DiZ Startup Error", 
							MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				//If they choose yes, well, set us to windowed mode (fullscreen = false)
				info.fullscreen = false;
			}else {
				//Otherwise, we return an error and exit
				return false;
			}
		}
	}

	//Depending on whether or not we're still in fullscreen mode, we set our window styles accordingly
	if (info.fullscreen) {
		dwStyle		= WS_POPUP;								//No window edge
		dwExStyle	= WS_EX_APPWINDOW;						//Makes sure other windows are out of the way
	}else {
		dwStyle		= WS_OVERLAPPEDWINDOW;					//Gives us a title, menu etc. in our window
		dwExStyle	= WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	//Gives our window a border
	}

	//Now check if we want to show/hide the cursor and change accordingly
	if (!info.showCursor) {
		ShowCursor(false);
	}

	//Now using our style info, adjust our wndRect structure
	//This gives us window dimensions that give us the viewport size we want while adding in border sizes, menu etc.
	AdjustWindowRectEx(&wndRect, dwStyle, info.menu, dwExStyle);

	//Now attempt to create our actual window
	hWnd = CreateWindowEx(	dwExStyle,										//Use our Extended Styles we set
							"DiZ Window",									//Name of the Window Class
							info.title,										//Use our specified window title
							WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,	//Set some main styles
							0, 0,											//X/Y position of the window
							wndRect.right - wndRect.left,					//Width of the window
							wndRect.bottom - wndRect.top,					//Height of the window
							NULL,											//Parent Window (none)
							NULL,											//Menu (none)
							hInstance,										//Our instance handle
							NULL);											//Any extra params (none)
	//Then check to make sure it succeeded
	if (!hWnd) {
		//If it failed, we kill of our window, report an error and exit
		killWindow();
		MessageBox(NULL, "Could not create window.", "DiZ Startup Error", MB_OK | MB_ICONINFORMATION);
		return false;
	}

	//Check for any errors with our OpenGL creation
	if (!createGL()) {
		return false;
	}

	//Now we set our window to be shown
	ShowWindow(hWnd, SW_SHOW);
	//Now force it in front of other windows
	SetForegroundWindow(hWnd);
	//Get keyboard focus for our application
	SetFocus(hWnd);

	//And exit
	return true;
}

//This function creates our OpenGL scene
bool DIZ_WINDOW::createGL() {
	//First declare a handle for our pixel format for later on
	GLuint pixelFormat;

	//Here we declare our Pixel Format Descriptor- one of the more fiddly bits
	//This is one part of window setup I don't completely get- plenty of values I don't understand
	static PIXELFORMATDESCRIPTOR pfd = {	sizeof(PIXELFORMATDESCRIPTOR),		//Indicator of the size of our structure
											1,									//Version number of the PFD
											PFD_DRAW_TO_WINDOW |				//Here we set some styles we need- Draw to a Window
											PFD_SUPPORT_OPENGL |				//Must of course support OpenGL as well
											PFD_DOUBLEBUFFER |					//Must support Double-Buffering
											PFD_TYPE_RGBA,						//And must have RGBA colour format
											(BYTE)info.bits,					//Set our wanted colour bits
											0, 0, 0, 0, 0, 0,					//We ignore the colour bits
											0,									//Don't worry about an alpha buffer
											0,									//We ignore the shift bit
											0,									//We don't want an accumulation buffer
											0, 0, 0, 0,							//Forget about accumulation bits
											16,									//We want a 16 Bit z-buffer
											0,									//We don't have a stencil buffer
											0,									//And no auxiliary buffer
											PFD_MAIN_PLANE,						//We want to draw to the Main Plane
											0,									//Reserved Data
											0, 0, 0								//And we ignore Layer Masks too
	};

	//Now we try to get a Device Context for our window, check for errors
	hDC = GetDC(hWnd);
	if (!hDC) {
		killWindow();
		MessageBox(NULL, "Can't create DC.", "DiZ Startup Error", MB_OK | MB_ICONINFORMATION);
		return false;
	}

	//And try to find a suitable Pixel Format to suit our descriptor
	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (!pixelFormat) {
		killWindow();
		MessageBox(NULL, "Can't find a pixel format.", "DiZ Startup Error", MB_OK | MB_ICONINFORMATION);
		return false;
	}

	//Attempt to use our Pixel Format once we have it
	if (!SetPixelFormat(hDC, pixelFormat, &pfd)) {
		killWindow();
		MessageBox(NULL, "Can't set pixel format.", "DiZ Startup Error", MB_OK | MB_ICONINFORMATION);
		return false;
	}

	//And attempt to create our Rendering Context
	hRC = wglCreateContext(hDC);
	if (!hRC) {
		killWindow();
		MessageBox(NULL, "Can't create RC.", "DiZ Startup Error", MB_OK | MB_ICONINFORMATION);
		return false;
	}

	//Attempt to set our Rendering Context to use
	if (!wglMakeCurrent(hDC, hRC)) {
		killWindow();
		MessageBox(NULL, "Can't set RC.", "DiZ Startup Error", MB_OK | MB_ICONINFORMATION);
		return false;
	}

	//And set our OpenGL scene to the right size
	resizeGL();

	//Finally, attempt to initiate all our OpenGL settings
	if (!initGL()) {
		killWindow();
		MessageBox(NULL, "Couldn't initiate OpenGL.", "DiZ Startup Error", MB_OK | MB_ICONINFORMATION);
		return false;
	}

	//Then exit
	return true;
}