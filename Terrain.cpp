#include<windows.h>
#include<windowsx.h>
#include<stdio.h>
#include<stdlib.h>
#define _USE_MATH_DEFINES 1
//#include<math.h>
#include<CommCtrl.h>

//*** OpenGL Headers ***//
#include<gl/GL.h>
#include<gl/GLU.h>

//*** MACRO'S ***
#define WIN_WIDTH 800
#define WIN_HIGHT 600
#define PI 3.141526
#define TERRAIN_WIDTH  257
#define TERRAIN_LENGTH 257
#define STB_IMAGE_IMPLEMENTATION
#define SPLINE_POINTS 4
#define SPLINE_WEIGHT 0.001
#define FADE_IN 0
#define FADE_OUT 1
#define LEN 64

enum
{
	USESMOOTHERSTEP = 0,
	USELINEAR,
	USELINEARINCREAMENT,
	USEBAZIERSPLINE,
	USEBAZIERCURVE
};

//*** link With OpenGL Library//
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "winmm.lib")

//My Headers
FILE* gpFILE                   = NULL;
GLfloat cloudRot               = 0.0f;
float xxx                      = 0.0f;
GLfloat fovy                   = 45.0f;
float yyy                      = 0.0f;
float zzz                      = 0.0f;
float xx                       = 0.0f;
float yy                       = 0.0f;
float zz                       = 0.0f;
float rrr                      = 0.0f;
HDC ghdc                       = NULL;
BOOL bExtrude                  = TRUE;
static int position            = 0;
BOOL bProgressBar              = FALSE;

#include "Headers/tTime.h"
#include "Headers/OGL.h"
#include "Headers/Utilities.h"
#include "Headers/font.h"

//*** Globle Function Declarations ***
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK splashScreenWndProc(HWND, UINT, WPARAM, LPARAM);

//*** Globle Variable Declaration ***
BOOL gbActive                  = FALSE;
DWORD dwStyle                  = 0;
WINDOWPLACEMENT wpPrev         = { sizeof(WINDOWPLACEMENT) };
BOOL gbFullscreen              = FALSE;
GLUquadric* quadric            = NULL;
HWND ghwnd;
//*** OpenGL Related Globle Variables ***
PIXELFORMATDESCRIPTOR pfd;
int iPexelFormatIndex          = 0;
HGLRC ghrc                     = NULL;
//Time
Timer fpsTimer;
//Camera VAriables
GLuint initialCameraStand      = 0;
Camera camera;
BOOL isMouseDragging           = FALSE;
Vec2f lastMouse, mouse, delta, canvas;
float RotWingOne               = -60.0f;
float RotWingTwo               = -45.0f;
float RotWingThree             = -30.0f;
float sinAngle                 = 0.0f;
BOOL bSinAngle                 = TRUE;
BOOL bBirdRot                  = TRUE;
GLuint scene                   = 1;

//*** MY VARIABLES ***//
// GLfloat shoulder            = 60;
GLfloat shoulder               = 60;
GLfloat elbow                  = 0.0f;
GLfloat elbow1                 = -60.0f;
BOOL bShoulder                 = TRUE;
BOOL bElbow                    = TRUE;
//Texture Object Variable BMP
GLuint texture_bee1;
GLuint texture_test;
GLuint texture_wing;
GLuint texture_beemid;
GLuint texture_treeTrunk;
GLuint texture_treeRoots;
GLuint texture_treeTrunk1;
GLuint texture_treeLeaf;
GLuint texture_treeLeaf1;
GLuint texture_initialSky;
GLuint texture_seaSky;
GLuint texture_rock1;
GLuint texture_sceneOneTerrain;
GLuint texture_seaWaves;
GLuint texture_seaLavaWaves;
//PNG
GLuint pngTree                 = 0;
unsigned char* pngImage        = NULL;

//Light Variables
BOOL bLight                    = TRUE;
GLfloat lightSpecular[]        = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightAmbient[]         = { 0.5f,0.5f,0.5f,1.0f };//Grey Light
GLfloat lightDiffuse[]         = { 2.0f,2.0f,2.0f,1.0f };//White Light
GLfloat lightPosition[]        = { 50.0f,50.0f,10.0f,1.0f };

GLfloat lightSpecular1[]       = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightAmbient1[]        = { -0.3,-0.3,-0.3,1.0f };//Grey Light
GLfloat lightDiffuse1[]        = { -1.8,-1.8,-1.8,1.0f };//White Light
GLfloat lightPosition1[]       = { 50.0f,50.0f,10.0f,1.0f };

GLfloat lightSpecular2[]       = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightAmbient2[]        = { 0.5f,0.5f,0.5f,1.0f };//Grey Light
GLfloat lightDiffuse2[]        = { 2.0f,2.0f,2.0f,1.0f };//White Light
GLfloat lightPosition2[]       = { 0.0f, 0.0f, 1411.0f,1.0f };

GLfloat materialAmbient[]      = { 0.0f,0.0f,0.0f,1.0f };
GLfloat materialDiffuse[]      = { 0.5f,0.2f,0.7f,1.0f };
GLfloat materialSpecular[]     = { 1.0f,1.0f,1.0f,1.0f };
GLfloat materialShininess[]    = { 128.0f };


Transformation bigBee, smallBee;

//Terrain Variables
GLfloat terrain[TERRAIN_WIDTH][TERRAIN_LENGTH][24];
GLfloat water[TERRAIN_WIDTH][TERRAIN_LENGTH][4];
GLfloat terrain_scene01Mountain[TERRAIN_WIDTH][TERRAIN_LENGTH][24];
GLfloat pixSize                = 0.1f;
GLfloat terrainHeight          = 0.2f;
GLfloat colorIntensity;
Vec3f terrainHighColor         = { 0.494, 0.451, 0.322 };
Vec3f terrainLowColor          =  { 0.569, 0.8, 0.424 };

Vec3f waterHighColor           = { 0.42,0.82,0.96 };
Vec3f waterLowColor            = { 0.38,0.78,0.92 };//light
double waterWave               = 0.0f;
double holdVal                 = 0.0f;

//Trials
BOOL colorChange               = FALSE;
BOOL scene1                    = FALSE;
GLuint splCount1               = 0;
GLuint splCount2               = 0;
GLuint splCount3               = 0;
GLuint splCount5               = 0;
GLuint splCount6               = 0;
GLuint splCount7               = 0;
GLuint splCount8               = 0;
GLuint splCount9               = 0;
GLuint splCount10              = 0;
float value                    = 0.03f;
float value1                   = 42.02f;
float value2                   = 62.51f;
float value3                   = 85.01f;
Vec3f spline1[SPLINE_POINTS];
Vec3f spline2[SPLINE_POINTS];
Vec3f spline3[SPLINE_POINTS];
Vec3f spline4[SPLINE_POINTS];
Vec3f spline5[SPLINE_POINTS];
Vec3f spline6[SPLINE_POINTS];
Vec3f spline7[SPLINE_POINTS];
Vec3f spline8[SPLINE_POINTS];
Vec3f spline9[SPLINE_POINTS];
Vec3f spline10[SPLINE_POINTS];
float iWeight                  = 0.0f;
float iWeight1                 = 0.0f;
float iWeight2                 = 0.0f;
float iWeight3                 = 0.0f;
float iWeight4                 = 0.0f;
float iWeight5                 = 0.0f;
float iWeight6                 = 0.0f;
float iWeight7                 = 0.0f;
float iWeight8                 = 0.0f;
float iWeight9                 = 0.0f;
float wingDefaultRotation      = 0.0f;
float lightIntensity           = 1.0f;
///GLOBAL VAR
Vec3f splineData1[18000];
Vec3f splineData2[18000];
Vec3f splineData3[18000];
Vec3f splineData4[18000];
Vec3f splineData5[18000];
Vec3f splineData6[18000];
Vec3f splineData7[18000];
Vec3f splineData8[18000];
Vec3f splineData9[18000];
Vec3f splineData10[18000];
BOOL bScene2                   = TRUE;
BOOL bScene3                   = TRUE;
BOOL bScene4                   = TRUE;
BOOL bScene5                   = TRUE;
BOOL bSmallBeeFlying           = FALSE;
BOOL bSmallBeeFlyingWithBigBee = TRUE;
GLfloat scene3Weight           = 0.01f;
BOOL bInterpolate              = TRUE;
GLfloat birdXRotate            = 0.0f;
GLfloat birdZRotate            = 0.0f;
GLfloat birdYRotate            = 0.0f;
GLfloat bigBeeUpDowsTrans      = 0.0f;
///Fog Variables
GLfloat fogColor[]             = { 0.5f,0.5f,0.5f,1.0f };
///Display List
GLuint treeList                = 0;
GLuint terrainList             = 0;
GLuint rockList                = 0;
GLuint cutedPlantList          = 0;
GLuint mountainList            = 0;
GLuint bigTreeList             = 0;
GLuint honeyCombList           = 0;

GLint mainScene                = 1;
//Alpha
GLfloat alpha                  = 1.0f;
BOOL bBeeWalk                  = FALSE;
BOOL bToggleWalk               = FALSE;
//INTEO AND OUTRO VARIABLES
BOOL bIntro                    = TRUE;
BOOL bOutro                    = TRUE;
GLfloat cAngle                 = 0.0f;
GLfloat pAngle                 = 0.0f;
GLuint viewPortLocationID      = 1;
GLfloat scaleAstromedicomp     = 0.0f;
GLfloat scaleOrtho             = 0.0f;
//SplashScreen And Progress Bar
HWND splashScreenhwnd          = NULL;
HBITMAP hSplashScreenBmp;
BITMAP bitmap;
HDC hSplashScreenDC;
HDC hMemoryDC;
HWND prograceBarHandle;
UINT_PTR timer                 = 0;
HWND staticTextHandle_01;
///
GLfloat plantFallX             = -75.0f;
GLfloat plantFallY             = -75.0f;
GLfloat plantFallZ             = -75.0f;
BOOL bPlantFall                = FALSE;
BOOL bTexSwitchSea             = TRUE;
BOOL bTexSwitchLava            = FALSE;
GLfloat alphaSeaTexture        = 1.0f;
GLfloat alphaSeaLavaTexture    = 0.0f;
GLint ctr                      = 0;
float circle[3600][2];
GLint circleArrayCounter       = 0;
float i                        = 0;
float theata                   = 0;

//*** Enrty-point Function ***
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//*** Function Declarations ***
	int initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);
	void CALLBACK onTimerTick(HWND, UINT, UINT_PTR, DWORD);

	//*** Local Variable Declarations ***
	WNDCLASSEX wndclass;
	WNDCLASSEX splashWindowClass;
	HWND hwnd;
	TCHAR szAppName[] = TEXT("Tushar_Wagdare_Chi_Window");
	TCHAR splashScreenWindowClassName[] = TEXT("SplashScreenWindowClass");
	MSG msg;
	int iResult = 0;
	BOOL bDone = FALSE;
	//For resolution
	int iWinWidth, iWinHight;
	iWinWidth = GetSystemMetrics(SM_CXSCREEN);
	iWinHight = GetSystemMetrics(SM_CYSCREEN);


	//*** Code ***
	gpFILE = fopen("Log.txt", "w");
	if (gpFILE == NULL)
	{
		MessageBox(NULL, TEXT("Log Create Hou Shakat Nahi"), TEXT("Error"), MB_OK || MB_ICONERROR);
		exit(0);
	}
	fprintf(gpFILE, "Program Started Successfulle\n");
	fprintf(gpFILE, "W = %d  H = %d\n", iWinWidth, iWinHight);


	//*** WNDCLASSEX INITIALIZATION ***
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));


	//*** REGISTER WNDCLASSEX ***
	RegisterClassEx(&wndclass);


	//*** CREATE WINDOW ***
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Tushar Tulshiram Wagdare"),
		WS_OVERLAPPEDWINDOW,
		(iWinWidth / 2) - (800 / 2),
		(iWinHight / 2) - (600 / 2),
		WIN_WIDTH,
		WIN_HIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;


	//*** WNDCLASSEX INITIALIZATION FOR SPLASH SCREEN ***
	splashWindowClass.cbSize = sizeof(WNDCLASSEX);
	splashWindowClass.style = 0;
	splashWindowClass.cbClsExtra = 0;
	splashWindowClass.cbWndExtra = 0;
	splashWindowClass.lpfnWndProc = (WNDPROC)splashScreenWndProc;
	splashWindowClass.hInstance = hInstance;
	splashWindowClass.hIcon = NULL;
	splashWindowClass.hIconSm = NULL;
	splashWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	splashWindowClass.lpszClassName = splashScreenWindowClassName;
	splashWindowClass.lpszMenuName = NULL;
	splashWindowClass.hbrBackground = NULL;

	if (!RegisterClassEx(&splashWindowClass))
	{
		MessageBox(NULL, TEXT("Failed To Register Splash Window Class"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return(0);
	}

	RECT DesktopRect;
	GetWindowRect(GetDesktopWindow(), &DesktopRect);

	hSplashScreenBmp = LoadBitmap(hInstance, MAKEINTRESOURCE(SPLASHSCREEN_ID));
	if (!hSplashScreenBmp)
	{
		MessageBox(NULL, TEXT("Failed To Load BMP"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return(0);
	}


	GetObject(hSplashScreenBmp, sizeof(BITMAP), &bitmap);


	splashScreenhwnd = CreateWindowEx(0,
		splashScreenWindowClassName,
		TEXT("SplashScreen"),
		WS_POPUP,
		(DesktopRect.right - bitmap.bmWidth) / 2,
		(DesktopRect.bottom - bitmap.bmHeight) / 2,
		bitmap.bmWidth,
		bitmap.bmHeight,
		NULL,
		NULL,
		hInstance,
		NULL);

	prograceBarHandle = CreateWindowEx(0,
		PROGRESS_CLASS,
		nullptr,
		WS_CHILD | PBS_MARQUEE | WS_VISIBLE,
		150 * 0.6354, 950 * 0.6351,
		1600 * 0.6354, 40 * 0.6351,
		splashScreenhwnd,
		nullptr,
		nullptr,
		nullptr);

	COLORREF green = RGB(124, 91, 5);
	//COLORREF green = RGB(184, 134, 11);
	COLORREF grey = RGB(127, 127, 127);

	//Setting The status of ProgressBar
	SendMessage(prograceBarHandle, PBM_SETPOS, 0, 100);
	SendMessage(prograceBarHandle, PBM_SETBARCOLOR, 0, (LPARAM)green);
	SendMessage(prograceBarHandle, PBM_SETBKCOLOR, 0, (LPARAM)grey);

	//Set Timer for Progress BAr
	if (bProgressBar == FALSE)
		timer = SetTimer(splashScreenhwnd, 0, 10, onTimerTick);///20 sec final 

	hSplashScreenDC = GetDC(splashScreenhwnd);
	hMemoryDC = CreateCompatibleDC(hSplashScreenDC);
	SelectObject(hMemoryDC, (HGDIOBJ)hSplashScreenBmp);
	ShowWindow(splashScreenhwnd, SW_SHOW);
	UpdateWindow(splashScreenhwnd);


	//*** Initialization ***
	iResult = initialize();
	if (iResult != 0)
	{
		MessageBox(hwnd, TEXT("initialize() Failed !"), TEXT("Error"), MB_OK || MB_ICONERROR);
		DestroyWindow(hwnd);
	}
	fprintf(gpFILE, "iResult initialization successfull\n");


	//Initializing the frequency and start counter
	fpsTimer.initializeTimer();


	//*** Game LOOP ***
	while (bDone == FALSE)
	{
		fpsTimer.startTimer();
		fpsTimer.getElapsedTime();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bDone = TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActive == TRUE && bProgressBar)
			{
				//*** Render ***
				display();

				//*** Update ***
				update();

				fpsTimer.displayTime(hwnd);

				fpsTimer.frameCount++;

				fpsTimer.evaluateTimer();
			}
		}
	}


	//*** Uninitialization ***
	uninitialize();


	return((int)msg.wParam);
}



//*** CALLBACK FUNCTION ***
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//*** Function Declaration ***
	void ToggleFullscreen(void);
	void resize(int, int);


	//*** Code ***
	switch (iMsg)
	{
	case WM_SETFOCUS:
		gbActive = TRUE;
		break;

	case WM_KILLFOCUS:
		gbActive = FALSE;
		break;

	case WM_SIZE:
		canvas.x = LOWORD(lParam);
		canvas.y = HIWORD(lParam);
		resize(canvas.x, canvas.y);
		break;

	case WM_ERASEBKGND:
		return(0);

	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		isMouseDragging = TRUE;
		lastMouse.x = GET_X_LPARAM(lParam);
		lastMouse.y = GET_Y_LPARAM(lParam);
		SetCapture(ghwnd);
		break;

	case WM_MOUSEMOVE:
		if (isMouseDragging)
		{
			mouse.x = GET_X_LPARAM(lParam);
			mouse.y = GET_Y_LPARAM(lParam);

			delta.x = mouse.x - lastMouse.x;
			delta.y = mouse.y - lastMouse.y;

			camera.yaw += delta.x * 0.01f;
			camera.pitch -= delta.y * 0.01f;

			if (camera.pitch > 89.0f)
			{
				camera.pitch = 89.0f;
			}
			if (camera.pitch < -89.0f)
			{
				camera.pitch = -89.0f;
			}

			camera.front.x = cos(toRadians(camera.yaw)) * cos(toRadians(camera.pitch));
			camera.front.y = sin(toRadians(camera.pitch));
			camera.front.z = sin(toRadians(camera.yaw)) * cos(toRadians(camera.pitch));
			NormalizeVec3f(&camera.front);

			camera.up = { 0.0f,1.0f,0.0f };
		}
		break;

	case WM_LBUTTONUP:
		isMouseDragging = FALSE;
		ReleaseCapture();
		break;

	case WM_CHAR:
		switch (LOWORD(wParam))
		{
		case 'F':
		case 'f':
			if (gbFullscreen == FALSE)
			{
				ToggleFullscreen();
				gbFullscreen = TRUE;
			}
			else
			{
				ToggleFullscreen();
				gbFullscreen = FALSE;
			}
			break;

		case 'L':
		case 'l':
			if (bLight == FALSE)
			{
				glEnable(GL_LIGHTING);
				bLight = TRUE;
			}
			else
			{
				glDisable(GL_LIGHTING);
				bLight = FALSE;
			}
			break;

		case 'W':
		case 'w':
			camera.position.x += camera.front.x * 10;
			camera.position.y += camera.front.y * 10;
			camera.position.z += camera.front.z * 10;
			break;

		case 'S':
		case 's':
			camera.position.x -= camera.front.x * 10;
			camera.position.y -= camera.front.y * 10;
			camera.position.z -= camera.front.z * 10;
			break;

		case 'A':
		case 'a':
			camera.right = cross(camera.front, camera.up);
			camera.position.x -= camera.right.x * 10;
			camera.position.y -= camera.right.y * 10;
			camera.position.z -= camera.right.z * 10;

			//camera.position.x -= 0.5f;
			break;

		case 'D':
		case 'd':
			camera.right = cross(camera.front, camera.up);
			camera.position.x += camera.right.x * 10;
			camera.position.y += camera.right.y * 10;
			camera.position.z += camera.right.z * 10;
			break;

		case 'E':
		case 'e':
			camera.position.y += camera.up.y * 10;
			break;

		case 'Q':
		case 'q':
			camera.position.y -= camera.up.y * 10;
			break;

		case 'm':
		case 'M':
			if (bigBee.bBeeFly == TRUE)
			{
				bigBee.bBeeFly = FALSE;
				bigBee.bBeeWings = FALSE;

				smallBee.bBeeFly = FALSE;
				smallBee.bBeeWings = FALSE;
			}
			else
			{
				bigBee.bBeeFly = TRUE;
				bigBee.bBeeWings = TRUE;

				smallBee.bBeeFly = TRUE;
				smallBee.bBeeWings = TRUE;
			}
			break;

		case 'o':
			scene++;
			scene1 = TRUE;
			break;

		case 'X':
			xxx = xxx + 0.01f;

			break;

		case 'x':
			xxx = xxx - 0.01f;
			break;

		case 'Y':
			yyy = yyy + 0.01f;
			break;

		case 'y':
			yyy = yyy - 0.01f;
			break;

		case 'Z':
			zzz = zzz + 0.01f;
			break;

		case 'z':
			zzz = zzz - 0.01f;
			break;


		case 'H':
			/*camera.position.x += camera.front.x * 0.1f;
			camera.position.y += camera.front.y * 0.1f;
			camera.position.z += camera.front.z * 0.1f;*/
			//xxx += 0.1f;
			alpha += 0.01f;
			break;

		case 'h':
			/*camera.position.x -= camera.front.x * 0.1f;
			camera.position.y -= camera.front.y * 0.1f;
			camera.position.z -= camera.front.z * 0.1f;*/
			//xxx -= 0.1f;
			alpha -= 0.01f;
			break;

		case 'J':
			/*camera.right = cross(camera.front, camera.up);
			camera.position.x -= camera.right.x * 0.1f;
			camera.position.y -= camera.right.y * 0.1f;
			camera.position.z -= camera.right.z * 0.1f;*/
			yyy += 0.1f;
			break;

		case 'j':
			/*camera.right = cross(camera.front, camera.up);
			camera.position.x += camera.right.x * 0.1f;
			camera.position.y += camera.right.y * 0.1f;
			camera.position.z += camera.right.z * 0.1f;*/
			yyy -= 0.1f;
			break;

		case 'K':
			//camera.position.y += camera.up.y * 0.1f;
			zzz += 0.1f;
			break;

		case 'k':
			//camera.position.y -= camera.up.y * 0.1f;
			zzz -= 0.1f;
			break;

		case 'R':
			rrr = rrr + 0.01f;
			break;

		case 'r':
			rrr = rrr - 0.01f;
			break;

		case 'N':
			fovy += 0.1f;
			resize(canvas.x, canvas.y);
			break;

		case 'n':
			fovy -= 0.1f;
			resize(canvas.x, canvas.y);
			break;


		}
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}


	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}




LRESULT CALLBACK splashScreenWndProc(HWND hwnd_Splash, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_ERASEBKGND:
		BitBlt((HDC)wParam, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hMemoryDC, 0, 0, SRCCOPY);
		break;

	default:
		break;
	}

	return(DefWindowProc(hwnd_Splash, iMsg, wParam, lParam));
}




void CALLBACK onTimerTick(HWND hwnd, UINT msg, UINT_PTR igEvent, DWORD time)
{
	void ToggleFullscreen(void);
	void resize(int, int);

	if (position == 100)
	{
		///SPLASH SCREEN
		{
			DeleteObject(hSplashScreenBmp);
			ReleaseDC(splashScreenhwnd, hSplashScreenDC);
			ReleaseDC(splashScreenhwnd, hMemoryDC);
			DestroyWindow(splashScreenhwnd);

			//*** SHOW THE WINDOW ***
			ShowWindow(ghwnd, SW_SHOW);
			UpdateWindow(ghwnd);
			SetForegroundWindow(ghwnd);//mazya window la vr ann...
			SetFocus(ghwnd);
		}
		//Play Sound
		PlaySound(MAKEINTRESOURCE(MY_AUDIO), NULL, SND_RESOURCE | SND_ASYNC);

		ToggleFullscreen();

		bProgressBar = TRUE;
	}
	else
	{
		position += 1;
		SendMessage(prograceBarHandle, PBM_SETPOS, position, 0);
	}
}




void ToggleFullscreen(void)
{
	//*** Local Variable Declaration ****
	MONITORINFO mi = { sizeof(MONITORINFO) };


	//*** Code ***
	if (gbFullscreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			fprintf(gpFILE, "Window Contains WS_OVERLAPPEDWINDOW\n");

			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
	}
	else
	{
		fprintf(gpFILE, "Window Is Now Already In Fullscreen Mode\n");
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
		fprintf(gpFILE, "Now Window Is Normal\n");
	}
}



