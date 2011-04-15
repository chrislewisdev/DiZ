/********************************************
*Dizx-window.cpp by Chris Lewis
Defines the DIZX_WINDOW class in dizx-graphics.h
********************************************/

//Include our graphics header
#include "../Include/dizx-graphics.h"

//Define our Constructor- basically initalises all our properties to safe values
DIZX_WINDOW::DIZX_WINDOW() {
	//We don't need to worry about our DC/RC etc. handles so we just initialise our INFO structure
	info.title		= "Default DiZ Window";
	info.width		= 640;
	info.height		= 480;
	info.bits		= 16;
	info.fullscreen	= false;
	info.menu		= false;
	info.showCursor	= true;
	info.wndProc	= NULL;

	//Create our D3D interface and then initialise our settings
	interface3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	//Clear out our settings
	ZeroMemory(&info.d3d, sizeof(D3DPRESENT_PARAMETERS));

	//Back Buffer Count and Format
	info.d3d.BackBufferCount			= 1;
	info.d3d.BackBufferFormat			= D3DFMT_R5G6B5;
	//Multisampling Settings
	info.d3d.MultiSampleType			= D3DMULTISAMPLE_NONE;
	info.d3d.MultiSampleQuality			= 0;
	//Buffer-Swap Setting
	info.d3d.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	//No specific flags
	info.d3d.Flags						= 0;
	//Set our Refresh Rate settings
	info.d3d.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	info.d3d.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;
	//Set our Depth Buffer settings
	info.d3d.EnableAutoDepthStencil		= true;
	info.d3d.AutoDepthStencilFormat		= D3DFMT_D16_LOCKABLE;
	//Set fullscreen settings
	info.d3d.Windowed					= true;
}

//Define our Deconstructor
DIZX_WINDOW::~DIZX_WINDOW() {

}

//This function kills our window
void DIZX_WINDOW::killWindow() {
	//Check if we need to re-show the cursor
	if (!info.showCursor) {
		//And, uh, do so if necessary
		ShowCursor(true);
	}

	//Perform our kill routine
	killD3D();

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

//This function destroys our Direct3D scene
void DIZX_WINDOW::killD3D() {
	//Release our interface
	interface3D9->Release();
}

//This function creates the window according to our info
bool DIZX_WINDOW::createWindow() {
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

	//Now create our graphics scene
	if (!createD3D()) {
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

//This function will create our Direct3D scene
bool DIZX_WINDOW::createD3D() {
	//Declare a HRESULT variable for error checking
	HRESULT hr;

	//Create our Direct3D device
	hr = interface3D9->CreateDevice(	D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&info.d3d,
										&device3D9);
	//Check for an error
	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create Direct3D Device.", "DiZ Startup Error", MB_OK | MB_ICONINFORMATION);
		return false;
	}

	return true;
}