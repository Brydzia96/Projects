#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>


//The width and height of the window
#define WIDTH  1675
#define HEIGHT 1025
//The width and height of the drawing space and toolbars
#define DRAWING_SPACE_WIDTH 1300
#define DRAWING_SPACE_HEIGHT 870
#define TOOLBAR_TOP_WIDTH 1650
#define TOOLBAR_TOP_HEIGHT 80
#define TOOLBAR_LEFT_WIDTH 300
#define TOOLBAR_LEFT_HEIGHT 920


/*Constants values for toolbars. 
START_POINTX_COLOUR_WINDOW : coordinate X of window with colour picker.
START_POINTY_COLOUR_WINDOW : coordinate Y of window with colour picker.
COLOUR_WINDOW_SIZE : the dimensions of squere colour picker window.
START_POINTX_COLOUR_BUTTON : coordinate X of button with colour picker.
START_POINTY_COLOUR_BUTTON : coordinate Y of button with colour picker.
COLOUR_BUTTON_WIDTH : width of button with colour picker.
COLOUR_BUTTON_HEIGHT : height of button with colour picker. */
#define START_POINTX_COLOUR_WINDOW 601
#define START_POINTY_COLOUR_WINDOW 276
#define COLOUR_WINDOW_SIZE 449
#define START_POINTX_COLOUR_BUTTON  28
#define START_POINTY_COLOUR_BUTTON  900
#define COLOUR_BUTTON_WIDTH 188
#define COLOUR_BUTTON_HEIGHT 76

// Declaration of RGBcolour struct which is used to hold RGB colour values. 
typedef struct RGBcolour
{
	Uint8 red;
	Uint8 green;
	Uint8 blue;
} RGBcolour;

//Global variables used in program
// option: the value used in switch in main loop to choose function mode.
int option = 0;
// optionChecker: a variable used for checking the previous mode after call of colour picker function.
int optionChecker=0;
// size: size of brushes, lines, the contours of shapes.
int size;	
// oldMouseX : a variable used in drawing function for holding the value of previous x coordinate.
int oldMouseX;
// oldMouseY : a variable used in drawing function for holding the value of previous y coordinate.
int oldMouseY;	
// outline: draw outline shapes.
int outline=0;
// filled: draw filled shapes.
int filled=0;
// x, y, width: variables for squere function.
int x, y, width;
// p, q:  variables for circle function.
int p, q;
//pointCircle: a variable used for circle mode.
int pointCircle=1;
// colourWheelOn: checks if colourWheel is on the screen.
int colourWheelOn = 0;
// count: checks if a file was previous saved.
int count = 0;
//fileName: an array for name of saving file.
char fileName[200];
//dropDownMenuOn: checks if drop down menu from top toolbar is on the screen.
int dropDownMenuOn = 0;
//locked: if variable == 1 it means that drawing on drawing space is unabled.
int locked = 0;
//openCloseMenu: a variable used for checking if drop down menu on top toolbar is closed or opened.
int openCloseMenu = 0;
// OpenCloseColourWindow: a variable used for checking if colour wheel window is closed or opened.
int OpenCloseColourWindow = 0;
// checkPicker: a variable used for control the colour picker function.
int checkPicker=0;
// triangleChecker: a variable used for different triangle modes.
int triangleChecker=1;
//uploadFileChecker=0, saveAsFileChecker=0, saveFileChecker=0: variables used for controling uploading and saving modes.
int uploadFileChecker=0, saveAsFileChecker=0, saveFileChecker=0;
//newFileChecker: a variable used for controling newFile option.
int newFileChecker=0;
//leftMouseButtonDown: a variable used for checking whether left mouse button is pressed. 
int leftMouseButtonDown = 0;
// leftMouseButtonUp: a variable used for checking whether left mouse button is released.
int leftMouseButtonUp = 1;
// rightMouseButtonDown: a variable used for checking whether right mouse button is pressed.
int rightMouseButtonDown = 0;
//rightMouseButtonUp: a variable used for checking whether right mouse button is released.
int rightMouseButtonUp = 1;



RGBcolour nPcol; // a variable which holds RGB data in flood fill function.
RGBcolour colour; //a variable which holds RGB data for drawing colour.
RGBcolour colour1; //a variable which holds RGB data for eraser colour.
RGBcolour *colourPointer1 = &colour1; //pointer to the variable which holds data for eraser colour.
RGBcolour *colourPointer = &colour; //pointer to the variable which holds data for drawing colour.
RGBcolour *nPcolPointer = &nPcol; //pointer to the variable which holds RGB data in flood fill function.
RGBcolour src;//: a variable which holds RGB source data in flood fill function.
RGBcolour dst;//a variable which holds RGB destination data in flood fill function.
RGBcolour *srcPointer = &src;//pointer to the variable which holds RGB source data in flood fill function.
RGBcolour *dstPointer = &dst;//pointer to the variable which holds RGB destination data in flood fill function.

/*-------------------------------------RECTS USED FOR THE CREATION OF THE INTERFACE---------------------------------*/
SDL_Rect topToolbarRect = {0, 0, WIDTH, 80};
SDL_Rect rectColourPalette = {0, 80, TOOLBAR_LEFT_WIDTH-50, TOOLBAR_LEFT_HEIGHT};
SDL_Rect leftBarRect = {250, 80, 50, TOOLBAR_LEFT_HEIGHT};
SDL_Rect rightBarRect = {DRAWING_SPACE_WIDTH + TOOLBAR_LEFT_WIDTH, 80, 50, TOOLBAR_LEFT_HEIGHT};
SDL_Rect bottomBarRect = {TOOLBAR_LEFT_WIDTH ,950 ,DRAWING_SPACE_WIDTH ,50};		
SDL_Rect rightSmallBarRect = {1650, 80, 25, 945};					
SDL_Rect bottomSmallBarRect = {0, HEIGHT-25, WIDTH, 25};


//Pointers declaration
SDL_Window *window;
SDL_Renderer *renderer;

/*OPTIONS: ENUMERATION OF DIFFERENT MODES IN PAINT PROGRAM
	PENCIL: DRAW A SINGLE LINE WHICH IS BUILT WITH ONE PIXEL (p button on keyboard)
	DRAW_LINE: DRAW A STRAIGHT LINE, WITH BUTTONS FROM 1-9 YOU CAN CHOOSE THE SIZE OF THIS LINE (l button on keyboard)
	DRAW_RECTANGLE: DRAW A RECTANGLE, WITH BUTTONS FROM 1-9 YOU CAN CHOOSE THE SIZE OF THE CONTOURS (r button on keyboard)
	DRAW_CIRCLE: DRAW A CIRCLE, WITH BUTTONS FROM 1-9 YOU CAN CHOOSE THE SIZE OF THE CONTOURS,  (c button on keyboard)
	DRAW_SQUERE: DRAW A SQUERE, WITH BUTTONS FROM 1-9 YOU CAN CHOOSE THE SIZE OF THIS LINE, WITH N AND M BUTTONS  YOU CAN CHOOSE THE BEGGINIG OF THE CIRCLE(N- FROM THE SIDE, M FROM THE MIDDLE) (e button on keyboard)
	DRAW_TRIANGLE: DRAW A TRIANGLE, WITH BUTTONS: Z: OUTLINED TRIANGLE BUILT FROM SIDE, X-OUTLINED TRIANGLE FROM THE VERTEX, N-FILLED TRIANGLE FROM THE SIDE, M-FILLED TRIANGLE FROM THE VERTEX (t button on keyboard)
	BUCKET: FILL WITH COLOUR THE CHOOSEN SURFACE. -------------WARNING!!!!----------------: FUNCTION IS VERY SLOW, CHOOSE VERY SMALL FURFACE TO FILLING (b button on keyboard)
	PICKER: COLOUR PICKER, YOU CAN CHOOSE COLOURS FROM THE LEFT TOOLBAR, THE DRAWING SPACE, OR THE COLOUR WHEEL WINDOW WHICH IS OPENED WHEN YOU CLICK ON THE COLOURFUL BUTTON ON THE LEFT TOOLBAR (i button on keyboard)
	PAINT: PAINT WITH DIFFERENT SIZED BRUSHES WITH BUTTON FROM 1-9 YOU CAN CHOOSE THE SIZE OF THE BRUSH (b button on keyboard)
	ERASER: DPAINT WITH DIFFERENT SIZED ERASER, WITH BUTTON FROM 1-9 YOU CAN CHOOSE THE SIZE OF THE ERASER(right mouse button pressed)
	*/
enum OPTIONS
{
	PENCIL,
	DRAW_LINE,
	DRAW_RECTANGLE,
	DRAW_CIRCLE,
	DRAW_SQUERE,
	DRAW_TRIANGLE,
	BUCKET,
	PICKER,
	PAINT,
	ERASER
};
/*---------------DIMENTIONS OF RECTANGLES USED FOR BUTTONS ON THE TOP TOOLBAR---------------*/
//Rectangles from the uploaded surface(parts of the surface converted to texture)			
SDL_Rect newFileRect = {450, 0, 50, 50};
SDL_Rect saveAsRect = {0, 0, 50, 50};
SDL_Rect saveRect = {50, 0, 50, 50};
SDL_Rect uploadRect = {100, 0, 50, 50};
SDL_Rect sizeRect = {150, 0, 50, 50};
SDL_Rect bucketRect = {200, 0, 50, 50};
SDL_Rect moveRect = {400, 0, 50, 50};
SDL_Rect rotateRect = {250, 0, 50, 50};
SDL_Rect flipVerticalRect = {300, 0, 50, 50};
SDL_Rect flipHotizonlatRect = {350, 0, 50, 50};
SDL_Rect undoRect = {0, 50, 25, 25};	
SDL_Rect redoRect = {25, 50, 25, 25};	
SDL_Rect pickerRect = {100, 50, 25, 25};		
SDL_Rect rubberRect = {125, 50, 25, 25};		
SDL_Rect filledCircleRect = {175, 50, 25, 25};			
SDL_Rect filledRectangleRect = {225, 50, 25, 25};	
SDL_Rect filledSquereRect = {275, 50, 25, 25};
SDL_Rect filledTriangleLeftRect = {475, 50, 25, 25};			
SDL_Rect filledTriangleTopRect = {350, 50, 25, 25};		
SDL_Rect filledUpsideDownTriangleLeftRect = {375, 50, 25, 25};			
SDL_Rect filledUpsideDownTriangleTopRect = {450, 50, 25, 25};	
SDL_Rect brushRect = {50, 50, 25, 25};
SDL_Rect pencilRect = {75, 50, 25, 25};
SDL_Rect outlinedCircleRect = {150, 50, 25, 25};
SDL_Rect outlinedRectangleRect = {200, 50, 25, 25};
SDL_Rect outlinedSquereRect = {250, 50, 25, 25};
SDL_Rect outlinedTriangleLeftRect = {300, 50, 25, 25};
SDL_Rect outlinedTriangleTopRect = {325, 50, 25, 25};
SDL_Rect outlinedUpsideDownTriangleLeftRect = {400, 50, 25, 25};
SDL_Rect outlinedUpsideDownTriangleTopRect = {425, 50, 25, 25};
SDL_Rect copyRect = {500, 50, 25, 25};		
SDL_Rect pasteRect = {550, 50, 25, 25};			
SDL_Rect cutRect = {575, 50, 25, 25};		
SDL_Rect scaleRect = {625, 50, 25, 25};				
SDL_Rect selectRect = {525, 50, 25, 25};
SDL_Rect selectAllRect = {600, 50, 25, 25};	
SDL_Rect lineRect = {0, 75, 25, 50};