int initialize(void)
{
	//*** Function Declarations ***
	void resize(int, int);
	BOOL loadGLTexture(GLuint*, TCHAR[]);
	void initializeTerrain(void);
	void initializeTerrain1(void);
	void drawTerrainScene1(void);
	void initializeLight(void);
	void drawTree(void);
	void ToggleFullscreen(void);
	void drawCutedTree(void);
	void drawTerrainMountain(void);
	void drawHoneyComb(void);
	void generatePerlinNoise(void);

	//Variable Declaration
	BOOL bResult = FALSE;
	bigBee.yRotate = 180.0f;
	smallBee.yRotate = 10.0f;

	//01 -  Initialization Of PFD 
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;


	//02 - Get The DC
	ghdc = GetDC(ghwnd);
	if (ghdc == NULL)
	{
		fprintf(gpFILE, "GetDC Failed !!!\n\n");
		return(-1);
	}

	//03 - Tell OS Hya PFD shi match honara PFD de
	iPexelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPexelFormatIndex == 0)
	{
		fprintf(gpFILE, "ChoosepixelFormat() Failed\n\n");
		return(-2);
	}

	//04 - Set Obtained Pixel Format
	if (SetPixelFormat(ghdc, iPexelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFILE, "SetPixelFormat() Failed\n\n");
		return(-3);
	}

	//05 - Tell WGL Bridging library To Give OpenGL Copatible DC From 'ghdc'
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		fprintf(gpFILE, "wglCreateContex() Failed\n\n");
		return(-4);
	}

	//06 - Now 'ghdc' End Its Roll And Give Controll To 'ghrc'
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFILE, "wglMakeCurrent() failed\n\n");
		return(-5);
	}

	//Font
	buildFont(1);

	//07 - 2nd step Enabling Depth
	glShadeModel(GL_SMOOTH);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//08 - Set the Clear Color of Window To Blue
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);


	//Initialize Quadric
	quadric = gluNewQuadric();


	///BMP TEXTURE LOADING
	bResult = loadGLTexture(&texture_bee1, MAKEINTRESOURCE(TEXTURE_BEE));
	bResult = loadGLTexture(&texture_wing, MAKEINTRESOURCE(TEXTURE_WING));
	bResult = loadGLTexture(&texture_beemid, MAKEINTRESOURCE(TEXTURE_BEEMID));
	bResult = loadGLTexture(&texture_treeTrunk, MAKEINTRESOURCE(TEXTURE_TREE_TRUNK));
	bResult = loadGLTexture(&texture_treeRoots, MAKEINTRESOURCE(TEXTURE_TREE_ROOTS));
	bResult = loadGLTexture(&texture_treeTrunk1, MAKEINTRESOURCE(TEXTURE_TREE_TRUNK1));
	bResult = loadGLTexture(&texture_treeLeaf, MAKEINTRESOURCE(TEXTURE_TREE_LEAF));
	bResult = loadGLTexture(&texture_treeLeaf1, MAKEINTRESOURCE(TEXTURE_TREE_LEAF1));
	bResult = loadGLTexture(&texture_initialSky, MAKEINTRESOURCE(TEXTURE_INITIAL_SKY));
	bResult = loadGLTexture(&texture_test, MAKEINTRESOURCE(TEXTURE_TEST));
	bResult = loadGLTexture(&texture_rock1, MAKEINTRESOURCE(TEXTURE_ROCK1));
	bResult = loadGLTexture(&texture_seaSky, MAKEINTRESOURCE(TEXTURE_SEA_SKY));
	bResult = loadGLTexture(&texture_sceneOneTerrain, MAKEINTRESOURCE(TEXTURE_TERRAIN_ONE));
	bResult = loadGLTexture(&texture_seaWaves, MAKEINTRESOURCE(TEXTURE_SEA_WAVES));
	bResult = loadGLTexture(&texture_seaLavaWaves, MAKEINTRESOURCE(TEXTURE_SEA_LAVA_WAVES));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "loadGLTexture() failed\n\n");
		return(-6);
	}

	//Enable texture for quadric object
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricTexture(quadric, TRUE);
	gluQuadricNormals(quadric, GLU_SMOOTH);



	//Tell OpenGL To Enable Texture
	//Light Related Initialization
	initializeLight();
	glEnable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/

	//Enable Light

	//Initialize Terrain
	initializeTerrain();
	initializeTerrain1();
	generatePerlinNoise();

	//Initialize Camera
	camera = createCamera({ -155.0f, 5.0f, 50.0f }, { -155.0f, 5.0f, -3.0f }, { 0.0f, 1.0f, 0.0f }, 270.0f, 0.0f, -5.0f);

	// Tell OpenGL To Enable The Texture
	glEnable(GL_TEXTURE_2D);

	//png blending

	///SPLINE DATA
	{
		//scene 1 camera
		spline1[0] = { -400.0f,40.0f,125.0f };
		spline1[1] = { -300.0f,5.0f,0.0f };
		spline1[2] = { -100.0f,5.0f,150.0f };
		spline1[3] = { -17.0f,10.0f,51.0f };

		//scene 1 lookat
		spline2[0] = { -400.0f,40.0f,125.0f };
		spline2[1] = { -300.0f,15.0f,100.0f };
		spline2[2] = { -200.0f,5.0f,75.0f };
		spline2[3] = { -19.75f,9.6f,51.88f };

		//scene 2 position
		spline3[0] = { 14.75, 0.2f,  70.0f };
		spline3[1] = { 20.0f, 0.2f,  5.0f };
		spline3[2] = { 15.0f, 0.2f, -19.0f };
		spline3[3] = { 18.0f, 0.2f, -150.00f };

		//scene 2 camera
		spline4[0] = { 20.0,  1.5f,    80.0f };
		spline4[1] = { 20.0f, 2.25f,   14.0f };
		spline4[2] = { 15.0f, 2.25f,  -19.0f };
		spline4[3] = { 100.0f,2.8f,   -150.0f };

		//scene 3 camera
		spline5[0] = { -120.8f,  100.0f,-300.0f };
		spline5[1] = { -80.0f, 90.0f, -255.0f };
		spline5[2] = { -100.0f, 2.0f,  -170.0f };
		spline5[3] = { -22.0f, 3.5f,  -218.0f };

		//bird
		spline6[0] = { 150.0f,   150.0f,  -100.0f };
		spline6[1] = { 80.0f,    70.0f,  -150.0f };
		spline6[2] = { 30.0f,   30.5f,  -200.0f };
		spline6[3] = { -50.0f,  -0.0f,  -250.0f };

		//Bee flying with his kid
		spline7[0] = { 0.0f,   0.0f,  0.0f };
		spline7[1] = { 50,    50.0f,  -100.0f };
		spline7[2] = { 100,   100.5f,  -200.0f };
		spline7[3] = { 150.0f,  150.0f,  -300.0f };


		///main scene 2 camera
		spline8[0] = { -400.0f,40.0f,125.0f };
		spline8[1] = { -300.0f,5.0f,0.0f };
		spline8[2] = { -100.0f,5.0f,150.0f };
		spline8[3] = { -17.0f,10.5f,51.0f };

		///main scene 2 lookat
		spline9[0] = { -400.0f,40.0f,100.0f };
		spline9[1] = { -300.0f,45.0f,100.0f };
		spline9[2] = { -200.0f,45.0f,100.0f };
		spline9[3] = { -19.75f,40.0f,100.0f };

		///main scene 2 bigBeeCircle
		/*spline10[0] = { -352.819336, 50.977524, 112.288536 };
		spline10[1] = { -353.819336, 50.977524, 113.288536 };
		spline10[2] = { -352.819336, 50.977524, 114.288536 };
		spline10[3] = { -353.819336, 50.977524, 113.288536 };*/

		spline10[0] = { -350.0, 50.0,  112.0 };
		spline10[1] = { -370.0, 120.0, 300.0 };
		spline10[2] = { -400.0, 190.0, 500.0 };
		spline10[3] = { -500.0, 250.0, 113.0 };


		drawSpline(spline1, splineData1, sizeof(spline1), SPLINE_WEIGHT, USEBAZIERCURVE, USESMOOTHERSTEP);
		drawSpline(spline2, splineData2, sizeof(spline2), SPLINE_WEIGHT, USEBAZIERCURVE, USESMOOTHERSTEP);
		drawSpline(spline3, splineData3, sizeof(spline3), 0.0005, USEBAZIERCURVE, USESMOOTHERSTEP);
		drawSpline(spline4, splineData4, sizeof(spline4), 0.0005, USEBAZIERCURVE, USESMOOTHERSTEP);
		drawSpline(spline5, splineData5, sizeof(spline5), 0.0005, USEBAZIERCURVE, USESMOOTHERSTEP);
		drawSpline(spline6, splineData6, sizeof(spline6), 0.0005, USEBAZIERCURVE, USESMOOTHERSTEP);
		drawSpline(spline7, splineData7, sizeof(spline7), 0.0005, USEBAZIERCURVE, USESMOOTHERSTEP);
		drawSpline(spline8, splineData8, sizeof(spline8), 0.0005, USEBAZIERCURVE, USESMOOTHERSTEP);
		drawSpline(spline9, splineData9, sizeof(spline9), 0.0005, USEBAZIERCURVE, USESMOOTHERSTEP);
		drawSpline(spline10, splineData10, sizeof(spline10), 0.0005, USEBAZIERCURVE, USESMOOTHERSTEP);
	}

	float theata = 0.0f;
	float x, y;
	///Circle
	for (int i = 0.0f; i < 3600.0f; i++)
	{
		theata = ((float)i / 10.0f) * (M_PI / 180);

		x = 1.7f * cos(theata);
		y = 1.7f * sin(theata);

		circle[i][0] = x;
		circle[i][1] = y;
	}
	

	///FOG BLOCK
	{
		glFogi(GL_FOG_MODE, GL_EXP2);//Setting the fog mode
		glFogfv(GL_FOG_COLOR, fogColor);//setting the fog color
		glFogf(GL_FOG_DENSITY, 0.00165);//setting the fog density
		glFogf(GL_FOG_START, 5.0f);
		glFogf(GL_FOG_END, 10.0f);
		glHint(GL_FOG_HINT, GL_NICEST);
		glEnable(GL_FOG);
	}


	///DISPLAY LIST
	{
		treeList = glGenLists(7);
		glNewList(treeList, GL_COMPILE);
		colorChange = TRUE;
		drawTree();
		glEndList();

		terrainList = treeList + 1;
		glNewList(terrainList, GL_COMPILE);
		drawTerrainScene1();
		glEndList();

		rockList = treeList + 2;
		glNewList(rockList, GL_COMPILE);
		///STONES
		{
			glPushMatrix();
			glTranslatef(-20, 0, 50);
			glBindTexture(GL_TEXTURE_2D, texture_rock1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 10.0f, 6, 5);
			glTranslatef(0.0f, 0.0f, -10.0f);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 7.0f, 6, 5);
			glTranslatef(5, -1, 1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 5.0f, 6, 5);
			glBindTexture(GL_TEXTURE_2D, 0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(90.0f, 0.0f, -90.0f);

			glBindTexture(GL_TEXTURE_2D, texture_rock1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 10.0f, 6, 5);
			glTranslatef(0.0f, 0.0f, -10.0f);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 7.0f, 6, 5);
			glTranslatef(5, -1, 1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 5.0f, 6, 5);
			glBindTexture(GL_TEXTURE_2D, 0);

			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(110, 0, 30);
			glRotatef(70, 0.0f, 1.0f, 0.0f);

			glBindTexture(GL_TEXTURE_2D, texture_rock1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 10.0f, 6, 5);
			glTranslatef(0.0f, 0.0f, -10.0f);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 7.0f, 6, 5);
			glTranslatef(5, -1, 1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 5.0f, 6, 5);
			glBindTexture(GL_TEXTURE_2D, 0);

			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(240, 0, -90);
			glRotatef(-90, 0.0f, 1.0f, 0.0f);

			glBindTexture(GL_TEXTURE_2D, texture_rock1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 10.0f, 6, 5);
			glTranslatef(0.0f, 0.0f, -10.0f);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 7.0f, 6, 5);
			glTranslatef(5, -1, 1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 5.0f, 6, 5);
			glBindTexture(GL_TEXTURE_2D, 0);

			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(150, 0, -212);
			glRotatef(-157, 0.0f, 1.0f, 0.0f);

			glBindTexture(GL_TEXTURE_2D, texture_rock1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 10.0f, 6, 5);
			glTranslatef(0.0f, 0.0f, -10.0f);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 7.0f, 6, 5);
			glTranslatef(5, -1, 1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 5.0f, 6, 5);
			glBindTexture(GL_TEXTURE_2D, 0);

			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(151, 0, 188);

			glBindTexture(GL_TEXTURE_2D, texture_rock1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 10.0f, 6, 5);
			glTranslatef(0.0f, 0.0f, -10.0f);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 7.0f, 6, 5);
			glTranslatef(5, -1, 1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 5.0f, 6, 5);
			glBindTexture(GL_TEXTURE_2D, 0);

			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(272, 0, 110);
			glRotatef(-166, 0.0f, 1.0f, 0.0f);

			glBindTexture(GL_TEXTURE_2D, texture_rock1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 10.0f, 6, 5);
			glTranslatef(0.0f, 0.0f, -10.0f);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 7.0f, 6, 5);
			glTranslatef(5, -1, 1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 5.0f, 6, 5);
			glBindTexture(GL_TEXTURE_2D, 0);

			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(240, 0, -5);
			glRotatef(5, 0.0f, 1.0f, 0.0f);

			glBindTexture(GL_TEXTURE_2D, texture_rock1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 10.0f, 6, 5);
			glTranslatef(0.0f, 0.0f, -10.0f);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 7.0f, 6, 5);
			glTranslatef(5, -1, 1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 5.0f, 6, 5);
			glBindTexture(GL_TEXTURE_2D, 0);

			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();




			glPushMatrix();
			glTranslatef(2 * -90.0f, 0.0f, -90.0f);

			glBindTexture(GL_TEXTURE_2D, texture_rock1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 10.0f, 6, 5);
			glTranslatef(2 * -0.0f, 0.0f, -10.0f);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 7.0f, 6, 5);
			glTranslatef(2 * -5, -1, 1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 5.0f, 6, 5);
			glBindTexture(GL_TEXTURE_2D, 0);

			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(2 * -110, 0, 30);
			glRotatef(70, 0.0f, 1.0f, 0.0f);

			glBindTexture(GL_TEXTURE_2D, texture_rock1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 10.0f, 6, 5);
			glTranslatef(2 * -0.0f, 0.0f, -10.0f);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 7.0f, 6, 5);
			glTranslatef(2 * -5, -1, 1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 5.0f, 6, 5);
			glBindTexture(GL_TEXTURE_2D, 0);

			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(2 * -240, 0, -90);
			glRotatef(-90, 0.0f, 1.0f, 0.0f);

			glBindTexture(GL_TEXTURE_2D, texture_rock1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 10.0f, 6, 5);
			glTranslatef(2 * -0.0f, 0.0f, -10.0f);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 7.0f, 6, 5);
			glTranslatef(2 * -5, -1, 1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 5.0f, 6, 5);
			glBindTexture(GL_TEXTURE_2D, 0);

			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(2 * -150, 0, -212);
			glRotatef(-157, 0.0f, 1.0f, 0.0f);

			glBindTexture(GL_TEXTURE_2D, texture_rock1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 10.0f, 6, 5);
			glTranslatef(2 * -0.0f, 0.0f, -10.0f);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 7.0f, 6, 5);
			glTranslatef(2 * -5, -1, 1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 5.0f, 6, 5);
			glBindTexture(GL_TEXTURE_2D, 0);

			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(2 * -151, 0, 188);

			glBindTexture(GL_TEXTURE_2D, texture_rock1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 10.0f, 6, 5);
			glTranslatef(2 * -0.0f, 0.0f, -10.0f);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 7.0f, 6, 5);
			glTranslatef(2 * -5, -1, 1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 5.0f, 6, 5);
			glBindTexture(GL_TEXTURE_2D, 0);

			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(2 * -272, 0, 110);
			glRotatef(-166, 0.0f, 1.0f, 0.0f);

			glBindTexture(GL_TEXTURE_2D, texture_rock1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 10.0f, 6, 5);
			glTranslatef(2 * -0.0f, 0.0f, -10.0f);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 7.0f, 6, 5);
			glTranslatef(2 * -5, -1, 1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 5.0f, 6, 5);
			glBindTexture(GL_TEXTURE_2D, 0);

			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(2 * -240, 0, -5);
			glRotatef(5, 0.0f, 1.0f, 0.0f);

			glBindTexture(GL_TEXTURE_2D, texture_rock1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 10.0f, 6, 5);
			glTranslatef(2 * -0.0f, 0.0f, -10.0f);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 7.0f, 6, 5);
			glTranslatef(2 * -5, -1, 1);
			glColor3f(0.25, 0.25, 0.25);
			gluSphere(quadric, 5.0f, 6, 5);
			glBindTexture(GL_TEXTURE_2D, 0);

			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();
		}
		glEndList();

		cutedPlantList = treeList + 3;
		glNewList(cutedPlantList, GL_COMPILE);
		///CUTED PLANTS -X
		{
			glPushMatrix();
			glTranslatef(-90.0f, -1.0f, -90.0f);
			glScalef(0.5f, 0.5f, 0.5f);
			drawCutedTree();
			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-110, 1, 30);
			glScalef(0.5f, 0.5f, 0.5f);
			glRotatef(70, 0.0f, 1.0f, 0.0f);
			drawCutedTree();
			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-240, -1, -90);
			glScalef(0.5f, 0.5f, 0.5f);
			glRotatef(-90, 0.0f, 1.0f, 0.0f);
			drawCutedTree();
			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-150, -1, -212);
			glScalef(0.5f, 0.5f, 0.5f);
			glRotatef(-157, 0.0f, 1.0f, 0.0f);
			drawCutedTree();
			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-151, 0.0, 188);
			glScalef(0.5f, 0.5f, 0.5f);
			drawCutedTree();
			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-272, -2, 110);
			glScalef(0.5f, 0.5f, 0.5f);
			glRotatef(-166, 0.0f, 1.0f, 0.0f);
			drawCutedTree();
			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-240, -1, -5);
			glScalef(0.5f, 0.5f, 0.5f);
			glRotatef(5, 0.0f, 1.0f, 0.0f);
			drawCutedTree();
			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-64, 2, 92);
			glScalef(0.5f, 0.5f, 0.5f);
			glRotatef(-120 , 0.0f, 1.0f, 0.0f);
			drawCutedTree();
			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();
		}
		glEndList();

		mountainList = treeList + 4;
		glNewList(mountainList, GL_COMPILE);
		///TERRAIN 2
		{
			glPushMatrix();
			glPushMatrix();

			glTranslatef(-570.0f, 0.0f, -3.0f);
			glRotatef(-10, 0.0f, 1.0f, 0.0f);
			glScalef(8.0f, 800.0f, 70.0f);
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			drawTerrainMountain();
			glColor3f(1.0f, 1.0f, 1.0f);

			glPopMatrix();

			glTranslatef(-260.0f, 0.0f, 460.0f);
			glRotatef(-130, 0.0f, 1.0f, 0.0f);
			glScalef(8.0f, 800.0f, 70.0f);
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			drawTerrainMountain();
			glColor3f(1.0f, 1.0f, 1.0f);

			glPopMatrix();
		}
		glEndList();

		bigTreeList = treeList + 5;
		glNewList(bigTreeList, GL_COMPILE);
		///Draw Big TREES
		{
			glEnable(GL_LIGHT0);
			glPushMatrix();
			glPushMatrix();

			glPushMatrix();
			glPushMatrix();
			glPushMatrix();

			glPushMatrix();
			glPushMatrix();
			glPushMatrix();
			glPushMatrix();

			glTranslatef(-22, 1, -240);
			glScalef(2.0f, 1.5f, 2.0f);
			colorChange = FALSE;
			glPushMatrix();
			drawTree();
			glPopMatrix();
			glTranslatef(0.0f, -5.0f, 00.0f);
			glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
			drawTree();
			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();

			glTranslatef(71.0f, 0.0f, -434.0f);
			glScalef(1.5f, 1.0f, 1.5f);
			glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
			colorChange = FALSE;
			drawTree();
			glPopMatrix();

			glTranslatef(-220.0f, 0.0f, -500.0f);
			glScalef(1.0f, 1.0f, 1.0f);
			glRotatef(-50.0f, 0.0f, 1.0f, 0.0f);
			colorChange = FALSE;
			drawTree();
			glPopMatrix();

			glTranslatef(-100.0f, 0.0f, -600.0f);
			glScalef(0.75f, 0.75f, 0.75f);
			glRotatef(-70.0f, 0.0f, 1.0f, 0.0f);
			colorChange = FALSE;
			drawTree();
			glPopMatrix();

			glTranslatef(0.0f, 0.0f, -700.0f);
			glScalef(0.5f, 0.5f, 0.5f);
			glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
			colorChange = FALSE;
			drawTree();
			glPopMatrix();



			glTranslatef(-150.0f, 0.0f, -434.0f);
			glScalef(1.5f, 1.0f, 1.5f);
			glRotatef(120.0f, 0.0f, 1.0f, 0.0f);
			colorChange = FALSE;
			drawTree();
			glPopMatrix();

			glTranslatef(-250.0f, 0.0f, -500.0f);
			glScalef(1.0f, 1.0f, 1.0f);
			glRotatef(150.0f, 0.0f, 1.0f, 0.0f);
			colorChange = FALSE;
			drawTree();
			glPopMatrix();

			glTranslatef(-350.0f, 0.0f, -600.0f);
			glScalef(0.75f, 0.75f, 0.75f);
			glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			colorChange = FALSE;
			drawTree();
			glPopMatrix();

			glTranslatef(-420.0f, 0.0f, -700.0f);
			glScalef(0.5f, 0.5f, 0.5f);
			glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
			colorChange = FALSE;
			drawTree();
			glPopMatrix();
		}
		glEndList();

		honeyCombList = treeList + 6;
		glNewList(honeyCombList, GL_COMPILE);
		drawHoneyComb();
		glEndList();
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return(0);
}


BOOL loadGLTexture(GLuint* texture, TCHAR imageResourceID[])
{
	//Local Variable Declarations
	HBITMAP hBitmap = NULL;
	BITMAP bmp;

	//Code
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (hBitmap == FALSE)
	{
		fprintf(gpFILE, "LoadImage() failed\n\n");
		return(FALSE);
	}

	//Get Image Data
	GetObject(hBitmap, sizeof(BITMAP), &bmp);

	//Create Texture
	glGenTextures(1, texture);

	//Bind To CREATED TEXTURE OBJECT
	glBindTexture(GL_TEXTURE_2D, *texture);

	//Unpack Image Data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//Set Texture Parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Create Mipmap Images For Resolution
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, (void*)bmp.bmBits);

	//glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, (void*)bmp.bmBits);

	//Unbind Texture from Bind Texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//Delete Image Resource
	DeleteObject(hBitmap);
	hBitmap = NULL;

	return(TRUE);
}



void resize(int width, int height)
{
	//*** Code ***
	if (height <= 0)
	{
		height = 1;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fovy, (GLfloat)width / (GLfloat)height, 0.1f, 5000.0f);


	if (viewPortLocationID == 1)
	{
		glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	}
	else if (viewPortLocationID == 2)
	{
		glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	}
}



