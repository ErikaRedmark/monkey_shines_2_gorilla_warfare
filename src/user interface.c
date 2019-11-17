#include	"globals extern.h"void	DrawControlScreen( Boolean  ){	Rect	tempRect;	PicHandle tempPict;	// draw the picture offscreen	SetGWorld(theGWorlds.screenGWorld, nil);	UseResFile(theFileStuff.GWDataFileNumber);	tempPict = GetPicture(3000);	if (tempPict == nil) FatalError("Error loading PICT 3000", true);	SetRect(&tempRect, 0, 0, 640, 480);	DrawPicture(tempPict, &tempRect);	ReleaseResource((Handle)tempPict);	PreloadBlackBox();	ForeColor(blackColor);	SetGWorld(&theUserInterface.myCWindowRecord.port, GetMainDevice());	UseResFile(theFileStuff.GWDataFileNumber);	theUserInterface.newGameButton = GetNewControl(128, theUserInterface.theWindow);	theUserInterface.highScoresButton = GetNewControl(129, theUserInterface.theWindow);	theUserInterface.quitButton = GetNewControl(130,theUserInterface.theWindow);	theUserInterface.controlsButton = GetNewControl(131, theUserInterface.theWindow);	theUserInterface.helpButton = GetNewControl(132, theUserInterface.theWindow);	theUserInterface.musicButton = GetNewControl(137, theUserInterface.theWindow);	theUserInterface.soundButton = GetNewControl(138, theUserInterface.theWindow);	UseResFile(theFileStuff.applicationFile);			// copy it to the screen	FlashFadeIn();	StoreMenuBar();			ShowControl(theUserInterface.newGameButton);	ShowControl(theUserInterface.highScoresButton);	ShowControl(theUserInterface.quitButton);	ShowControl(theUserInterface.controlsButton);	ShowControl(theUserInterface.helpButton);	ShowControl(theUserInterface.musicButton);	ShowControl(theUserInterface.soundButton);	#ifdef	DEMO	if (theUserInterface.registerInfoButton) ShowControl(theUserInterface.registerInfoButton);	#else	if (theUserInterface.enterCodeButton) ShowControl(theUserInterface.enterCodeButton);	#endif	CustomShowCursor();}void	PreloadBlackBox(void){	PicHandle tempPict;	Rect	tempRect;	#ifdef DEMO			// buttons		UseResFile(theFileStuff.GWDataFileNumber);		tempPict = GetPicture(904);		if (tempPict == nil) FatalError("Error loading PICT 904", true);		SetRect(&tempRect, 320, 300, 600, 450);		DrawPicture(tempPict, &tempRect);		ReleaseResource((Handle)tempPict);		theUserInterface.registerInfoButton = GetNewControl(134, theUserInterface.theWindow);		UseResFile(theFileStuff.applicationFile);		#else	short	width;	Str32	tempString;			CheckRegistration();		tempPict = GetPicture(904 + (gRegistered > 0));		SetRect(&tempRect, 320, 300, 600, 450);		DrawPicture(tempPict, &tempRect);		ReleaseResource((Handle)tempPict);		if (gRegistered == 135)		{			// write name						#ifdef	DEMO			theUserInterface.registerInfoButton = 0;			#else			theUserInterface.enterCodeButton = 0;			#endif			ForeColor(blackColor);			TextFace(bold);			MoveTo(372, 420);			width = StringWidth((**gPrefs).registerName);						if (width > 192)			{				CopyString(tempString, (**gPrefs).registerName);								while (StringWidth(tempString) > 192) tempString[0]--;								DrawString(tempString);				tempString[0] = (**gPrefs).registerName[0];					}			else DrawString((**gPrefs).registerName);		}		else theUserInterface.enterCodeButton = GetNewControl(135, theUserInterface.theWindow);		UseResFile(theFileStuff.applicationFile);	#endif	// will this work if the window isn't the current drawing port ?}void	DrawBlackBox( void ){	Rect	tempRect;		// draw it in the safe gworld		SetGWorld(theGWorlds.screenGWorld, nil);	UseResFile(theFileStuff.GWDataFileNumber);	PreloadBlackBox();		// now need to copy it to the screen	SetGWorld(&theUserInterface.myCWindowRecord.port, GetMainDevice());	SetRect(&tempRect, 320, 300, 600, 450);	CopyBits( &((GrafPtr)theGWorlds.screenGWorld)->portBits, &((GrafPtr)(&theUserInterface.myCWindowRecord.port))->portBits,			&tempRect, &tempRect, srcCopy, ( RgnHandle )0L );	#ifdef	DEMO	if (theUserInterface.registerInfoButton) ShowControl(theUserInterface.registerInfoButton);	#else	if (theUserInterface.enterCodeButton) ShowControl(theUserInterface.enterCodeButton);	#endif	ForeColor(blackColor);}void	PrepareForDialog( void ){	// gray out the other buttons		HiliteControl(theUserInterface.newGameButton, 255);	HiliteControl(theUserInterface.highScoresButton, 255);	HiliteControl(theUserInterface.quitButton, 255);	HiliteControl(theUserInterface.controlsButton, 255);	HiliteControl(theUserInterface.helpButton, 255);	HiliteControl(theUserInterface.musicButton, 255);	HiliteControl(theUserInterface.soundButton, 255);		#ifdef	DEMO	DisposeControl(theUserInterface.registerInfoButton);											#else	DisposeControl(theUserInterface.enterCodeButton);	#endif}void	CleanupAfterDialog( void ){	// reset the other buttons	HiliteControl(theUserInterface.newGameButton, 0);	HiliteControl(theUserInterface.highScoresButton, 0);	HiliteControl(theUserInterface.quitButton, 0);	HiliteControl(theUserInterface.controlsButton, 0);	HiliteControl(theUserInterface.helpButton, 0);	HiliteControl(theUserInterface.musicButton, 0);	HiliteControl(theUserInterface.soundButton, 0);	DrawBlackBox();}void	CustomShowCursor(void){	ShowCursor();	theUserInterface.cursorHidden = false;}void	CustomHideCursor(void){	HideCursor();	theUserInterface.cursorHidden = true;}