/* Rectangles which create the place on the toolbar (coordinates of the button and its size)*/
SDL_Rect newFileRectToolbar = {40, 11, 50, 50};
SDL_Rect saveAsRectToolbar = {100, 11, 50, 50};
SDL_Rect saveRectToolbar = {161, 11, 50, 50};
SDL_Rect uploadRectToolbar = {221, 11, 50, 50};
SDL_Rect sizeRectToolbar = {408, 11, 50, 50};
SDL_Rect bucketRectToolbar = {752, 11, 50, 50};
SDL_Rect moveRectToolbar = {939, 11, 50, 50};
SDL_Rect rotateRectToolbar = {1005, 11, 50, 50};
SDL_Rect flipVerticalRectToolbar = {1071, 11, 50, 50};
SDL_Rect flipHotizonlatRectToolbar = {1137, 11, 50, 50};
SDL_Rect undoRectToolbar = {285, 9, 25, 25};	
SDL_Rect redoRectToolbar = {285, 38, 25, 25};	
SDL_Rect pickerRectToolbar = {338, 38, 25, 25};		
SDL_Rect rubberRectToolbar = {371, 38, 25, 25};		
SDL_Rect filledCircleRectToolbar = {470, 38, 25, 25};			
SDL_Rect filledRectangleRectToolbar = {503, 38, 25, 25};	
SDL_Rect filledSquereRectToolbar = {536, 38, 25, 25};
SDL_Rect filledTriangleLeftRectToolbar = {606, 38, 25, 25};			
SDL_Rect filledTriangleTopRectToolbar = {639, 38, 25, 25};		
SDL_Rect filledUpsideDownTriangleLeftRectToolbar = {672, 38, 25, 25};			
SDL_Rect filledUpsideDownTriangleTopRectToolbar = {705, 38, 25, 25};	
SDL_Rect brushRectToolbar = {338, 9, 25, 25};
SDL_Rect pencilRectToolbar = {371, 9, 25, 25};
SDL_Rect outlinedCircleRectToolbar = {470, 9, 25, 25};
SDL_Rect outlinedRectangleRectToolbar = {503, 9, 25, 25};
SDL_Rect outlinedSquereRectToolbar = {536, 9, 25, 25};
SDL_Rect outlinedTriangleLeftRectToolbar = {606, 9, 25, 25};
SDL_Rect outlinedTriangleTopRectToolbar = {639, 9, 25, 25};
SDL_Rect outlinedUpsideDownTriangleLeftRectToolbar = {672, 9, 25, 25};
SDL_Rect outlinedUpsideDownTriangleTopRectToolbar = {705, 9, 25, 25};
SDL_Rect copyRectToolbar = {824, 6, 25, 25};		
SDL_Rect pasteRectToolbar = {824, 42, 25, 25};			
SDL_Rect cutRectToolbar = {861, 42, 25, 25};		
SDL_Rect scaleRectToolbar = {898, 42, 25, 25};				
SDL_Rect selectRectToolbar = {861, 6, 25, 25};
SDL_Rect selectAllRectToolbar = {898, 6, 25, 25};	
SDL_Rect lineRectToolbar = {571, 11, 25, 50};	

//Surfaces used in program
SDL_Surface *leftToolbarSurface;
SDL_Surface *barsSurface;
SDL_Surface *topToolbarSurface;
SDL_Surface *openColourWindow;		

/*Function for the squere root used in the circle functions. The code is based on the code from 17 presentation of Computing for Graphics Unit by Dr Eike Falk Anderson.*/
float squereRoot(float x)
{	
	float result = 1;
	for(int i =0; i<500; i++)
	{
		result = (x/result + result)/2;
	}
	return result;
}
//Textures used in program.
SDL_Texture *blankTexture;
SDL_Texture *replacedTexture;
SDL_Texture *leftToolbarTexture;
SDL_Texture *leftToolbarTextureButton;
SDL_Texture *colourWheelTexture;
SDL_Texture *leftBarTexture;
SDL_Texture *rightBarTexture;
SDL_Texture *bottomBarTexture;
SDL_Texture *bottomSmallBarTexture;
SDL_Texture *rightSmallBarTexture;
SDL_Texture *topToolbarTexture;
SDL_Texture *topToolbarTextureBright;
SDL_Texture *topToolbarMenuTexture;
SDL_Texture *topToolbarMenuBrightTexture;
SDL_Texture *topToolbarMenuReplacementTexture;
SDL_Texture *topToolbarDarkTexture;
//Rectangle of the drawing space.
SDL_Rect drawingTextureRect = {TOOLBAR_LEFT_WIDTH , TOOLBAR_TOP_HEIGHT, TOOLBAR_LEFT_WIDTH + DRAWING_SPACE_WIDTH, TOOLBAR_TOP_HEIGHT + DRAWING_SPACE_HEIGHT};	
void createTextures(void)
{
	/* Function which creates textures at the beginning of main loop*/
	blankTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
	replacedTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT); //Texture used in functions for replacement of main (blank) texture.
	
}


//Function for saving file
int saveFile(void)
{
	SDL_Rect rect = {TOOLBAR_LEFT_WIDTH, TOOLBAR_TOP_HEIGHT, DRAWING_SPACE_WIDTH, DRAWING_SPACE_HEIGHT}; //The creation of the rect for drawing space which will be saved. 
	if (count == 1)
	{
		SDL_Surface *drawingSurface = SDL_CreateRGBSurface(0, DRAWING_SPACE_WIDTH, DRAWING_SPACE_HEIGHT, 32, 0, 0, 0, 0);
		SDL_SetRenderTarget(renderer, replacedTexture);
		SDL_RenderCopyEx(renderer, blankTexture, NULL, NULL, 0, NULL, SDL_FLIP_VERTICAL); 
		SDL_RenderReadPixels(renderer, &rect, SDL_PIXELFORMAT_ARGB8888, drawingSurface->pixels, drawingSurface->pitch); //The creation of surface used for saving file as BMP.
		SDL_SaveBMP(drawingSurface, fileName);
		SDL_SetRenderTarget(renderer, blankTexture);
		
	}
	
	else if(count == 0)//If count is 0 it meas that user choose the option SAVE AS(a button) or user is saving the file for the first time.
	{
	    printf("Save as: \n");
		scanf("%s", fileName); /*Function uses the terminal to retrieve the file name*/
		fflush(stdin);
		SDL_Surface *drawingSurface = SDL_CreateRGBSurface(0, DRAWING_SPACE_WIDTH, DRAWING_SPACE_HEIGHT, 32, 0, 0, 0, 0);
		SDL_SetRenderTarget(renderer, replacedTexture);
		SDL_RenderCopyEx(renderer, blankTexture, NULL, NULL, 0, NULL, SDL_FLIP_VERTICAL); 
		SDL_RenderReadPixels(renderer, &rect, SDL_PIXELFORMAT_ARGB8888, drawingSurface->pixels, drawingSurface->pitch); //The creation of surface used for saving file as BMP.
		SDL_SaveBMP(drawingSurface, fileName);
		SDL_SetRenderTarget(renderer, blankTexture);
	}
	printf("The file was successfully saved as %s .\n", fileName); /*if file is saved the information is printed in the terminal*/	
	return 1;	 			
}

//Function for uploading file
void uploadFile(void)
{
	printf("Type the file name: \n"); /*user can type in terminal the file name which is supposed to be open.*/
	scanf("%s", fileName);
	SDL_Surface *uploadedFile = SDL_LoadBMP(fileName);
	SDL_Texture *newTexture = SDL_CreateTextureFromSurface(renderer, uploadedFile); /*New texture is created from surface*/
	SDL_SetRenderTarget(renderer, blankTexture); /*Target is set on the main(blank) texture)*/
	SDL_Rect rect = {TOOLBAR_LEFT_WIDTH, TOOLBAR_TOP_HEIGHT, DRAWING_SPACE_WIDTH, DRAWING_SPACE_HEIGHT};  
	SDL_RenderCopy(renderer, newTexture, NULL, &rect);	/*The uploaded texture is displayed on the main texture*/					
}
//Function for new file
void newFile(void)
{
	/*Function which resets the screen and sets the default data*/	
 colour1.red= 0xFF; 
 colour1.green = 0xFF;
 colour1.blue = 0xFF; 
 SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
 SDL_Rect newRect = {TOOLBAR_LEFT_WIDTH, TOOLBAR_TOP_HEIGHT, DRAWING_SPACE_WIDTH, DRAWING_SPACE_HEIGHT}; 
 SDL_RenderFillRect(renderer, &newRect);
 colour.red= 0x00; 
 colour.green = 0x00;
 colour.blue = 0x00;
 optionChecker=0;			
}
//Function for cursor in drawing space
int cursorInDrawingSpace(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY)
{
	/*Function which checks if cursor is located in drawing space*/
	if((LMB_PressedX>TOOLBAR_LEFT_WIDTH) && (LMB_PressedX<TOOLBAR_LEFT_WIDTH + DRAWING_SPACE_WIDTH) && (LMB_PressedY>TOOLBAR_TOP_HEIGHT) && (LMB_PressedY<TOOLBAR_TOP_HEIGHT + DRAWING_SPACE_HEIGHT))  
	return 1;
	else
	return 0; 
}

/*Function for colour picker*/
RGBcolour getPixelColour(int LMB_PressedX, int LMB_PressedY, RGBcolour *pcolPointer)
{	
	/*Function uses pointer to variables and returns the pointer*/ /*Function is based on the code from 19 presentation of Dr Eike Falk Anderson*/
	pcolPointer->red = 0;
	pcolPointer->green = 0;
	pcolPointer->blue = 0;
	Uint32 pixels[10];

	SDL_Surface *screenSurface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
	SDL_Surface *newSurface = SDL_ConvertSurfaceFormat(screenSurface, SDL_PIXELFORMAT_RGBA8888, 0);
	SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, screenSurface->pixels, screenSurface->pitch); /*Render read pixels maps the RGB data for the created surface*/
	SDL_Rect rect = {LMB_PressedX, HEIGHT-LMB_PressedY-1, 1, 1}; /*The second argument of this function is equal to HEIGHT-LMB_PressedY-1 because the pixels are horizontal flipped*/
	SDL_RenderReadPixels(renderer, &rect, SDL_PIXELFORMAT_RGBA8888, pixels, 5);
	
	SDL_GetRGB(pixels[0], newSurface->format, &(pcolPointer->red), &(pcolPointer->green), &(pcolPointer->blue));
	SDL_FreeSurface(screenSurface);
	SDL_FreeSurface(newSurface);
			
	
	return *pcolPointer;			
}


//Function for same colour
int sameColour(RGBcolour firstColour, RGBcolour secondColour) 
{
	/*The code for this function is based on code from 19 presentation of Dr Eike Anderson*/
	return ((firstColour.red == secondColour.red) && (firstColour.green == secondColour.green) && (firstColour.blue == secondColour.blue));
} 


//Function for flood fill	
void fill(int LMB_PressedX, int LMB_PressedY)
{
	/*Function os recursive functon and works very slow. First, it checks if the source colour (the colour of the chosen place) is the same as destination colour. 
	 *Next it comperes the colour of neighbouring pixels. The function repeatedly checks the same pixels. The code for this function is based on code from 19 presentation of Dr Eike Anderson. */ 
	if(sameColour(src, dst)) return;	
	nPcol = getPixelColour(LMB_PressedX, LMB_PressedY, &nPcol);	
	if(!sameColour(src, nPcol)) return;
	SDL_SetRenderDrawColor(renderer, dstPointer->red, dstPointer->green, dstPointer->blue, 0xFF);	
	SDL_RenderDrawPoint(renderer, LMB_PressedX, LMB_PressedY);
	fill(LMB_PressedX, LMB_PressedY+1);
	fill(LMB_PressedX-1, LMB_PressedY);
	fill(LMB_PressedX, LMB_PressedY-1);
	fill(LMB_PressedX+1, LMB_PressedY);	 			
	

}			

//Function for setting colour
void drawingColour(void)
{
	/*Function sets the colour for drawing pencil*/
 SDL_SetRenderDrawColor(renderer, colour.red, colour.green, colour.blue, 0xFF);
}
//Function for eraser colour
void eraserColour(void)
{
	/*Function sets the colour for the eraser*/
SDL_SetRenderDrawColor(renderer, colour1.red, colour1.green, colour1.blue, 0xFF);
}