void display(void)
{
	//Function Declaration
	void drawBigBee(void);
	void drawSmallBee(void);
	void stickman1(void);
	void stickman2(void);
	void cube(void);
	void drawBird(void);
	void drawHoneyComb(void);
	void drawLeaf(void);
	void drawLeafs(void);
	void drawLeafBunch(void);
	void drawCutedTree(void);
	void drawTree(void);
	void drawTerrainScene1(void);
	void drawTerrainMountain(void);
	void updateScene(void);
	void fading_in_out(BOOL);
	void initializeWater(void);
	void drawWater(void);
	void resize(int, int);
	void initializeLastSceneMountain(void);

	//*** Code ***
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewPortLocationID = 2;
	resize(canvas.x, canvas.y);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	/*xxx = camera.position.x;
	yyy = camera.position.y;
	zzz = camera.position.z;*/

	

	gluLookAt(camera.position.x, camera.position.y, camera.position.z, camera.front.x, camera.front.y, camera.front.z, camera.up.x, camera.up.y, camera.up.z);

	//gluLookAt(camera.position.x, camera.position.y, camera.position.z, camera.front.x + camera.position.x, camera.front.y + camera.position.y, camera.front.z + camera.position.z, camera.up.x, camera.up.y, camera.up.z);



	glPushMatrix();///First Push


	if (mainScene == 1)
	{
		///SKY
		{
			glPushMatrix();
			glTranslatef(0.0f, -2.0f, 0.0f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			glRotatef(cloudRot, 0.0f, 0.0f, 1.0f);
			glColor3f(1.0f, 1.0f, 1.0f);
			glBindTexture(GL_TEXTURE_2D, texture_initialSky);
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glScalef(6, 6, 0);
			glMatrixMode(GL_MODELVIEW);
			gluQuadricOrientation(quadric, GLU_INSIDE);
			gluSphere(quadric, 1000, 20, 20);
			//glCallList(treeList);
			gluQuadricOrientation(quadric, GLU_OUTSIDE);
			glBindTexture(GL_TEXTURE_2D, 0);
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}

		///TERRAIN
		{
			glPushMatrix();

			glTranslatef(0.0f, 0.0f, -3.0f);
			glScalef(45.0f, 25.0f, 40.0f);
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			glCallList(terrainList);
			glColor3f(1.0f, 1.0f, 1.0f);

			glBindTexture(GL_TEXTURE_2D, texture_sceneOneTerrain);
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glScalef(1000, 1000, 0);
			glMatrixMode(GL_MODELVIEW);

			glTranslatef(0.0f, 0.0f, 0.02f);
			gluDisk(quadric, 0.0f, 1000.0f, 30, 30);
			glBindTexture(GL_TEXTURE_2D, 0);
			glColor3f(1.0f, 1.0f, 1.0f);

			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}

		///Stones
		glCallList(rockList);

		/// SCENE 1 ///
		glPushMatrix();
		if (scene == 1)
		{
			///Cuted Plants
			glCallList(cutedPlantList);

			bPlantFall = TRUE;
			if (plantFallZ <= 0.0f && time > 6.0f)
				plantFallZ += 0.075f;

			///Falling Plant
			{
				glPushMatrix();
				glTranslatef(-211, 3, 136);
				glScalef(0.5f, 0.5f, 0.5f);
				glRotatef(230, 0.0f, 1.0f, 0.0f);
				drawCutedTree();
				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-296, 3, 48);
				glScalef(0.5f, 0.5f, 0.5f);
				glRotatef(-267, 0.0f, 1.0f, 0.0f);
				drawCutedTree();
				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-424, 3, 85);
				glScalef(0.5f, 0.5f, 0.5f);
				glRotatef(515, 0.0f, 1.0f, 0.0f);
				drawCutedTree();
				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-345, 2, 130);
				glScalef(0.5f, 0.5f, 0.5f);
				glRotatef(619, 0.0f, 1.0f, 0.0f);
				drawCutedTree();
				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-233, 0, 198);
				glScalef(0.5f, 0.5f, 0.5f);
				glRotatef(690, 0.0f, 1.0f, 0.0f);
				drawCutedTree();
				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-270, -1, -50);
				glScalef(0.5f, 0.5f, 0.5f);
				glRotatef(30, 0.0f, 1.0f, 0.0f);
				drawCutedTree();
				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-120, 2, 320);
				glScalef(0.5f, 0.5f, 0.5f);
				glRotatef(45, 0.0f, 1.0f, 0.0f);
				drawCutedTree();
				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();
			}
			

			///Mountains
			glCallList(mountainList);

			///HONEY COMB 1
			{
				glEnable(GL_LIGHT1);

				glPointSize(7);

				glPushMatrix();
				glTranslatef(-73, 2.5, 74);
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glRotatef(-117, 1.0f, 0.0f, 0.0f);
				glColor3f(0.2f, 0.2f, 0.2f);
				glCallList(honeyCombList);
				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(0.0f, 2.5f, 0.0f);
				glRotatef(time * 1000.0f, 1.0f, 0.0f, 1.0f);
				glTranslatef(0.0f, 2.5f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(2.5f, 0.0f, 0.0f);
				glRotatef(-time * 1000.0f, 0.0f, 1.0f, 1.0f);
				glTranslatef(2.5f, 0.0f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(0.0f, 0.0f, 2.5f);
				glRotatef(time * 1000.0f, 1.0f, 1.0f, 0.0f);
				glTranslatef(0.0f, 0.0f, 2.5f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(2.5f, 2.5f, 0.0f);
				glRotatef(-time * 1000.0f, 1.0f, 0.0f, 0.0f);
				glTranslatef(2.5f, 2.5f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();



				glTranslatef(0.0f, -3.5f, 0.0f);
				glRotatef(time * 1000.0f, 1.0f, 0.0f, 1.0f);
				glTranslatef(0.0f, -3.5f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(-3.5f, 0.0f, 0.0f);
				glRotatef(-time * 1000.0f, 0.0f, 1.0f, 1.0f);
				glTranslatef(-3.5f, 0.0f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(0.0f, 0.0f, -3.5f);
				glRotatef(time * 1000.0f, 1.0f, 1.0f, 0.0f);
				glTranslatef(0.0f, 0.0f, -3.5f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(-3.5f, -3.5f, 0.0f);
				glRotatef(-time * 1000.0f, 1.0f, 0.0f, 0.0f);
				glTranslatef(-3.5f, -3.5f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(5.5f, -0.0f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(0.0f, -0.0f, -5.5f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glDisable(GL_LIGHT1);
			}

			///HONEY COMB 2
			{
				glEnable(GL_LIGHT1);

				glPushMatrix();
				glTranslatef(-73 - 53.6, 3 - 3.1, 74 - 24.9);
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glRotatef(-117, 1.0f, 0.0f, 0.0f);
				glColor3f(0.2f, 0.2f, 0.2f);
				glCallList(honeyCombList);
				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(0.0f, 2.5f, 0.0f);
				glRotatef(time * 1000.0f, 1.0f, 0.0f, 1.0f);
				glTranslatef(0.0f, 2.5f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(2.5f, 0.0f, 0.0f);
				glRotatef(-time * 1000.0f, 0.0f, 1.0f, 1.0f);
				glTranslatef(2.5f, 0.0f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(0.0f, 0.0f, 2.5f);
				glRotatef(time * 1000.0f, 1.0f, 1.0f, 0.0f);
				glTranslatef(0.0f, 0.0f, 2.5f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(2.5f, 2.5f, 0.0f);
				glRotatef(-time * 1000.0f, 1.0f, 0.0f, 0.0f);
				glTranslatef(2.5f, 2.5f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();



				glTranslatef(0.0f, -3.5f, 0.0f);
				glRotatef(time * 1000.0f, 1.0f, 0.0f, 1.0f);
				glTranslatef(0.0f, -3.5f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(-3.5f, 0.0f, 0.0f);
				glRotatef(-time * 1000.0f, 0.0f, 1.0f, 1.0f);
				glTranslatef(-3.5f, 0.0f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(0.0f, 0.0f, -3.5f);
				glRotatef(time * 1000.0f, 1.0f, 1.0f, 0.0f);
				glTranslatef(0.0f, 0.0f, -3.5f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(-3.5f, -3.5f, 0.0f);
				glRotatef(-time * 1000.0f, 1.0f, 0.0f, 0.0f);
				glTranslatef(-3.5f, -3.5f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(5.5f, -0.0f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(0.0f, -0.0f, -5.5f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glDisable(GL_LIGHT1);
			}

			///HONEY COMB 3
			{
				glEnable(GL_LIGHT1);

				glPushMatrix();
				glTranslatef(-73 - 173, 3 - 2.1, 74 + 26.7);
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glRotatef(-117, 1.0f, 0.0f, 0.0f);
				glColor3f(0.2f, 0.2f, 0.2f);
				glCallList(honeyCombList);
				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(0.0f, 2.5f, 0.0f);
				glRotatef(time * 1000.0f, 1.0f, 0.0f, 1.0f);
				glTranslatef(0.0f, 2.5f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(2.5f, 0.0f, 0.0f);
				glRotatef(-time * 1000.0f, 0.0f, 1.0f, 1.0f);
				glTranslatef(2.5f, 0.0f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(0.0f, 0.0f, 2.5f);
				glRotatef(time * 1000.0f, 1.0f, 1.0f, 0.0f);
				glTranslatef(0.0f, 0.0f, 2.5f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(2.5f, 2.5f, 0.0f);
				glRotatef(-time * 1000.0f, 1.0f, 0.0f, 0.0f);
				glTranslatef(2.5f, 2.5f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();



				glTranslatef(0.0f, -3.5f, 0.0f);
				glRotatef(time * 1000.0f, 1.0f, 0.0f, 1.0f);
				glTranslatef(0.0f, -3.5f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(-3.5f, 0.0f, 0.0f);
				glRotatef(-time * 1000.0f, 0.0f, 1.0f, 1.0f);
				glTranslatef(-3.5f, 0.0f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(0.0f, 0.0f, -3.5f);
				glRotatef(time * 1000.0f, 1.0f, 1.0f, 0.0f);
				glTranslatef(0.0f, 0.0f, -3.5f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(-3.5f, -3.5f, 0.0f);
				glRotatef(-time * 1000.0f, 1.0f, 0.0f, 0.0f);
				glTranslatef(-3.5f, -3.5f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(5.5f, -0.5f, 0.0f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glTranslatef(0.0f, -0.5f, -5.5f);
				glColor3f(0.25f, 0.25f, 0.25f);
				
				glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
				glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				glDisable(GL_LIGHT1);

				glPointSize(1);
			}

			///STICK MANS POS 1
			{
				glPushMatrix();
				glTranslatef(-124 - (3.0 * time), 5.5, 12);
				glScalef(1.5f, 1.0f, 1.5f);
				glRotatef(-83, 0.0f, 1.0f, 0.0f);
				glColor3f(1.0f, 1.0f, 1.0f);
				stickman1();
				glTranslatef(10.0f, 2.0f, 00.0f);
				stickman2();
				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();
			}

			///STICK MANS POS 2
			{
				glPushMatrix();
				glTranslatef(-252 - (3.0 * time), 6, 66);
				glScalef(1.5f, 1.0f, 1.5f);
				glRotatef(-75, 0.0f, 1.0f, 0.0f);
				glColor3f(1.0f, 1.0f, 1.0f);
				stickman1();
				glTranslatef(10.0f, 2.0f, 00.0f);
				stickman2();
				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();
			}

			///STICK MANS POS 2
			{
				glPushMatrix();
				glTranslatef(-70 - (3.0 * time), 6, 150);
				glScalef(1.5f, 1.0f, 1.5f);
				glRotatef(-75, 0.0f, 1.0f, 0.0f);
				glColor3f(1.0f, 1.0f, 1.0f);
				stickman1();
				glTranslatef(10.0f, 2.0f, 00.0f);
				stickman2();
				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();
			}

			///TREE
			{
				glPushMatrix();
				glTranslatef(-294, 2, 160);
				glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
				glScalef(0.5f, 0.5f, 0.5f);
				//drawTree();
				glCallList(treeList);
				colorChange = FALSE;
				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();

				/*glPushMatrix();
				glTranslatef(-300, 2, -60);
				colorChange = TRUE;
				glScalef(0.5f, 0.3f, 0.5f);
				drawTree();
				colorChange = FALSE;
				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();*/
			}

			///BIG BEE
			{
				glPushMatrix();
				glEnable(GL_LIGHT1);
				Transformation(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				glTranslatef(splineData2[splCount2].x + bigBee.xTrans, splineData2[splCount2].y + bigBee.yTrans, splineData2[splCount2].z + bigBee.zTrans);

				glRotatef(bigBee.xRotate, 1.0f, 0.0f, 0.0f);
				glRotatef(bigBee.yRotate, 0.0f, 1.0f, 0.0f);
				glRotatef(bigBee.zRotate, 0.0f, 0.0f, 1.0f);

				glScalef(0.15f, 0.15f, 0.15f);

				drawBigBee();
				glDisable(GL_LIGHT1);
				glPopMatrix();
			}

			///SMALL BEE
			{
				glPushMatrix();
				glEnable(GL_LIGHT1);
				Transformation(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				glTranslatef(-19.9 + smallBee.xTrans, 9.6 + smallBee.yTrans, 50.5 + smallBee.zTrans);

				glRotatef(smallBee.xRotate, 1.0f, 0.0f, 0.0f);
				glRotatef(smallBee.yRotate, 0.0f, 1.0f, 0.0f);
				glRotatef(smallBee.zRotate, 0.0f, 0.0f, 1.0f);

				glScalef(0.05f, 0.05f, 0.05f);

				drawBigBee();
				glDisable(GL_LIGHT1);
				glPopMatrix();
			}

			glPopMatrix();
		}
		glPopMatrix();


		/// SCENE 2 ///
		glPushMatrix();
		if (scene == 2)
		{
			glPushMatrix();
			if (bScene2)
			{
				updateScene();
			}
			glPopMatrix();

			///Big Trees
			glCallList(bigTreeList);
			

			///BIG BEE
			{
				glPushMatrix();
				glEnable(GL_LIGHT1);
				bigBee.bBeeFly = TRUE;
				Transformation(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				glTranslatef(0.0, 2.0f, 0.0f);
				glTranslatef(splineData3[splCount3].x + bigBee.xTrans, splineData3[splCount3].y + bigBee.yTrans + 0.15 * sin(time * 1.21), splineData3[splCount3].z + bigBee.zTrans);

				glRotatef(bigBee.xRotate, 1.0f, 0.0f, 0.0f);
				glRotatef(bigBee.yRotate, 0.0f, 1.0f, 0.0f);
				glRotatef(bigBee.zRotate, 0.0f, 0.0f, 1.0f);

				glScalef(0.15f, 0.15f, 0.15f);

				drawBigBee();
				glDisable(GL_LIGHT1);
				glPopMatrix();
			}

			///SMALL BEE
			{
				glPushMatrix();
				glEnable(GL_LIGHT1);
				bigBee.bBeeFly = FALSE;
				Transformation(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				glTranslatef(0.0, 2.0f, 0.0f);
				glTranslatef(splineData3[splCount3].x + smallBee.xTrans, splineData3[splCount3].y + smallBee.yTrans - 0.13 + 0.15 * sin(time * 1.21), splineData3[splCount3].z + smallBee.zTrans - 0.96);

				glRotatef(smallBee.xRotate, 1.0f, 0.0f, 0.0f);
				glRotatef(smallBee.yRotate, 0.0f, 1.0f, 0.0f);
				glRotatef(smallBee.zRotate, 0.0f, 0.0f, 1.0f);

				glScalef(0.05f, 0.05f, 0.05f);

				drawBigBee();
				glDisable(GL_LIGHT1);
				glPopMatrix();
			}
		}
		glPopMatrix();

		/// SCENE 3 ///
		glPushMatrix();
		if (scene == 3)
		{
			glPushMatrix();
			//glTranslatef(xxx, yyy, zzz);
			//gluSphere(quadric, 0.5f, 10, 10);
			glPopMatrix();

			glPushMatrix();
			if (bScene3)
			{
				updateScene();
			}
			glPopMatrix();

			///Big Trees
			glCallList(bigTreeList);

			///BIG BEE
			{
				glPushMatrix();
				glEnable(GL_LIGHT1);
				Transformation(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				glTranslatef(-24.0f + bigBee.xTrans + splineData7[splCount7].x, 2.8f + bigBee.yTrans + splineData7[splCount7].y, -223.0f + bigBee.zTrans + splineData7[splCount7].z);

				glRotatef(bigBee.xRotate, 1.0f, 0.0f, 0.0f);
				glRotatef(bigBee.yRotate, 0.0f, 1.0f, 0.0f);
				glRotatef(bigBee.zRotate, 0.0f, 0.0f, 1.0f);

				glScalef(0.15f, 0.15f, 0.15f);

				drawBigBee();
				glDisable(GL_LIGHT1);
				glPopMatrix();
			}

			///SMALL BEE
			{
				glPushMatrix();
				glEnable(GL_LIGHT1);
				Transformation(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				glTranslatef(-20.5f + smallBee.xTrans + splineData7[splCount7].x, 2.8f + smallBee.yTrans + splineData7[splCount7].y, -223.0f + smallBee.zTrans + splineData7[splCount7].z);

				glRotatef(smallBee.xRotate, 1.0f, 0.0f, 0.0f);
				glRotatef(smallBee.yRotate, 0.0f, 1.0f, 0.0f);
				glRotatef(smallBee.zRotate, 0.0f, 0.0f, 1.0f);

				glScalef(0.05f, 0.05f, 0.05f);

				drawSmallBee();
				glDisable(GL_LIGHT1);
				glPopMatrix();
			}

			///Bird
			{
				glPushMatrix();
				glTranslatef(splineData6[splCount6 + 1].x, splineData6[splCount6 + 1].y + sin(sinAngle), splineData6[splCount6 + 1].z);
				glScalef(0.85f, 0.85f, 0.85f);
				glRotatef(birdXRotate, 1.0f, 0.0f, 0.0f);
				glRotatef(birdYRotate, 0.0f, 1.0f, 0.0f);
				glRotatef(birdZRotate, 0.0f, 0.0f, 1.0f);
				drawBird();
				glColor3f(1.0f, 1.0f, 1.0f);
				glPopMatrix();
			}

		}
		glPopMatrix();
	}


	glPopMatrix();///First Pop


	if (mainScene == 2)
	{
		//GLfloat lightSpecular2[] = { 0.0f + (xxx * 1000),0.0f + (xxx * 1000),0.0f + (xxx * 1000),1.0f };
		//GLfloat lightAmbient2[] = { 0.5f + (yyy * 1000),0.5f + (yyy * 1000),0.5f + (yyy * 1000),1.0f };//Grey Light
		//GLfloat lightDiffuse2[] = { 2.0f + (zzz * 1000),2.0f + (zzz * 1000),2.0f + (zzz * 1000),1.0f };//White Light
		//GLfloat lightPosition2[] = { 0.0f, 0.0f, 1411.0f,1.0f };

		//glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbient2);
		//glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse2);
		//glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpecular2);
		//glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);

		//glDisable(GL_LIGHT0);
		//glEnable(GL_LIGHT2);
		///FOG SETUP FOR LAST SCENE
		if (1)
		{
			glFogf(GL_FOG_DENSITY, 0.001 + (-0.065 * 0.01f));//setting the fog density
			glFogf(GL_FOG_START, 100.0);
			glFogf(GL_FOG_END, 1500);
			glHint(GL_FOG_HINT, GL_NICEST);
			glEnable(GL_FOG);
		}
		updateScene();
		//glDisable(GL_FOG);

		///SKY
		{
			glPushMatrix();
			glPushMatrix();
			glPushMatrix();

			glTranslatef(0.0f, -2.0f, 0.0f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			glRotatef(-cloudRot * 2, 0.0f, 0.0f, 1.0f);
			glColor3f(1.0f, 1.0f, 1.0f);
			glBindTexture(GL_TEXTURE_2D, texture_seaSky);
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glScalef(6, 6, 0);
			glMatrixMode(GL_MODELVIEW);
			gluQuadricOrientation(quadric, GLU_INSIDE);
			gluSphere(quadric, 4000, 16, 16);
			gluQuadricOrientation(quadric, GLU_OUTSIDE);
			glBindTexture(GL_TEXTURE_2D, 0);
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();

			
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glTranslatef(0.0f, 0.0f, 1411.0f);
			glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
			/*gluDisk(quadric, 0.0f, 50.0f + (0.1 * 100), 50, 50);

			glTranslatef(0.0f, 00.0f, 1.0f);
			glColor4f(0.0f, 0.0f, 1.0f, 0.75f);
			gluDisk(quadric, 0.0f, 50.0f + (0.80 * 100), 50, 50);

			glTranslatef(0.0f, 00.0f, 1.0f);
			glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
			gluDisk(quadric, 0.0f, 50.0f + (1.8 * 100), 50, 50);

			glTranslatef(0.0f, 00.0f, 1.0f);
			glColor4f(1.0f, 1.0f, 0.0f, 0.25f);*/
			//gluDisk(quadric, 0.0f, 50.0f + (3.0 * 100), 50, 50);
			glPopMatrix();
			
			float theataA = 0.0f;
			glTranslatef((3.09 - (0.30 + 0.08)) * 1000 - 19.75f, 0.10 * 1000 + 40.0f, 0 * 1000 + 51.88f + ((1.50 - 1.0f) * 100));
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			//glNormal3f(-1.0f, 0.0f, 0.0f);
			glBegin(GL_TRIANGLE_FAN);
			//glColor4f(1., 0.482, 0.122, 1.0f);
			glColor4f(1., 0.5, 0.15, 1.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			for (int i = 0; i <= 3600; i++)
			{
				theataA = (i / 10.0f) * (M_PI / 180);
				glColor4f(4.0, 2.0f, 0.5, 0.5f);
				glVertex3f((2000.0f + 0.75f * 1000) * cos(theataA), (2000.0f + 0.32f * 1000) * sin(theataA), 0.0f);
			}
			glEnd();

			glPopMatrix();
		}

		///TERRAIN
		{
			glPushMatrix();
			glPushMatrix();

			glTranslatef(0.0f, 0.0f, 0.0f);
			glScalef(100.0f, 50.0f, 100.0f);
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			initializeWater();

			glTranslatef(0.0f, 0.0f, 0.175f);
			glPushMatrix();

			glColor4f(1.0f, 1.0f, 1.0f, alphaSeaTexture);
			glBindTexture(GL_TEXTURE_2D, texture_seaWaves);
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glScalef(250.0f, 250.0f, 250.0f);
			glMatrixMode(GL_MODELVIEW);
			gluDisk(quadric, 0.0f, 1500, 20, 20);
			glBindTexture(GL_TEXTURE_2D, 0);
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);

			glPopMatrix();

			glTranslatef(0.0f, 0.0f, -0.003f);
			glColor4f(1.0f, 1.0f, 1.0f, alphaSeaLavaTexture);
			glBindTexture(GL_TEXTURE_2D, texture_seaLavaWaves);
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glScalef(250.0f, 250.0f, 250.0f);
			glMatrixMode(GL_MODELVIEW);
			gluDisk(quadric, 0.0f, 1500, 20, 20);
			glBindTexture(GL_TEXTURE_2D, 0);
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			
			glPopMatrix();

			glPushMatrix();
			///Background Scene 1
			{
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				//glScalef(1.0f + (xxx * 10), 1.0f + (xxx * 10), 1.0f + (xxx * 10));
				//glTranslatef(xxx * 1000, yyy * 1000, zzz * 1000);
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glTranslatef(1200 + (0.19 * 1000) + 1.50 * 1000, -60, 0.0f );
				glScalef(100 + (-0.07 * 1000), 5000 + (-3.40 * 1000), 1200 + (-0.99 * 1000));
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				initializeLastSceneMountain();
				glColor3f(1.0f, 1.0f, 1.0f);

				glPopMatrix();
				glRotatef(90.0f + (-0.13 * 1000), 0.0f, 1.0f, 0.0f);
				glTranslatef(1200 + (0.2 * 1000) + 1.50 * 1000, -60, 0.0f + (0.11 * 1000));
				glScalef(100 + (-0.07 * 1000), 5000 + (-3.40 * 1000), 1200 + (-0.99 * 1000));
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				initializeLastSceneMountain();
				glColor3f(1.0f, 1.0f, 1.0f);

				glPopMatrix();
				glRotatef(90.0f + (-0.05 * 1000), 0.0f, 1.0f, 0.0f);
				glTranslatef(1200 + (0.10 * 1000) + 1.50 * 1000, -60, 0.0f + (0.01 * 1000));
				glScalef(100 + (-0.07 * 1000), 5000 + (-3.40 * 1000), 1200 + (-0.99 * 1000));
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				initializeLastSceneMountain();
				glColor3f(1.0f, 1.0f, 1.0f);

				glPopMatrix();
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
				glTranslatef(1200 + (0.08 * 1000) + 1.50 * 1000, -60 , 0.0f + (0.08 * 1000));
				glScalef(100 + (-0.07 * 1000), 5000 + (-3.40 * 1000), 1200 + (-0.99 * 1000));
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				initializeLastSceneMountain();
				glColor3f(1.0f, 1.0f, 1.0f);

				glPopMatrix();
				glRotatef(90.0f + (-0.18 * 1000), 0.0f, 1.0f, 0.0f);
				glTranslatef(1200 + (0.13 * 1000) + 1.50 * 1000, -60, 0.0f);
				glScalef(100 + (-0.07 * 1000), 5000 + (-3.40 * 1000), 1200 + (-0.99 * 1000));
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				initializeLastSceneMountain();
				glColor3f(1.0f, 1.0f, 1.0f);

				glPopMatrix();
				glRotatef(90.0f + (-0.23 * 1000), 0.0f, 1.0f, 0.0f);
				glTranslatef(1200 + (0.14 * 1000) + 1.50 * 1000, -60, 0.0f + (-0.08 * 1000));
				glScalef(100 + (-0.07 * 1000), 5000 + (-3.40 * 1000), 1200 + (-0.99 * 1000));
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				initializeLastSceneMountain();
				glColor3f(1.0f, 1.0f, 1.0f);

				glPopMatrix();
				glRotatef(90.0f + (-0.28 * 1000), 0.0f, 1.0f, 0.0f);
				glTranslatef(1200 + (0.02 * 1000) + 1.50 * 1000, -60, 0.0f + (-0.06 * 1000));
				glScalef(100 + (-0.07 * 1000), 5000 + (-3.40 * 1000), 1200 + (-0.99 * 1000));
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				initializeLastSceneMountain();
				glColor3f(1.0f, 1.0f, 1.0f);

				glPopMatrix();
				glRotatef(90.0f + (-0.31 * 1000), 0.0f, 1.0f, 0.0f);
				glTranslatef(1200 + (0.16 * 1000) + 1.50 * 1000, -60, 0.0f);
				glScalef(100 + (-0.07 * 1000), 5000 + (-3.40 * 1000), 1200 + (-0.99 * 1000));
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				initializeLastSceneMountain();
				glColor3f(1.0f, 1.0f, 1.0f);
			}

			glPopMatrix();

			///Background Scene 2
			{
				//glTranslatef(xxx * 1000, yyy * 1000, zzz * 1000);
				//glScalef(1.0f + (xxx * 10), 1.0f + (xxx * 10), 1.0f + (xxx * 10));
				//glRotatef(xxx * 1000, 0.0f, 1.0f, 0.0f);
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glPushMatrix();
				glRotatef((-0.25f * 100), 0.0f, 1.0f, 0.0f);
				glTranslatef(1200 + (0.19 * 1000) + 1.50 * 1000, -60, 0.0f);
				glScalef(100 + (-0.07 * 1000), 5000 + (-3.40 * 1000), 1200 + (-0.99 * 1000));
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				initializeLastSceneMountain();
				glColor3f(1.0f, 1.0f, 1.0f);

				glPopMatrix();
				glRotatef(90.0f + (-0.13 * 1000) + (-0.25f * 100), 0.0f, 1.0f, 0.0f);
				glTranslatef(1200 + (0.2 * 1000) + 1.50 * 1000, -60, 0.0f + (0.11 * 1000));
				glScalef(100 + (-0.07 * 1000), 5000 + (-3.40 * 1000), 1200 + (-0.99 * 1000));
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				initializeLastSceneMountain();
				glColor3f(1.0f, 1.0f, 1.0f);

				glPopMatrix();
				glRotatef(90.0f + (-0.05 * 1000) + (-0.25f * 100), 0.0f, 1.0f, 0.0f);
				glTranslatef(1200 + (0.10 * 1000) + 1.50 * 1000, -60, 0.0f + (0.01 * 1000));
				glScalef(100 + (-0.07 * 1000), 5000 + (-3.40 * 1000), 1200 + (-0.99 * 1000));
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				initializeLastSceneMountain();
				glColor3f(1.0f, 1.0f, 1.0f);

				glPopMatrix();
				glRotatef(90.0f + (-0.25f * 100), 0.0f, 1.0f, 0.0f);
				glTranslatef(1200 + (0.08 * 1000) + 1.50 * 1000, -60, 0.0f + (0.08 * 1000));
				glScalef(100 + (-0.07 * 1000), 5000 + (-3.40 * 1000), 1200 + (-0.99 * 1000));
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				initializeLastSceneMountain();
				glColor3f(1.0f, 1.0f, 1.0f);

				glPopMatrix();
				glRotatef(90.0f + (-0.18 * 1000) + (-0.25f * 100), 0.0f, 1.0f, 0.0f);
				glTranslatef(1200 + (0.13 * 1000) + 1.50 * 1000, -60, 0.0f);
				glScalef(100 + (-0.07 * 1000), 5000 + (-3.40 * 1000), 1200 + (-0.99 * 1000));
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				initializeLastSceneMountain();
				glColor3f(1.0f, 1.0f, 1.0f);

				glPopMatrix();
				glRotatef(90.0f + (-0.23 * 1000) + (-0.25f * 100), 0.0f, 1.0f, 0.0f);
				glTranslatef(1200 + (0.14 * 1000) + 1.50 * 1000, -60, 0.0f + (-0.08 * 1000));
				glScalef(100 + (-0.07 * 1000), 5000 + (-3.40 * 1000), 1200 + (-0.99 * 1000));
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				initializeLastSceneMountain();
				glColor3f(1.0f, 1.0f, 1.0f);

				glPopMatrix();
				glRotatef(90.0f + (-0.28 * 1000) + (-0.25f * 100), 0.0f, 1.0f, 0.0f);
				glTranslatef(1200 + (0.02 * 1000) + 1.50 * 1000, -60, 0.0f + (-0.06 * 1000));
				glScalef(100 + (-0.07 * 1000), 5000 + (-3.40 * 1000), 1200 + (-0.99 * 1000));
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				initializeLastSceneMountain();
				glColor3f(1.0f, 1.0f, 1.0f);

				glPopMatrix();
				glRotatef(90.0f + (-0.31 * 1000) + (-0.25f * 100), 0.0f, 1.0f, 0.0f);
				glTranslatef(1200 + (0.16 * 1000) + 1.50 * 1000, -60, 0.0f);
				glScalef(100 + (-0.07 * 1000), 5000 + (-3.40 * 1000), 1200 + (-0.99 * 1000));
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				initializeLastSceneMountain();
				glColor3f(1.0f, 1.0f, 1.0f);
			}



			glPopMatrix();
		}

		///BIG BEE
		{
			glPushMatrix();
			glEnable(GL_LIGHT1);
			Transformation(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			glTranslatef(bigBee.xTrans + splineData9[splCount9].x, bigBee.yTrans + splineData9[splCount9].y + 0.12 * sin(time * 1.21), bigBee.zTrans + splineData9[splCount9].z);

			//fprintf(gpFILE, "BBX:%f,BBY:%f,BBZ:%f\n", bigBee.xTrans + splineData9[splCount9].x, bigBee.yTrans + splineData9[splCount9].y, bigBee.zTrans + splineData9[splCount9].z);
			glRotatef(bigBee.xRotate, 1.0f, 0.0f, 0.0f);
			glRotatef(bigBee.yRotate, 0.0f, 1.0f, 0.0f);
			glRotatef(bigBee.zRotate, 0.0f, 0.0f, 1.0f);

			glScalef(0.15f, 0.15f, 0.15f);

			drawBigBee();
			glDisable(GL_LIGHT1);
			glPopMatrix();
		}

		///SMALL BEE
		{
			glPushMatrix();
			glEnable(GL_LIGHT1);
			Transformation(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			if (splCount9)
			{
				glTranslatef(smallBee.xTrans + splineData9[splCount9].x, smallBee.yTrans + splineData9[splCount9].y + 0.12 * sin(time * 1.21), smallBee.zTrans + splineData9[splCount9].z);
			}



			glRotatef(smallBee.xRotate, 1.0f, 0.0f, 0.0f);
			glRotatef(smallBee.yRotate, 0.0f, 1.0f, 0.0f);
			glRotatef(smallBee.zRotate, 0.0f, 0.0f, 1.0f);

			glScalef(0.05f, 0.05f, 0.05f);

			drawSmallBee();
			glDisable(GL_LIGHT1);
			glPopMatrix();
		}
	}

	///First Scene Fade in And Out
	if(1)
	{
		glPushMatrix();
		if (time >= 42.0f && time < 46.0f)
		{
			fading_in_out(FADE_OUT);
		}
		glPopMatrix();

		glPushMatrix();
		if (time >= 46.0f && time <= 50.0f)
		{
			fading_in_out(FADE_IN);
		}
		glPopMatrix();
	}

	///Second Scene Fade in And Out
	if(1)
	{
		glPushMatrix();
		if (time >= 62.5f && time < 66.54f)
		{
			fading_in_out(FADE_OUT);
		}
		glPopMatrix();

		glPushMatrix();
		if (time >= 66.5f && time <= 68.5f)
		{
			fading_in_out(FADE_IN);
		}
		glPopMatrix();
	}

	/// Scene Fade in And Out
	{
		glPushMatrix();
		if (time >= 104.0f && time < 109.0f)
		{
			fading_in_out(FADE_OUT);
		}
		glPopMatrix();

		glPushMatrix();
		if (time >= 109.0f && time <= 115.0f)
		{
			fading_in_out(FADE_IN);
		}
		glPopMatrix();
	}


	///INTRO AND OUTRO
	{
		viewPortLocationID = 1;
		resize(canvas.x, canvas.y);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if (bOutro)
		{
			if (time >= 165.0f && time <= (165.0f + 3.0f))
			{
				glTranslatef(0.0, 0.0, -10.0f);
				glScalef(1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f));
				//glPushMatrix();
				glColor3f(0.525, 0.655, 0.706);
				writeUsingFont("\"Vaat Disu De\"");
				/*glPopMatrix();
				glTranslatef(0.0f, -2.0, 0.0f);
				glScalef(0.75f, 0.75f, 0.75f);
				glColor3f(0.753, 0.761, 0.8);
				writeUsingFont("ORTHO");*/
			}
			else if (time > (165.0f + 3.0f) && time <= (165.0f + 6.0f))
			{
				glTranslatef(0.0, 1.0, -10.0f);
				glScalef(1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f));
				glPushMatrix();
				glColor3f(0.525, 0.655, 0.706);
				writeUsingFont("BY");
				//writeUsingFont("TECHNOLOGIES USED");
				glPopMatrix();
				glTranslatef(0.0f, -2.0, 0.0f);
				glScalef(0.75f, 0.75f, 0.75f);
				glColor3f(0.5, 0.5, 0.5);
				//writeUsingFont("OpenGL & WIN32SDK");
				writeUsingFont("Tushar Wagdare");
			}
			else if (time > (165.0f + 6.0f) && time <= (165.0f + 9.0f))
			{
				glTranslatef(0.0, 1.0, -10.0f);
				glScalef(1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f));
				glPushMatrix();
				glColor3f(0.525, 0.655, 0.706);
				writeUsingFont("TECHNOLOGIES USED");
				glPopMatrix();
				glTranslatef(0.0, -2.0, 0.0f);
				glScalef(0.75f, 0.75f, 0.75f);
				glColor3f(0.5, 0.5, 0.5);
				writeUsingFont("OpenGL FFP & WIN32SDK");
			}
			else if (time > (165.0f + 9.0f) && time <= (165.0f + 12.0f))
			{
				glTranslatef(0.0, 1.0, -10.0f);
				glScalef(1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f));
				glPushMatrix();
				glColor3f(0.525, 0.655, 0.706);
				writeUsingFont("DATA STRUCTURE USED");
				glPopMatrix();
				glTranslatef(0.0, -2.0, 0.0f);
				glScalef(0.75f, 0.75f, 0.75f);
				glColor3f(0.5, 0.5, 0.5);
				writeUsingFont("LINKED LIST");
			}
			else if (time > (165.0f + 12.0f) && time <= (165.0f + 15.0f))
			{
				glTranslatef(0.0, 1.0, -10.0f);
				glScalef(1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f));
				glPushMatrix();
				glColor3f(0.525, 0.655, 0.706);
				writeUsingFont("REFERENCES");
				glPopMatrix();
				glTranslatef(0.0f, -2.0, 0.0f);
				glScalef(0.75f, 0.75f, 0.75f);
				glPushMatrix();
				glColor3f(0.5, 0.5, 0.5);
				writeUsingFont("RTR 5.0 ASSIGNMENTS");
				glPopMatrix();
				glTranslatef(0.0f, -1.0, 0.0f);
				writeUsingFont("nehe.gamedev.net");
			}
			else if (time > (165.0f + 15.0f) && time <= (165.0f + 18.0f))
			{
				glTranslatef(0.0, 1.0, -10.0f);
				glScalef(1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f));
				glPushMatrix();
				glColor3f(0.525, 0.655, 0.706);
				writeUsingFont("GROUP LEADER");
				glPopMatrix();
				glTranslatef(0.0, -2.0, 0.0f);
				glScalef(0.75f, 0.75f, 0.75f);
				glColor3f(0.5, 0.5, 0.5);
				writeUsingFont("Parikshit Joshi");
			}
			else if (time > (165.0f + 18.0f) && time <= (165.0f + 21.0f))
			{
				glTranslatef(0.0, 1.0, -10.0f);
				glScalef(1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f));
				glPushMatrix();
				glColor3f(0.525, 0.655, 0.706);
				writeUsingFont("SPECIAL THANKS");
				glPopMatrix();
				glTranslatef(0.0, -1.75, 0.0f);
				glScalef(0.75f, 0.75f, 0.75f);
				glPushMatrix();

				glColor3f(0.5, 0.5, 0.5);
				writeUsingFont("Yogesh Jadhav");
				glPopMatrix();

				glTranslatef(0.0, -1.5, 0.0f);
				writeUsingFont("Smita Patil");
			}
			else if (time > (165.0f + 21.0f) && time <= (165.0f + 24.0f))
			{
				glTranslatef(0.0, 1.0, -10.0f);
				glScalef(1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f));
				glPushMatrix();
				glColor3f(0.525, 0.655, 0.706);
				writeUsingFont("INVALUABLE GUIDANCE");
				glPopMatrix();
				glTranslatef(0.0, -1.75, 0.0f);
				glScalef(0.75f, 0.75f, 0.75f);
				glPushMatrix();

				glColor3f(0.5, 0.5, 0.5);
				writeUsingFont("Dr. Rama Gokhale Ma'am");
				glPopMatrix();

				glTranslatef(0.0, -1.0, 0.0f);
				writeUsingFont("Pradnya Gokhale Ma'am");
				}
			else if (time > (165.0f + 24.0f) && time <= (165.0f + 27.0f))
			{
				glTranslatef(0.0, 1.0, -10.0f);
				glScalef(1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f));
				glPushMatrix();
				glColor3f(0.525, 0.655, 0.706);
				writeUsingFont("INDEBTED TO");
				glPopMatrix();
				glTranslatef(0.0, -2.0, 0.0f);
				glScalef(0.75f, 0.75f, 0.75f);
				glColor3f(0.5, 0.5, 0.5);
				writeUsingFont("Dr. Vijay Gokhale Sir");
			}
			else if (time > (165.0f + 27.0f) && time <= (165.0f + 30.0f))
			{
				glTranslatef(0.0, 0.0, -10.0f);
				glScalef(1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f), 1.5f * sin(pAngle * 0.025f));
				glColor3f(0.525, 0.655, 0.706);
				writeUsingFont("THANK YOU");
			}
		}

		///DRAW TEXT
		if (1)
		{
			//glScalef(fabs(sin(cAngle * 0.01f)), fabs(sin(cAngle * 0.01f)), fabs(sin(cAngle * 0.01f)));
			//glRotatef(cAngle - 90.0f, 1.0f, 0.0f, 0.0f); 
			//glRotatef(-cAngle + 90.0f, 0.0f, 0.0f, 1.0f);
			//glRotatef(cAngle, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.0f, 0.0f, -10.0);
			if (time <= 3.0f)
			{
				glTranslatef(0.0f, 1.5f, 0.0f);
				if (scaleAstromedicomp <= 1.0f)
				{
					glScalef(scaleAstromedicomp, scaleAstromedicomp, scaleAstromedicomp);
					scaleAstromedicomp = 0.3333 * time;
				}
				glColor4f(0.984, 0.909, 0.706, 1);
				//glColor4f(0.984, 0.909, 0.706, time / 3.0f);
				//glColor4f(0.329, 0.035, 0.165, time / 3.0f);
				writeUsingFont("ASTROMEDICOMP'S");
			}
			else if (time > 3.0f && time <= 6.0f)
			{
				glTranslatef(0.0f, 1.5f, 0.0f);
				glColor4f(0.984, 0.909, 0.706, 1.0f - ((time - 3) / 3.0f));
				writeUsingFont("ASTROMEDICOMP'S");
			}
			else if (time > 6.0f && time <= 9.0f)
			{
				glTranslatef(0.0f, 2.0f, 0.0);
				if (scaleOrtho <= 1.0f)
				{
					glScalef(scaleOrtho, scaleOrtho, scaleOrtho);
					scaleOrtho = 0.3333 * (time - 6.0f);
				}
				glPushMatrix();
				glColor4f(0.984, 0.909, 0.706, 1);
				writeUsingFont("ORTHO GROUP");
				glPopMatrix();
				glTranslatef(0.0f, -4.0f, 0.0);
				glScalef(0.75f, 0.75f, 0.75f);
				writeUsingFont("PRESENTS");
			}
			else if (time > 9.0f && time <= 12.0f)
			{
				glTranslatef(0.0f, 2.0f, 0.0);
				glPushMatrix();
				glColor4f(0.984, 0.909, 0.706, 1 - ((time - 9) / 3.0f));
				writeUsingFont("ORTHO GROUP");
				glPopMatrix();
				glTranslatef(0.0f, -4.0f, 0.0);
				glScalef(0.75f, 0.75f, 0.75f);
				writeUsingFont("PRESENTS");
			}
		}
	}


	SwapBuffers(ghdc);
}



void update(void)
{
	//Function Declaration
	void stickman1Update(void);
	void updateBird(void);
	void initializeLight(void);
	float interpolate(float, float, float);
	void LightIntensity(float);
	void updateScene(void);
	void resize(int, int);

	///INTRO OUTRO UPDATE
	{
		//Cube Update
		if (cAngle <= 180.0f && bIntro)
		{
			cAngle = cAngle + 0.5f;
		}
		else if (cAngle >= 0.0f)
		{
			bIntro = FALSE;
			cAngle = cAngle - 0.5f;
		}
		else
		{
			bIntro = TRUE;
		}

		///OUTRO
		if(1)
		{
			//Group
			if (pAngle <= 90.0f && time > (165.0f + 0.0f) && time <= (165.0f + 1.5f))
			{
				pAngle += 1.5f;
			}
			else if (pAngle > 0.0f && time > (165.0f + 1.5f) && time <= (165.0f + 3.0f))
			{
				pAngle -= 1.5f;
			}
			//Tech Used
			if (pAngle <= 90.0f && time > (165.0f + 3.0f) && time <= (165.0f + 4.5f))
			{
				pAngle += 1.5f;
			}
			else if (pAngle > 0.0f && time > (165.0f + 4.5f) && time <= (165.0f + 6.0f))
			{
				pAngle -= 1.5f;
			}
			//Data Structor Use
			if (pAngle <= 90.0f && time > (165.0f + 6.0f) && time <= (165.0f + 7.5f))
			{
				pAngle += 1.5f;
			}
			else if (pAngle > 0.0f && time > (165.0f + 7.5f) && time <= (165.0f + 9.0f))
			{
				pAngle -= 1.5f;
			}
			//Tools Used
			if (pAngle <= 90.0f && time > (165.0f + 9.0f) && time <= (165.0f + 10.5f))
			{
				pAngle += 1.5f;
			}
			else if (pAngle > 0.0f && time > (165.0f + 10.5f) && time <= (165.0f + 12.0f))
			{
				pAngle -= 1.5f;
			}
			//Ref
			if (pAngle <= 90.0f && time > (165.0f + 12.0f) && time <= (165.0f + 13.5f))
			{
				pAngle += 1.5f;
			}
			else if (pAngle > 0.0f && time > (165.0f + 13.5f) && time <= (165.0f + 15.0f))
			{
				pAngle -= 1.5f;
			}
			//Thank You
			if (pAngle <= 90.0f && time > (165.0f + 15.0f) && time <= (165.0f + 16.5f))
			{
				pAngle += 1.5f;
			}
			else if (pAngle > 0.0f && time > (165.0f + 16.5f) && time <= (165.0f + 18.0f))
			{
				pAngle -= 1.5f;
			}
			//Tools Used
			if (pAngle <= 90.0f && time > (165.0f + 18.0f) && time <= (165.0f + 19.5f))
			{
				pAngle += 1.5f;
			}
			else if (pAngle > 0.0f && time > (165.0f + 19.5f) && time <= (165.0f + 21.0f))
			{
				pAngle -= 1.5f;
			}
			//Ref
			if (pAngle <= 90.0f && time > (165.0f + 21.0f) && time <= (165.0f + 22.5f))
			{
				pAngle += 1.5f;
			}
			else if (pAngle > 0.0f && time > (165.0f + 22.5f) && time <= (165.0f + 24.0f))
			{
				pAngle -= 1.5f;
			}
			//Thank You
			if (pAngle <= 90.0f && time > (165.0f + 24.0f) && time <= (165.0f + 25.5f))
			{
				pAngle += 1.5f;
			}
			else if (pAngle > 0.0f && time > (165.0f + 25.5f) && time <= (165.0f + 27.0f))
			{
				pAngle -= 1.5f;
			}
			//Thank You
			if (pAngle <= 90.0f && time > (165.0f + 27.0f) && time <= (165.0f + 28.5))
			{
				pAngle += 1.5f;
			}
			else if (pAngle > 0.0f && time > (165.0f + 28.5) && time <= (165.0f + 30.0f))
			{
				pAngle -= 1.5f;
			}
		}
	}



	if (1)
	{
		///DEFAULT ANIMATIONS
		{
			///MAIN
			{
				if (fps)
				{
					cloudRot += 1.00 * deltaTime;
				}
			}

			///Default bigBee Animation
			{
				///ROTATE ANTENA
				if (bigBee.rotAntena <= 10.0f && bigBee.bRotAntena == TRUE)
				{
					bigBee.rotAntena = bigBee.rotAntena + 0.05f;
				}
				else if (bigBee.rotAntena >= 0.0f)
				{
					bigBee.bRotAntena = FALSE;
					bigBee.rotAntena = bigBee.rotAntena - 0.05f;
				}
				else
				{
					bigBee.bRotAntena = TRUE;
				}

				///BEE HEAD
				if (((int)floor(time) % 4) == 0 && bigBee.headMove <= 15.0f && bigBee.bHeadMove)
				{
					bigBee.headMove += 0.1f;
				}
				else if (((int)floor(time) % 4) == 0 && bigBee.headMove >= 0.0f)
				{
					bigBee.bHeadMove = FALSE;
					bigBee.headMove -= 0.1f;
				}
				else
				{
					bigBee.bHeadMove = TRUE;
				}

				///BEE EYE
				if (bigBee.eyeMovement <= 0.3f && bigBee.bEye && ((int)floor(time) % 3) == 0)
				{
					bigBee.eyeMovement += 0.01f;
				}
				else if (bigBee.eyeMovement >= 0.0f && ((int)floor(time) % 6) == 0)
				{
					bigBee.bEye = FALSE;
					bigBee.eyeMovement -= 0.01f;
				}
				else
				{
					bigBee.bEye = TRUE;
				}
			}

			///Default smallBee Animation
			{
				///ROTATE ANTENA
				if (smallBee.rotAntena <= 12.0f && smallBee.bRotAntena == TRUE)
				{
					smallBee.rotAntena = smallBee.rotAntena + 6.0f * deltaTime;
				}
				else if (smallBee.rotAntena >= 0.0f)
				{
					smallBee.bRotAntena = FALSE;
					smallBee.rotAntena = smallBee.rotAntena - 6.0f * deltaTime;
				}
				else
				{
					smallBee.bRotAntena = TRUE;
				}

				///BEE HEAD
				if (((int)floor(time) % 3) == 0 && smallBee.headMove <= 18.0f && smallBee.bHeadMove)
				{
					smallBee.headMove += 18.0f * deltaTime;
				}
				else if (((int)floor(time) % 3) == 0 && smallBee.headMove >= 0.0f)
				{
					smallBee.bHeadMove = FALSE;
					smallBee.headMove -= 18.0f * deltaTime;
				}
				else
				{
					smallBee.bHeadMove = TRUE;
				}

				///BEE EYE
				if (smallBee.eyeMovement <= 0.35f && smallBee.bEye && ((int)floor(time) % 2) == 0)
				{
					smallBee.eyeMovement += 0.02f;

				}
				else if (smallBee.eyeMovement >= 0.0f && ((int)floor(time) % 5) == 0)
				{
					smallBee.bEye = FALSE;
					smallBee.eyeMovement -= 0.02f;
				}
				else
				{
					smallBee.bEye = TRUE;
				}
			}
		}

		if (mainScene == 1)
		{
			//updateBird();

			///SCENE 1
			if (scene == 1)
			{
				stickman1Update();

				///CAMERA
				if (time > value && splCount1 < 1000 && time <= 30.0f)
				{
					if (splCount2 > 960)
					{

						if (bigBee.yRotate <= 340.0f)
							bigBee.yRotate += deltaTime * 500.0f;

						bigBeeUpDowsTrans = time;

						if(splCount2 > 990)
						bigBee.bBeeFly = FALSE;
						
						bigBee.yTrans = 0.12f + (1 - (splCount2 / 1000.0f)) * sin(bigBeeUpDowsTrans);
					}
					else
					{
						bigBeeUpDowsTrans = time;
						bigBee.yTrans = (1 - (splCount2 / 1000.0f)) * sin(bigBeeUpDowsTrans);
					}
					camera.position = splineData1[splCount1];
					camera.front = splineData2[splCount2];
					splCount1 += 1;
					splCount2 += 1;
					value += 0.025f;
				}
				else if (time > 32.0f && time <= 35.0f)
				{
					smallBee.zTrans += 0.2f * deltaTime;
				}
				else
				{
					if (iWeight <= 1.0f && time > 35.0f)
					{
						camera.position.x = lerpf(-17.0, -21.0f, iWeight, TRUE);
						camera.position.y = lerpf(10.0, 9.75, iWeight, TRUE);
						camera.position.z = lerpf(51.0, 50.0, iWeight, TRUE);

						iWeight += deltaTime * 0.08f;
					}
				}


				///BEE
				if (bigBee.wingRotate.x <= -60.0f && bigBee.bBeeWings == TRUE)
				{
					bigBee.wingRotate.x += 1000.0f * deltaTime;
				}
				else if (bigBee.wingRotate.x >= -120.0f)
				{
					bigBee.bBeeWings = FALSE;
					bigBee.wingRotate.x -= 1000.0f * deltaTime;
				}
				else
				{
					bigBee.bBeeWings = TRUE;
				}

				if (time >= 44.0f)
				{
					scene = 2;
				}
			}

			if (scene == 2)
			{
				///BEE
				if (bigBee.wingRotate.x <= -60.0f && bigBee.bBeeWings == TRUE)
				{
					bigBee.wingRotate.x += 1000.0f * deltaTime;
				}
				else if (bigBee.wingRotate.x >= -120.0f)
				{
					bigBee.bBeeWings = FALSE;
					bigBee.wingRotate.x -= 1000.0f * deltaTime;
				}
				else
				{
					bigBee.bBeeWings = TRUE;
				}

				if (splCount3 < 2000 && time > value1)
				{
					/*if (time <= 65.0f && time > 43)
					{
						bBeeWalk = TRUE;
					}
					else
					{
						bBeeWalk = FALSE;
					}*/

					camera.front = splineData3[splCount3];
					camera.position = splineData4[splCount3];
					splCount3 += 1;
					value1 += 0.02f;
				}
				else if (time >= 65.0f)
				{
					scene = 3;
				}
			}

			if (scene == 3)
			{
				bBeeWalk = FALSE;

				if (splCount5 < 2000 && time > value2)
				{
					camera.position = splineData5[splCount5];
					splCount5 += 2;
					value2 += 0.01f;
				}

				if (time >= 75.0f && time <= 88.0f)
				{
					bigBee.bBeeFly = TRUE;
					if (scene3Weight <= 1.0f)
					{
						scene3Weight += 0.005f;
					}
					bigBee.yTrans = lerpf(0.0f, 1.0f, scene3Weight, TRUE) + sin(time) * 0.4f;

					///BEE FLY
					if (bigBee.wingRotate.x <= -60.0f && bigBee.bBeeWings == TRUE)
					{
						bigBee.wingRotate.x += 1000.0f * deltaTime;
					}
					else if (bigBee.wingRotate.x >= -120.0f)
					{
						bigBee.bBeeWings = FALSE;
						bigBee.wingRotate.x -= 1000.0f * deltaTime;
					}
					else
					{
						bigBee.bBeeWings = TRUE;
					}
				}
				/*else if (time >= 79.0f && time <= 83.0f)
				{
					if (scene3Weight >= 0.0f)
					{
						scene3Weight -= 0.005f;

						///BEE FLY
						if (bigBee.wingRotate.x <= -60.0f && bigBee.bBeeWings == TRUE)
						{
							bigBee.wingRotate.x += 1000.0f * deltaTime;
						}
						else if (bigBee.wingRotate.x >= -120.0f)
						{
							bigBee.bBeeWings = FALSE;
							bigBee.wingRotate.x -= 1000.0f * deltaTime;
						}
						else
						{
							bigBee.bBeeWings = TRUE;
						}

						bigBee.yTrans = lerpf(0.0f, 1.0f, scene3Weight, TRUE) + sin(time) * 0.4f;
					}
					else
					{
						bigBee.bBeeFly = FALSE;
					}
				}*/


				if (time >= 83.0f && time <= 88.0f)
				{
					///BEE FLY
					smallBee.bBeeFly = TRUE;
					if (smallBee.wingRotate.x <= -60.0f && smallBee.bBeeWings == TRUE)
					{
						smallBee.wingRotate.x += 100.0f * deltaTime;
						smallBee.yTrans += 0.01f;
						smallBee.zTrans += 0.01f;
						smallBee.xTrans += 0.005f;
					}
					else if (smallBee.wingRotate.x >= -120.0f)
					{
						smallBee.bBeeWings = FALSE;
						smallBee.wingRotate.x -= 100.0f * deltaTime;
						smallBee.yTrans -= 0.005f;
						smallBee.zTrans -= 0.02f;
						smallBee.xTrans -= 0.005f;
					}
					else
					{
						smallBee.bBeeWings = TRUE;
					}
					smallBee.yRotate += deltaTime * 200.0f;
				}


				if (time >= 87.0f && time <= 152.0f)
				{
					if (splCount6 < 2000 && time > value3)
					{
						if (time < 92.0f)
						{
							camera.position.x = splineData6[splCount6].x * 1.1f;
							camera.position.y = splineData6[splCount6].y * 1.05f;
							camera.position.z = splineData6[splCount6].z * 0.985f;
							birdYRotate += 12.0f * deltaTime;
							updateScene();
						}

						splCount6 += 2;
						value3 += 0.001f;
						
						updateBird();
					}
				}

				if (time > 92.0f && time <= 152.0f)
				{
					///BEE flying
					{
						bigBee.bBeeFly = TRUE;

						if (bigBee.wingRotate.x <= -60.0f && bigBee.bBeeWings == TRUE)
						{
							bigBee.wingRotate.x += 1000.0f * deltaTime;
						}
						else if (bigBee.wingRotate.x >= -120.0f)
						{
							bigBee.bBeeWings = FALSE;
							bigBee.wingRotate.x -= 1000.0f * deltaTime;
						}
						else
						{
							bigBee.bBeeWings = TRUE;
						}
					}

					if (iWeight1 <= 1.0f)
					{
						bigBee.xTrans = lerpf(0.0f, 3.1f, iWeight1, TRUE);
						bigBee.yTrans = lerpf(0.0f, 0.25f, iWeight1, TRUE);
						bigBee.zTrans = lerpf(0.0f, 0.0f, iWeight1, TRUE);

						camera.front = { -22.0f,4.0f,-232.0f };
						camera.position = { -22.0f, 3.5f,  -218.0f };
						smallBee.yRotate = 180.0f;

						iWeight1 += deltaTime * 0.2f;
					}
					else if (splCount7 <= 2000)
					{
						if (smallBee.yRotate >= 280.0f)
						{
							bigBee.yRotate += deltaTime * 7.0f;
							smallBee.yRotate += deltaTime * 7.0f;
						}

						if (splCount7 > 200)
						{
							camera.front.x = splineData7[splCount7].x - 20.5f;
							camera.front.y = splineData7[splCount7].y + 2.8f;
							camera.front.z = splineData7[splCount7].z - 223.0f;

							camera.position.x = splineData7[splCount7].x - 21.8;
							camera.position.y = splineData7[splCount7].y + 3.0;
							camera.position.z = splineData7[splCount7].z - 225.8;
						}

						splCount7++;
					}
				}

				if (time >= 109.0f)
				{
					mainScene = 2;
				}

			}
		}

		if (mainScene == 2)
		{
			///BIG BEE FLY
			{
				bigBee.bBeeFly = TRUE;
				if (bigBee.wingRotate.x <= -60.0f && bigBee.bBeeWings == TRUE)
				{
					bigBee.wingRotate.x += 1000.0f * deltaTime;
				}
				else if (bigBee.wingRotate.x >= -120.0f)
				{
					bigBee.bBeeWings = FALSE;
					bigBee.wingRotate.x -= 1000.0f * deltaTime;
				}
				else
				{
					bigBee.bBeeWings = TRUE;
				}
			}
			if (splCount9 < 500)
			{
				if (ctr == 2)
				{
					camera.front.x = splineData9[splCount9 + 3].x + bigBee.xTrans + 0.4f;
					camera.front.y = splineData9[splCount9 + 3].y + bigBee.yTrans;
					camera.front.z = splineData9[splCount9 + 3].z + bigBee.zTrans;

					camera.position.x = splineData9[splCount9].x + smallBee.xTrans + 0.1;
					camera.position.y = splineData9[splCount9].y + smallBee.yTrans - 0.05;
					camera.position.z = splineData9[splCount9].z + smallBee.zTrans - 1.9f;

					splCount9++;
					ctr = 0;
					//BBX:-352.819336, BBY : 41.977524, BBZ : 113.288536
					//fprintf(gpFILE, "BBX:%f,BBY:%f,BBZ:%f\n", bigBee.xTrans + splineData9[splCount9].x, bigBee.yTrans + splineData9[splCount9].y, bigBee.zTrans + splineData9[splCount9].z);
				}	
				else
				{
					ctr++;
				}
			}
			else if (bSmallBeeFlying == FALSE)
			{
				bBeeWalk = TRUE;

				camera.front.x = splineData9[splCount9].x + smallBee.xTrans + 0.4f;
				camera.front.y = splineData9[splCount9].y + smallBee.yTrans;
				camera.front.z = splineData9[splCount9].z + smallBee.zTrans;

				smallBee.xRotate += 1.5f;
				smallBee.yRotate += 0.3f;
				smallBee.zRotate += 0.7f;

				if (smallBee.yTrans > -7.0f)
				{
					//camera.position.x = splineData9[splCount9].x + smallBee.xTrans;
					//camera.position.y = splineData9[splCount9].y + smallBee.yTrans + 0.70f;
					//camera.position.z = splineData9[splCount9].z + smallBee.zTrans - 1.70f;

					//fprintf(gpFILE, "BBX:%f,BBY:%f,BBZ:%f\n", bigBee.xTrans + splineData9[splCount9].x, bigBee.yTrans + splineData9[splCount9].y, bigBee.zTrans + splineData9[splCount9].z);
					//BBX:-352.819336, BBY : 41.977524, BBZ : 113.288536

					smallBee.yTrans = lerpf(smallBee.yTrans, -9.0f, iWeight2, TRUE);
					iWeight2 += deltaTime * 0.075f;
				}
				else if (smallBee.yTrans > -25.0f)
				{
					smallBee.yTrans = lerpf(smallBee.yTrans, -30.0f, iWeight3, TRUE);
					iWeight3 += deltaTime * 0.0075f;


					camera.position.x = -352.819336 +  (cos(theata) * (15.0f + (i * 0.15)));
					camera.position.y = 41.977524 - 13.0f;
					camera.position.z = 113.288536 +  (sin(theata) * (15.0f + (i * 0.15)));
					theata = (i) * (M_PI / 180);

					/*camera.front.x = -352.819336;
					camera.front.y = 41.977524;
					camera.front.z = 113.288536;*/

					/*camera.position.x = splineData10[splCount10].x;
					camera.position.y = splineData10[splCount10].y;
					camera.position.z = splineData10[splCount10].z;*/
					//camera.position.x = -352.819336 + circle[circleArrayCounter][0];
					//camera.position.y = 41.977524;
					//camera.position.z = 113.288536 + circle[circleArrayCounter][1];

					resize(canvas.x, canvas.y);
					fovy += 0.0375f;
					i += 0.25f;
				
					//splCount10 += 1;
				}
				else if (smallBee.yTrans > -50.0f)
				{
					smallBee.yTrans = lerpf(smallBee.yTrans, -50.0f, iWeight8, TRUE);
					iWeight8 += deltaTime * 0.075f;

					camera.position.x = splineData9[splCount9].x + smallBee.xTrans;
					camera.position.y = splineData9[splCount9].y - 30.0f + 0.70f;
					camera.position.z = splineData9[splCount9].z + smallBee.zTrans - 5.70f;
					fovy = 45.0f;
				}
				else if(bSmallBeeFlying == FALSE)
				{
					camera.position.x = splineData9[splCount9].x + smallBee.xTrans;
					camera.position.y = splineData9[splCount9].y - 30.60f + 0.70f;
					camera.position.z = splineData9[splCount9].z + smallBee.zTrans - 1.50f;
					fovy = 45.0f;
					bSmallBeeFlying = TRUE;
				}
			}

			///SMALL BEE FLY
			if (bSmallBeeFlying)
			{
				bBeeWalk = FALSE;

				smallBee.bBeeFly = TRUE;
				if (smallBee.wingRotate.x <= -60.0f && smallBee.bBeeWings == TRUE)
				{
					smallBee.wingRotate.x += 1000.0f * deltaTime;
				}
				else if (smallBee.wingRotate.x >= -120.0f)
				{
					smallBee.bBeeWings = FALSE;
					smallBee.wingRotate.x -= 1000.0f * deltaTime;
				}
				else
				{
					smallBee.bBeeWings = TRUE;
				}
			}

			if (bSmallBeeFlying && splCount9 < 2000)
			{
				if (smallBee.yTrans < -30.0f && bSmallBeeFlyingWithBigBee)
				{
					camera.front.x = splineData9[splCount9].x + smallBee.xTrans;
					camera.front.y = splineData9[splCount9].y + smallBee.yTrans;
					camera.front.z = splineData9[splCount9].z + smallBee.zTrans;

					//updateScene();
					smallBee.xRotate = 0.0f;
					smallBee.yRotate = 180.0f;
					smallBee.zRotate = 0.0f;

					smallBee.yTrans = lerpf(smallBee.yTrans, -30.0f, iWeight4, TRUE);
					iWeight4 += deltaTime * 0.05f;
				}
				else if (smallBee.yTrans < 5.25f)
				{
					camera.front.x = splineData9[splCount9].x + smallBee.xTrans;
					camera.front.y = splineData9[splCount9].y + smallBee.yTrans;
					camera.front.z = splineData9[splCount9].z + smallBee.zTrans;

					bSmallBeeFlyingWithBigBee = FALSE;
					camera.position.x = splineData9[splCount9].x + smallBee.xTrans;
					camera.position.y = splineData9[splCount9].y + smallBee.yTrans + 0.70f;
					camera.position.z = splineData9[splCount9].z + smallBee.zTrans - 5.70f;

					smallBee.xTrans += 0.001f;
					//smallBee.zTrans -= 0.001f;

					smallBee.yTrans = lerpf(smallBee.yTrans, 5.25f, iWeight5, TRUE);
					iWeight5 += deltaTime * 0.05f;
				}
				else if (splCount9 < 2000 || 1)
				{
					
					if (ctr == 2)
					{
						camera.front.x = splineData9[splCount9 + 3].x + bigBee.xTrans;
						camera.front.y = splineData9[splCount9 + 3].y + bigBee.yTrans;
						camera.front.z = splineData9[splCount9 + 3].z + bigBee.zTrans;

						if(splCount9 < 2000)
						splCount9++;

						ctr = 0;
					}
					else
					{
						ctr++;
					}
				}


			}
		}
	}

	

}



void uninitialize(void)
{
	//*** Function Declarations ***
	void ToggleFullscreen(void);


	//*** Code ***
	if (gbFullscreen == TRUE)
	{
		ToggleFullscreen();
		gbFullscreen = FALSE;
	}


	//*** Make The hdc As Current DC ***//
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}


	//*** Destroy Rendering Contex***
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}


	//*** Release HDC ***
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}


	//*** Destroy Window ***
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}


	//Delete Textures


	//*** Close Log File ***
	if (gpFILE)
	{
		fprintf(gpFILE, "Program Ended Successfully\n");
		fclose(gpFILE);
		gpFILE = NULL;
	}
}



void drawBigBee(void)
{
	//gluLookAt(camera.position.x, camera.position.y, camera.position.z, camera.front.x + camera.position.x, camera.front.y + camera.position.y, camera.front.z + camera.position.z, camera.up.x, camera.up.y, camera.up.z);
	glPushMatrix();
	glPushMatrix();//body

	glPushMatrix();//head

	glPushMatrix();//Leg 1

	glPushMatrix();//Leg 2

	glPushMatrix();//Leg 3

	glPushMatrix();//leg4

	glPushMatrix();//Leg5

	glPushMatrix();//Leg6

	glPushMatrix();//wings1
	glPushMatrix();//wings2
	glPushMatrix();//wings3


	//BODY
	{
		glScalef(0.7f, 1.0f + sin(time) * deltaTime * 2.0f, 1.0f + sin(time) * deltaTime * 2.0f);
		glPushMatrix();
		glPushMatrix();

		glColor3f(1., 0.733, 0.008);
		gluSphere(quadric, 1.0f, 30, 30);

		glTranslatef(0.5f, -0.05f, 0.0f);
		glColor3f(0.4, 0.243, 0.243);
		gluSphere(quadric, 0.93f, 30, 30);

		glTranslatef(0.5f, -0.05f, 0.0f);
		glColor3f(1., 0.733, 0.008);
		gluSphere(quadric, 0.86f, 30, 30);

		glTranslatef(0.5f, -0.05f, 0.0f);
		glColor3f(0.4, 0.243, 0.243);
		gluSphere(quadric, 0.79f, 30, 30);

		glTranslatef(0.5f, -0.05f, 0.0f);
		glColor3f(1., 0.733, 0.008);
		gluSphere(quadric, 0.6f, 6, 6);

		glTranslatef(0.5f, -0.05f, 0.0f);
		glColor3f(0.1f, 0.1f, 0.1f);
		gluSphere(quadric, 0.3f, 4, 4);
		glPopMatrix();


		glTranslatef(-0.5f, 0.0f, 0.0f);
		glColor3f(0.4, 0.243, 0.243);
		gluSphere(quadric, 0.93f, 30, 30);

		glTranslatef(-0.5f, 0.0f, 0.0f);
		glColor3f(1., 0.733, 0.008);
		gluSphere(quadric, 0.86f, 30, 30);

		glTranslatef(-0.5f, 0.0f, 0.0f);
		glColor3f(0.4, 0.243, 0.243);
		gluSphere(quadric, 0.79f, 30, 30);

		glTranslatef(-0.5f, 0.0f, 0.0f);
		glColor3f(1., 0.733, 0.008);
		gluSphere(quadric, 0.72f, 30, 30);

		glTranslatef(-0.5f, 0.00f, 0.0f);
		glColor3f(0.4, 0.243, 0.243);
		gluSphere(quadric, 0.6f, 6, 6);
		glPopMatrix();
		glPopMatrix();

		glTranslatef(-2.85f, 0.00f, 0.0f);
		glScalef(1.0f, 0.7f, 1.0f);
		glColor3f(0.408, 0.129, 0.012);
		gluSphere(quadric, 1.2f, 14, 10);
	}

	glPopMatrix();//body

	//HEAD
	{
		glRotatef(bigBee.headMove, 1.0f, 0.0f, 0.0f);
		glPushMatrix();
		glPushMatrix();
		glPushMatrix();
		glPushMatrix();

		glTranslatef(-4.5f, -0.2f, 0.0f);
		glScalef(0.75f, 1.0f, 1.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		glColor3f(0.5f, 0.5f, 0.1f);
		gluSphere(quadric, 0.8f, 50, 50);
		glPopMatrix();

		//mouth
		glTranslatef(-4.5, -0.35f, 0.0f);
		glColor3f(0.1f, 0.1f, 0.1f);
		glScalef(0.75f, 1.0f, 1.0f);
		gluSphere(quadric, 0.7f, 50, 50);
		glPopMatrix();

		//EYE
		{
			glTranslatef(-4.55f, -0.2f, 0.525f);
			glScalef(1.0f, 1.5f, 1.0f);
			//glRotatef(bigBee.headMove, 1.0f, 0.0f, 0.0f);
			glPushMatrix();

			//Outer
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			glColor3f(1, 1, 1);
			gluSphere(quadric, 0.3f, 30, 30);

			//Middle
			glTranslatef(-0.05f, -0.2f, 0.0f);
			glScalef(0.7f + bigBee.eyeMovement, 0.7f + bigBee.eyeMovement, 0.7f + bigBee.eyeMovement);
			glColor3f(0.243, 0.137, 0.071);
			gluSphere(quadric, 0.15f, 30, 30);

			//Inner
			glTranslatef(0.0f, -0.08f, 0.0f);
			glColor3f(1, 1, 1);
			gluSphere(quadric, 0.075f, 30, 30);

			glPopMatrix();

			//EYE MOVEMENT 2
			//Outer
			glTranslatef(0.0f, 0.0f, -0.525f * 2);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			gluSphere(quadric, 0.3f, 30, 30);

			//Middle
			glTranslatef(-0.05f, 0.2f, 0.0f);
			glScalef(0.7f + bigBee.eyeMovement, 0.7f + bigBee.eyeMovement, 0.7f + bigBee.eyeMovement);
			glColor3f(0.243, 0.137, 0.071);
			gluSphere(quadric, 0.15f, 30, 30);

			//Inner
			glTranslatef(0.0f, 0.08f, 0.0f);
			glColor3f(1, 1, 1);
			gluSphere(quadric, 0.075f, 30, 30);
		}
		glPopMatrix();

		//ANTENA1
		{
			glTranslatef(-5.7f, 0.9f, 0.4f);
			glRotatef(-20.0f + bigBee.rotAntena, 0.0f, 0.0f, 1.0f);
			glPushMatrix();
			glPushMatrix();

			glScalef(1.2f, 0.1f, 0.1f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			gluSphere(quadric, 0.5f, 30, 30);
			glPopMatrix();


			glTranslatef(-0.6f, 0.0f, 0.0f);
			gluSphere(quadric, 0.05f, 20, 20);
			glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(-0.6f, 0.0f, 0.0f);

			glScalef(1.2f, 0.1f, 0.1f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			gluSphere(quadric, 0.5f, 30, 30);

			glTranslatef(-0.6f, 0.0f, 0.00f);
			glScalef(0.833f, 10.0f, 10.0f);
			glColor3f(0.992, 0.867, 0.498);
			gluSphere(quadric, 0.2f, 30, 30);
			glColor3f(1, 1, 1);

			glPopMatrix();

			glTranslatef(0.6f, 0.0f, 0.0f);
			gluSphere(quadric, 0.05f, 20, 20);
			glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.6f, 0.0f, 0.0f);

			glScalef(1.2f, 0.1f, 0.1f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			gluSphere(quadric, 0.5f, 30, 30);
		}

		glPopMatrix();

		//ANTENA2
		{
			glTranslatef(-5.7f, 0.9f, -0.4f);
			glRotatef(-30.0f - bigBee.rotAntena, 0.0f, 0.0f, 1.0f);
			glPushMatrix();
			glPushMatrix();

			glScalef(1.2f, 0.1f, 0.1f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			gluSphere(quadric, 0.5f, 30, 30);
			glPopMatrix();


			glTranslatef(-0.6f, 0.0f, 0.0f);
			gluSphere(quadric, 0.05f, 20, 20);
			glRotatef(50.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(-0.6f, 0.0f, 0.0f);

			glScalef(1.2f, 0.1f, 0.1f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			gluSphere(quadric, 0.5f, 30, 30);

			glTranslatef(-0.6f, 0.0f, 0.00f);
			glScalef(0.833f, 10.0f, 10.0f);
			glColor3f(0.992, 0.867, 0.498);
			gluSphere(quadric, 0.2f, 30, 30);
			glColor3f(1, 1, 1);

			glPopMatrix();

			glTranslatef(0.6f, 0.0f, 0.0f);
			gluSphere(quadric, 0.05f, 20, 20);
			glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.6f, 0.0f, 0.0f);

			glScalef(1.2f, 0.1f, 0.1f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			gluSphere(quadric, 0.5f, 30, 30);
		}
	}

	glPopMatrix();//Head

	//RIGHT LEGS
	{
		//Leg1 
		glTranslatef(-3.5f, -0.75f, 1.0f);
		if (bBeeWalk != TRUE)
		{
			if (bigBee.bBeeFly == TRUE && bigBee.rLegs.x <= 30.0f)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.1f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.1f;
			}
			else
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
			}
		}
		else
		{
			if (bigBee.rLegs.x <= 30.0f && bToggleWalk != TRUE)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.02f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				bToggleWalk = TRUE;
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.02f;
			}
			else
			{
				bToggleWalk = FALSE;
			}
		}

		glPushMatrix();

		glScalef(0.1f, 0.8f, 0.1f);
		glColor3f(0.5f, 0.5f, 0.5f);
		gluSphere(quadric, 0.8f, 10, 10);
		glPopMatrix();

		glTranslatef(0.3f, -1.0f, 0.0f);
		glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.1f, 0.6f, 0.1f);
		glColor3f(0.5f, 0.5f, 0.5f);
		gluSphere(quadric, 0.8f, 10, 10);

		glPopMatrix();//Leg1


		//Leg2 
		glTranslatef(-3.0f, -0.75f, 1.0f);
		if (bBeeWalk != TRUE)
		{
			if (bigBee.bBeeFly == TRUE && bigBee.rLegs.x <= 30.0f)
			{
				glRotatef(-20.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.1f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				glRotatef(-20.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.1f;
			}
			else
			{
				glRotatef(-20.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
			}
		}
		else
		{
			if (bigBee.rLegs.x <= 30.0f && bToggleWalk != TRUE)
			{
				glRotatef(-20.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.02f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				bToggleWalk = TRUE;
				glRotatef(-20.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.02f;
			}
			else
			{
				bToggleWalk = FALSE;
			}
		}

		//glRotatef(-20.0f, 0.0f, 0.0f, 1.0f);
		glPushMatrix();

		glScalef(0.1f, 0.8f, 0.1f);
		glColor3f(0.5f, 0.5f, 0.5f);
		gluSphere(quadric, 0.8f, 10, 10);
		glPopMatrix();

		glTranslatef(0.3f, -1.0f, 0.0f);
		glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.1f, 0.6f, 0.1f);
		glColor3f(0.5f, 0.5f, 0.5f);
		gluSphere(quadric, 0.8f, 10, 10);

		glPopMatrix();//Leg2


		//Leg3
		glTranslatef(-2.5f, -0.75f, 1.0f);
		if (bBeeWalk != TRUE)
		{
			if (bigBee.bBeeFly == TRUE && bigBee.rLegs.x <= 30.0f)
			{
				glRotatef(0.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.1f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				glRotatef(0.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.1f;
			}
			else
			{
				glRotatef(0.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
			}
		}
		else
		{
			if (bigBee.rLegs.x <= 30.0f && bToggleWalk != TRUE)
			{
				glRotatef(0.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.02f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				bToggleWalk = TRUE;
				glRotatef(0.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.02f;
			}
			else
			{
				bToggleWalk = FALSE;
			}
		}

		//glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
		glPushMatrix();

		glScalef(0.1f, 0.8f, 0.1f);
		glColor3f(0.5f, 0.5f, 0.5f);
		gluSphere(quadric, 0.8f, 10, 10);
		glPopMatrix();

		glTranslatef(0.3f, -1.0f, 0.0f);
		glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.1f, 0.6f, 0.1f);
		glColor3f(0.5f, 0.5f, 0.5f);
		gluSphere(quadric, 0.8f, 10, 10);

		glPopMatrix();//Leg3
	}


	//LEFT LEGS
	{
		//Leg1 
		glTranslatef(-3.5f, -0.75f, -1.0f);
		if (bBeeWalk != TRUE)
		{
			if (bigBee.bBeeFly == TRUE && bigBee.rLegs.x <= 30.0f)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.1f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.1f;
			}
			else
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
			}
		}
		else
		{
			if (bigBee.rLegs.x <= 30.0f && bToggleWalk != TRUE)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.02f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				bToggleWalk = TRUE;
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.02f;
			}
			else
			{
				bToggleWalk = FALSE;
			}
		}

		//glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
		glPushMatrix();

		glScalef(0.1f, 0.8f, 0.1f);
		glColor3f(0.949, 0.91, 0.863);
		gluSphere(quadric, 0.8f, 10, 10);
		glPopMatrix();

		glTranslatef(0.3f, -1.0f, 0.0f);
		glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.1f, 0.6f, 0.1f);
		glColor3f(0.949, 0.91, 0.863);
		gluSphere(quadric, 0.8f, 10, 10);

		glPopMatrix();// Leg4


		//Leg2 
		glTranslatef(-3.0f, -0.75f, -1.0f);
		if (bBeeWalk != TRUE)
		{
			if (bigBee.bBeeFly == TRUE && bigBee.rLegs.x <= 30.0f)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.1f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.1f;
			}
			else
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
			}
		}
		else
		{
			if (bigBee.rLegs.x <= 30.0f && bToggleWalk != TRUE)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.02f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				bToggleWalk = TRUE;

				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.02f;
			}
			else
			{
				bToggleWalk = FALSE;
			}
		}

		//glRotatef(-20.0f, 0.0f, 0.0f, 1.0f);
		glPushMatrix();

		glScalef(0.1f, 0.8f, 0.1f);
		glColor3f(0.949, 0.91, 0.863);
		gluSphere(quadric, 0.8f, 10, 10);
		glPopMatrix();

		glTranslatef(0.3f, -1.0f, 0.0f);
		glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.1f, 0.6f, 0.1f);
		glColor3f(0.949, 0.91, 0.863);
		gluSphere(quadric, 0.8f, 10, 10);

		glPopMatrix();//Leg5


		//Leg3
		glTranslatef(-2.5f, -0.75f, -1.0f);
		if (bBeeWalk != TRUE)
		{
			if (bigBee.bBeeFly == TRUE && bigBee.rLegs.x <= 30.0f)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.1f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.1f;
			}
			else
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
			}
		}
		else
		{
			if (bigBee.rLegs.x <= 30.0f && bToggleWalk != TRUE)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.02f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				bToggleWalk = TRUE;
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.02f;
			}
			else
			{
				bToggleWalk = FALSE;
			}
		}

		//glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
		glPushMatrix();

		glScalef(0.1f, 0.8f, 0.1f);
		glColor3f(0.949, 0.91, 0.863);
		gluSphere(quadric, 0.8f, 10, 10);
		glPopMatrix();

		glTranslatef(0.3f, -1.0f, 0.0f);
		glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.1f, 0.6f, 0.1f);
		glColor3f(0.949, 0.91, 0.863);
		gluSphere(quadric, 0.8f, 10, 10);

		glPopMatrix();//Leg6
	}




	//WINGS
	{
		glBindTexture(GL_TEXTURE_2D, texture_wing);
		if (bigBee.bBeeFly == TRUE)
		{
			glTranslatef(-3.0f, 0.0f, 0.0f);
			glRotatef(bigBee.wingRotate.x + 10.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -2.0f, 0.0f);
		}
		else
		{
			if ((int)(floor(time)) % 5 == 0)
			{
				glTranslatef(-1.3f, 1.2f, 0.9f);
				glRotatef(120.0f + wingDefaultRotation, 0.0f, 0.0f, 1.0f);
				glRotatef(-wingDefaultRotation, 1.0f, 0.0f, 0.0f);
				glRotatef(wingDefaultRotation, 0.0f, 1.0f, 0.0f);

				///Default Wing A	
				wingDefaultRotation += 0.025f;
			}
			else
			{
				glTranslatef(-1.3f, 1.2f, 0.9f);
				glRotatef(120.0f + wingDefaultRotation, 0.0f, 0.0f, 1.0f);
				glRotatef(-wingDefaultRotation, 1.0f, 0.0f, 0.0f);
				glRotatef(wingDefaultRotation, 0.0f, 1.0f, 0.0f);

				if (wingDefaultRotation >= 0.0f)
					wingDefaultRotation -= 0.025f;
			}

		}
		glScalef(1.0f, 2.0f, 0.05f);
		gluDisk(quadric, .0f, 1.0f, 12, 12);
		glPopMatrix();//Wings1


		if (bigBee.bBeeFly == TRUE)
		{
			glTranslatef(-2.3f, 0.0f, 0.0f);
			glRotatef(bigBee.wingRotate.x - 10.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -2.0f, 0.0f);
		}
		else
		{
			if ((int)(floor(time)) % 5 == 0)
			{
				glTranslatef(-1.5f, 1.05f, 0.8f);
				glRotatef(110.0f + wingDefaultRotation, 0.0f, 0.0f, 1.0f);
				glRotatef(-wingDefaultRotation, 1.0f, 0.0f, 0.0f);
				glRotatef(wingDefaultRotation, 0.0f, 1.0f, 0.0f);
			}
			else
			{
				glTranslatef(-1.5f, 1.05f, 0.8f);
				glRotatef(110.0f + wingDefaultRotation, 0.0f, 0.0f, 1.0f);
				glRotatef(-wingDefaultRotation, 1.0f, 0.0f, 0.0f);
				glRotatef(wingDefaultRotation, 0.0f, 1.0f, 0.0f);
			}

		}
		glScalef(1.0f, 2.2f, 0.05f);
		gluDisk(quadric, .0f, 1.0f, 12, 12);
		glPopMatrix();//Wings2


		if (bigBee.bBeeFly == TRUE)
		{
			glTranslatef(-3.0f, 0.0f, 0.0f);
			glRotatef(-bigBee.wingRotate.x + 10.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -2.0f, 0.0f);
		}
		else
		{
			if ((int)(floor(time)) % 5 == 0)
			{
				glTranslatef(-1.3f, 1.2f, -0.9f);
				glRotatef(120.0f + wingDefaultRotation, 0.0f, 0.0f, 1.0f);
				glRotatef(wingDefaultRotation, 1.0f, 0.0f, 0.0f);
				glRotatef(-wingDefaultRotation, 0.0f, 1.0f, 0.0f);
			}
			else
			{
				glTranslatef(-1.3f, 1.2f, -0.9f);
				glRotatef(120.0f + wingDefaultRotation, 0.0f, 0.0f, 1.0f);
				glRotatef(wingDefaultRotation, 1.0f, 0.0f, 0.0f);
				glRotatef(-wingDefaultRotation, 0.0f, 1.0f, 0.0f);
			}

		}
		glScalef(1.0f, 2.0f, 0.05f);
		gluDisk(quadric, .0f, 1.0f, 12, 12);
		glPopMatrix();//Wings3


		if (bigBee.bBeeFly == TRUE)
		{
			glTranslatef(-2.3f, 0.0f, 0.0f);
			glRotatef(-bigBee.wingRotate.x - 10.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -2.0f, 0.0f);
		}
		else
		{
			if ((int)(floor(time)) % 5 == 0)
			{
				glTranslatef(-1.5f, 1.05f, -0.8f);
				glRotatef(110.0f + wingDefaultRotation, 0.0f, 0.0f, 1.0f);
				glRotatef(wingDefaultRotation, 1.0f, 0.0f, 0.0f);
				glRotatef(-wingDefaultRotation, 0.0f, 1.0f, 0.0f);
			}
			else
			{
				glTranslatef(-1.5f, 1.05f, -0.8f);
				glRotatef(110.0f + wingDefaultRotation, 0.0f, 0.0f, 1.0f);
				glRotatef(wingDefaultRotation, 1.0f, 0.0f, 0.0f);
				glRotatef(-wingDefaultRotation, 0.0f, 1.0f, 0.0f);
			}

		}
		glScalef(1.0f, 2.2f, 0.05f);
		gluDisk(quadric, .0f, 1.0f, 12, 12);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void drawSmallBee(void)
{
	//gluLookAt(camera.position.x, camera.position.y, camera.position.z, camera.front.x + camera.position.x, camera.front.y + camera.position.y, camera.front.z + camera.position.z, camera.up.x, camera.up.y, camera.up.z);
	glPushMatrix();
	glPushMatrix();//body

	glPushMatrix();//head

	glPushMatrix();//Leg 1

	glPushMatrix();//Leg 2

	glPushMatrix();//Leg 3

	glPushMatrix();//leg4

	glPushMatrix();//Leg5

	glPushMatrix();//Leg6

	glPushMatrix();//wings1
	glPushMatrix();//wings2
	glPushMatrix();//wings3


	//BODY
	{
		glScalef(0.7f, 1.0f + sin(time) * deltaTime * 2.0f, 1.0f + sin(time) * deltaTime * 2.0f);
		glPushMatrix();
		glPushMatrix();

		glColor3f(1., 0.733, 0.008);
		gluSphere(quadric, 1.0f, 30, 30);

		glTranslatef(0.5f, -0.05f, 0.0f);
		glColor3f(0.4, 0.243, 0.243);
		gluSphere(quadric, 0.93f, 30, 30);

		glTranslatef(0.5f, -0.05f, 0.0f);
		glColor3f(1., 0.733, 0.008);
		gluSphere(quadric, 0.86f, 30, 30);

		glTranslatef(0.5f, -0.05f, 0.0f);
		glColor3f(0.4, 0.243, 0.243);
		gluSphere(quadric, 0.79f, 30, 30);

		glTranslatef(0.5f, -0.05f, 0.0f);
		glColor3f(1., 0.733, 0.008);
		gluSphere(quadric, 0.6f, 6, 6);

		glTranslatef(0.5f, -0.05f, 0.0f);
		glColor3f(0.1f, 0.1f, 0.1f);
		gluSphere(quadric, 0.3f, 4, 4);
		glPopMatrix();


		glTranslatef(-0.5f, 0.0f, 0.0f);
		glColor3f(0.4, 0.243, 0.243);
		gluSphere(quadric, 0.93f, 30, 30);

		glTranslatef(-0.5f, 0.0f, 0.0f);
		glColor3f(1., 0.733, 0.008);
		gluSphere(quadric, 0.86f, 30, 30);

		glTranslatef(-0.5f, 0.0f, 0.0f);
		glColor3f(0.4, 0.243, 0.243);
		gluSphere(quadric, 0.79f, 30, 30);

		glTranslatef(-0.5f, 0.0f, 0.0f);
		glColor3f(1., 0.733, 0.008);
		gluSphere(quadric, 0.72f, 30, 30);

		glTranslatef(-0.5f, 0.00f, 0.0f);
		glColor3f(0.4, 0.243, 0.243);
		gluSphere(quadric, 0.6f, 6, 6);
		glPopMatrix();
		glPopMatrix();

		glTranslatef(-2.85f, 0.00f, 0.0f);
		glScalef(1.0f, 0.7f, 1.0f);
		glColor3f(0.408, 0.129, 0.012);
		gluSphere(quadric, 1.2f, 14, 10);
	}

	glPopMatrix();//body

	//HEAD
	{
		glRotatef(-bigBee.headMove, 1.0f, 0.0f, 0.0f);
		glPushMatrix();
		glPushMatrix();
		glPushMatrix();
		glPushMatrix();

		glTranslatef(-4.5f, -0.2f, 0.0f);
		glScalef(0.75f, 1.0f, 1.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		glColor3f(0.5f, 0.5f, 0.1f);
		gluSphere(quadric, 0.8f, 50, 50);
		glPopMatrix();

		//mouth
		glTranslatef(-4.5, -0.35f, 0.0f);
		glColor3f(0.1f, 0.1f, 0.1f);
		glScalef(0.75f, 1.0f, 1.0f);
		gluSphere(quadric, 0.7f, 50, 50);
		glPopMatrix();

		//EYE
		{
			glTranslatef(-4.55f, -0.2f, 0.525f);
			glScalef(1.0f, 1.5f, 1.0f);
			//glRotatef(bigBee.headMove, 1.0f, 0.0f, 0.0f);
			glPushMatrix();

			//Outer
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			glColor3f(1, 1, 1);
			gluSphere(quadric, 0.3f, 30, 30);

			//Middle
			glTranslatef(-0.05f, -0.2f, 0.0f);
			glScalef(0.7f + smallBee.eyeMovement, 0.7f + smallBee.eyeMovement, 0.7f + smallBee.eyeMovement);
			glColor3f(0.243, 0.137, 0.071);
			gluSphere(quadric, 0.15f, 30, 30);

			//Inner
			glTranslatef(0.0f, -0.08f, 0.0f);
			glColor3f(1, 1, 1);
			gluSphere(quadric, 0.075f, 30, 30);

			glPopMatrix();

			//EYE MOVEMENT 2
			//Outer
			glTranslatef(0.0f, 0.0f, -0.525f * 2);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			gluSphere(quadric, 0.3f, 30, 30);

			//Middle
			glTranslatef(-0.05f, 0.2f, 0.0f);
			glScalef(0.7f + smallBee.eyeMovement, 0.7f + smallBee.eyeMovement, 0.7f + smallBee.eyeMovement);
			glColor3f(0.243, 0.137, 0.071);
			gluSphere(quadric, 0.15f, 30, 30);

			//Inner
			glTranslatef(0.0f, 0.08f, 0.0f);
			glColor3f(1, 1, 1);
			gluSphere(quadric, 0.075f, 30, 30);
		}
		glPopMatrix();

		//ANTENA1
		{
			glTranslatef(-5.7f, 0.9f, 0.4f);
			glRotatef(-20.0f + smallBee.rotAntena, 0.0f, 0.0f, 1.0f);
			glPushMatrix();
			glPushMatrix();

			glScalef(1.2f, 0.1f, 0.1f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			gluSphere(quadric, 0.5f, 30, 30);
			glPopMatrix();


			glTranslatef(-0.6f, 0.0f, 0.0f);
			gluSphere(quadric, 0.05f, 20, 20);
			glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(-0.6f, 0.0f, 0.0f);

			glScalef(1.2f, 0.1f, 0.1f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			gluSphere(quadric, 0.5f, 30, 30);

			glTranslatef(-0.6f, 0.0f, 0.00f);
			glScalef(0.833f, 10.0f, 10.0f);
			glColor3f(0.992, 0.867, 0.498);
			gluSphere(quadric, 0.2f, 30, 30);
			glColor3f(1, 1, 1);

			glPopMatrix();

			glTranslatef(0.6f, 0.0f, 0.0f);
			gluSphere(quadric, 0.05f, 20, 20);
			glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.6f, 0.0f, 0.0f);

			glScalef(1.2f, 0.1f, 0.1f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			gluSphere(quadric, 0.5f, 30, 30);
		}

		glPopMatrix();

		//ANTENA2
		{
			glTranslatef(-5.7f, 0.9f, -0.4f);
			glRotatef(-30.0f - smallBee.rotAntena, 0.0f, 0.0f, 1.0f);
			glPushMatrix();
			glPushMatrix();

			glScalef(1.2f, 0.1f, 0.1f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			gluSphere(quadric, 0.5f, 30, 30);
			glPopMatrix();


			glTranslatef(-0.6f, 0.0f, 0.0f);
			gluSphere(quadric, 0.05f, 20, 20);
			glRotatef(50.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(-0.6f, 0.0f, 0.0f);

			glScalef(1.2f, 0.1f, 0.1f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			gluSphere(quadric, 0.5f, 30, 30);

			glTranslatef(-0.6f, 0.0f, 0.00f);
			glScalef(0.833f, 10.0f, 10.0f);
			glColor3f(0.992, 0.867, 0.498);
			gluSphere(quadric, 0.2f, 30, 30);
			glColor3f(1, 1, 1);

			glPopMatrix();

			glTranslatef(0.6f, 0.0f, 0.0f);
			gluSphere(quadric, 0.05f, 20, 20);
			glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.6f, 0.0f, 0.0f);

			glScalef(1.2f, 0.1f, 0.1f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			gluSphere(quadric, 0.5f, 30, 30);
		}
	}

	glPopMatrix();//Head

	//RIGHT LEGS
	{
		//Leg1 
		glTranslatef(-3.5f, -0.75f, 1.0f);
		if (bBeeWalk != TRUE)
		{
			if (bigBee.bBeeFly == TRUE && bigBee.rLegs.x <= 30.0f)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.1f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.1f;
			}
			else
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
			}
		}
		else
		{
			if (bigBee.rLegs.x <= 30.0f && bToggleWalk != TRUE)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.02f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				bToggleWalk = TRUE;
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.02f;
			}
			else
			{
				bToggleWalk = FALSE;
			}
		}
		glPushMatrix();

		glScalef(0.1f, 0.8f, 0.1f);
		glColor3f(0.5f, 0.5f, 0.5f);
		gluSphere(quadric, 0.8f, 10, 10);
		glPopMatrix();

		glTranslatef(0.3f, -1.0f, 0.0f);
		glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.1f, 0.6f, 0.1f);
		glColor3f(0.5f, 0.5f, 0.5f);
		gluSphere(quadric, 0.8f, 10, 10);

		glPopMatrix();//Leg1


		//Leg2 
		glTranslatef(-3.0f, -0.75f, 1.0f);
		if (bBeeWalk != TRUE)
		{
			if (bigBee.bBeeFly == TRUE && bigBee.rLegs.x <= 30.0f)
			{
				glRotatef(-20.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.1f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				glRotatef(-20.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.1f;
			}
			else
			{
				glRotatef(-20.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
			}
		}
		else
		{
			if (bigBee.rLegs.x <= 30.0f && bToggleWalk != TRUE)
			{
				glRotatef(-20.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.02f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				bToggleWalk = TRUE;
				glRotatef(-20.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.02f;
			}
			else
			{
				bToggleWalk = FALSE;
			}
		}
		//glRotatef(-20.0f, 0.0f, 0.0f, 1.0f);
		glPushMatrix();

		glScalef(0.1f, 0.8f, 0.1f);
		glColor3f(0.5f, 0.5f, 0.5f);
		gluSphere(quadric, 0.8f, 10, 10);
		glPopMatrix();

		glTranslatef(0.3f, -1.0f, 0.0f);
		glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.1f, 0.6f, 0.1f);
		glColor3f(0.5f, 0.5f, 0.5f);
		gluSphere(quadric, 0.8f, 10, 10);

		glPopMatrix();//Leg2


		//Leg3
		glTranslatef(-2.5f, -0.75f, 1.0f);
		if (bBeeWalk != TRUE)
		{
			if (bigBee.bBeeFly == TRUE && bigBee.rLegs.x <= 30.0f)
			{
				glRotatef(0.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.1f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				glRotatef(0.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.1f;
			}
			else
			{
				glRotatef(0.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
			}
		}
		else
		{
			if (bigBee.rLegs.x <= 30.0f && bToggleWalk != TRUE)
			{
				glRotatef(0.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.02f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				bToggleWalk = TRUE;
				glRotatef(0.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.02f;
			}
			else
			{
				bToggleWalk = FALSE;
			}
		}
		//glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
		glPushMatrix();

		glScalef(0.1f, 0.8f, 0.1f);
		glColor3f(0.5f, 0.5f, 0.5f);
		gluSphere(quadric, 0.8f, 10, 10);
		glPopMatrix();

		glTranslatef(0.3f, -1.0f, 0.0f);
		glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.1f, 0.6f, 0.1f);
		glColor3f(0.5f, 0.5f, 0.5f);
		gluSphere(quadric, 0.8f, 10, 10);

		glPopMatrix();//Leg3
	}


	//LEFT LEGS
	{
		//Leg1 
		glTranslatef(-3.5f, -0.75f, -1.0f);
		if (bBeeWalk != TRUE)
		{
			if (bigBee.bBeeFly == TRUE && bigBee.rLegs.x <= 30.0f)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.1f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.1f;
			}
			else
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
			}
		}
		else
		{
			if (bigBee.rLegs.x <= 30.0f && bToggleWalk != TRUE)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.02f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				bToggleWalk = TRUE;
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.02f;
			}
			else
			{
				bToggleWalk = FALSE;
			}
		}
		//glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
		glPushMatrix();

		glScalef(0.1f, 0.8f, 0.1f);
		glColor3f(0.949, 0.91, 0.863);
		gluSphere(quadric, 0.8f, 10, 10);
		glPopMatrix();

		glTranslatef(0.3f, -1.0f, 0.0f);
		glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.1f, 0.6f, 0.1f);
		glColor3f(0.949, 0.91, 0.863);
		gluSphere(quadric, 0.8f, 10, 10);

		glPopMatrix();// Leg4


		//Leg2 
		glTranslatef(-3.0f, -0.75f, -1.0f);
		if (bBeeWalk != TRUE)
		{
			if (bigBee.bBeeFly == TRUE && bigBee.rLegs.x <= 30.0f)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.1f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.1f;
			}
			else
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
			}
		}
		else
		{
			if (bigBee.rLegs.x <= 30.0f && bToggleWalk != TRUE)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.02f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				bToggleWalk = TRUE;

				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.02f;
			}
			else
			{
				bToggleWalk = FALSE;
			}
		}
		//glRotatef(-20.0f, 0.0f, 0.0f, 1.0f);
		glPushMatrix();

		glScalef(0.1f, 0.8f, 0.1f);
		glColor3f(0.949, 0.91, 0.863);
		gluSphere(quadric, 0.8f, 10, 10);
		glPopMatrix();

		glTranslatef(0.3f, -1.0f, 0.0f);
		glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.1f, 0.6f, 0.1f);
		glColor3f(0.949, 0.91, 0.863);
		gluSphere(quadric, 0.8f, 10, 10);

		glPopMatrix();//Leg5


		//Leg3
		glTranslatef(-2.5f, -0.75f, -1.0f);
		if (bBeeWalk != TRUE)
		{
			if (bigBee.bBeeFly == TRUE && bigBee.rLegs.x <= 30.0f)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.1f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.1f;
			}
			else
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
			}
		}
		else
		{
			if (bigBee.rLegs.x <= 30.0f && bToggleWalk != TRUE)
			{
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x += 0.02f;

			}
			else if (bigBee.rLegs.x >= 0.0f)
			{
				bToggleWalk = TRUE;
				glRotatef(-30.0f + bigBee.rLegs.x, 0.0f, 0.0f, 1.0f);
				bigBee.rLegs.x -= 0.02f;
			}
			else
			{
				bToggleWalk = FALSE;
			}
		}
		//glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
		glPushMatrix();

		glScalef(0.1f, 0.8f, 0.1f);
		glColor3f(0.949, 0.91, 0.863);
		gluSphere(quadric, 0.8f, 10, 10);
		glPopMatrix();

		glTranslatef(0.3f, -1.0f, 0.0f);
		glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.1f, 0.6f, 0.1f);
		glColor3f(0.949, 0.91, 0.863);
		gluSphere(quadric, 0.8f, 10, 10);

		glPopMatrix();//Leg6
	}




	//WINGS
	{
		glBindTexture(GL_TEXTURE_2D, texture_wing);
		if (smallBee.bBeeFly == TRUE)
		{
			glTranslatef(-3.0f, 0.0f, 0.0f);
			glRotatef(smallBee.wingRotate.x + 10.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -2.0f, 0.0f);
		}
		else
		{
			glTranslatef(-1.3f, 1.2f, 0.9f);
			glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
		}
		glScalef(1.0f, 2.0f, 0.05f);
		gluDisk(quadric, .0f, 1.0f, 12, 12);
		glPopMatrix();//Wings1


		if (smallBee.bBeeFly == TRUE)
		{
			glTranslatef(-2.3f, 0.0f, 0.0f);
			glRotatef(smallBee.wingRotate.x + 10.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -2.0f, 0.0f);
		}
		else
		{
			glTranslatef(-1.5f, 1.05f, 0.8f);
			glRotatef(110.0f, 0.0f, 0.0f, 1.0f);
		}
		glScalef(1.0f, 2.2f, 0.05f);
		gluDisk(quadric, .0f, 1.0f, 12, 12);
		glPopMatrix();//Wings2


		if (smallBee.bBeeFly == TRUE)
		{
			glTranslatef(-3.0f, 0.0f, 0.0f);
			glRotatef(-smallBee.wingRotate.x + 30.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -2.0f, 0.0f);
		}
		else
		{
			glTranslatef(-1.3f, 1.2f, -0.9f);
			glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
		}
		glScalef(1.0f, 2.0f, 0.05f);
		gluDisk(quadric, .0f, 1.0f, 12, 12);
		glPopMatrix();//Wings3


		if (smallBee.bBeeFly == TRUE)
		{
			glTranslatef(-2.3f, 0.0f, 0.0f);
			glRotatef(-smallBee.wingRotate.x + 30.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -2.0f, 0.0f);
		}
		else
		{
			glTranslatef(-1.5f, 1.05f, -0.8f);
			glRotatef(110.0f, 0.0f, 0.0f, 1.0f);
		}
		glScalef(1.0f, 2.2f, 0.05f);
		gluDisk(quadric, .0f, 1.0f, 12, 12);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void stickman1(void)
{
	void arm(void);
	void arm2(void);
	void middlePart(void);
	void face(void);
	void leg1(void);
	void leg2(void);


	face();
	middlePart();
	arm();
	arm2();
	leg1();
	leg2();
}

void stickman2(void)
{
	void arm(void);
	void arm3(void);
	void middlePart(void);
	void face(void);
	void leg1(void);
	void leg2(void);


	face();
	middlePart();
	arm();
	arm3();
	leg1();
	leg2();
}

void arm(void) {
	//setPolygoan(although filled polygoan mode hence not needed)
	//glLoadIdentity();
	//gluLookAt(camera.position.x, camera.position.y, camera.position.z, camera.front.x + camera.position.x, camera.front.y + camera.position.y, camera.front.z + camera.position.z, camera.up.x, camera.up.y, camera.up.z);
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTranslatef(0.45f, 1.8f, -12.0f);//do initial translation for better visiblity
	glPushMatrix();


	//_______________________________________________________________________//
	glColor3f(0.2f, 0.0f, 0.1f);
	gluSphere(quadric, 0.2, 10, 10);
	glRotatef(-(GLfloat)shoulder - 180.0f, 1.0f, 0.0f, 0.0f);//z axis in the x axis
	glPushMatrix();


	//_______________________________ draw arm __________________________________//
	glColor3f(0.2f, 0.0f, 0.1f);
	glRotatef(90, 0, 0, 1);
	gluCylinder(quadric, 0.2, 0.28, 2, 10, 10);
	glPopMatrix();


	//do transformation for forarm
	glTranslatef(0.0f, 0.0f, 2.0f);
	gluSphere(quadric, 0.3, 10, 10);
	glRotatef((GLfloat)elbow, 1.0f, 0.0f, 0.0f);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	gluCylinder(quadric, 0.3, 0.2, 1.0, 10, 10);
	//wrist
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluSphere(quadric, 0.2, 10, 10);
	gluCylinder(quadric, 0.2, 0.35, 0.5, 50, 50);

	//hand
	glScalef(1.0f, 1.0f, 1.2f);
	glTranslatef(0.0f, 0.0f, 0.4f);
	gluSphere(quadric, 0.35, 10, 10);


	glPopMatrix();

	glPopMatrix();
}

void arm2(void) {

	//setPolygoan(although filled polygoan mode hence not needed)
	//glLoadIdentity();
	//gluLookAt(camera.position.x, camera.position.y, camera.position.z, camera.front.x + camera.position.x, camera.front.y + camera.position.y, camera.front.z + camera.position.z, camera.up.x, camera.up.y, camera.up.z);
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTranslatef(-0.45f, 1.8f, -12.0f);//do initial translation for better visiblity
	glPushMatrix();

	//_______________________________________________________________________//
	glColor3f(0.2f, 0.0f, 0.1f);
	gluSphere(quadric, 0.2, 10, 10);
	glRotatef(60, 1.0f, 0.0f, 0.0f);//z axis in the x axis
	glPushMatrix();


	//_______________________________ draw arm __________________________________//
	glColor3f(0.2f, 0.0f, 0.1f);
	glRotatef(-90, 0, 0, 1);
	gluCylinder(quadric, 0.2, 0.28, 2, 10, 10);
	glPopMatrix();


	//do transformation for forarm
	glTranslatef(0.0f, 0.0f, 2.0f);
	gluSphere(quadric, 0.3, 10, 10);
	glRotatef(-110, 1.0f, 0.0f, 0.0f);
	glRotatef(-90, 0.0f, 0.0f, 1.0f);
	gluCylinder(quadric, 0.3, 0.2, 1.0, 10, 10);
	//wrist
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluSphere(quadric, 0.2, 10, 10);
	gluCylinder(quadric, 0.2, 0.35, 0.5, 50, 50);

	//hand
	glScalef(1.0f, 1.0f, 1.2f);
	glTranslatef(0.0f, 0.0f, 0.4f);
	gluSphere(quadric, 0.35, 10, 10);

	//Axe
	glColor3f(0.2f, 0.2f, 0.2f);
	glRotatef(-105.0f, 0.0f, 1.0f, 0.0f);
	glScalef(0.1f, 0.1f, 5.0f);
	gluSphere(quadric, 1.0f, 10, 10);


	glPopMatrix();
	glPopMatrix();
}

void arm3(void) {

	//setPolygoan(although filled polygoan mode hence not needed)
	//glLoadIdentity();
	//gluLookAt(camera.position.x, camera.position.y, camera.position.z, camera.front.x + camera.position.x, camera.front.y + camera.position.y, camera.front.z + camera.position.z, camera.up.x, camera.up.y, camera.up.z);
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTranslatef(-0.45f, 1.8f, -12.0f);//do initial translation for better visiblity
	glPushMatrix();

	//_______________________________________________________________________//
	glColor3f(0.2f, 0.0f, 0.1f);
	gluSphere(quadric, 0.2, 10, 10);
	glRotatef(60, 1.0f, 0.0f, 0.0f);//z axis in the x axis
	glPushMatrix();


	//_______________________________ draw arm __________________________________//
	glColor3f(0.2f, 0.0f, 0.1f);
	glRotatef(-90, 0, 0, 1);
	gluCylinder(quadric, 0.2, 0.28, 2, 10, 10);
	glPopMatrix();


	//do transformation for forarm
	glTranslatef(0.0f, 0.0f, 2.0f);
	gluSphere(quadric, 0.3, 10, 10);
	glRotatef(-110, 1.0f, 0.0f, 0.0f);
	glRotatef(-90, 0.0f, 0.0f, 1.0f);
	gluCylinder(quadric, 0.3, 0.2, 1.0, 10, 10);
	//wrist
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluSphere(quadric, 0.2, 10, 10);
	gluCylinder(quadric, 0.2, 0.35, 0.5, 50, 50);

	//hand
	glScalef(1.0f, 1.0f, 1.2f);
	glTranslatef(0.0f, 0.0f, 0.4f);
	gluSphere(quadric, 0.35, 10, 10);

	//Axe Handle
	glPushMatrix();
	glColor3f(0.2f, 0.2f, 0.2f);
	glTranslatef(-2.0f, 0.0f, -0.4f);
	glRotatef(-105.0f, 0.0f, 1.0f, 0.0f);
	glScalef(0.15f, 0.15f, 4.0f);
	gluSphere(quadric, 1.0f, 10, 10);
	glPopMatrix();
	//Axe 
	glTranslatef(-4.2f, 0.0f, -1.4f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glScalef(1.0f, 0.5f, 1.0f);

	glColor3f(0.2f, 0.2f, 0.2f);
	gluDisk(quadric, 0.0f, 1.0f, 4, 4);



	glPopMatrix();
	glPopMatrix();
}

void middlePart(void)
{
	glPushMatrix();
	glTranslatef(0.0f, 1.95f, -12.0f);
	glColor3f(0.2f, 0.2f, 0.2f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadric, 0.3f, 0.3f, 1.25f, 10, 10);

	gluSphere(quadric, 0.3f, 10, 10);
	glTranslatef(0.0f, 0.0f, 1.25f);
	gluSphere(quadric, 0.3f, 10, 10);
	gluCylinder(quadric, 0.2f, 0.2f, 1.25f, 10, 10);
	glTranslatef(0.0f, 0.0f, 1.25f);
	gluSphere(quadric, 0.3f, 10, 10);
	gluCylinder(quadric, 0.3f, 0.3f, 1.25f, 10, 10);
	glTranslatef(0.0f, 0.0f, 1.25f);
	gluSphere(quadric, 0.3f, 10, 10);


	glLoadIdentity();
	glPopMatrix();
}

void face(void) {
	//setPolygoan(although filled polygoan mode hence not needed)
	//glLoadIdentity();
	//gluLookAt(camera.position.x, camera.position.y, camera.position.z, camera.front.x + camera.position.x, camera.front.y + camera.position.y, camera.front.z + camera.position.z, camera.up.x, camera.up.y, camera.up.z);
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//do initial translation for better visiblity
	glTranslatef(0.0f, 3.0f, -12.0f);
	glScalef(2.0f, 2.0f, 2.0f);

	glPushMatrix();
	glColor3f(0.2f, 0.2f, 0.2f);
	gluSphere(quadric, 0.4, 50, 50);
	glPopMatrix();
	glPopMatrix();
}

void leg1(void) {
	//setPolygoan(although filled polygoan mode hence not needed)
	//glLoadIdentity();
	//gluLookAt(camera.position.x, camera.position.y, camera.position.z, camera.front.x + camera.position.x, camera.front.y + camera.position.y, camera.front.z + camera.position.z, camera.up.x, camera.up.y, camera.up.z);
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTranslatef(0.25f, -2.15f, -12.0f);//do initial translation for better visiblity
	glPushMatrix();


	//_______________________________________________________________________//
	glColor3f(0.2f, 0.0f, 0.1f);
	gluSphere(quadric, 0.25, 10, 10);
	glRotatef(10, 0.0f, 1.0f, 0.0f);//z axis in the x axis
	glRotatef((GLfloat)shoulder, 1.0f, 0.0f, 0.0f);
	glPushMatrix();


	//_______________________________ draw arm __________________________________//
	glColor3f(0.2f, 0.0f, 0.1f);
	glRotatef(90, 0, 0, 1);
	gluCylinder(quadric, 0.25, 0.32, 2.5, 10, 10);
	glPopMatrix();


	//do transformation for forarm
	glTranslatef(0.0f, 0.0f, 2.5f);
	gluSphere(quadric, 0.35, 10, 10);
	glRotatef(-(GLfloat)elbow, 1.0f, 0.0f, 0.0f);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	gluCylinder(quadric, 0.35, 0.25, 1.5, 10, 10);
	//wrist
	glTranslatef(0.0f, 0.0f, 1.5f);
	glColor3f(0.5f, 0.5f, 0.5f);
	gluSphere(quadric, 0.3, 10, 10);
	glRotatef(85.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadric, 0.3, 0.35, 1.0, 10, 10);

	//hand
	glScalef(1.0f, 1.0f, 1.2f);
	glTranslatef(0.0f, 0.0f, 0.9f);
	gluSphere(quadric, 0.35, 10, 10);

	glPopMatrix();
	glPopMatrix();
}

void leg2(void) {
	//setPolygoan(although filled polygoan mode hence not needed)
	//glLoadIdentity();
	//gluLookAt(camera.position.x, camera.position.y, camera.position.z, camera.front.x + camera.position.x, camera.front.y + camera.position.y, camera.front.z + camera.position.z, camera.up.x, camera.up.y, camera.up.z);
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTranslatef(-0.25f, -2.15f, -12.0f);//do initial translation for better visiblity
	glPushMatrix();


	//_______________________________________________________________________//
	glColor3f(0.2f, 0.0f, 0.1f);
	gluSphere(quadric, 0.25, 10, 10);
	glRotatef(-10, 0.0f, 1.0f, 0.0f);//z axis in the x axis
	glRotatef(-(GLfloat)shoulder, 1.0f, 0.0f, 0.0f);//z axis in the x axis
	glPushMatrix();


	//_______________________________ draw arm __________________________________//
	glColor3f(0.2f, 0.0f, 0.1f);
	glRotatef(-180, 0, 1, 0);
	gluCylinder(quadric, 0.25, 0.32, 2.5, 10, 10);
	glPopMatrix();


	//do transformation for forarm
	glTranslatef(0.0f, 0.0f, -2.5f);
	gluSphere(quadric, 0.35, 10, 10);
	glRotatef(-(GLfloat)elbow1, 1.0f, 0.0f, 0.0f);
	glRotatef(-180, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadric, 0.35, 0.25, 1.5, 10, 10);
	//wrist
	glTranslatef(0.0f, 0.0f, 1.5f);
	glColor3f(0.5f, 0.5f, 0.5f);
	gluSphere(quadric, 0.3, 10, 10);
	glRotatef(85.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadric, 0.3, 0.35, 1.0, 10, 10);

	//hand
	glScalef(1.0f, 1.0f, 1.2f);
	glTranslatef(0.0f, 0.0f, 0.9f);
	gluSphere(quadric, 0.35, 10, 10);

	glPopMatrix();
	glPopMatrix();


}

void stickman1Update(void)
{
	if (shoulder <= 115.0f && bShoulder)
	{
		shoulder = shoulder + 15.0f * deltaTime;
		elbow1 = elbow1 + 15.0f * deltaTime;
		elbow = elbow - 15.0f * deltaTime;

	}
	else if (shoulder >= 65.0f)
	{
		bShoulder = FALSE;
		shoulder = shoulder - 15.0f * deltaTime;
		elbow1 = elbow1 - 15.0f * deltaTime;
		elbow = elbow + 15.0f * deltaTime;
	}
	else
	{
		bShoulder = TRUE;
	}


	if (elbow <= 0.0f && bElbow)
	{
		//elbow = elbow + 0.3;
	}
	else if (elbow >= -30.0f)
	{
		bElbow = FALSE;
		//elbow = elbow - 0.3f;
	}
	else
	{
		bElbow = TRUE;
	}
}

void drawBird(void)
{
	glPushMatrix();//0th push
	glPushMatrix();//1st push
	glPushMatrix();//2nd push
	glPushMatrix();//3rd push
	glPushMatrix();//4th push
	glPushMatrix();//5th push
	glPushMatrix();//6th push

	//Mid Body 
	{
		glScalef(3.0f, 1.0f, 1.0f);
		glColor3f(0.1, 0.1, 0.1);
		gluSphere(quadric, 1.0f, 18, 10);
		glPopMatrix();//0rd pop
	}

	//Head Plus Choch
	{

		glTranslatef(-3.45f, 0.0f, 0.0f);
		glScalef(1.5f, 0.8f, 1.0f);
		glPushMatrix();
		glColor3f(0.1, 0.1, 0.1);
		gluSphere(quadric, 0.6f, 12, 12);

		glScalef(0.25f, 0.25f, 1.2f);
		glColor3f(0.5, 0.1, 0.1);
		gluSphere(quadric, 0.6f, 12, 12);
		glPopMatrix();

		glTranslatef(-0.9f, -0.2f, 0.0f);
		glRotatef(20.0f, 0.0f, 0.0f, 1.0f);
		glScalef(1.5f, 0.4f, 0.6f);
		glColor3f(0.15, 0.15, 0.15);
		gluSphere(quadric, 0.6f, 12, 12);
		glPopMatrix();//1st pop
	}

	//Back Side
	{
		glTranslatef(3.45f, 0.0f, 0.0f);
		glScalef(5.0f, 0.25f, 1.0f);
		glColor3f(0.1, 0.1, 0.1);
		gluSphere(quadric, 0.6f, 6, 6);
		glPopMatrix();//2nd pop
	}


	//Leg Code
	{
		glTranslatef(2.5f, -0.5f, 0.3f);
		glRotatef(85.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.08f, 1.0f, 0.08f);
		glColor3f(0.1, 0.1, 0.1);
		gluSphere(quadric, 1.0f, 20, 20);
		glPopMatrix();//3rd pop

		glTranslatef(2.5f, -0.5f, -0.3f);
		glRotatef(85.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.08f, 1.0f, 0.08f);
		glColor3f(0.1, 0.1, 0.1);
		gluSphere(quadric, 1.0f, 20, 20);
		glPopMatrix();//4nd pop
	}



	//Right side wing
	{
		glTranslatef(0.0f, 0.0f, 1.0f);
		glRotatef(RotWingOne, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, 0.5f);
		glPushMatrix();//first wing movement

		glScalef(2.0f, 0.3f, 1.0f);
		//glColor3f(0.396, 0.408, 0.596);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glColor3f(0.1, 0.1, 0.1);
		gluSphere(quadric, 1.0f, 8, 8);
		glPopMatrix();

		glTranslatef(0.0f, 0.0f, 1.0f);
		glRotatef(RotWingTwo, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, 0.8f);
		glPushMatrix();//2nd wing movement

		glScalef(1.5f, 0.2f, 0.8f);
		//glColor3f(0.396, 0.408, 0.596);
		glColor3f(0.1, 0.1, 0.1);
		gluSphere(quadric, 1.0f, 8, 8);
		glPopMatrix();

		glTranslatef(0.0f, 0.0f, 0.8f);
		glRotatef(RotWingThree, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, 0.6f);

		glScalef(1.0f, 0.1f, 0.6f);
		//glColor3f(0.396, 0.408, 0.596);
		glColor3f(0.1, 0.1, 0.1);
		gluSphere(quadric, 1.0f, 8, 8);

		glPopMatrix();//Right Wing Completed Here 5st pop
	}



	//Left Side Wing
	{
		glScalef(3.0f, 1.0f, -1.0f);
		gluSphere(quadric, 1.0f, 18, 10);
		glPopMatrix();//6th pop

		glTranslatef(0.0f, 0.0f, -1.0f);
		glRotatef(-RotWingOne, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -0.5f);
		glPushMatrix();//first wing movement

		glScalef(2.0f, 0.3f, 1.0f);
		//glColor3f(0.396, 0.408, 0.596);
		glColor3f(0.1, 0.1, 0.1);
		gluSphere(quadric, 1.0f, 8, 8);
		glPopMatrix();

		glTranslatef(0.0f, 0.0f, -1.0f);
		glRotatef(-RotWingTwo, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -0.8f);
		glPushMatrix();//2nd wing movement

		glScalef(1.5f, 0.2f, 0.8f);
		//glColor3f(0.396, 0.408, 0.596);
		glColor3f(0.1, 0.1, 0.1);
		gluSphere(quadric, 1.0f, 8, 8);
		glPopMatrix();

		glTranslatef(0.0f, 0.0f, -0.8f);
		glRotatef(-RotWingThree, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -0.6f);

		glScalef(1.0f, 0.1f, 0.6f);
		//glColor3f(0.396, 0.408, 0.596);
		glColor3f(0.1, 0.1, 0.1);
		gluSphere(quadric, 1.0f, 8, 8);
		//glPopMatrix();//Right Wing Completed Here
	}
}

void drawBird2(void)
{
	glPushMatrix();//0th push
	glPushMatrix();//1st push
	glPushMatrix();//2nd push
	glPushMatrix();//3rd push
	glPushMatrix();//4th push
	glPushMatrix();//5th push
	glPushMatrix();//6th push

	//Mid Body 
	{
		glScalef(3.0f, 1.0f, 1.0f);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(quadric, 1.0f, 18, 10);
		glPopMatrix();//0rd pop
	}

	//Head Plus Choch
	{

		glTranslatef(-3.45f, 0.0f, 0.0f);
		glScalef(1.5f, 0.8f, 1.0f);
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(quadric, 0.6f, 12, 12);

		glScalef(0.25f, 0.25f, 1.2f);
		glColor3f(0.5, 1.0, 1.0);
		gluSphere(quadric, 0.6f, 12, 12);
		glPopMatrix();

		glTranslatef(-0.9f, -0.2f, 0.0f);
		glRotatef(20.0f, 0.0f, 0.0f, 1.0f);
		glScalef(1.5f, 0.4f, 0.6f);
		glColor3f(1.05, 1.05, 1.05);
		gluSphere(quadric, 0.6f, 12, 12);
		glPopMatrix();//1st pop
	}

	//Back Side
	{
		glTranslatef(3.45f, 0.0f, 0.0f);
		glScalef(5.0f, 0.25f, 1.0f);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(quadric, 0.6f, 6, 6);
		glPopMatrix();//2nd pop
	}


	//Leg Code
	{
		glTranslatef(2.5f, -0.5f, 0.3f);
		glRotatef(85.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.08f, 1.0f, 0.08f);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(quadric, 1.0f, 20, 20);
		glPopMatrix();//3rd pop

		glTranslatef(2.5f, -0.5f, -0.3f);
		glRotatef(85.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.08f, 1.0f, 0.08f);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(quadric, 1.0f, 20, 20);
		glPopMatrix();//4nd pop
	}



	//Right side wing
	{
		glTranslatef(0.0f, 0.0f, 1.0f);
		glRotatef(RotWingOne, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, 0.5f);
		glPushMatrix();//first wing movement

		glScalef(2.0f, 0.3f, 1.0f);
		//glColor3f(0.396, 0.408, 0.596);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(quadric, 1.0f, 8, 8);
		glPopMatrix();

		glTranslatef(0.0f, 0.0f, 1.0f);
		glRotatef(RotWingTwo, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, 0.8f);
		glPushMatrix();//2nd wing movement

		glScalef(1.5f, 0.2f, 0.8f);
		//glColor3f(0.396, 0.408, 0.596);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(quadric, 1.0f, 8, 8);
		glPopMatrix();

		glTranslatef(0.0f, 0.0f, 0.8f);
		glRotatef(RotWingThree, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, 0.6f);

		glScalef(1.0f, 0.1f, 0.6f);
		//glColor3f(0.396, 0.408, 0.596);
		glColor3f(1.0f, 1.0f, 1.0f);
		gluSphere(quadric, 1.0f, 8, 8);

		glPopMatrix();//Right Wing Completed Here 5st pop
	}



	//Left Side Wing
	{
		glScalef(3.0f, 1.0f, -1.0f);
		gluSphere(quadric, 1.0f, 18, 10);
		glPopMatrix();//6th pop

		glTranslatef(0.0f, 0.0f, -1.0f);
		glRotatef(-RotWingOne, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -0.5f);
		glPushMatrix();//first wing movement

		glScalef(2.0f, 0.3f, 1.0f);
		//glColor3f(0.396, 0.408, 0.596);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(quadric, 1.0f, 8, 8);
		glPopMatrix();

		glTranslatef(0.0f, 0.0f, -1.0f);
		glRotatef(-RotWingTwo, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -0.8f);
		glPushMatrix();//2nd wing movement

		glScalef(1.5f, 0.2f, 0.8f);
		//glColor3f(0.396, 0.408, 0.596);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(quadric, 1.0f, 8, 8);
		glPopMatrix();

		glTranslatef(0.0f, 0.0f, -0.8f);
		glRotatef(-RotWingThree, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -0.6f);

		glScalef(1.0f, 0.1f, 0.6f);
		//glColor3f(0.396, 0.408, 0.596);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(quadric, 1.0f, 8, 8);
		//glPopMatrix();//Right Wing Completed Here
	}
}

void updateBird(void)
{
	if (sinAngle <= 90.0f && bSinAngle)
	{
		sinAngle += 0.03f;
	}
	else if (sinAngle >= 0.0f)
	{
		bSinAngle = FALSE;
		sinAngle -= 0.03f;
	}
	else
	{
		bSinAngle = TRUE;
	}


	if (RotWingOne <= 60.0f && bBirdRot)
	{
		RotWingOne += 0.1f * 10.0f;
		RotWingTwo += 0.075f * 10.0f;
		RotWingThree += 0.05f * 10.0f;
	}
	else if (RotWingOne >= -60.0f)
	{
		bBirdRot = FALSE;
		RotWingOne -= 0.1f * 10.0f;
		RotWingTwo -= 0.075f * 10.0f;
		RotWingThree -= 0.05f * 10.0f;
	}
	else
	{
		bBirdRot = TRUE;
	}
}

void drawHoneyComb(void)
{
	glPushMatrix();
	glPushMatrix();

	glScalef(1.5f, 0.5f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.902, 0.565, 0.043);
	gluSphere(quadric, 1.0f, 30, 30);

	glTranslatef(0.0f, 0.0f, 0.5f);
	glColor3f(0.965, 0.816, 0.314);
	gluSphere(quadric, 0.9f, 30, 30);

	glTranslatef(0.0f, 0.0f, 0.5f);
	glColor3f(0.902, 0.565, 0.043);
	gluSphere(quadric, 0.75f, 30, 30);

	glTranslatef(0.0f, 0.0f, 0.5f);
	glColor3f(0.965, 0.816, 0.314);
	gluSphere(quadric, 0.6f, 30, 30);

	glTranslatef(0.0f, 0.0f, 0.5f);
	glColor3f(0.902, 0.565, 0.043);
	gluSphere(quadric, 0.4f, 30, 30);

	glTranslatef(0.0f, 0.0f, 0.4f);
	glColor3f(0.965, 0.816, 0.314);
	gluSphere(quadric, 0.15f, 30, 30);

	glPopMatrix();

	glScalef(1.5f, 0.5f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.902, 0.565, 0.043);
	gluSphere(quadric, 1.0f, 30, 30);

	glTranslatef(0.0f, 0.0f, -0.5f);
	glColor3f(0.965, 0.816, 0.314);
	gluSphere(quadric, 0.9f, 30, 30);

	glTranslatef(0.0f, 0.0f, -0.5f);
	glColor3f(0.902, 0.565, 0.043);
	gluSphere(quadric, 0.75f, 30, 30);

	glTranslatef(0.0f, 0.0f, -0.5f);
	glColor3f(0.965, 0.816, 0.314);
	gluSphere(quadric, 0.6f, 30, 30);

	glTranslatef(0.0f, 0.0f, -0.5f);
	glColor3f(0.902, 0.565, 0.043);
	gluSphere(quadric, 0.4f, 30, 30);

	glTranslatef(0.0f, 0.0f, -0.4f);
	glColor3f(0.965, 0.816, 0.314);
	gluSphere(quadric, 0.2f, 30, 30);


	glTranslatef(0.0f, 0.0f, -0.6f);
	glScalef(0.2f, 0.2f, 6.0f);
	glColor3f(0.204, 0.055, 0.016);
	gluSphere(quadric, 0.2f, 30, 30);
	glPopMatrix();


	glTranslatef(0.0f, 0.0f, -1.0f);
	gluCylinder(quadric, 0.25f, 0.25f, 2.0f, 30, 30);
	glColor3f(0.251, 0.153, 0.035);
	gluDisk(quadric, 0.0f, 0.25f, 30, 30);

	glTranslatef(0.0f, 0.0f, 2.0f);
	glColor3f(0.251, 0.153, 0.035);
	gluDisk(quadric, 0.0f, 0.25f, 30, 30);
}

void drawLeaf(void)
{

	glRotatef(-10.0f, 1.0f, 0.0f, 0.0f);
	glScalef(0.6f, 1.0f, 0.12f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.3, 0.3, 0.3);
	gluSphere(quadric, 1.0f, 4, 10);

}

void drawLeafs(void)
{
	glPushMatrix();
	drawLeaf();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.9f, 1.5f, 0.0f);
	glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
	drawLeaf();
	glPopMatrix();

	glTranslatef(-0.9f, 1.5f, 0.0f);
	glRotatef(240.0f, 0.0f, 0.0f, 1.0f);
	drawLeaf();
}

void drawLeafBunch(void)
{
	if (colorChange)
	{
		glBindTexture(GL_TEXTURE_2D, texture_treeLeaf1);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture_treeLeaf);
	}

	for (int i = 0; i < 30; i++)
	{
		glPushMatrix();
		glRotatef(i * 12, 0.0f, 0.0f, 1.0f);
		glTranslatef(15.0f, 0.0f, 0.0f);
		glRotatef(i * 12 * sin(i), 0.0f, 0.0f, 1.0f);
		glScalef(2.0f, 2.0f, 2.0f);
		drawLeafs();
		glPopMatrix();
	}

	for (int i = 0; i < 30; i++)
	{
		glPushMatrix();
		glRotatef(i * 12 + 5, 0.0f, 0.0f, 1.0f);
		glTranslatef(12.0f, 0.0f, -1.0f);
		glRotatef(i * 12 * sin(i), 0.0f, 0.0f, 1.0f);
		glScalef(1.8f, 1.8f, 1.8f);
		drawLeafs();
		glPopMatrix();
	}

	for (int i = 0; i < 24; i++)
	{
		glPushMatrix();
		glRotatef(i * 15 + 4, 0.0f, 0.0f, 1.0f);
		glTranslatef(9.0f, 0.0f, -2.0f);
		glRotatef(i * 15 * sin(i), 0.0f, 0.0f, 1.0f);
		glScalef(1.6f, 1.6f, 1.6f);
		drawLeafs();
		glPopMatrix();
	}

	for (int i = 0; i < 20; i++)
	{
		glPushMatrix();
		glRotatef(i * 18 + 3, 0.0f, 0.0f, 1.0f);
		glTranslatef(5.0f, 0.0f, -3.0f);
		glRotatef(i * 18 * sin(i), 0.0f, 0.0f, 1.0f);
		glScalef(1.4f, 1.4f, 1.4f);
		drawLeafs();
		glPopMatrix();
	}

	for (int i = 0; i < 3; i++)
	{
		glPushMatrix();
		glRotatef(i * 120 + 2, 0.0f, 0.0f, 1.0f);
		glTranslatef(2.0f, 0.0f, -4.0f);
		glRotatef(i * 120 * sin(i), 0.0f, 0.0f, 1.0f);
		glScalef(1.2f, 1.2f, 1.2f);
		drawLeafs();
		glPopMatrix();
	}

	glPushMatrix();
	drawLeafs();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void drawTree(void)
{
	glBindTexture(GL_TEXTURE_2D, texture_treeRoots);
	gluSphere(quadric, 4.0f, 10, 30);
	glBindTexture(GL_TEXTURE_2D, 0);

	//1st trunk
	glBindTexture(GL_TEXTURE_2D, texture_treeTrunk1);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadric, 3.0f, 2.5f, 10.0f, 10, 30);


	//2st trunk
	glTranslated(0.0f, 0.0f, 10.0f);
	gluSphere(quadric, 2.5f, 30, 30);

	glRotatef(-20.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadric, 2.5f, 2.0f, 8.0f, 10, 30);

	//3st trunk
	glTranslated(0.0f, 0.0f, 8.0f);
	gluSphere(quadric, 2.0f, 10, 30);

	glRotatef(-10.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadric, 2.0f, 1.5f, 10.0f, 10, 30);

	//4st trunk
	glTranslated(0.0f, 0.0f, 10.0f);
	gluSphere(quadric, 1.5f, 10, 30);

	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadric, 1.5f, 1.0f, 5.0f, 10, 30);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Leaf Trunks
	glTranslated(0.0f, 0.0f, 5.0f);
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();

	gluSphere(quadric, 1.0f, 10, 30);

	//1st Leaf Trunk
	glPushMatrix();
	glRotatef(60.0f, 0.0f, 1.0f, 0.0f);
	glColor3f(0.3, 0.2, 0.1);
	gluCylinder(quadric, 1.0f, 0.5f, 15.0f, 20, 30);
	glPopMatrix();

	glTranslatef(12.0f, 0.0f, 5.0f);
	glRotatef(-180.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-20.0f, 0.0f, 1.0f, 0.0f);
	drawLeafBunch();
	glPopMatrix();

	//2st Leaf Trunk
	glPushMatrix();
	glRotatef(-60.0f, 0.0f, 1.0f, 0.0f);
	glColor3f(0.3, 0.2, 0.1);
	gluCylinder(quadric, 1.0f, 0.5f, 20.0f, 20, 30);
	glPopMatrix();

	glTranslatef(-16.0f, 0.0f, 10.0f);
	glRotatef(-180.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	drawLeafBunch();
	glPopMatrix();

	//3st Leaf Trunk
	glPushMatrix();
	glRotatef(60.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.3, 0.2, 0.1);
	gluCylinder(quadric, 1.0f, 0.5f, 15.0f, 20, 30);
	glPopMatrix();

	glTranslatef(0.0f, -12.0f, 5.0f);
	glRotatef(-180.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(20.0f, 1.0f, 00.0f, 0.0f);
	drawLeafBunch();
	glPopMatrix();

	//4st Leaf Trunk
	glPushMatrix();
	glRotatef(-60.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.3, 0.2, 0.1);
	gluCylinder(quadric, 1.0f, 0.5f, 20.0f, 20, 30);
	glPopMatrix();

	glTranslatef(0.0f, 16.0f, 10.0f);
	glRotatef(-180.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
	drawLeafBunch();
	glPopMatrix();

	//5st Leaf Trunk
	glPushMatrix();
	glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.3, 0.2, 0.1);
	gluCylinder(quadric, 1.0f, 0.5f, 23.0f, 20, 30);
	glPopMatrix();

	glTranslatef(0.0f, -6.0f, 20.0f);
	glRotatef(-180.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(10.0f, 1.0f, 00.0f, 0.0f);
	drawLeafBunch();
}

void drawCutedTree(void)
{
	glPushMatrix();//Todalele khod
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, texture_treeRoots);
	gluDisk(quadric, 0.0f, 7.0f, 10, 10);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, texture_treeTrunk1);
	gluCylinder(quadric, 3.0f, 3.0f, 7.0f, 30, 30);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, texture_treeTrunk);
	glTranslatef(0.0f, 0.0f, 7.0f);
	gluDisk(quadric, 0.0f, 3.0f, 30, 30);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();


	//1st trunk
	if (bPlantFall)
	{
		glRotatef(plantFallZ + xxx, 0.0f, 0.0f, 1.0f);
	}
	glRotatef(-10.0f, 0.0f, 0.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texture_treeTrunk1);
	glTranslatef(-10.0f, 3.0f, -10.0f);
	glRotatef(-100.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadric, 3.0f, 2.5f, 10.0f, 10, 30);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, texture_treeTrunk);
	gluDisk(quadric, 0.0f, 3.0f, 10, 30);
	glBindTexture(GL_TEXTURE_2D, 0);


	//2st trunk
	glBindTexture(GL_TEXTURE_2D, texture_treeTrunk1);
	glTranslated(0.0f, 0.0f, 10.0f);
	gluSphere(quadric, 2.5f, 30, 30);

	glRotatef(-20.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadric, 2.5f, 2.0f, 8.0f, 10, 30);

	//3st trunk
	glTranslated(0.0f, 0.0f, 8.0f);
	gluSphere(quadric, 2.0f, 10, 30);

	glRotatef(-10.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadric, 2.0f, 1.5f, 10.0f, 10, 30);

	//4st trunk
	glTranslated(0.0f, 0.0f, 10.0f);
	gluSphere(quadric, 1.5f, 10, 30);

	glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadric, 1.5f, 1.0f, 5.0f, 10, 30);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Leaf Trunks
	glTranslated(0.0f, 0.0f, 5.0f);
	gluSphere(quadric, 1.0f, 10, 30);

	//1st Leaf Trunk
	glPushMatrix();
	glRotatef(60.0f, 0.0f, 1.0f, 0.0f);
	glColor3f(0.15, 0.1, 0.1);
	gluCylinder(quadric, 1.0f, 0.5f, 15.0f, 20, 30);
	glPopMatrix();

	//2st Leaf Trunk
	glPushMatrix();
	glRotatef(-60.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadric, 1.0f, 0.5f, 20.0f, 20, 30);
	glPopMatrix();

	//3st Leaf Trunk
	glPushMatrix();
	glRotatef(60.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadric, 1.0f, 0.5f, 15.0f, 20, 30);
	glPopMatrix();

	//4st Leaf Trunk
	glPushMatrix();
	glRotatef(-60.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadric, 1.0f, 0.5f, 20.0f, 20, 30);
	glPopMatrix();

	//5st Leaf Trunk
	glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadric, 1.0f, 0.5f, 23.0f, 20, 30);
}

void initializeTerrain(void)
{
	float perlin(float, float);

	float colorX1;
	float colorY1;
	float colorZ1;
				 
	float colorX2;
	float colorY2;
	float colorZ2;
				 
	for (int j = TERRAIN_LENGTH / 2; j > -TERRAIN_LENGTH / 2; j--)
	{
		for (int i = -TERRAIN_WIDTH / 2; i < TERRAIN_WIDTH / 2; i++)
		{
			//RIGHT TOP
			{
				colorIntensity = ((((perlin((i * pixSize) + pixSize, (j * pixSize)) / terrainHeight) + 0.5f)) + 0.3f);

				if (colorIntensity >= 0.5f)
				{
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][12] = colorX1 * colorIntensity;
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][13] = colorY1 * colorIntensity;
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][14] = colorZ1 * colorIntensity;
				}
				else
				{
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][12] = colorX2 * colorIntensity;
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][13] = colorY2 * colorIntensity;
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][14] = colorZ2 * colorIntensity;
				}
				terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][0] = (i * pixSize) + pixSize;
				terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][1] = (j * pixSize);
				terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][2] = perlin(((i * pixSize) + pixSize), (j * pixSize));
			}


			//LEFT TOP
			{
				colorIntensity = (((perlin((i * pixSize), (j * pixSize)) / terrainHeight) + 0.5f)) + 0.3f;
				if (colorIntensity >= 0.5f)
				{
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][15] = colorX1 * colorIntensity;
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][16] = colorY1 * colorIntensity;
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][17] = colorZ1 * colorIntensity;
				}
				else
				{
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][15] = colorX2 * colorIntensity;
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][16] = colorY2 * colorIntensity;
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][17] = colorZ2 * colorIntensity;
				}
				terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][3] = (i * pixSize);
				terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][4] = (j * pixSize);
				terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][5] = perlin((i * pixSize), (j * pixSize));
			}


			//LEFT BOTTOM
			{
				colorIntensity = (((perlin((i * pixSize), (j * pixSize) - pixSize) / terrainHeight) + 0.5f)) + 0.3f;
				if (colorIntensity >= 0.5f)
				{
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][18] = colorX1 * colorIntensity;
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][19] = colorY1 * colorIntensity;
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][20] = colorZ1 * colorIntensity;
				}
				else
				{
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][18] = colorX2 * colorIntensity;
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][19] = colorY2 * colorIntensity;
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][20] = colorZ2 * colorIntensity;
				}
				terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][6] = (i * pixSize);
				terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][7] = (j * pixSize) - pixSize;
				terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][8] = perlin((i * pixSize), ((j * pixSize) - pixSize));
			}


			//RIGHT BOTTOM
			{
				colorIntensity = (((perlin((i * pixSize) + pixSize, (j * pixSize) - pixSize) / terrainHeight) + 0.5f)) + 0.3f;
				if (colorIntensity >= 0.5f)
				{
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][21] = colorX1 * colorIntensity;
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][22] = colorY1 * colorIntensity;
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][23] = colorZ1 * colorIntensity;
				}
				else
				{
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][21] = colorX2 * colorIntensity;
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][22] = colorY2 * colorIntensity;
					terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][23] = colorZ2 * colorIntensity;
				}
				terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][9] = (pixSize * i) + pixSize;
				terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][10] = (pixSize * j) - pixSize;
				terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][11] = perlin(((i * pixSize) + pixSize), ((j * pixSize) - pixSize));
			}
		}
	}
}