//FUNCTIONS FOR TOOLBARS:
//Function for creating left toolbar
void leftToolbar(void)
{
	/*Function creates the textures from the surfaces on the left toolbar*/
	leftToolbarSurface = SDL_LoadBMP("colour_palette_dark.bmp");
	leftToolbarTexture = SDL_CreateTextureFromSurface(renderer, leftToolbarSurface);
	leftToolbarSurface = SDL_LoadBMP("colour_palette_bright.bmp");
	leftToolbarTextureButton = SDL_CreateTextureFromSurface(renderer, leftToolbarSurface);
	leftToolbarSurface = SDL_LoadBMP("colourWheel.bmp");
	colourWheelTexture = SDL_CreateTextureFromSurface(renderer, leftToolbarSurface);	
	SDL_SetRenderTarget(renderer, blankTexture);
					
}
//Function for creating bars
void bars(void)
{/*Function creates the textures from the surfaces for the bars beetwon toolbars and drawing space*/
	barsSurface = SDL_LoadBMP("bar_left.bmp");
	leftBarTexture = SDL_CreateTextureFromSurface(renderer, barsSurface); 	
	barsSurface =  SDL_LoadBMP("bar_right.bmp");
	rightBarTexture = SDL_CreateTextureFromSurface(renderer, barsSurface); 
	barsSurface =  SDL_LoadBMP("bar_bottom.bmp");
	bottomBarTexture = SDL_CreateTextureFromSurface(renderer, barsSurface); 
	barsSurface =  SDL_LoadBMP("bar_small_bottom.bmp");
	bottomSmallBarTexture = SDL_CreateTextureFromSurface(renderer, barsSurface); 
	barsSurface =  SDL_LoadBMP("bar_small_right.bmp");
	rightSmallBarTexture = SDL_CreateTextureFromSurface(renderer, barsSurface); 	
}
//Function for creating top toolbar
void topToolbar(void)
{  /*Function creates the textures from the surfaces on the left toolbar*/
	topToolbarSurface = SDL_LoadBMP("toolbar_top_final.bmp");
	topToolbarTexture = SDL_CreateTextureFromSurface(renderer, topToolbarSurface);
	topToolbarSurface = SDL_LoadBMP("all_icons.bmp"); 	
	topToolbarTextureBright = SDL_CreateTextureFromSurface(renderer, topToolbarSurface);
	topToolbarSurface = SDL_LoadBMP("window_top_dark.bmp"); 
	topToolbarMenuTexture = SDL_CreateTextureFromSurface(renderer, topToolbarSurface);
	topToolbarSurface = SDL_LoadBMP("window_top_bright.bmp"); 
	topToolbarMenuBrightTexture = SDL_CreateTextureFromSurface(renderer, topToolbarSurface);
	topToolbarSurface = SDL_LoadBMP("all_icons_no_light.bmp"); 
	topToolbarDarkTexture = SDL_CreateTextureFromSurface(renderer, topToolbarSurface);
}
//Function for running top toolbar

int topToolbarProc(int mouseMotionX, int mouseMotionY, int option, int leftMouseButtonDown, int LMB_PressedX, int LMB_PressedY, SDL_Rect rect)
{	/*Function creates the small squeres on the buttons if the mouse motion is on the exact button*/
	
	int width = 54;
	int height = 54;
	int startPointX = 38;
	int startPointY = 9;
	SDL_SetRenderDrawColor(renderer,0x70 ,0xB9, 0xFF, 0xFF);   					
	for(int i=0; i<10; i++)
	{/*First loop is for the bigger buttons(50x50px)*/
		if(i==2) startPointX = 159;
		else if(i == 4) startPointX = 406;
		else if(i == 5) startPointX = 750;
		else if(i == 6) startPointX = 937;
		else if(i>6) startPointX += 66;							
		SDL_Rect secondRect = {startPointX, startPointY, width, height};		
		if((mouseMotionX>startPointX) && (mouseMotionX<startPointX + height) &&(mouseMotionY>startPointY) && (mouseMotionY<startPointY+height)) SDL_RenderDrawRect(renderer, &secondRect);
				
		if(i<6) startPointX += 60;
	}

		
	width = 31;
	height = 31;
	startPointX = 282;
	startPointY = 6;
	for(int i=0; i<2; i++)
	{/*The second loop is for the smaller buttons(25x25px). It creates two rectangles on the first two buttons (first up button and (after first row in the nex loop) down button)*/
		startPointX = 282;	
		SDL_Rect secondRect = {startPointX+1, startPointY+1, width-2, height-2};		
		if((mouseMotionX>startPointX) && (mouseMotionX<startPointX + height) &&(mouseMotionY>startPointY) && (mouseMotionY<startPointY+height)) SDL_RenderDrawRect(renderer, &secondRect);		
		
		startPointY += 29;
		for(int j=0; j<11; j++)
		{/*In this part function creates rectangles on all left buttons in a row*/					
			SDL_Rect secondRect = {startPointX+1, startPointY-28, width-2, height-2};			
			if((mouseMotionX>startPointX) && (mouseMotionX<startPointX + height) && (mouseMotionY>startPointY-29) && (mouseMotionY<startPointY-29+height)) SDL_RenderDrawRect(renderer, &secondRect);																																																		
			if(j==1) startPointX = 335;
			else if(j==3) startPointX = 467;
			else if(j==6) startPointX = 603;		
			else if(j==2 || (j>3 && j<6) || (j>6 && j<10)) startPointX += 33;													
		}			
	}							
	height = 31;
	startPointX = 822;
	startPointY = 3;	
	for(int i=0; i<2; i++)
	{/*The third loop is for the left 6 smaller buttons(25x25px) which are not highlighted becase the are not used. First it creates two rectangles on the first two buttons (up and down)*/						
		startPointX = 821;	
		SDL_Rect secondRect = {startPointX+1, startPointY+1, width-2, height-2};
		if((mouseMotionX>startPointX) && (mouseMotionX<startPointX + height) &&(mouseMotionY>startPointY) && (mouseMotionY<startPointY+height)) SDL_RenderDrawRect(renderer, &secondRect);											
		startPointY += 36;
		for(int j=0; j<3; j++)
		{/*In this loop creates rectangles on all the left buttons in a row*/						
			SDL_Rect secondRect = {startPointX+1, startPointY-35, width-2, height-2};			
			if((mouseMotionX>startPointX) && (mouseMotionX<startPointX + height) && (mouseMotionY>startPointY-29) && (mouseMotionY<startPointY-29+height)) SDL_RenderDrawRect(renderer, &secondRect);																	
			startPointX += 37;				
		}	
	}	
		width = 29;
		height = 54;
		startPointX = 569;
		startPointY = 9;	// Here is the creation of the line button which has different dimentions.
		SDL_Rect secondRect = {startPointX, startPointY, width, height};	
		if((mouseMotionX>startPointX) && (mouseMotionX<startPointX + width) &&(mouseMotionY>startPointY) && (mouseMotionY<startPointY+height)) SDL_RenderDrawRect(renderer, &secondRect);
		if((mouseMotionX>startPointX) && (mouseMotionX<startPointX+width) && (mouseMotionY>startPointY) && (mouseMotionY<startPointY+height) && (leftMouseButtonDown))
		{
			SDL_Rect buttons = {startPointX+2, startPointY+2, width-4, height-4};
		}				
	return 1;				
}
//Function for buttons backlight
void backlightButtons(int mouseMotionX, int mouseMotionY, int LMB_PressedX, int LMB_PressedY, int leftMouseButtonDown)
{/*Function connects the buttons with appropriate functions and replaced appropriate texture when button is pressed*/
	/*--------------------IN THIS PART ARE CONDITIONS FOR PLACES FOR BUTTONS AND FOR APPROPRIATE VARIABLES------------------------*/
	if((mouseMotionX>338) && (mouseMotionX<363) && (mouseMotionY>38) && (mouseMotionY<63) && (leftMouseButtonDown))
	{
		option = PICKER;
		
	} 
	if((mouseMotionX>338) && (mouseMotionX<363) && (mouseMotionY>9) && (mouseMotionY<34) && (leftMouseButtonDown))
	{
		option = PAINT;
		optionChecker=8;
	}
	if((mouseMotionX>752) && (mouseMotionX<802) && (mouseMotionY>11) && (mouseMotionY<61) && (leftMouseButtonDown))
	{
		option=BUCKET;
		optionChecker=6;
	}
	if((mouseMotionX>371) && (mouseMotionX<396) && (mouseMotionY>9) && (mouseMotionY<34) && (leftMouseButtonDown))
	{
		option=PENCIL;
		optionChecker=0;
	}
	if((mouseMotionX>571) && (mouseMotionX<596) && (mouseMotionY>11) && (mouseMotionY<61) && (leftMouseButtonDown))
	{
		option=DRAW_LINE;
		optionChecker=1;
	}
	if((mouseMotionX>503) && (mouseMotionX<528) && (mouseMotionY>9) && (mouseMotionY<34) && (leftMouseButtonDown))
	{
		option=DRAW_RECTANGLE;
		outline=1;
		filled=0;
		optionChecker=2;
	}
	if((mouseMotionX>503) && (mouseMotionX<528) && (mouseMotionY>38) && (mouseMotionY<63) && (leftMouseButtonDown))
	{
		option=DRAW_RECTANGLE;
		filled=1;
		outline=0;
		optionChecker=2;
	}
	if((mouseMotionX>470) && (mouseMotionX<495) && (mouseMotionY>9) && (mouseMotionY<34) && (leftMouseButtonDown))
	{
		option=DRAW_CIRCLE;
		outline=1;
		filled=0;
		optionChecker=3;
	}
	if((mouseMotionX>470) && (mouseMotionX<495) && (mouseMotionY>38) && (mouseMotionY<63) && (leftMouseButtonDown))
	{
		option=DRAW_CIRCLE;
		filled=1;
		outline=0;
		optionChecker=3;
	}

	if((mouseMotionX>536) && (mouseMotionX<561) && (mouseMotionY>9) && (mouseMotionY<34) && (leftMouseButtonDown))
	{
		option=DRAW_SQUERE;
		outline=1;
		filled=0;
		optionChecker=4;
	}
	if((mouseMotionX>536) && (mouseMotionX<561) && (mouseMotionY>38) && (mouseMotionY<63) && (leftMouseButtonDown))
	{
		option=DRAW_SQUERE;
		filled=1;
		outline=0;
		optionChecker=4;
	}
	if((mouseMotionX>606) && (mouseMotionX<631) && (mouseMotionY>9) && (mouseMotionY<34) && (leftMouseButtonDown))
	{
		option=DRAW_TRIANGLE;
		triangleChecker=1;
		optionChecker=5;
	}
	if((mouseMotionX>606) && (mouseMotionX<631) && (mouseMotionY>38) && (mouseMotionY<63) && (leftMouseButtonDown))
	{
		option=DRAW_TRIANGLE;
		triangleChecker=3;
		optionChecker=5;
	}
	if((mouseMotionX>639) && (mouseMotionX<664) && (mouseMotionY>9) && (mouseMotionY<34) && (leftMouseButtonDown))
	{
		option=DRAW_TRIANGLE;
		triangleChecker=2;
		optionChecker=5;
	}
	if((mouseMotionX>639) && (mouseMotionX<664) && (mouseMotionY>38) && (mouseMotionY<63) && (leftMouseButtonDown))
	{
		option=DRAW_TRIANGLE;
		triangleChecker=4;
		optionChecker=5;
	}
	if((mouseMotionX>100) && (mouseMotionX<150) && (mouseMotionY>11) && (mouseMotionY<61) && (leftMouseButtonDown))
	{	
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &saveAsRect, &saveAsRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {100, 11, 50, 50};
		SDL_RenderDrawRect(renderer, &highlightRect);			
		count = 0;
		saveFile();		
	}
	if((mouseMotionX>161) && (mouseMotionX<211) && (mouseMotionY>11) && (mouseMotionY<61) && (leftMouseButtonDown))
	{		
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &saveRect,  &saveRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {161, 11, 50, 50};
		SDL_RenderDrawRect(renderer, &highlightRect);				
		saveFile();				
	}
	if((mouseMotionX>221) && (mouseMotionX<271) && (mouseMotionY>11) && (mouseMotionY<61) && (leftMouseButtonDown))
	{					
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &uploadRect, &uploadRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {221, 11, 50, 50};
		SDL_RenderDrawRect(renderer, &highlightRect);
		uploadFile();		
	}
	if((mouseMotionX>371) && (mouseMotionX<396) && (mouseMotionY>38) && (mouseMotionY<63) && (leftMouseButtonDown))
	{
		option=ERASER;
		optionChecker=9;
	}
	if((mouseMotionX>40) && (mouseMotionX<90) && (mouseMotionY>11) && (mouseMotionY<63) && (leftMouseButtonDown))
	{
		newFile();
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &newFileRect,  &newFileRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {40, 11, 50, 50};
		SDL_RenderDrawRect(renderer, &highlightRect);
	}
	/*------------------------IN THIS PART ARE CONDITIONS FOR DISPLAYING TEXTURES AND HIGHLIGHTED RECTANGLES------------------------------*/
	/*------------------------fIRST THE HIGHLIGHTED TEXTURE IS DISPAYED AND SUBSEQUENTLY THE RECTANGLE-----------------------------------------*/
	if(dropDownMenuOn==1)
	{
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &sizeRect, &sizeRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {408, 11, 50, 50 };
		SDL_RenderDrawRect(renderer, &highlightRect);
	}
	else if(option == BUCKET)
	{
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &bucketRect, &bucketRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {752, 11, 50, 50};
		SDL_RenderDrawRect(renderer, &highlightRect);
	}

	 if(option == PICKER)
	 {
		  
	  SDL_RenderCopy(renderer, topToolbarTextureBright,  &pickerRect, & pickerRectToolbar);
	  SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
	  SDL_Rect highlightRect = {338, 38, 25, 25 };
	  SDL_RenderDrawRect(renderer, &highlightRect);
	}
	else if(option==ERASER)
	{
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &rubberRect, &rubberRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {371, 38, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect);
	}	 
	else if(option==DRAW_CIRCLE && filled==1)
	{
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &filledCircleRect, &filledCircleRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {470, 38, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect);
	}
	else if(option==DRAW_RECTANGLE && filled==1)
	{
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &filledRectangleRect, &filledRectangleRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {503, 38, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect);
	}
	else if(option==DRAW_SQUERE && filled==1)
	{
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &filledSquereRect, &filledSquereRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {536, 38, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect);
	}
	
	else if(option==DRAW_TRIANGLE && triangleChecker==3)
	{
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &filledTriangleLeftRect, &filledTriangleLeftRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {606, 38, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect);
	}
	else if(option==DRAW_TRIANGLE && triangleChecker==4)
	{
	SDL_RenderCopy(renderer, topToolbarTextureBright,  &filledTriangleTopRect, &filledTriangleTopRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {639, 38, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect);
	}

	else if(option == PAINT)
	{
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &brushRect, &brushRectToolbar);
	 	SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {338, 9, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect);
	}
	else if(option==PENCIL)
	{
	 SDL_RenderCopy(renderer, topToolbarTextureBright,  &pencilRect, &pencilRectToolbar);
			SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {371, 9, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect);
	}
	else if(option==DRAW_CIRCLE && outline==1 )
	{
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &outlinedCircleRect, &outlinedCircleRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {470, 9, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect);
	}
	else if(option==DRAW_RECTANGLE && outline==1)
	{
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &outlinedRectangleRect, &outlinedRectangleRectToolbar);
	 	SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {503, 9, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect);
    }
	else if(option==DRAW_SQUERE && outline==1) 
	{
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &outlinedSquereRect, &outlinedSquereRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {536, 9, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect);
    }
	else if(option==DRAW_TRIANGLE && triangleChecker==1) 
	{
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &outlinedTriangleLeftRect, &outlinedTriangleLeftRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {606, 9, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect);
    }
	else if(option==DRAW_TRIANGLE && triangleChecker==2) 
	{
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &outlinedTriangleTopRect, &outlinedTriangleTopRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {639, 9, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect);
	}

	else if(option==DRAW_LINE)
	{
		SDL_RenderCopy(renderer, topToolbarTextureBright,  &lineRect, &lineRectToolbar);
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_Rect highlightRect = {571, 11, 25, 50 };
		SDL_RenderDrawRect(renderer, &highlightRect);
    }
	

}