void initializeWater(void)
{
	float perlin(float, float);

	/*float colorX2 = waterHighColor.x;
	float colorY2 = waterHighColor.y;
	float colorZ2 = waterHighColor.z;

	float colorX1 = waterLowColor.x;
	float colorY1 = waterLowColor.y;
	float colorZ1 = waterLowColor.z;*/

	float colorX1 = 0.569 + 1.2f;
	float colorY1 = 0.847 + 1.2f;
	float colorZ1 = 0.925 + 1.5f;

	float colorX2 = 0.039;
	float colorY2 = 0.463; 
	float colorZ2 = 0.745;

	float colorX3 = 0.035; 
	float colorY3 = 0.357;
	float colorZ3 = 0.584;

	float colorX4 = 0.024;  
	float colorY4 = 0.282;
	float colorZ4 = 0.502;

	GLfloat perlinResult = 0.0f;

	/*Vec3f a = { (water[-64 + (LEN)][64 + (LEN)][0] - water[-64 + (LEN)][64 + (LEN)][3]), (water[-64 + (LEN)][64 + (LEN)][1] - water[-64 + (LEN)][64 + (LEN)][4]), (water[-64 + (LEN)][64 + (LEN)][2] - water[-64 + (LEN)][64 + (LEN)][5]) };

	Vec3f b = { (water[-64 + (LEN)][64 + (LEN)][6] - water[-64 + (LEN)][64 + (LEN)][3]), (water[-64 + (LEN)][64 + (LEN)][7] - water[-64 + (LEN)][64 + (LEN)][4]), (water[-64 + (LEN)][64 + (LEN)][8] - water[-64 + (LEN)][64 + (LEN)][5]) };

	Vec3f c = cross(a, b);

	glNormal3f(-c.x, -c.y, -c.z);*/

	float s = 0.0f;
	float t = 0.0f;

	glPushMatrix();

	///1st Texture
	{
		glColor4f(1.0f, 1.0f, 1.0f, alphaSeaTexture);
		glBindTexture(GL_TEXTURE_2D, texture_seaWaves);

		if (alphaSeaTexture >= 0.0f && time >= 131.5f && time <= 145.0f)
			alphaSeaTexture -= 0.0025f;
		else if(time >= 145.0f)
			if(alphaSeaTexture <= 1.0f)
			alphaSeaTexture += 0.0025f;
	
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glTranslatef(time * 0.005, time * 0.005, 0.0);
		glMatrixMode(GL_MODELVIEW);

		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, -1.0f);
		for (int j = LEN; j >= -LEN; j--)
		{
			for (int i = -LEN; i <= LEN; i++)
			{

				perlinResult = water[i + LEN][j + LEN][0];
				s = 64.0f + i;
				t = j + 64.0f;
				glTexCoord2f((s + 1.0) / 128.0, (t) / 128.0);
				glVertex3f((i * pixSize) + pixSize, (j * pixSize), (0.02 * sin(holdVal)) + (0.1 * sin(time * 2.0) * perlinResult));///Right TOP 

				perlinResult = water[i + LEN][j + LEN][1];
				glTexCoord2f((s / 128.0), t / 128.0);
				glVertex3f((i * pixSize), (j * pixSize), (0.02 * sin(holdVal)) + (0.1 * sin(time * 2.0) * perlinResult));///LEFT TOP aaa

				perlinResult = water[i + LEN][j + LEN][2];
				glTexCoord2f((s / 128.0), (t - 1.0) / 128.0);
				glVertex3f((i * pixSize), (j * pixSize) - pixSize, (0.02 * sin(waterWave)) + (0.1 * sin(time * 2.0) * perlinResult));///LEFT BOTTOM 


				perlinResult = water[i + LEN][j + LEN][3];
				glTexCoord2f((s + 1.0) / 128.0, (t - 1.0) / 128.0);
				glVertex3f((pixSize * i) + pixSize, (pixSize * j) - pixSize, (0.02 * sin(waterWave)) + (0.1 * sin(time * 2.0) * perlinResult));///Right Bottom 
			}
			holdVal = waterWave;
			waterWave += (2.25);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
	}
	/*if (bTexSwitchSea == TRUE)
	{
		glColor4f(1.0, 1.0, 1.0, alphaSeaTexture);
		glBindTexture(GL_TEXTURE_2D, texture_seaWaves);

		if (alphaSeaTexture >= 0.0)
			alphaSeaTexture -= 0.001f;

		bTexSwitchSea = FALSE;
		bTexSwitchLava = TRUE;
	}
	
	if(bTexSwitchLava == TRUE)
	{
		glColor4f(1.0, 1.0, 1.0, alphaSeaLavaTexture);
		glBindTexture(GL_TEXTURE_2D, texture_seaLavaWaves);
		
		if (alphaSeaLavaTexture <= 1.0)
			alphaSeaLavaTexture += 0.001f;

		bTexSwitchLava = FALSE;
		bTexSwitchSea = TRUE;
	}*/
	
	glPopMatrix();

	///2nd Texture
	{
		glTranslatef(0.0, 0.0, -0.0025f - 0.03f);
		//glScalef(0.95, 0.95, 0.95);
		glColor4f(1.0, 1.0, 1.0, alphaSeaLavaTexture);
		glBindTexture(GL_TEXTURE_2D, texture_seaLavaWaves);

		if (alphaSeaLavaTexture <= 1.0 && time >= 131.5f && time <= 145.0) //128  // 144
			alphaSeaLavaTexture += 0.0025f;
		else if (time >= 145.0)
			if (alphaSeaLavaTexture >= 0.0)
				alphaSeaLavaTexture -= 0.003f;
		
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glTranslatef(time * 0.005f, time * 0.005f, 0.0);
		glMatrixMode(GL_MODELVIEW);

		glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, -1.0);
		for (int j = LEN; j >= -LEN; j--)
		{
			for (int i = -LEN; i <= LEN; i++)
			{

				perlinResult = water[i + LEN][j + LEN][0];
				s = 64.0 + i;
				t = j + 64.0;
				glTexCoord2f((s + 1.0) / 128.0, (t) / 128.0);
				glVertex3f((i * pixSize) + pixSize, (j * pixSize), (0.02 * sin(holdVal)) + (0.1 * sin(time * 2.0) * perlinResult));///Right TOP 

				perlinResult = water[i + LEN][j + LEN][1];
				glTexCoord2f((s / 128.0), t / 128.0);
				glVertex3f((i * pixSize), (j * pixSize), (0.02 * sin(holdVal)) + (0.1 * sin(time * 2.0) * perlinResult));///LEFT TOP aaa

				perlinResult = water[i + LEN][j + LEN][2];
				glTexCoord2f((s / 128.0), (t - 1.0) / 128.0);
				glVertex3f((i * pixSize), (j * pixSize) - pixSize, (0.02 * sin(waterWave)) + (0.1 * sin(time * 2.0) * perlinResult));///LEFT BOTTOM 


				perlinResult = water[i + LEN][j + LEN][3];
				glTexCoord2f((s + 1.0) / 128.0, (t - 1.0) / 128.0);
				glVertex3f((pixSize * i) + pixSize, (pixSize * j) - pixSize, (0.02 * sin(waterWave)) + (0.1 * sin(time * 2.0) * perlinResult));///Right Bottom 
			}
			holdVal = waterWave;
			waterWave += (2.25);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);


		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
	}

	///TEST
	{
		//glBegin(GL_QUADS);
		//glNormal3f(0.0f, 0.0f, -1.0f);
		//for (int j = LEN; j >= -LEN; j--)
		//{
		//	for (int i = -LEN; i <= LEN; i++)
		//	{

		//		perlinResult = water[i + LEN][j + LEN][0];
		//		//perlinResult = perlin((i * pixSize) + pixSize, (j * pixSize));
		//		colorIntensity = ((perlinResult / terrainHeight) + 0.7f) / 1.3f;
		//		if (colorIntensity <= 0.25f)
		//		{
		//			colorIntensity = colorIntensity * 4.0f;
		//			//glColor4f(lerpf(colorX1, colorX2,colorIntensity,FALSE), lerpf(colorY1, colorY2, colorIntensity, FALSE), lerpf(colorZ1, colorZ2, colorIntensity, FALSE), 0.85f);
		//		}
		//		else if (colorIntensity > 0.25f && colorIntensity <= 0.5f)
		//		{
		//			colorIntensity = (colorIntensity - 0.25f) * 4.0f;
		//			//glColor4f(lerpf(colorX2, colorX3, colorIntensity, FALSE), lerpf(colorY2, colorY3, colorIntensity, FALSE), lerpf(colorZ2, colorZ3, colorIntensity, FALSE), 0.85f);
		//		}
		//		else if (colorIntensity > 0.5f && colorIntensity <= 0.75f)
		//		{
		//			colorIntensity = (colorIntensity - 0.5f) * 4.0f;
		//			//glColor4f(lerpf(colorX3, colorX4, colorIntensity, FALSE), lerpf(colorY3, colorY4, colorIntensity, FALSE), lerpf(colorZ3, colorZ4, colorIntensity, FALSE), 0.85f);
		//		}
		//		else
		//		{
		//			colorIntensity = (colorIntensity - 0.75f) * 4.0f;
		//			//glColor4f(lerpf(colorX4, 0.25f, colorIntensity, FALSE), lerpf(colorY4, 0.5f, colorIntensity, FALSE), lerpf(colorZ4, 1.0f, colorIntensity, FALSE), 0.85f);
		//		}
		//		s = 64.0f + i;
		//		t = j + 64.0f;
		//		glTexCoord2f((s + 1.0f) / 128.0f, (t) / 128.0f);
		//		glVertex3f((i * pixSize) + pixSize, (j * pixSize), (0.02 * sin(holdVal)) + (1.5f * sin(time * 2.0f) * perlinResult));///Right TOP 

		//		perlinResult = water[i + LEN][j + LEN][1];
		//		//perlinResult = perlin((i * pixSize), (j * pixSize));
		//		colorIntensity = (((perlinResult / terrainHeight) + 0.7f)) / 1.3f;
		//		if (colorIntensity <= 0.25f)
		//		{
		//			colorIntensity = colorIntensity * 4.0f;
		//			//glColor4f(lerpf(colorX1, colorX2, colorIntensity, FALSE), lerpf(colorY1, colorY2, colorIntensity, FALSE), lerpf(colorZ1, colorZ2, colorIntensity, FALSE), 0.85f);
		//		}
		//		else if (colorIntensity > 0.25f && colorIntensity <= 0.5f)
		//		{
		//			colorIntensity = (colorIntensity - 0.25f) * 4.0f;
		//			//glColor4f(lerpf(colorX2, colorX3, colorIntensity, FALSE), lerpf(colorY2, colorY3, colorIntensity, FALSE), lerpf(colorZ2, colorZ3, colorIntensity, FALSE), 0.85f);
		//		}
		//		else if (colorIntensity > 0.5f && colorIntensity <= 0.75f)
		//		{
		//			colorIntensity = (colorIntensity - 0.5f) * 4.0f;
		//			//glColor4f(lerpf(colorX3, colorX4, colorIntensity, FALSE), lerpf(colorY3, colorY4, colorIntensity, FALSE), lerpf(colorZ3, colorZ4, colorIntensity, FALSE), 0.85f);
		//		}
		//		else
		//		{
		//			colorIntensity = (colorIntensity - 0.75f) * 4.0f;
		//			//glColor4f(lerpf(colorX4, 0.25f, colorIntensity, FALSE), lerpf(colorY4, 0.5f, colorIntensity, FALSE), lerpf(colorZ4, 1.0f, colorIntensity, FALSE), 0.85f);
		//		}
		//		glTexCoord2f((s / 128.0f), t / 128.0f);
		//		glVertex3f((i * pixSize), (j * pixSize), (0.02 * sin(holdVal)) + (1.5f * sin(time * 2.0f) * perlinResult));///LEFT TOP aaa

		//		perlinResult = water[i + LEN][j + LEN][2];
		//		//perlinResult = perlin((i * pixSize), (j * pixSize) - pixSize);
		//		colorIntensity = ((perlinResult / terrainHeight) + 0.7f) / 1.3f;
		//		if (colorIntensity <= 0.25f)
		//		{
		//			colorIntensity = colorIntensity * 4.0f;
		//			//glColor4f(lerpf(colorX1, colorX2, colorIntensity, FALSE), lerpf(colorY1, colorY2, colorIntensity, FALSE), lerpf(colorZ1, colorZ2, colorIntensity, FALSE), 0.85f);
		//		}
		//		else if (colorIntensity > 0.25f && colorIntensity <= 0.5f)
		//		{
		//			colorIntensity = (colorIntensity - 0.25f) * 4.0f;
		//			//glColor4f(lerpf(colorX2, colorX3, colorIntensity, FALSE), lerpf(colorY2, colorY3, colorIntensity, FALSE), lerpf(colorZ2, colorZ3, colorIntensity, FALSE), 0.85f);
		//		}
		//		else if (colorIntensity > 0.5f && colorIntensity <= 0.75f)
		//		{
		//			colorIntensity = (colorIntensity - 0.5f) * 4.0f;
		//			//glColor4f(lerpf(colorX3, colorX4, colorIntensity, FALSE), lerpf(colorY3, colorY4, colorIntensity, FALSE), lerpf(colorZ3, colorZ4, colorIntensity, FALSE), 0.85f);
		//		}
		//		else
		//		{
		//			colorIntensity = (colorIntensity - 0.75f) * 4.0f;
		//			//glColor4f(lerpf(colorX4, 0.25f, colorIntensity, FALSE), lerpf(colorY4, 0.5f, colorIntensity, FALSE), lerpf(colorZ4, 1.0f, colorIntensity, FALSE), 0.85f);
		//		}
		//		glTexCoord2f((s / 128.0f), (t - 1.0f) / 128.0f);
		//		glVertex3f((i * pixSize), (j * pixSize) - pixSize, (0.02 * sin(waterWave)) + (1.5f * sin(time * 2.0f) * perlinResult));///LEFT BOTTOM 


		//		perlinResult = water[i + LEN][j + LEN][3];
		//		//perlinResult = perlin((i * pixSize) + pixSize, (j * pixSize) - pixSize);
		//		colorIntensity = (((perlinResult / terrainHeight) + 0.7f)) / 1.3f;
		//		if (colorIntensity <= 0.25f)
		//		{
		//			colorIntensity = colorIntensity * 4.0f;
		//			//glColor4f(lerpf(colorX1, colorX2, colorIntensity, FALSE), lerpf(colorY1, colorY2, colorIntensity, FALSE), lerpf(colorZ1, colorZ2, colorIntensity, FALSE), 0.85f);
		//		}
		//		else if (colorIntensity > 0.25f && colorIntensity <= 0.5f)
		//		{
		//			colorIntensity = (colorIntensity - 0.25f) * 4.0f;
		//			//glColor4f(lerpf(colorX2, colorX3, colorIntensity, FALSE), lerpf(colorY2, colorY3, colorIntensity, FALSE), lerpf(colorZ2, colorZ3, colorIntensity, FALSE), 0.85f);
		//		}
		//		else if (colorIntensity > 0.5f && colorIntensity <= 0.75f)
		//		{
		//			colorIntensity = (colorIntensity - 0.5f) * 4.0f;
		//			//glColor4f(lerpf(colorX3, colorX4, colorIntensity, FALSE), lerpf(colorY3, colorY4, colorIntensity, FALSE), lerpf(colorZ3, colorZ4, colorIntensity, FALSE), 0.85f);
		//		}
		//		else
		//		{
		//			colorIntensity = (colorIntensity - 0.75f) * 4.0f;
		//			//glColor4f(lerpf(colorX4, 0.25f, colorIntensity, FALSE), lerpf(colorY4, 0.5f, colorIntensity, FALSE), lerpf(colorZ4, 1.0f, colorIntensity, FALSE), 0.85f);
		//		}
		//		glTexCoord2f((s + 1.0f) / 128.0f, (t - 1.0f) / 128.0f);
		//		glVertex3f((pixSize * i) + pixSize, (pixSize * j) - pixSize, (0.02 * sin(waterWave)) + (1.5f * sin(time * 2.0f) * perlinResult));///Right Bottom 
		//	}
		//	holdVal = waterWave;
		//	waterWave += (1.13 + 0.77);
		//}
		//glEnd();
	}
}

void initializeLastSceneMountain(void)
{
	float perlin(float, float);

	float colorX1 = 0.569 + 1.2f;
	float colorY1 = 0.847 + 1.2f;
	float colorZ1 = 0.925 + 1.5f;

	float colorX2 = 0.039;
	float colorY2 = 0.463;
	float colorZ2 = 0.745;

	float colorX3 = 0.035;
	float colorY3 = 0.357;
	float colorZ3 = 0.584;

	float colorX4 = 0.024;
	float colorY4 = 0.282;
	float colorZ4 = 0.502;

	GLfloat perlinResult = 0.0f;

	/*Vec3f a = { (water[-64 + (LEN)][64 + (LEN)][0] - water[-64 + (LEN)][64 + (LEN)][3]), (water[-64 + (LEN)][64 + (LEN)][1] - water[-64 + (LEN)][64 + (LEN)][4]), (water[-64 + (LEN)][64 + (LEN)][2] - water[-64 + (LEN)][64 + (LEN)][5]) };

	Vec3f b = { (water[-64 + (LEN)][64 + (LEN)][6] - water[-64 + (LEN)][64 + (LEN)][3]), (water[-64 + (LEN)][64 + (LEN)][7] - water[-64 + (LEN)][64 + (LEN)][4]), (water[-64 + (LEN)][64 + (LEN)][8] - water[-64 + (LEN)][64 + (LEN)][5]) };

	Vec3f c = cross(a, b);

	glNormal3f(-c.x, -c.y, -c.z);*/

	int len = 64 + (int)(-3.33 * 10);

	float s = 0.0f;
	float t = 0.0f;

	///1st Texture
	{
		glColor4f(1.0f, 1.0f, 1.0f, alphaSeaTexture);
		glBindTexture(GL_TEXTURE_2D, texture_rock1);

		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, -1.0f);
		for (int j = len; j >= -len; j--)
		{
			for (int i = -len; i <= len; i++)
			{

				perlinResult = water[i + len][j + len][0];
				s = 64.0f + i;
				t = j + 64.0f;
				glTexCoord2f((s + 1.0f) / 128.0f, (t) / 128.0f);
				glVertex3f((i * pixSize) + pixSize, (j * pixSize), perlinResult);///Right TOP 

				perlinResult = water[i + len][j + len][1];
				glTexCoord2f((s / 128.0f), t / 128.0f);
				glVertex3f((i * pixSize), (j * pixSize), perlinResult);///LEFT TOP aaa

				perlinResult = water[i + len][j + len][2];
				glTexCoord2f((s / 128.0f), (t - 1.0f) / 128.0f);
				glVertex3f((i * pixSize), (j * pixSize) - pixSize, perlinResult);///LEFT BOTTOM 

				perlinResult = water[i + len][j + len][3];
				glTexCoord2f((s + 1.0f) / 128.0f, (t - 1.0f) / 128.0f);
				glVertex3f((pixSize * i) + pixSize, (pixSize * j) - pixSize, perlinResult);///Right Bottom 
			}
			holdVal = waterWave;
			waterWave += (0.9 + 2.85);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void initializeTerrain1(void)
{
	float perlin(float, float);

	float colorX2 = 0.4;
	float colorY2 = 0.3;
	float colorZ2 = 0.3;

	float colorX1 = terrainHighColor.x;
	float colorY1 = terrainHighColor.y;
	float colorZ1 = terrainHighColor.z;

	for (int j = TERRAIN_LENGTH / 2; j > -TERRAIN_LENGTH / 2; j--)
	{
		for (int i = -TERRAIN_WIDTH / 2; i < TERRAIN_WIDTH / 2; i++)
		{
			//RIGHT TOP
			{
				colorIntensity = (((perlin((i * pixSize) + pixSize, (j * pixSize)) / terrainHeight) + 0.5f)) + 0.3f;

				if (colorIntensity >= 0.5f)
				{
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][12] = colorX1 * colorIntensity;
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][13] = colorY1 * colorIntensity;
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][14] = colorZ1 * colorIntensity;
				}
				else
				{
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][12] = colorX2 * colorIntensity;
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][13] = colorY2 * colorIntensity;
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][14] = colorZ2 * colorIntensity;
				}
				terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][0] = (i * pixSize) + pixSize;
				terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][1] = (j * pixSize);
				terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][2] = perlin(((i * pixSize) + pixSize), (j * pixSize));
			}


			//LEFT TOP
			{
				colorIntensity = (((perlin((i * pixSize), (j * pixSize)) / terrainHeight) + 0.5f)) + 0.3f;
				if (colorIntensity >= 0.5f)
				{
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][15] = colorX1 * colorIntensity;
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][16] = colorY1 * colorIntensity;
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][17] = colorZ1 * colorIntensity;
				}
				else
				{
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][15] = colorX2 * colorIntensity;
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][16] = colorY2 * colorIntensity;
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][17] = colorZ2 * colorIntensity;
				}
				terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][3] = (i * pixSize);
				terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][4] = (j * pixSize);
				terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][5] = perlin((i * pixSize), (j * pixSize));
			}


			//LEFT BOTTOM
			{
				colorIntensity = (((perlin((i * pixSize), (j * pixSize) - pixSize) / terrainHeight) + 0.5f)) + 0.3f;
				if (colorIntensity >= 0.5f)
				{
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][18] = colorX1 * colorIntensity;
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][19] = colorY1 * colorIntensity;
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][20] = colorZ1 * colorIntensity;
				}
				else
				{
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][18] = colorX2 * colorIntensity;
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][19] = colorY2 * colorIntensity;
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][20] = colorZ2 * colorIntensity;
				}
				terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][6] = (i * pixSize);
				terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][7] = (j * pixSize) - pixSize;
				terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][8] = perlin((i * pixSize), ((j * pixSize) - pixSize));
			}


			//RIGHT BOTTOM
			{
				colorIntensity = (((perlin((i * pixSize) + pixSize, (j * pixSize) - pixSize) / terrainHeight) + 0.5f)) + 0.3f;
				if (colorIntensity >= 0.5f)
				{
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][21] = colorX1 * colorIntensity;
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][22] = colorY1 * colorIntensity;
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][23] = colorZ1 * colorIntensity;
				}
				else
				{
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][21] = colorX2 * colorIntensity;
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][22] = colorY2 * colorIntensity;
					terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][23] = colorZ2 * colorIntensity;
				}
				terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][9] = (pixSize * i) + pixSize;
				terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][10] = (pixSize * j) - pixSize;
				terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][11] = perlin(((i * pixSize) + pixSize), ((j * pixSize) - pixSize));
			}
		}
	}
}