//Function for drop down menu
void dropDownMenu(int mouseMotionX, int mouseMotionY, int option, int leftMouseButtonDown, int LMB_PressedX, int LMB_PressedY)
{/* In this function the drop down menu is created on the top toolbar*/
	if((!((LMB_PressedX>398) && (LMB_PressedX<468) && (LMB_PressedY>80) && (LMB_PressedY<570)) && !((LMB_PressedX<TOOLBAR_LEFT_WIDTH) && (LMB_PressedY>TOOLBAR_TOP_HEIGHT)) && (leftMouseButtonDown) && (openCloseMenu == 1)))
	{/*If drop down menu was open and there was a click somewhere else then on its surface, the drop down menu is replaced by the previous texture which is held in 'replaced texture'*/									
		SDL_RenderCopy(renderer, replacedTexture, NULL, NULL);
		openCloseMenu = 0;
		locked = 0;	
		dropDownMenuOn = 0;							
	}
		
	if((LMB_PressedX>407) && (LMB_PressedX<458) && (LMB_PressedY>10) && (LMB_PressedY<61) && leftMouseButtonDown && openCloseMenu == 0)	
	{/*The replacement texture is created for holding the data which whre on the drawing space before the drop down menu was opened*/								
		SDL_SetRenderTarget(renderer, replacedTexture);
		SDL_RenderCopy(renderer, blankTexture, NULL, NULL);
		SDL_SetRenderTarget(renderer, blankTexture);					
		SDL_Rect rect1 = {398, 80, 70, 490};
		SDL_RenderCopy(renderer, topToolbarMenuTexture, NULL, &rect1);
		openCloseMenu = 1;
		locked = 1;
		dropDownMenuOn = 1;																					
	}
	
	if(openCloseMenu == 1)
	{/*Different sizes are changing in a case of clicking on the appropriate place on the drop down menu */		
		SDL_Rect rectToolbar = {398, 80, 70, 490};
		SDL_RenderCopy(renderer, blankTexture, NULL, NULL);
		if((mouseMotionX>398) && (mouseMotionX<469) && (mouseMotionY>80) && (mouseMotionY<134) && leftMouseButtonDown) size = 1;				
		else if((mouseMotionX>398) && (mouseMotionX<469) && (mouseMotionY>134) && (mouseMotionY<188) && leftMouseButtonDown) size = 2;
		else if((mouseMotionX>398) && (mouseMotionX<469) && (mouseMotionY>188) && (mouseMotionY<242) && leftMouseButtonDown) size = 3;	
		else if((mouseMotionX>398) && (mouseMotionX<469) && (mouseMotionY>242) && (mouseMotionY<296) && leftMouseButtonDown) size = 4;	
		else if((mouseMotionX>398) && (mouseMotionX<469) && (mouseMotionY>296) && (mouseMotionY<350) && leftMouseButtonDown) size = 5;	
		else if((mouseMotionX>398) && (mouseMotionX<469) && (mouseMotionY>350) && (mouseMotionY<405) && leftMouseButtonDown) size = 6;	
		else if((mouseMotionX>398) && (mouseMotionX<469) && (mouseMotionY>405) && (mouseMotionY<460) && leftMouseButtonDown) size = 7;
		else if((mouseMotionX>398) && (mouseMotionX<469) && (mouseMotionY>460) && (mouseMotionY<515) && leftMouseButtonDown) size = 8;	
		else if((mouseMotionX>398) && (mouseMotionX<469) && (mouseMotionY>515) && (mouseMotionY<570) && leftMouseButtonDown) size = 9;								 				
		/*Highlighted textures are displaced when there is a click on the drop down menu*/
		if(size == 1)
		{
		SDL_RenderCopy(renderer, topToolbarMenuTexture, NULL, &rectToolbar);
		SDL_Rect rect1 = {0, 0, 70, 54};
		SDL_Rect shapeRect = {398,80, 70, 54};					
		SDL_RenderCopy(renderer, topToolbarMenuBrightTexture, &rect1, &shapeRect);
		}
		else if(size == 2)
		{
			SDL_RenderCopy(renderer, topToolbarMenuTexture, NULL, &rectToolbar);
			SDL_Rect rect1 = {0, 54, 70, 54};	
			SDL_Rect shapeRect = {398, 134, 70, 54};				
			SDL_RenderCopy(renderer, topToolbarMenuBrightTexture, &rect1, &shapeRect);
		}
		else if(size == 3)
		{		
			SDL_RenderCopy(renderer, topToolbarMenuTexture, NULL, &rectToolbar);
			SDL_Rect rect1 = {0, 108, 70, 54};	
			SDL_Rect shapeRect = {398, 188, 70, 54};				
			SDL_RenderCopy(renderer, topToolbarMenuBrightTexture, &rect1, &shapeRect);				
		}
		else if(size == 4)
		{			
			SDL_RenderCopy(renderer, topToolbarMenuTexture, NULL, &rectToolbar);
			SDL_Rect rect1 = {0, 162, 70, 54};	
			SDL_Rect shapeRect = {398, 242, 70, 54};				
			SDL_RenderCopy(renderer, topToolbarMenuBrightTexture, &rect1, &shapeRect);
		}
		else if(size == 5)
		{			
			SDL_RenderCopy(renderer, topToolbarMenuTexture, NULL, &rectToolbar);
			SDL_Rect rect1 = {0, 216, 70, 54};	
			SDL_Rect shapeRect = {398, 296, 70, 54};				
			SDL_RenderCopy(renderer, topToolbarMenuBrightTexture, &rect1, &shapeRect);
		}
		else if(size == 6)
		{			
			SDL_RenderCopy(renderer, topToolbarMenuTexture, NULL, &rectToolbar);
			SDL_Rect rect1 = {0, 270, 70, 55};	
			SDL_Rect shapeRect = {398, 350, 70, 55};				
			SDL_RenderCopy(renderer, topToolbarMenuBrightTexture, &rect1, &shapeRect);
		}
		else if(size == 7)
		{			
			SDL_RenderCopy(renderer, topToolbarMenuTexture, NULL, &rectToolbar);
			SDL_Rect rect1 = {0, 325, 70, 55};	
			SDL_Rect shapeRect = {398, 405, 70, 55};				
			SDL_RenderCopy(renderer, topToolbarMenuBrightTexture, &rect1, &shapeRect);			
		}
		else if(size == 8)
		{
			SDL_RenderCopy(renderer, topToolbarMenuTexture, NULL, &rectToolbar);
			SDL_Rect rect1 = {0, 380, 70, 55};	
			SDL_Rect shapeRect = {398, 460, 70, 55};				
			SDL_RenderCopy(renderer, topToolbarMenuBrightTexture, &rect1, &shapeRect);			
		}
		else if(size ==9)
		{			
			SDL_RenderCopy(renderer, topToolbarMenuTexture, NULL, &rectToolbar);
			SDL_Rect rect1 = {0, 435, 70, 55};	
			SDL_Rect shapeRect = {398, 515, 70, 55};				
			SDL_RenderCopy(renderer, topToolbarMenuBrightTexture, &rect1, &shapeRect);
		}															
	}							
}
	