void generatePerlinNoise(void)
{
	float perlin(float, float);

	for (int j = LEN; j > -LEN; j--)
	{
		for (int i = -LEN; i < LEN; i++)
		{
			//RIGHT TOP
			{
				water[i + (LEN)][j + (LEN)][0] = perlin((i * pixSize) + pixSize, (j * pixSize));
			}


			//LEFT TOP
			{
				water[i + (LEN)][j + (LEN)][1] = perlin((i * pixSize), (j * pixSize));
			}


			//LEFT BOTTOM
			{
				water[i + (LEN)][j + (LEN)][2] = perlin((i * pixSize), ((j * pixSize) - pixSize));
			}


			//RIGHT BOTTOM
			{
				water[i + (LEN)][j + (LEN)][3] = perlin(((i * pixSize) + pixSize), ((j * pixSize) - pixSize));
			}
		}
	}
}

void drawTerrainScene1(void)
{
	glBegin(GL_QUADS);
	for (int j = TERRAIN_LENGTH / 2; j > -TERRAIN_LENGTH / 2; j--)
	{
		float perlin(float, float);

		for (int i = -TERRAIN_WIDTH / 2; i < TERRAIN_WIDTH / 2; i++)
		{
			//glTexCoord2f(1.0f, 1.0f);
			glNormal3f(0.0f, 0.0f, -1.0f);
			glColor3f(terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][12], terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][13], terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][14]);
			glVertex3f(terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][0], terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][1], terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][2]);

			//glTexCoord2f(0.0f, 1.0f);
			glColor3f(terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][15], terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][16], terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][17]);
			glVertex3f(terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][3], terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][4], terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][5]);

			//glTexCoord2f(0.0f, 0.0f);
			glColor3f(terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][18], terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][19], terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][20]);
			glVertex3f(terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][6], terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][7], terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][8]);

			//glTexCoord2f(1.0f, 0.0f);
			glColor3f(terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][21], terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][22], terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][23]);
			glVertex3f(terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][9], terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][10], terrain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][11]);
		}
	}
	glEnd();
}

//void drawWater(void)
//{
//	glBegin(GL_QUADS);
//
//	Vec3f a = { (water[-64 + (LEN)][64 + (LEN)][0] - water[-64 + (LEN)][64 + (LEN)][3]), (water[-64 + (LEN)][64 + (LEN)][1] - water[-64 + (LEN)][64 + (LEN)][4]), (water[-64 + (LEN)][64 + (LEN)][2] - water[-64 + (LEN)][64 + (LEN)][5]) };
//
//	Vec3f b = { (water[-64 + (LEN)][64 + (LEN)][6] - water[-64 + (LEN)][64 + (LEN)][3]), (water[-64 + (LEN)][64 + (LEN)][7] - water[-64 + (LEN)][64 + (LEN)][4]), (water[-64 + (LEN)][64 + (LEN)][8] - water[-64 + (LEN)][64 + (LEN)][5]) };
//
//	Vec3f c = cross(a, b);
//	glNormal3f(-c.x, -c.y, -c.z);
//
//	for (int j = LEN; j > -LEN; j--)
//	{
//		for (int i = -LEN; i < LEN; i++)
//		{
//
//
//			//glTexCoord2f(1.0f, 1.0f);
//			//glNormal3f((water[i + (LEN)][j + (LEN)][4] * water[i + (LEN)][j + (LEN)][11]) - (water[i + (LEN)][j + (LEN)][5] * water[i + (LEN)][j + (LEN)][10]), (water[i + (LEN)][j + (LEN)][5] * water[i + (LEN)][j + (LEN)][9]) - (water[i + (LEN)][j + (LEN)][3] * water[i + (LEN)][j + (LEN)][11]), (water[i + (LEN)][j + (LEN)][3] * water[i + (LEN)][j + (LEN)][7]) - (water[i + (LEN)][j + (LEN)][4] * water[i + (LEN)][j + (LEN)][9]));
//			glColor4f(water[i + (LEN)][j + (LEN)][12], water[i + (LEN)][j + (LEN)][13], water[i + (LEN)][j + (LEN)][14], 0.8f);
//			glVertex3f(water[i + (LEN)][j + (LEN)][0], water[i + (LEN)][j + (LEN)][1], water[i + (LEN)][j + (LEN)][2]);///Right TOP 
//
//			//glTexCoord2f(0.0f, 0.8f);
//			//glNormal3f((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), ///(a.x * b.y) - (a.y * b.x));
//			//glNormal3f((water[i + (LEN)][j + (LEN)][1] * water[i + (LEN)][j + (LEN)][8]) - (water[i + (LEN)][j + (LEN)][2] * water[i + (LEN)][j + (LEN)][7]), (water[i + (LEN)][j + (LEN)][2] * water[i + (LEN)][j + (LEN)][6]) - (water[i + (LEN)][j + (LEN)][0] * water[i + (LEN)][j + (LEN)][8]), (water[i + (LEN)][j + (LEN)][0] * water[i + (LEN)][j + (LEN)][7]) - (water[i + (LEN)][j + (LEN)][1] * water[i + (LEN)][j + (LEN)][6]));
//			glColor4f(water[i + (LEN)][j + (LEN)][15], water[i + (LEN)][j + (LEN)][16], water[i + (LEN)][j + (LEN)][17], 0.8f);
//			glVertex3f(water[i + (LEN)][j + (LEN)][3], water[i + (LEN)][j + (LEN)][4], water[i + (LEN)][j + (LEN)][5]);///LEFT TOP aaa
//
//			//glTexCoord2f(0.0f, 0.0f);
//			//glNormal3f((water[i + (LEN)][j + (LEN)][4] * water[i + (LEN)][j + (LEN)][11]) - (water[i + (LEN)][j + (LEN)][5] * water[i + (LEN)][j + (LEN)][10]), (water[i + (LEN)][j + (LEN)][5] * water[i + (LEN)][j + (LEN)][9]) - (water[i + (LEN)][j + (LEN)][3] * water[i + (LEN)][j + (LEN)][11]), (water[i + (LEN)][j + (LEN)][3] * water[i + (LEN)][j + (LEN)][10]) - (water[i + (LEN)][j + (LEN)][4] * water[i + (LEN)][j + (LEN)][10]));
//			glColor4f(water[i + (LEN)][j + (LEN)][18], water[i + (LEN)][j + (LEN)][19], water[i + (LEN)][j + (LEN)][20], 0.8f);
//			glVertex3f(water[i + (LEN)][j + (LEN)][6], water[i + (LEN)][j + (LEN)][7], water[i + (LEN)][j + (LEN)][8]);///LEFT BOTTOM 
//
//			//glTexCoord2f(1.0f, 0.0f);
//			//glNormal3f((water[i + (LEN)][j + (LEN)][7] * water[i + (LEN)][j + (LEN)][2]) - (water[i + (LEN)][j + (LEN)][8] * water[i + (LEN)][j + (LEN)][1]), (water[i + (LEN)][j + (LEN)][8] * water[i + (LEN)][j + (LEN)][0]) - (water[i + (LEN)][j + (LEN)][6] * water[i + (LEN)][j + (LEN)][2]), (water[i + (LEN)][j + (LEN)][6] * water[i + (LEN)][j + (LEN)][1]) - (water[i + (LEN)][j + (LEN)][7] * water[i + (LEN)][j + (LEN)][0]));
//
//			glColor4f(water[i + (LEN)][j + (LEN)][21], water[i + (LEN)][j + (LEN)][22], water[i + (LEN)][j + (LEN)][23], 0.8f);
//			glVertex3f(water[i + (LEN)][j + (LEN)][9], water[i + (LEN)][j + (LEN)][10], water[i + (LEN)][j + (LEN)][11]);///Right Bottom 
//		}
//	}
//	glEnd();
//}