//Function for running left toolbar
int leftToolbarProc(int mouseMotionX, int mouseMotionY, int option, int leftMouseButtonDown, int LMB_PressedX, int LMB_PressedY, SDL_Rect rect)
{	/*----------------------FUNCTION FOR HIGHLIGHTING COLOUR PALETTE ON THE LEFT TOOLBAR AND CONNECTING IT WITH COLOUR PICKER FUNCTION-----------------------------*/
	SDL_RenderCopy(renderer, leftToolbarTexture, NULL, &rectColourPalette);
	int width = 36;
	int height = 36;
	int startPointX = 28; 
	int startPointY = TOOLBAR_TOP_HEIGHT + 20;
	SDL_SetRenderDrawColor(renderer, 0x70 ,0xB9, 0xFF, 0xFF);   
    
	for(int j=0; j<20; j++)
	{	/*First in this loop are created three lines for the firs rectangle*/			
		startPointX = 28;
		SDL_Rect shapeRect = {startPointX, startPointY, width, height};
		
		if((mouseMotionX>startPointX) && (mouseMotionX<startPointX + height) && (mouseMotionY>startPointY) && (mouseMotionY<startPointY+height)) SDL_RenderDrawRect(renderer, &shapeRect);
				
		SDL_Rect secondRect = {startPointX+1, startPointY+1, width-2, height-2};
		SDL_SetRenderDrawColor(renderer,0xC6 ,0xEB, 0xEA, 0xFF);
		if((mouseMotionX>startPointX) && (mouseMotionX<startPointX + height) &&(mouseMotionY>startPointY) && (mouseMotionY<startPointY+height)) SDL_RenderDrawRect(renderer, &secondRect);
			
							
		SDL_Rect thirdRect = {startPointX+2, startPointY+2, width-4, height-4};
		if((mouseMotionX>startPointX) && (mouseMotionX<startPointX + height) && (mouseMotionY>startPointY) && (mouseMotionY<startPointY+height)) 
		{
		SDL_RenderDrawRect(renderer, &thirdRect);
			if(leftMouseButtonDown && (mouseMotionX>startPointX+2) && (mouseMotionX<startPointX+2 + height-4) && (mouseMotionY>startPointY+2) && (mouseMotionY<startPointY+2+height-4))
			{/*Here is the condition for colour picker*/
				if(optionChecker==9)
				{/*colour 1 == eraser colour*/			
					colour1 = getPixelColour(LMB_PressedX, LMB_PressedY, &colour1);
					eraserColour();	
				}
				else
				{/*colour 2 == drawing colour*/
					colour = getPixelColour(LMB_PressedX, LMB_PressedY, &colour);
					drawingColour();
				}
				
				/*Function allow get the colour even if before the colour picker button was clicked*/
							
				if(((mouseMotionX<TOOLBAR_LEFT_WIDTH) && (mouseMotionY>80)) && (leftMouseButtonDown))
				{				
					option=optionChecker;									
					if(checkPicker)	
					{
						if(leftMouseButtonDown)
						{
							if(optionChecker==9)
							{
								colour1 = getPixelColour(LMB_PressedX, LMB_PressedY, &colour1);
							}
							else
							{
								colour = getPixelColour(LMB_PressedX, LMB_PressedY, &colour);
							}
					
						}			
					}
				}	
			}		
		}							
		SDL_SetRenderDrawColor(renderer,0x70 ,0xB9, 0xFF, 0xFF);							
		startPointY += 40;										
		for(int i=0; i<5; i++)
		{	/*Highlighting in a row*/							
			SDL_Rect shapeRect = {startPointX, startPointY-40, width, height};
			SDL_Rect secondRect = {startPointX+1, startPointY-39, width-2, height-2};
			SDL_Rect thirdRect = {startPointX+2, startPointY-38, width-4, height-4};
								
			if((mouseMotionX>startPointX) && (mouseMotionX<startPointX + height) &&(mouseMotionY>startPointY-40) && (mouseMotionY<startPointY-40+height)) SDL_RenderDrawRect(renderer, &shapeRect);
										
			SDL_SetRenderDrawColor(renderer,0xC6 ,0xEB, 0xEA, 0xFF);
			if((mouseMotionX>startPointX) && (mouseMotionX<startPointX + height) && (mouseMotionY>startPointY-40) && (mouseMotionY<startPointY-40+height)) SDL_RenderDrawRect(renderer, &secondRect);			
			if((mouseMotionX>startPointX) && (mouseMotionX<startPointX + height) &&(mouseMotionY>startPointY-40) && (mouseMotionY<startPointY-40+height))
			{
				SDL_RenderDrawRect(renderer, &thirdRect);
				if(leftMouseButtonDown && (mouseMotionX>startPointX+2) && (mouseMotionX<startPointX+2 + height-4) && (mouseMotionY>startPointY-38) && (mouseMotionY<startPointY-38+height-4))
				{/*The same colour picker condition as in the first loop. It allows to get the colour from the all small rectangles*/
					if(optionChecker==9)
					{			
						colour1 = getPixelColour(LMB_PressedX, LMB_PressedY, &colour1);
						eraserColour();	
					}
					else
					{
						colour = getPixelColour(LMB_PressedX, LMB_PressedY, &colour);
						drawingColour();
					}
					
								
					if(((mouseMotionX<TOOLBAR_LEFT_WIDTH) && (mouseMotionY>80)) && (leftMouseButtonDown))
					{	
						option=optionChecker;
						if(checkPicker)	
						{
							if(leftMouseButtonDown)
							{
								if(optionChecker==9)
									{
									colour1 = getPixelColour(LMB_PressedX, LMB_PressedY, &colour1);
									}
								else
								{
								colour = getPixelColour(LMB_PressedX, LMB_PressedY, &colour);
								}		
							}

						}		
					}	
				}		
			}
				SDL_SetRenderDrawColor(renderer,0x70 ,0xB9, 0xFF, 0xFF);
				startPointX += 38;											
		}
	}
	return 1;
}								
//Function for left toolbar button
int leftToolbarButton(int mouseMotionX, int mouseMotionY, int leftMouseButtonDown, SDL_Rect rect)
{
	/*Function calls the colour texture to replace standard dark texture. It also makes highlight on the button by creating the rectangles(similar like on the palette with colours */
	
	
	if((mouseMotionX>START_POINTX_COLOUR_BUTTON) && (mouseMotionX<START_POINTX_COLOUR_BUTTON+COLOUR_BUTTON_WIDTH) && (mouseMotionY>START_POINTY_COLOUR_BUTTON) && (mouseMotionY<START_POINTY_COLOUR_BUTTON+COLOUR_BUTTON_HEIGHT) && !(leftMouseButtonDown))
	{
	
		
		SDL_RenderCopy(renderer, leftToolbarTextureButton, NULL, &rectColourPalette);//Coping the bright texture on the screen
		SDL_Rect colourButton = {START_POINTX_COLOUR_BUTTON, START_POINTY_COLOUR_BUTTON, COLOUR_BUTTON_WIDTH, COLOUR_BUTTON_HEIGHT};
		if((mouseMotionX>START_POINTX_COLOUR_BUTTON) && (mouseMotionX<START_POINTX_COLOUR_BUTTON  + COLOUR_BUTTON_WIDTH) && (mouseMotionY>START_POINTY_COLOUR_BUTTON) && (mouseMotionY<START_POINTY_COLOUR_BUTTON+COLOUR_BUTTON_HEIGHT) && !(leftMouseButtonDown))
		{			
			SDL_RenderDrawRect(renderer, &colourButton);
			SDL_SetRenderDrawColor(renderer,0xC6 ,0xEB, 0xEA, 0xFF);
			SDL_Rect colourButton1 = {START_POINTX_COLOUR_BUTTON+1, START_POINTY_COLOUR_BUTTON+1, COLOUR_BUTTON_WIDTH-2, COLOUR_BUTTON_HEIGHT-2};
			SDL_RenderDrawRect(renderer, &colourButton1);
			SDL_Rect colourButton2 = {START_POINTX_COLOUR_BUTTON+2, START_POINTY_COLOUR_BUTTON+2, COLOUR_BUTTON_WIDTH-4, COLOUR_BUTTON_HEIGHT-4};
			SDL_RenderDrawRect(renderer, &colourButton2);												
		}
	}
	return 1;
}
//Fuction for running colour wheel window

void colourWheel(int LMB_PressedX, int LMB_PressedY, int mouseMotionX, int mouseMotionY, int leftMouseButtonDown)
{	//Function allows to use the wheel colour picker//							
	
	if((!((LMB_PressedX>601) && (LMB_PressedX<1050) && (LMB_PressedY>276) && (LMB_PressedY<725)) && !((LMB_PressedX<TOOLBAR_LEFT_WIDTH) && (LMB_PressedY>TOOLBAR_TOP_HEIGHT)) && (leftMouseButtonDown) && (OpenCloseColourWindow ==1)  ))
	{	/*If there was a click some where besides the window, the texture is replaced by the previous texture*/														
		SDL_RenderCopy(renderer, replacedTexture, NULL, NULL);	
		OpenCloseColourWindow = 0;
		locked = 0;	
		colourWheelOn = 0;							
	}
							
	if((LMB_PressedX>START_POINTX_COLOUR_BUTTON) && (LMB_PressedX<START_POINTX_COLOUR_BUTTON + COLOUR_BUTTON_WIDTH) && (LMB_PressedY>START_POINTY_COLOUR_BUTTON) && (LMB_PressedY<START_POINTY_COLOUR_BUTTON+COLOUR_BUTTON_HEIGHT) && leftMouseButtonDown && OpenCloseColourWindow ==0 && !(openCloseMenu==1))
	{	/*Creation of the replace texture for holding the data*/
		
		
				
		SDL_SetRenderTarget(renderer, replacedTexture);		
		SDL_RenderCopy(renderer, blankTexture, NULL, NULL);
		SDL_SetRenderTarget(renderer, blankTexture);
							
		SDL_Rect rect1 = {START_POINTX_COLOUR_WINDOW, START_POINTY_COLOUR_WINDOW, COLOUR_WINDOW_SIZE, COLOUR_WINDOW_SIZE};
		
		SDL_RenderCopy(renderer, colourWheelTexture, NULL, &rect1);
	
		
		
		SDL_RenderCopy(renderer, colourWheelTexture, NULL, &rect1);
	
		
		OpenCloseColourWindow = 1;
		locked = 1;
		colourWheelOn = 1;		
	}	
	if(colourWheelOn== 1)
	{
		if(((mouseMotionX - 826)*(mouseMotionX- 826) + (mouseMotionY - 483)*(mouseMotionY - 483)<= 172*172) && (leftMouseButtonDown))
		{/*The possibility of choosing the colour*/
			SDL_Rect colourTest = {644, 676, 364, 22 };
			if(optionChecker==9)
				{			
					colour1 = getPixelColour(mouseMotionX, mouseMotionY, &colour1);
					eraserColour();	
					SDL_RenderFillRect(renderer, &colourTest);
				}
				else
				{
					colour = getPixelColour(mouseMotionX, mouseMotionY, &colour);
					drawingColour();
					SDL_RenderFillRect(renderer, &colourTest);
				}								
		}
	}		
						
}
//Function for colour boxes on top toolbar
void colourBoxes(void)
{/*It allows to display both colours - for drawing and eraser*/
	SDL_Rect drawingColourRect = {1236,12, 106,48};
	SDL_Rect rubberColourRect = {1366, 12, 106, 48};
	drawingColour();
	SDL_RenderFillRect(renderer, &drawingColourRect);
	eraserColour();	
	SDL_RenderFillRect(renderer, &rubberColourRect);
	drawingColour();
	
}
void userInterface(int mouseMotionX, int mouseMotionY, int LMB_PressedX, int LMB_PressedY, int leftmouseButtonDown)
{/*Function for dispaying bars and buttons darker textures for buttons which has not been used in the program yet*/
		SDL_Rect leftBarRect = {250, 80, 50, TOOLBAR_LEFT_HEIGHT};
		SDL_Rect rightBarRect = {DRAWING_SPACE_WIDTH + TOOLBAR_LEFT_WIDTH, 80, 50, TOOLBAR_LEFT_HEIGHT};
		SDL_Rect bottomBarRect = {TOOLBAR_LEFT_WIDTH ,950 ,DRAWING_SPACE_WIDTH ,50};		
		SDL_Rect rightSmallBarRect = {1650, 80, 25, 945};					
		SDL_Rect bottomSmallBarRect = {0, HEIGHT-25, WIDTH, 25};
		
		SDL_RenderCopy(renderer, leftBarTexture, NULL, &leftBarRect);
		SDL_RenderCopy(renderer, rightBarTexture, NULL, &rightBarRect);		
		SDL_RenderCopy(renderer, bottomBarTexture, NULL, &bottomBarRect);
		SDL_RenderCopy(renderer, rightSmallBarTexture, NULL, &rightSmallBarRect);	 
		SDL_RenderCopy(renderer, bottomSmallBarTexture, NULL, &bottomSmallBarRect);
		SDL_RenderCopy(renderer, leftToolbarTexture, NULL, &rectColourPalette);
		SDL_RenderCopy(renderer, topToolbarTexture, NULL, &topToolbarRect);
		backlightButtons(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY, leftMouseButtonDown);
		colourBoxes();
		
		
		SDL_SetRenderDrawColor(renderer,0x00 ,0x00, 0x00, 0xFF);
		SDL_RenderCopy(renderer,topToolbarDarkTexture,  &moveRect, &moveRectToolbar);
		SDL_Rect highlightRect = {939, 11, 50, 50 };
		SDL_RenderDrawRect(renderer, &highlightRect);
		
		SDL_RenderCopy(renderer, topToolbarDarkTexture,  &rotateRect, &rotateRectToolbar);
		SDL_Rect highlightRect1 = {1005, 11, 50, 50};
		SDL_RenderDrawRect(renderer, &highlightRect1);
		
		SDL_RenderCopy(renderer, topToolbarDarkTexture,  &flipVerticalRect, &flipVerticalRectToolbar);
		SDL_Rect highlightRect2 = {1071, 11, 50, 50};
		SDL_RenderDrawRect(renderer, &highlightRect2);
		
		SDL_RenderCopy(renderer, topToolbarDarkTexture,  &flipHotizonlatRect, &flipHotizonlatRectToolbar);
		SDL_Rect highlightRect3 = {1137, 11, 50, 50};
		SDL_RenderDrawRect(renderer, &highlightRect3);
		
		SDL_RenderCopy(renderer, topToolbarDarkTexture,  &undoRect, &undoRectToolbar);
		SDL_Rect highlightRect4 = {285, 9, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect4);
		
		SDL_RenderCopy(renderer, topToolbarDarkTexture,  &redoRect, &redoRectToolbar);
		SDL_Rect highlightRect5 = {285, 38, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect5);

		SDL_RenderCopy(renderer, topToolbarDarkTexture,  &filledUpsideDownTriangleLeftRect, &filledUpsideDownTriangleLeftRectToolbar);
		SDL_Rect highlightRect6 = {672, 38, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect6);
		
		SDL_RenderCopy(renderer, topToolbarDarkTexture,  &filledUpsideDownTriangleTopRect, &filledUpsideDownTriangleTopRectToolbar);
		SDL_Rect highlightRect7 = {705, 38, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect7);
		
		SDL_RenderCopy(renderer, topToolbarDarkTexture,  &outlinedUpsideDownTriangleLeftRect, &outlinedUpsideDownTriangleLeftRectToolbar);
		SDL_Rect highlightRect8 = {672, 38, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect8);
		
		SDL_RenderCopy(renderer, topToolbarDarkTexture,  &outlinedUpsideDownTriangleTopRect, &outlinedUpsideDownTriangleTopRectToolbar);
		SDL_Rect highlightRect9 = {705, 38, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect9);
		
		SDL_RenderCopy(renderer, topToolbarDarkTexture,  &copyRect, &copyRectToolbar);
		SDL_Rect highlightRect10 = {824, 6, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect10);
		
		SDL_RenderCopy(renderer, topToolbarDarkTexture,  &pasteRect, &pasteRectToolbar);
		SDL_Rect highlightRect11 = {824, 42, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect11);
		
		SDL_RenderCopy(renderer, topToolbarDarkTexture,  &cutRect, &cutRectToolbar);
		SDL_Rect highlightRect12 = {861, 42, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect12);
		
		SDL_RenderCopy(renderer, topToolbarDarkTexture,  &scaleRect, &scaleRectToolbar);
		SDL_Rect highlightRect13 = {898, 42, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect13);
		
		SDL_RenderCopy(renderer, topToolbarDarkTexture,  &selectRect, &selectRectToolbar);
		SDL_Rect highlightRect14 = {861, 6, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect14);
		
		SDL_RenderCopy(renderer, topToolbarDarkTexture,  &selectAllRect, &selectAllRectToolbar);
		SDL_Rect highlightRect15 = {861, 6, 25, 25 };
		SDL_RenderDrawRect(renderer, &highlightRect15);
		
}


//Function for rubber banding
void rubberBand( int LMB_PressedX, int LMB_PressedY, int mouseMotionX, int mouseMotionY)
{/* Function is using another texture for replacing the screen during using different functions*/
	if((LMB_PressedX == mouseMotionX) && (LMB_PressedY == mouseMotionY))
	{				
		SDL_SetRenderTarget(renderer, replacedTexture);
		SDL_RenderCopy(renderer, blankTexture, NULL, NULL);
		SDL_SetRenderTarget(renderer, blankTexture);
	}
			
	SDL_RenderCopy(renderer, replacedTexture, NULL, NULL);
} 
//Function for pencil

void basicDrawing(int mouseMotionX, int mouseMotionY, int LMB_PressedX, int LMB_PressedY)
{/*Function draw basic line like a pencil - it draws the points and connects the with the line in order to avoid the breaks between the points*/	
	if((LMB_PressedX == mouseMotionX) && (LMB_PressedY == mouseMotionY))
	{
		SDL_RenderDrawPoint(renderer, LMB_PressedX, LMB_PressedY);
		oldMouseX = LMB_PressedX;
		oldMouseY = LMB_PressedY;
	}
	else
	{   										
		SDL_RenderDrawLine(renderer, oldMouseX, oldMouseY, mouseMotionX, mouseMotionY);				
		oldMouseX = mouseMotionX;
		oldMouseY = mouseMotionY;
	}
}	
//Function for drawing straight line
void drawStraightLine(int mouseMotionX, int mouseMotionY, int LMB_PressedX, int LMB_PressedY)
{		
	rubberBand(LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY);
	SDL_RenderDrawLine(renderer, LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY);	        
}

//Function for drawing rectangle
void rectangle(int mouseMotionX, int mouseMotionY, int LMB_PressedX, int LMB_PressedY, int outline, int filled)
{
					
	rubberBand(LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY);	
	 
	if((mouseMotionX!=LMB_PressedX) && (mouseMotionY!=LMB_PressedY))
	{
		SDL_Rect shapeRect = {LMB_PressedX, LMB_PressedY, mouseMotionX-LMB_PressedX, mouseMotionY-LMB_PressedY};
		if(outline)//outlined rectangle
			SDL_RenderDrawRect(renderer, &shapeRect);
		else if(filled)//filled rectangle
			SDL_RenderFillRect(renderer, &shapeRect);
	}
}
//Function for drawing squere
void drawSquere(int mouseMotionX, int mouseMotionY, int LMB_PressedX, int LMB_PressedY, int outline, int filled)
{	
	rubberBand(LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY);
	/*the function accept different arguments according to the direction of drawing the squere. The data is changing because  SDL_Rect always start drawing the rectangle from the left up corner*/	
	if((mouseMotionX!=LMB_PressedX) && (mouseMotionY!=LMB_PressedY))
	{
		if((mouseMotionX>LMB_PressedX) && (mouseMotionY>LMB_PressedY))
		{
			
			
			x = LMB_PressedX;
			y = LMB_PressedY;
			width = mouseMotionY-LMB_PressedY;
			SDL_Rect shapeRect = {x, y, width, width};

			if(outline)//outline squere
				SDL_RenderDrawRect(renderer, &shapeRect);
			else if(filled)//filled squere
				SDL_RenderFillRect(renderer, &shapeRect);
		}
		else if((mouseMotionX>LMB_PressedX) && (mouseMotionY<LMB_PressedY))
		{
			x = LMB_PressedX;
			y = LMB_PressedY - (LMB_PressedY-mouseMotionY); 
			width = LMB_PressedY-mouseMotionY;
			SDL_Rect shapeRect = {x, y, width, width};
			
			if(outline)
				SDL_RenderDrawRect(renderer, &shapeRect);
			else if(filled)
				SDL_RenderFillRect(renderer, &shapeRect);
		}
		else if((LMB_PressedX>mouseMotionX) && (LMB_PressedY>mouseMotionY))
		{
			x = LMB_PressedX-(LMB_PressedY-mouseMotionY);
			y = LMB_PressedY - (LMB_PressedY-mouseMotionY); 
			width = LMB_PressedY-mouseMotionY;
			SDL_Rect shapeRect = {x, y, width, width};
			
			if(outline)
				SDL_RenderDrawRect(renderer, &shapeRect);
			else if(filled)
				SDL_RenderFillRect(renderer, &shapeRect);
		}				
		else if((LMB_PressedX>mouseMotionX) && (mouseMotionY>LMB_PressedY))
		{			
			x = LMB_PressedX-(mouseMotionY- LMB_PressedY);
			y = LMB_PressedY; 
			width = mouseMotionY-LMB_PressedY;
			SDL_Rect shapeRect = {x, y, width, width};
		
			if(outline)
				SDL_RenderDrawRect(renderer, &shapeRect);
			else if(filled)
				SDL_RenderFillRect(renderer, &shapeRect);
		}
		
	}/*after every fuction where mouseMotion might not be beyond the drawing space the userInterface function is used to avoid showing the shapes on the interface*/
	userInterface(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY, leftMouseButtonDown);
}
//Function for drawing circle

void circle(int mouseMotionX, int mouseMotionY, int LMB_PressedX, int LMB_PressedY, int outline, int filled)
{
		
	rubberBand(LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY);											
	float radius = squereRoot((LMB_PressedX-mouseMotionX)*(LMB_PressedX-mouseMotionX) + (LMB_PressedY-mouseMotionY)*(LMB_PressedY-mouseMotionY))/2;
	if(pointCircle==1)
	{
		p = (LMB_PressedX +mouseMotionX)/2;
		q = (LMB_PressedY+mouseMotionY)/2;
			
	}
	else if(pointCircle==2)
	{
		p = LMB_PressedX;
		q = LMB_PressedY;
		radius = radius*2;
	}
	int x = 0, y = radius;
	int dp = 1 - radius;
		if(outline)	
		{	
			do /*The part of the code (and in every place where it repeats in the program) is based on the code from the webside: https://en.wikipedia.org/wiki/Midpoint_circle_algorithm*/
			{
				SDL_RenderDrawPoint(renderer, p - x, q + y);
				SDL_RenderDrawPoint(renderer, p + x, q - y);
				SDL_RenderDrawPoint(renderer, p + y, q + x);
				SDL_RenderDrawPoint(renderer, p - y, q - x);
				if (dp < 0)
					dp = dp + 2 * (++x) + 3;
				else
					dp = dp + 2 * (++x) - 2 * (--y) + 5;
			
					//For the 8 octants
			//	SDL_RenderDrawPoint(renderer, p - x, q + y);
				SDL_RenderDrawPoint(renderer, p + x, q - y);          
				SDL_RenderDrawPoint(renderer, p - x, q - y);
				SDL_RenderDrawPoint(renderer, p + y, q + x);
				SDL_RenderDrawPoint(renderer, p - y, q + x);
				SDL_RenderDrawPoint(renderer, p + y, q - x);
				SDL_RenderDrawPoint(renderer, p - y, q - x);
				SDL_RenderDrawPoint(renderer, p + x, q + y);
				SDL_RenderDrawPoint(renderer, p - y, q - x);
							
			} 
			while (x < y);	
		
		}
		else if(filled)
		{
				
			do
			{
				SDL_RenderDrawPoint(renderer, p - x, q + y);
				SDL_RenderDrawPoint(renderer, p + x, q - y);
				SDL_RenderDrawPoint(renderer, p + y, q + x);
				SDL_RenderDrawPoint(renderer, p - y, q - x);
				if (dp < 0)
					dp = dp + 2 * (++x) + 3;
				else
					dp = dp + 2 * (++x) - 2 * (--y) + 5;
					
			     //For the 8 octants
		//	SDL_RenderDrawPoint(renderer, p - x, q + y);
				SDL_RenderDrawPoint(renderer, p + x, q - y);
				SDL_RenderDrawPoint(renderer, p - x, q - y);
				SDL_RenderDrawPoint(renderer, p + y, q + x);
				SDL_RenderDrawPoint(renderer, p - y, q + x);
				SDL_RenderDrawPoint(renderer, p + y, q - x);
				SDL_RenderDrawPoint(renderer, p - y, q - x);
				SDL_RenderDrawPoint(renderer, p + x, q + y);
				SDL_RenderDrawPoint(renderer, p - y, q - x);
			
				
				/*in order to fill the circle I drew the lines between the exact points and one extra line for the middle because there was a left space*/		
				SDL_RenderDrawLine(renderer, p+x, q+y,p+x,q-y );
				SDL_RenderDrawLine(renderer, p+y, q+x, p+y, q-x);	
				SDL_RenderDrawLine(renderer, p-x, q+y, p-x, q-y);
				SDL_RenderDrawLine(renderer, p-y, q+x, p-y, q-x);
				//SDL_RenderDrawLine(renderer, p+x, q+y, p-x, q-y);
				SDL_RenderDrawLine(renderer, p, q+y, p, q-y);
			
									
			} 
			while (x < y);									
		}	
		
		userInterface(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY, leftMouseButtonDown);
		
}	