void drawTerrainMountain(void)
{
	glBegin(GL_QUADS);
	for (int j = TERRAIN_LENGTH / 2; j > -TERRAIN_LENGTH / 2; j--)
	{
		float perlin(float, float);

		for (int i = -TERRAIN_WIDTH / 2; i < TERRAIN_WIDTH / 2; i++)
		{
			//glTexCoord2f(1.0f, 1.0f);
			glNormal3f(0.0f, 0.0f, -1.0f);
			glColor3f(terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][12], terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][13], terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][14]);
			glVertex3f(terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][0], terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][1], terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][2]);

			//glTexCoord2f(0.0f, 1.0f);
			glColor3f(terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][15], terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][16], terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][17]);
			glVertex3f(terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][3], terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][4], terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][5]);

			//glTexCoord2f(0.0f, 0.0f);
			glColor3f(terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][18], terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][19], terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][20]);
			glVertex3f(terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][6], terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][7], terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][8]);

			//glTexCoord2f(1.0f, 0.0f);
			glColor3f(terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][21], terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][22], terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][23]);
			glVertex3f(terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][9], terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][10], terrain_scene01Mountain[i + (TERRAIN_WIDTH / 2)][j + (TERRAIN_LENGTH / 2)][11]);
		}
	}
	glEnd();
}

Vec2f randomGradient(int ix, int iy)
{
	const unsigned int w = 8 * sizeof(unsigned int);
	const unsigned int s = w / 2;
	unsigned int a = ix;
	unsigned int b = iy;
	a *= 3284157443;
	b ^= a << s | a >> w - s;
	b *= 1911520717;
	a ^= b << s | b >> w - s;
	a *= 2048419325;
	float random = a * (3.14159265 / ~(~0u >> 1));

	Vec2f v;
	v.x = cos(random);
	v.y = sin(random);

	return v;
}

float dotGridGradient(int ix, int iy, float x, float y)
{
	Vec2f gradient = randomGradient(ix, iy);

	float dx = x - (float)ix;
	float dy = y - (float)iy;

	return (dx * gradient.x + dy * gradient.y);
}

float perlin(float xCordinate, float yCordinate)
{
	//Find Grid Cell
	int x0 = (int)floor(xCordinate);
	int x1 = x0 + 1;
	int y0 = (int)floor(yCordinate);
	int y1 = y0 + 1;

	//Find Interpolation Weight
	float weightX = xCordinate - (float)x0;
	float weightY = yCordinate - (float)y0;

	// Interpolate between grid point gradients
	float n0, n1, ix0, ix1, value;

	n0 = dotGridGradient(x0, y0, xCordinate, yCordinate);
	n1 = dotGridGradient(x1, y0, xCordinate, yCordinate);
	ix0 = lerpf(n0, n1, weightX, TRUE);

	n0 = dotGridGradient(x0, y1, xCordinate, yCordinate);
	n1 = dotGridGradient(x1, y1, xCordinate, yCordinate);
	ix1 = lerpf(n0, n1, weightX, TRUE);

	value = lerpf(ix0, ix1, weightY, TRUE);

	//value = (value + 0.5f) * 255;
	//fprintf(gpFILE, "Val:%f\n", value);

	return (value * terrainHeight);
}

void LightIntensity(float factor)
{
	lightAmbient[0] = lightAmbient[0] * factor;
	lightAmbient[1] = lightAmbient[1] * factor;
	lightAmbient[2] = lightAmbient[2] * factor;

	lightDiffuse[0] = lightDiffuse[0] * factor;
	lightDiffuse[1] = lightDiffuse[1] * factor;
	lightDiffuse[2] = lightDiffuse[2] * factor;

	lightSpecular[0] = lightSpecular[0] * factor;
	lightSpecular[1] = lightSpecular[1] * factor;
	lightSpecular[2] = lightSpecular[2] * factor;

	lightAmbient1[0] = lightAmbient1[0] * factor;
	lightAmbient1[1] = lightAmbient1[1] * factor;
	lightAmbient1[2] = lightAmbient1[2] * factor;

	lightDiffuse1[0] = lightDiffuse1[0] * factor;
	lightDiffuse1[1] = lightDiffuse1[1] * factor;
	lightDiffuse1[2] = lightDiffuse1[2] * factor;

	lightSpecular1[0] = lightSpecular1[0] * factor;
	lightSpecular1[1] = lightSpecular1[1] * factor;
	lightSpecular1[2] = lightSpecular1[2] * factor;
}

void initializeLight(void)
{
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpecular2);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);

	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);

	//glEnable(GL_LIGHT1);
}

void updateScene(void)
{
	if (mainScene == 1)
	{
		if (scene == 2)
		{
			bigBee.yRotate = -90.0f;
			smallBee.yRotate = -90.0f;
			bScene2 = FALSE;
		}

		if (scene == 3 && time < 88.0f)
		{
			bigBee.yRotate = 180.0f;
			smallBee.yRotate = 0.0f;
			camera.front = { -22.0f,4.0f,-232.0f };
			bScene3 = FALSE;

			bigBee.bBeeFly = FALSE;
			bigBee.bBeeWings = FALSE;

			smallBee.bBeeFly = FALSE;
			smallBee.bBeeWings = FALSE;

			birdYRotate = -90.0f;
		}

		if (time > 86 && time < 88)
		{
			smallBee.yRotate = 180.0f;
			bigBee.xTrans = 0.0f;
			bigBee.yTrans = 0.0f;
			bigBee.zTrans = 0.0f;

			smallBee.xTrans = 0.0f;
			smallBee.yTrans = 0.0f;
			smallBee.zTrans = 0.0f;
		}
	}

	if (bScene4 == TRUE && time >= 105.0f)
	{
		bigBee.xTrans = 0.0f;
		bigBee.yTrans = 5.25f;
		bigBee.zTrans = 0.0f;

		smallBee.xTrans = 0.25f;
		smallBee.yTrans = 5.0f;
		smallBee.zTrans = 0.0f;

		smallBee.yRotate = 180.0f;
		bigBee.yRotate = 180.0f;

		smallBee.bBeeFly = FALSE;
		smallBee.bBeeWings = FALSE;

		bScene4 = FALSE;
	}

	if (bScene5 == TRUE && 0)
	{
		smallBee.bBeeFly = TRUE;
		smallBee.bBeeWings = TRUE;

		smallBee.xRotate = 0.0f;
		smallBee.yRotate = 180.0f;
		smallBee.zRotate = 0.0f;

		bScene5 = FALSE;
	}

}

void fading_in_out(BOOL action)
{
	// fade in - out...
	static int enter_flag = 1;
	static GLfloat f_Alpha;

	if (action == 0)        // FADE_IN = 0
	{
		if (enter_flag == 1)
		{
			f_Alpha = 1.0f;
			enter_flag = 2;
		}

		glPushMatrix();

		glLoadIdentity();
		gluLookAt(camera.position.x, camera.position.y, camera.position.z, camera.front.x + camera.position.x, camera.front.y + camera.position.y, camera.front.z + camera.position.z, camera.up.x, camera.up.y, camera.up.z);

		glTranslatef(camera.position.x, camera.position.y, camera.position.z);
		glColor4f(0.0f, 0.0f, 0.0f, f_Alpha);
		gluSphere(quadric, 0.5f, 30, 30);

		glPopMatrix();

		if (f_Alpha >= 0.0f)
		{
			if (time >= 42.0f && time < 50.0f)
			{
				f_Alpha -= 0.01f;
			}
			else
			{
				f_Alpha -= 0.0065f;
			}
		}
		
			
		
		
	}

	if (action == 1)    // FADE_OUT = 1
	{
		if (enter_flag == 1)
		{
			f_Alpha = 0.0f;
			enter_flag = 2;
		}

		glPushMatrix();

		glLoadIdentity();
		gluLookAt(camera.position.x, camera.position.y, camera.position.z, camera.front.x + camera.position.x, camera.front.y + camera.position.y, camera.front.z + camera.position.z, camera.up.x, camera.up.y, camera.up.z);

		glTranslatef(camera.position.x, camera.position.y, camera.position.z);
		glColor4f(0.0f, 0.0f, 0.0f, f_Alpha);
		gluSphere(quadric, 0.5f, 30, 30);

		glPopMatrix();

		if (f_Alpha <= 1.0f)
		{
			if (time >= 42.0f && time < 50.0f)
			{
				f_Alpha += 0.01f;
			}
			else
			{
				f_Alpha += 0.0065f;
			}
		}
	}
}




//typedef struct TextureImageContext
//{
//	unsigned int texImageWidth;
//	unsigned int texImageHeight;
//
//	void* io_user_data;
//
//	int read_from_callbacks;
//	int bufferLength;
//	unsigned char buffer_start[128];
//	int callback_already_read;
//
//	unsigned char* img_buffer, * img_buffer_end;
//	unsigned char* img_buffer_original, * img_buffer_original_end;
//
//}TextureImageContext;
//
//TextureImageContext textureFileContext;
//
//unsigned char* loadPNGTexture(const char* textureFileName, int* textureWidth, int* textureHeight)
//{
//	FILE* file = NULL;
//	if (fopen_s(&file, textureFileName, "rb") != 0)
//	{
//		fprintf(gpFILE, "fopen_s() Failed..\n");
//		exit();
//	}
//
//	unsigned char* result;
//	//result = stbi_load_from_file(f, x, y, comp, req_comp);
//
//	textureFileContext.io_user_data = (void*)file;
//	textureFileContext.bufferLength          = sizeof(textureFileContext.buffer_start);
//	textureFileContext.read_from_callbacks   = 1;
//	textureFileContext.callback_already_read = 0;
//	textureFileContext.img_buffer_original   = textureFileContext.buffer_start;
//	textureFileContext.img_buffer            = textureFileContext.img_buffer_original;
//
//	//Refill Buffer
//	int n = textureFileContext.
//
//
//
//
//	fclose(file);
//	return result;
//}
//
//
//
//STBIDEF stbi_uc* stbi_load_from_file(FILE* f, int* x, int* y, int* comp, int req_comp)
//{
//	//unsigned char* result;
//	stbi__context s;
//	stbi__start_file(&s, f);
//	result = stbi__load_and_postprocess_8bit(&s, x, y, comp, req_comp);
//	//if (result) {
//	//	// need to 'unget' all the characters in the IO buffer
//	//	fseek(f, -(int)(s.img_buffer_end - s.img_buffer), SEEK_CUR);
//	//}
//	//return result;
//}
//}