//Function for circle with chosen radius
void circleRadius(float radius, int LMB_PressedX, int LMB_PressedY )
{/*Function differs from the previous one in radius which is the parameter of the function, the function is used in the next fuctions*/
	int p = LMB_PressedX;
	int q = LMB_PressedY;
	if(size==1) radius=2;
	if(size==2) radius=3;
	if(size==3) radius=6;
	if(size==4) radius=10;
	if(size==5) radius=15; 		 
	if(size==6) radius=20;
	if(size==7) radius=25; 
	if(size==8) radius=30; 
	if(size==9) radius=35;	 
			
		int x = 0, y = radius;
		int dp = 1 - radius;
		do
			{
				SDL_RenderDrawPoint(renderer, p - x, q + y);
				SDL_RenderDrawPoint(renderer, p + x, q - y);
				SDL_RenderDrawPoint(renderer, p + y, q + x);
				SDL_RenderDrawPoint(renderer, p - y, q - x);
				if (dp < 0)
					dp = dp + 2 * (++x) + 3;
				else
					dp = dp + 2 * (++x) - 2 * (--y) + 5;
					
			     //For the 8 octants
				SDL_RenderDrawPoint(renderer, p + x, q - y);
				SDL_RenderDrawPoint(renderer, p - x, q - y);
				SDL_RenderDrawPoint(renderer, p + y, q + x);
				SDL_RenderDrawPoint(renderer, p - y, q + x);
				SDL_RenderDrawPoint(renderer, p + y, q - x);
				SDL_RenderDrawPoint(renderer, p - y, q - x);
				SDL_RenderDrawPoint(renderer, p + x, q + y);
				SDL_RenderDrawPoint(renderer, p - y, q - x);
			
									
				SDL_RenderDrawLine(renderer, p+x, q+y,p+x,q-y );
				SDL_RenderDrawLine(renderer, p+y, q+x, p+y, q-x);	
				SDL_RenderDrawLine(renderer, p-x, q+y, p-x, q-y);
				SDL_RenderDrawLine(renderer, p-y, q+x, p-y, q-x);
				SDL_RenderDrawLine(renderer, p, q+y, p, q-y);												
			} 
			while (x < y);	
}			
 
 void bresenhamLine(int LMB_PressedX, int LMB_PressedY, int mouseMotionX, int mouseMotionY)
{/*The function code is based on the code from the 19 presentation of Dr Eike Falk Anderson*/
	/*Istead of dots, it draws filled circles for brushes and thick lines for shapes*/
	int radius;
	int dx = abs(mouseMotionX-LMB_PressedX), sx = LMB_PressedX<mouseMotionX ? 1 : -1;
	int dy = abs(mouseMotionY- LMB_PressedY), sy = LMB_PressedY<mouseMotionY ? 1 : -1;
	int error = (dx>dy ? dx : -dy)/2, e2;
	while(1)
	{
		if(size==1) radius=2;
		if(size==2) radius=3;
		if(size==3) radius=6;
		if(size==4) radius=10;
		if(size==5) radius=15; 		 
		if(size==6) radius=20;
		if(size==7) radius=25; 
		if(size==8) radius=30; 
		if(size==9) radius=35;	 
		
				
		circleRadius(radius, LMB_PressedX, LMB_PressedY);			
		if(LMB_PressedX==mouseMotionX && LMB_PressedY==mouseMotionY) break;
		e2 = error;
		if(e2>-dx) {error -= dy; LMB_PressedX +=sx;}
		if(e2<dy) { error += dx; LMB_PressedY += sy;}
	}					
}

void circleThickness(int mouseMotionX, int mouseMotionY, int LMB_PressedX, int LMB_PressedY)
{/*Function for thicker circle, it uses the brehensamLine function for drawing the circles instead of dots*/
	
	rubberBand(LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY);											
	float radius = squereRoot((LMB_PressedX-mouseMotionX)*(LMB_PressedX-mouseMotionX) + (LMB_PressedY-mouseMotionY)*(LMB_PressedY-mouseMotionY))/2;
	if(pointCircle==1)
	{
		p = (LMB_PressedX +mouseMotionX)/2;
		q = (LMB_PressedY+mouseMotionY)/2;
			
	}
	else if(pointCircle==2)
	{
		p = LMB_PressedX;
		q = LMB_PressedY;
		radius = radius*2;
	}
	
	
	int x = 0, y = radius;
	int dp = 1 - radius;
	
	int radius1;						
			do
			{
				circleRadius(radius, p - x, q + y);
				circleRadius(radius, p + x, q - y);
				circleRadius(radius, p + y, q + x);
				circleRadius(radius, p - y, q - x);
				if (dp < 0)
					dp = dp + 2 * (++x) + 3;
				else
					dp = dp + 2 * (++x) - 2 * (--y) + 5;
			
					//For the 8 octants
				circleRadius(radius, p + x, q - y);          
				circleRadius(radius, p - x, q - y);
				circleRadius(radius, p + y, q + x);
				circleRadius(radius, p - y, q + x);
				circleRadius(radius, p + y, q - x);
				circleRadius(radius, p - y, q - x);
				circleRadius(radius, p + x, q + y);
				circleRadius(radius, p - y, q - x);
	
			} 
			while (x < y);	
	
		
}


//Function for different brushes
void basicPainting(int mouseMotionX, int mouseMotionY, int LMB_PressedX, int LMB_PressedY)
{/*Function for brushes uses brehensamLine for drawing different sized dots/circles. It is connected with this function to avoid the breaks between the dots.*/
	float radius;
	if((LMB_PressedX == mouseMotionX) && (LMB_PressedY == mouseMotionY))
	{
				
		oldMouseX = LMB_PressedX;
		oldMouseY = LMB_PressedY;										
	}	
	else
	{  
		bresenhamLine(oldMouseX, oldMouseY, mouseMotionX, mouseMotionY);				
	
		oldMouseX = mouseMotionX;
		oldMouseY = mouseMotionY;
	}		
}
//Function for eraser 
void eraser(int mouseMotionX, int mouseMotionY, int RMB_PressedX, int RMB_PressedY)
{/*The fuction is based on the basicPainting fuunction*/
	eraserColour();	
	float radius;
	if((RMB_PressedX == mouseMotionX) && (RMB_PressedY == mouseMotionY))
	{
				
		oldMouseX = RMB_PressedX;
		oldMouseY = RMB_PressedY;										
	}	
	else
	{  
		bresenhamLine(oldMouseX, oldMouseY, mouseMotionX, mouseMotionY);				
	
		oldMouseX = mouseMotionX;
		oldMouseY = mouseMotionY;
	}	
	drawingColour();	
}

//Function for rectangle thickennes
void rectangleThickness(int mouseMotionX, int mouseMotionY, int LMB_PressedX, int LMB_PressedY)
{
	/*The function draws 4 connected lines and uses bresenhamLine for differen sizes for contours*/
	rubberBand(LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY);			
	 bresenhamLine(LMB_PressedX, LMB_PressedY, mouseMotionX, LMB_PressedY);			 
	 bresenhamLine(mouseMotionX, LMB_PressedY, mouseMotionX, mouseMotionY);
	 bresenhamLine(mouseMotionX, mouseMotionY, LMB_PressedX, mouseMotionY);
	 bresenhamLine(LMB_PressedX, mouseMotionY, LMB_PressedX, LMB_PressedY);	 	
}
void squereThickness(int mouseMotionX, int mouseMotionY, int LMB_PressedX, int LMB_PressedY)
{ /*Function required two cases for different directions*/
	rubberBand(LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY);
	
	
	 if(((LMB_PressedX<mouseMotionX) && (LMB_PressedY<mouseMotionY)) || ((LMB_PressedX>mouseMotionX) && (LMB_PressedY>mouseMotionY)))
	 {		
		bresenhamLine(LMB_PressedX, LMB_PressedY, mouseMotionX, LMB_PressedY);			 
		bresenhamLine(mouseMotionX, LMB_PressedY, mouseMotionX,LMB_PressedY +(mouseMotionX-LMB_PressedX) );
		bresenhamLine(mouseMotionX, LMB_PressedY +(mouseMotionX-LMB_PressedX), LMB_PressedX, LMB_PressedY +(mouseMotionX-LMB_PressedX));
		bresenhamLine(LMB_PressedX, LMB_PressedY +(mouseMotionX-LMB_PressedX), LMB_PressedX, LMB_PressedY);	 
		}
		
	else
	{
		bresenhamLine(LMB_PressedX, LMB_PressedY, mouseMotionX, LMB_PressedY);			 
		bresenhamLine(mouseMotionX, LMB_PressedY, mouseMotionX,  LMB_PressedY +(LMB_PressedX-mouseMotionX));
		bresenhamLine(mouseMotionX, LMB_PressedY +(LMB_PressedX-mouseMotionX), LMB_PressedX,  LMB_PressedY +(LMB_PressedX-mouseMotionX));
		bresenhamLine(LMB_PressedX,  LMB_PressedY +(LMB_PressedX-mouseMotionX), LMB_PressedX, LMB_PressedY);
	}	 	
		userInterface(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY, leftMouseButtonDown);
}	
//Function for drawing triangle
void triangle(int LMB_PressedX, int LMB_PressedY, int mouseMotionX, int mouseMotionY)
{	
	rubberBand(LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY);
	if(triangleChecker==1)	
	{/*Triangle which has the start point at the side*/
		SDL_RenderDrawLine(renderer, LMB_PressedX, LMB_PressedY, mouseMotionX+(mouseMotionX-LMB_PressedX), LMB_PressedY);
		SDL_RenderDrawLine(renderer, LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY);
		SDL_RenderDrawLine(renderer, mouseMotionX+(mouseMotionX-LMB_PressedX), LMB_PressedY, mouseMotionX, mouseMotionY);
	}
	else if(triangleChecker==3)
	{
		if(LMB_PressedX<mouseMotionX)
		{/*Different cases for drawing different directions of drawing the filled triangle. Filled triangled which has the start point from the side it is right angle triangle */
		
		int endPoint = mouseMotionX+(mouseMotionX-LMB_PressedX);
			if(LMB_PressedY>mouseMotionY)
			{
				while((LMB_PressedX)!= (mouseMotionX+(mouseMotionX-LMB_PressedX)))
				{
						SDL_RenderDrawLine(renderer, LMB_PressedX, LMB_PressedY, endPoint, LMB_PressedY);
						LMB_PressedX += 1;
						LMB_PressedY -= 1;
						endPoint -= 1;	
				}	
			}																				
				else if(LMB_PressedY<mouseMotionY)
				{
					while((LMB_PressedX)!= (mouseMotionX+(mouseMotionX-LMB_PressedX)))
					{
					SDL_RenderDrawLine(renderer, LMB_PressedX, LMB_PressedY, endPoint, LMB_PressedY);
					LMB_PressedX += 1;
					LMB_PressedY += 1;
					endPoint -= 1;		
					}
				}
		}				
			else if(LMB_PressedX>mouseMotionX)	
			{
				if(LMB_PressedY>mouseMotionY)
				{
				int endPoint = (mouseMotionX-(LMB_PressedX-mouseMotionX));
				while((LMB_PressedX)!= (mouseMotionX+(mouseMotionX-LMB_PressedX)))
				{
					SDL_RenderDrawLine(renderer, LMB_PressedX, LMB_PressedY, endPoint, LMB_PressedY);
					LMB_PressedX -= 1;
					LMB_PressedY -= 1;
					endPoint += 1;
				}
			}
			else if(LMB_PressedY<mouseMotionY)
			{
				  
				int endPoint = (mouseMotionX-(LMB_PressedX-mouseMotionX));
				while((LMB_PressedX)!= (mouseMotionX+(mouseMotionX-LMB_PressedX)))
				{
					SDL_RenderDrawLine(renderer, LMB_PressedX, LMB_PressedY, endPoint, LMB_PressedY);
					LMB_PressedX -= 1;
					LMB_PressedY += 1;
					endPoint += 1;
				}			  
			}
		}		
	}									
	else if(triangleChecker==2)
	{/*Triangle which has the start point at the vertex*/
		SDL_RenderDrawLine(renderer, LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY);
		SDL_RenderDrawLine(renderer, LMB_PressedX, LMB_PressedY, LMB_PressedX+(LMB_PressedX-mouseMotionX), mouseMotionY);
		SDL_RenderDrawLine(renderer, mouseMotionX, mouseMotionY, LMB_PressedX+(LMB_PressedX-mouseMotionX), mouseMotionY );
	}	
	else if(triangleChecker==4)
	{
		if(LMB_PressedX<mouseMotionX)
		{/*Filled triangle is the same like the outlined one it has two cases for directions*/
			SDL_RenderDrawLine(renderer, LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY);
			SDL_RenderDrawLine(renderer, LMB_PressedX, LMB_PressedY, LMB_PressedX+(LMB_PressedX-mouseMotionX), mouseMotionY);
			SDL_RenderDrawLine(renderer, mouseMotionX, mouseMotionY, LMB_PressedX+(LMB_PressedX-mouseMotionX), mouseMotionY);	
			int endPoint = mouseMotionX-2*(mouseMotionX-LMB_PressedX);	
			while((endPoint)!= (mouseMotionX))
				{
					SDL_RenderDrawLine(renderer, LMB_PressedX, LMB_PressedY, endPoint, mouseMotionY);					
					endPoint += 1;		
				}		
		}
		else if(LMB_PressedX>mouseMotionX)
		{			
			SDL_RenderDrawLine(renderer, LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY);
			SDL_RenderDrawLine(renderer, LMB_PressedX, LMB_PressedY, LMB_PressedX+(LMB_PressedX-mouseMotionX), mouseMotionY);
			SDL_RenderDrawLine(renderer, mouseMotionX, mouseMotionY, LMB_PressedX+(LMB_PressedX-mouseMotionX), mouseMotionY);
			int endPoint = mouseMotionX+2*(LMB_PressedX-mouseMotionX);	
			while((endPoint)!= (mouseMotionX))
			{
					SDL_RenderDrawLine(renderer, LMB_PressedX, LMB_PressedY, endPoint, mouseMotionY);					
					endPoint -= 1;
			}
		}									
	}
	userInterface(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY, leftMouseButtonDown);
}


int main(int argc, char *argv[])
{	
	//SDL Initialization
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL could not be initialize! SDL Error: %s\n", SDL_GetError());
		return -1;
	}
	
	//Create window
	window = SDL_CreateWindow("Draw & Paint",  200, 80, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if(window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return -1;		
	}
	
	//Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	//Create textures
	createTextures();
	
	//Create colour for background
    SDL_SetRenderTarget(renderer, blankTexture);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    colour1.red = 0xFF;
    colour1.green = 0xFF;
    colour1.blue = 0xFF;
    //Render Clear		
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    //Variables
    //go: duration of the program
	int go = 1;
	//Variables for pressed left and right mouse buttons.
	int LMB_PressedX = 0, LMB_PressedY = 0, LMB_ReleasedX = 0, LMB_ReleasedY = 0;		
	int RMB_PressedX = 0, RMB_PressedY = 0, RMB_ReleasedX = 0, RMB_ReleasedY = 0;  
	//Variables for mouse moustion coordinates X and Y.
	int mouseMotionX = 0, mouseMotionY = 0;
	int oldMouseX = LMB_PressedX;
	int oldMouseY = LMB_PressedY;
	int fillShape=0;
	int newFileRun=0;
			
	   		
	//Create UI
	leftToolbar();
	bars();
	topToolbar();

	size = 1;
	//Main running loop
	while(go)
	{
		SDL_Event event;
		
		while (SDL_PollEvent(&event))
		{
			switch(event.type)			
			{			
				case SDL_MOUSEBUTTONUP:
					if(event.button.button == SDL_BUTTON_LEFT)
					{   						
						leftMouseButtonDown = 0;
						leftMouseButtonUp = 1;
						rightMouseButtonDown = 0;
						rightMouseButtonUp = 1;
						SDL_GetMouseState(&LMB_ReleasedX, &LMB_ReleasedY);
					}
									
					else if(event.button.button == SDL_BUTTON_RIGHT) 
					{
						leftMouseButtonDown = 0;
						leftMouseButtonUp = 1;
						rightMouseButtonDown = 0;
						rightMouseButtonUp = 1;
						SDL_GetMouseState(&RMB_ReleasedX, &RMB_ReleasedY); 
					}
					break;																
				case SDL_MOUSEBUTTONDOWN:
				
					if(event.button.button == SDL_BUTTON_LEFT)
					{						
						leftMouseButtonDown = 1;
						leftMouseButtonUp = 0;
						rightMouseButtonDown = 0;
						rightMouseButtonUp = 1;
						SDL_GetMouseState(&LMB_PressedX, &LMB_PressedY);
					}
					else if(event.button.button == SDL_BUTTON_RIGHT)
					{
						leftMouseButtonDown = 0;
						leftMouseButtonUp = 1;
						rightMouseButtonDown = 1;
						rightMouseButtonUp = 0;
						SDL_GetMouseState(&RMB_PressedX, &RMB_PressedY);
				    }
								
				case SDL_MOUSEMOTION:
								
					mouseMotionX = event.motion.x;
					mouseMotionY = event.motion.y;																							    																	
				
					break;
																											
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_p)
					{
						option = PENCIL;				
					}
					else if(event.key.keysym.sym == SDLK_g)
					{
						newFileChecker=1;		
					}	
					else if(event.key.keysym.sym == SDLK_l)
					{
						option = DRAW_LINE;
					}			
					
					else if(event.key.keysym.sym == SDLK_r)
					{			
						option = DRAW_RECTANGLE;
					
					}
					else if(event.key.keysym.sym == SDLK_c)
					{
						option = DRAW_CIRCLE;
	
					} 
					else if(event.key.keysym.sym == SDLK_e)
					{
						option = DRAW_SQUERE;

					}
					else if(event.key.keysym.sym == SDLK_h)
					{
						option = BUCKET;
							
					}
					else if(event.key.keysym.sym == SDLK_t)	
					{
						option=DRAW_TRIANGLE;
					}	
					if(event.key.keysym.sym == SDLK_o)
					{
						outline = 1;
						filled = 0;
					}
					else if(event.key.keysym.sym == SDLK_f)
					{
						outline = 0;
						filled = 1;
					}
					if(event.key.keysym.sym == SDLK_s)
					{
						saveFile();
						saveFileChecker=0;
																								
					}	
					else if(event.key.keysym.sym == SDLK_a)
					{
						
						count = 0;
						saveFile();
						saveAsFileChecker=0;
						
						
						break;
					}	
					else if(event.key.keysym.sym == SDLK_u)	
					{
						uploadFileChecker=1;
						
					}
					else if(event.key.keysym.sym == SDLK_z)
					{
						triangleChecker=1;
					}
					else if(event.key.keysym.sym == SDLK_i)
					{
						option = PICKER;	
					}
					else if(event.key.keysym.sym == SDLK_b)
					{
						option = PAINT;	
					}	
					else if(event.key.keysym.sym == SDLK_m)
					{
						pointCircle = 2;
						triangleChecker=4;
					}
					else if(event.key.keysym.sym == SDLK_n)
					{
						pointCircle = 1; 
						triangleChecker=3;	
					}
					else if(event.key.keysym.sym == SDLK_x)
					{
						triangleChecker=2;
					}		
					
					if(event.key.keysym.sym == SDLK_1)
						size = 1;
					else if(event.key.keysym.sym == SDLK_2)
						size = 2;
					else if(event.key.keysym.sym == SDLK_3)
						size = 3;
					else if(event.key.keysym.sym == SDLK_4)
						size = 4;
					else if(event.key.keysym.sym == SDLK_5)
						size = 5;
					else if(event.key.keysym.sym == SDLK_6)
						size = 6;
					else if(event.key.keysym.sym == SDLK_7)
						size = 7;
					else if(event.key.keysym.sym == SDLK_8)
						size = 8;
					else if(event.key.keysym.sym == SDLK_9)
						size = 9;
														
					break;
																															
				case SDL_QUIT:
					go=0;
					break;
			}
	    }
		//Displaing the interface	    
	    SDL_SetRenderTarget(renderer, blankTexture);
	    leftToolbarProc(mouseMotionX, mouseMotionY, option, leftMouseButtonDown, LMB_PressedX, LMB_PressedY, rectColourPalette);   	    
		leftToolbarButton(mouseMotionX, mouseMotionY, leftMouseButtonDown, rectColourPalette);				
		colourWheel(LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY, leftMouseButtonDown);	
		dropDownMenu(mouseMotionX, mouseMotionY, option, leftMouseButtonDown, LMB_PressedX, LMB_PressedY);						
		SDL_RenderCopy(renderer, topToolbarTexture, NULL, &topToolbarRect);
		userInterface(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY, leftMouseButtonDown);
		 		
		topToolbarProc(mouseMotionX, mouseMotionY, option, leftMouseButtonDown, LMB_PressedX, LMB_PressedY, topToolbarRect);	

	    //Create colour for drawing 	    
	    drawingColour();
	    //Cases for drawing functions and the drawing area
		if(leftMouseButtonDown && cursorInDrawingSpace(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY) == 1 && locked == 0)
		{
			if(mouseMotionX>TOOLBAR_LEFT_WIDTH + DRAWING_SPACE_WIDTH)
			{
					mouseMotionX = TOOLBAR_LEFT_WIDTH + DRAWING_SPACE_WIDTH;
				
				if (mouseMotionY<TOOLBAR_TOP_HEIGHT)
				{
					mouseMotionY = TOOLBAR_TOP_HEIGHT+1;
				}
				else if(mouseMotionY>TOOLBAR_TOP_HEIGHT + DRAWING_SPACE_HEIGHT)
				{
					mouseMotionY = TOOLBAR_TOP_HEIGHT + DRAWING_SPACE_HEIGHT;
				}
			}
			else if(mouseMotionX<TOOLBAR_LEFT_WIDTH)
			{
				mouseMotionX = TOOLBAR_LEFT_WIDTH+1;
				if(mouseMotionY<TOOLBAR_TOP_HEIGHT)
				{
					mouseMotionY = TOOLBAR_TOP_HEIGHT+1;
				}
				else if((mouseMotionY>TOOLBAR_TOP_HEIGHT + DRAWING_SPACE_HEIGHT))
				{
					mouseMotionY = TOOLBAR_TOP_HEIGHT + DRAWING_SPACE_HEIGHT;
				}
			}
			else if(mouseMotionY<TOOLBAR_TOP_HEIGHT)
			{
			mouseMotionY = TOOLBAR_TOP_HEIGHT+1;
			}	
			else if(mouseMotionY>TOOLBAR_TOP_HEIGHT + DRAWING_SPACE_HEIGHT)
			{
			mouseMotionY = TOOLBAR_TOP_HEIGHT + DRAWING_SPACE_HEIGHT;
			}					
			switch (option)	
			{
				case PENCIL : basicDrawing(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY); optionChecker = 0;									
					break;
	
				case DRAW_LINE : 
				if(size==1)
				{		
						drawStraightLine(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY);
				}
				else
					
				{	 rubberBand(LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY);
					 bresenhamLine(LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY);
				}
				optionChecker = 1;
					break;
				
				case DRAW_RECTANGLE : 
				if(size==1 && (filled || outline))
				{
					
					rectangle(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY, outline, filled); 
				}
				else if(!(size==1 || filled))	
				{
					
					rectangleThickness(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY);
				}		
				optionChecker = 2;	
					
																										
				break;
	
				case DRAW_CIRCLE : 
				if(size==1 && (filled || outline))
				{		
					circle(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY, outline, filled);
					
				}
				else if(!(size==1 || filled))
				{		
					circleThickness(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY);
					
				}	
				
				optionChecker = 3;	
					break;
		
				case DRAW_SQUERE :	
				if(size==1 && (filled || outline))
				{	
					drawSquere(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY, outline, filled); 
				}
				else if(!(size==1 || filled))
				{
					
				     squereThickness(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY);
				}
				
				optionChecker = 4;								
					break;
	
				case DRAW_TRIANGLE : drawingColour(); triangle(LMB_PressedX, LMB_PressedY, mouseMotionX, mouseMotionY); optionChecker=5;
					break;
		
				case BUCKET :
				    optionChecker = 2; 
					src = getPixelColour(LMB_PressedX, LMB_PressedY, &src); 
					dst = colour;
					fill(LMB_PressedX, LMB_PressedY);
					break;	
				case PAINT : optionChecker = 8; basicPainting(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY); break;	
				case ERASER: optionChecker = 9; eraser(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY);  break;
				
			}					
		}
		if(option==PICKER)
		{
			if(leftMouseButtonDown && cursorInDrawingSpace(mouseMotionX, mouseMotionY, LMB_PressedX, LMB_PressedY) == 1 && locked == 0)
			{
			if(optionChecker==9)
				{			
				 colour1 = getPixelColour(LMB_PressedX, LMB_PressedY, &colour1);
				 eraserColour();	
				}
				else
				{
				colour = getPixelColour(LMB_PressedX, LMB_PressedY, &colour);
				drawingColour();
				}
			}
			if(!((mouseMotionX>338) && (mouseMotionX<363) && (mouseMotionY>38) && (mouseMotionY<63)) && (leftMouseButtonDown)) option=optionChecker;
			else checkPicker=1;						
			}
						
		
		if(rightMouseButtonDown && (RMB_PressedX>TOOLBAR_LEFT_WIDTH) && (RMB_PressedX<TOOLBAR_LEFT_WIDTH + DRAWING_SPACE_WIDTH) && (RMB_PressedY>TOOLBAR_TOP_HEIGHT) && (RMB_PressedY<TOOLBAR_TOP_HEIGHT + DRAWING_SPACE_HEIGHT) && locked == 0)
		{
			eraser(mouseMotionX, mouseMotionY, RMB_PressedX, RMB_PressedY);
			if(!rightMouseButtonDown) break;
			
		}
		
		if(newFileChecker)
		{
			newFile();
			newFileChecker=0;	
		}		
		if(uploadFileChecker)
		{
			uploadFile();
			uploadFileChecker=0;
		}
	   
	 	SDL_SetRenderTarget(renderer, NULL);	 	
		SDL_RenderCopy(renderer, blankTexture, NULL, NULL);		
		SDL_RenderPresent(renderer);		
		SDL_Delay(1);

	}	
	SDL_DestroyWindow( window );
	window = NULL;
	SDL_Quit();
	return 0;	
}	






















	
	


