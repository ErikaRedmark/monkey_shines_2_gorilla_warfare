#include	<Dialogs.h>#include	"stdio.h"#include	"string.h"#include	<ctype.h>#include	"globals extern.h"#ifdef DEMOvoid RegistrationInfoDialog(void){	RgnHandle	theRegion;	WindowPtr	dialogWindow;	ControlHandle	dialogOkayButton, tempControl;	Rect			tempRect;	PicHandle tempPict;	UseResFile(theFileStuff.GWDataFileNumber);	tempPict = GetPicture(903);	if (tempPict == nil) FatalError("Error loading PICT 903", true);	tempRect.left = 0;	tempRect.right = (**tempPict).picFrame.right;	tempRect.top = 0;	tempRect.bottom = (**tempPict).picFrame.bottom;		CenterRectInRect(&tempRect, &(**theUserInterface.gameGDevice).gdRect);		dialogWindow  = NewCWindow(nil, &tempRect, "\p", true,						plainDBox, (WindowPtr)-1L, false, (long)nil);	SetPort(dialogWindow);	DrawPicture(tempPict, &(**tempPict).picFrame);	ReleaseResource((Handle)tempPict);	UseResFile(theFileStuff.applicationFile);	SetRect(&tempRect, 180, 386, 240, 406); 	dialogOkayButton = NewControl( dialogWindow, &tempRect, "\pOkay", true, 0, 0, 0, 0, 0 );	tempControl =	MarksCustomDialog(dialogWindow);	if (tempControl == nil)	{		HiliteControl(dialogOkayButton, 10);		GWDelay(0, 10, false);		HiliteControl(dialogOkayButton, 0);	}								DisposeWindow(dialogWindow);	SetGWorld(&theUserInterface.myCWindowRecord.port, GetMainDevice());	// update the window behind the control	theRegion = theUserInterface.theWindow->visRgn;		tempRect.left = (**theRegion).rgnBBox.left;	tempRect.right = (**theRegion).rgnBBox.right;	tempRect.top = (**theRegion).rgnBBox.top;	tempRect.bottom = (**theRegion).rgnBBox.bottom;		CopyBits(  &((GrafPtr)theGWorlds.screenGWorld)->portBits, &((GrafPtr)(&theUserInterface.myCWindowRecord.port))->portBits,		&tempRect, &tempRect,  srcCopy, ( RgnHandle )0L );			UpdateControls(theUserInterface.theWindow, theRegion);}#else/**************************** register ****************************/void RegisterDialog(void){	Rect itemRect, tempRect;	short itemType, itemHit;	DialogPtr	theDialog;	Str255		nameText, codeText;	Handle		nameHandle, codeHandle, okItemHandle;	long		registrationCode;	RgnHandle	theRegion;	long		**tempCode;	EventRecord	event;	ModalFilterUPP	myModalFilter;	Boolean dialogDone = false;	// regcode is the calculated code for this name/number combo		UseResFile(theFileStuff.GWDataFileNumber);	theDialog = GetNewDialog(15000, nil, (WindowPtr) -1L);	GetDialogItem(theDialog, 4, &itemType, &nameHandle, &itemRect);	GetDialogItem(theDialog, 3, &itemType, &codeHandle, &itemRect);	GetDialogItem(theDialog, ok, &itemType, &okItemHandle, &itemRect);	SetDialogItemText(nameHandle, (**gPrefs).registerName);	dialogDone = false;	SetPort(theDialog);	DrawDialog(theDialog);    BeginUpdate(theDialog);    EndUpdate(theDialog);	FlushEvents(everyEvent,0);	SetDialogDefaultItem(theDialog, ok);	SetDialogCancelItem(theDialog, cancel);	SetDialogTracksCursor(theDialog, true);	SelectDialogItemText(theDialog, 4, 0, 32767);	myModalFilter = NewModalFilterProc((RoutineDescriptor *)&MyEventFilter);	while (!dialogDone)	{		ModalDialog(myModalFilter, &itemHit);				switch (itemHit)		{			case ok:			case cancel:				dialogDone = true;				break;		}	}	GetDialogItemText(nameHandle, nameText);	GetDialogItemText(codeHandle, codeText);	if ((codeText[0] > 0) && (codeText[0] < 32) && (codeText[1] != ' ')) StringToNum(codeText, &registrationCode);	else registrationCode = 0;		DisposeRoutineDescriptor(myModalFilter);		WaitNextEvent(everyEvent, &event, 20L, nil);	DisposeDialog(theDialog);	SetGWorld(&theUserInterface.myCWindowRecord.port, GetMainDevice());	// update the window behind the control	theRegion = theUserInterface.theWindow->visRgn;		tempRect.left = (**theRegion).rgnBBox.left;	tempRect.right = (**theRegion).rgnBBox.right;	tempRect.top = (**theRegion).rgnBBox.top;	tempRect.bottom = (**theRegion).rgnBBox.bottom;		CopyBits(  &((GrafPtr)theGWorlds.screenGWorld)->portBits, &((GrafPtr)(&theUserInterface.myCWindowRecord.port))->portBits,		&tempRect, &tempRect,  srcCopy, ( RgnHandle )0L );			UpdateControls(theUserInterface.theWindow, theRegion);	if (itemHit != cancel)	{		// name and registration number have been entered		// so put them into the prefs file		// check if caps lock down		// if cracks and numbers is installed, and caps lock is not pressed, set the		// registration code to a known bad one reg name 'bad code'		if ((cracksInstalled) && (!(event.modifiers & alphaLock))) registrationCode = 1253450503;		(**gPrefs).registrationCode = registrationCode;		CopyString((**gPrefs).registerName, nameText);		// write out the modified preferences to disk								UpdatePreferences();		CheckRegistration();		if (!gRegistered)	IncorrectCodeDialog();		else		{			ThankYouDialog();			DisposeControl(theUserInterface.enterCodeButton);			DrawBlackBox();			// now need to store the registration info in the application			// best to do this as a pref resource						UseResFile(theFileStuff.applicationFile);			tempCode = (long **)Get1Resource('CODE', 1);			(*tempCode)[143] = (**gPrefs).registrationCode;			(*tempCode)[318] = 99874;			ChangedResource((Handle)tempCode);			ReleaseResource((Handle)tempCode);			UpdateResFile(CurResFile());		}	}	else	{		// hit cancel	}	UseResFile(theFileStuff.applicationFile);	SetCCursor(theUserInterface.myCursor);	SetupRgn();}void CheckRegistration ( void ){	long		nameNumber, regcode;	Str255		nameText;	short t,tt;	long		temp;	int worldswitch[2][8] = {	   							1,4,5,11,18,21,24,27,	   							2,3,8,14,17,23,27,29	   						};   		// need to check for blacklisted registration codes	if (((**gPrefs).registerName[0] < 1) || ((**gPrefs).registerName[1] == ' '))	{		// invalid string					CopyString((**gPrefs).registerName, "\pYour name");			(**gPrefs).registrationCode = 0;			gRegistered = 0;			// write out the modified preferences to disk									UpdatePreferences();			return;	}	else if ((**gPrefs).registerName[0] > 31) (**gPrefs).registerName[0] = 31;	 	StringToNum((**gPrefs).registerName, &nameNumber);	CopyString(nameText, (**gPrefs).registerName);	nameNumber &= 0x7FFFFFFF;	regcode = (**serialNumberHandle).serialNumber + nameNumber;	for(t = 1;t <= nameText[0];t++)	/************* do bitswitch for name *************/	{		if (nameText[t] == 0) goto pushon;		nameText[t] = tolower(nameText[t]);		if (nameText[t] == 32) nameText[t] = 128;		temp = nameText[t]-97;		if (BitTst(&regcode,temp)) BitClr(&regcode,temp);		else BitSet(&regcode,temp);	}	 	pushon:	 		for(tt=0;tt<8;tt++) 	{			   		if (BitTst(&regcode,worldswitch[1][tt])) BitClr(&regcode,worldswitch[1][tt]);		else BitSet(&regcode,worldswitch[1][tt]);	}	regcode &= 0x7FFFFFFF;	if ((**gPrefs).registrationCode == regcode) gRegistered = 135;	else 	{		CopyString((**gPrefs).registerName, "\pYour name");		(**gPrefs).registrationCode = 0;		gRegistered = 0;		// write out the modified preferences to disk							UpdatePreferences();	}}void	ThankYouDialog( void ){	RgnHandle	theRegion;	WindowPtr	dialogWindow;	ControlHandle	dialogOkayButton, tempControl;	PicHandle		tempPict;	Rect			tempRect;	UseResFile(theFileStuff.GWDataFileNumber);	tempPict = GetPicture(906);	if (tempPict == nil) FatalError("Error loading PICT 906", true);	tempRect.left = 0;	tempRect.right = (**tempPict).picFrame.right;	tempRect.top = 0;	tempRect.bottom = (**tempPict).picFrame.bottom;		CenterRectInRect(&tempRect, &(**theUserInterface.gameGDevice).gdRect);		dialogWindow  = NewCWindow(nil, &tempRect, "\p", true, plainDBox, (WindowPtr)-1L, false, (long)nil);	SetPort(dialogWindow);	DrawPicture(tempPict, &(**tempPict).picFrame);	ReleaseResource((Handle)tempPict);	UseResFile(theFileStuff.applicationFile);	SetRect(&tempRect, 199, 177, 256, 202); 	dialogOkayButton = NewControl( dialogWindow, &tempRect, "\pOkay", true, 0, 0, 0, 0, 0 );	PlayASound(sndSerialRight);	tempControl =	MarksCustomDialog(dialogWindow);	if (tempControl == nil)	{		HiliteControl(dialogOkayButton, 10);		GWDelay(0, 10, false);		HiliteControl(dialogOkayButton, 0);	}								DisposeWindow(dialogWindow);	SetGWorld(&theUserInterface.myCWindowRecord.port, GetMainDevice());	// update the window behind the control	theRegion = theUserInterface.theWindow->visRgn;		tempRect.left = (**theRegion).rgnBBox.left;	tempRect.right = (**theRegion).rgnBBox.right;	tempRect.top = (**theRegion).rgnBBox.top;	tempRect.bottom = (**theRegion).rgnBBox.bottom;		CopyBits(  &((GrafPtr)theGWorlds.screenGWorld)->portBits, &((GrafPtr)(&theUserInterface.myCWindowRecord.port))->portBits,		&tempRect, &tempRect,  srcCopy, ( RgnHandle )0L );			UpdateControls(theUserInterface.theWindow, theRegion);}void	IncorrectCodeDialog( void ){	RgnHandle	theRegion;	WindowPtr	dialogWindow;	ControlHandle	dialogOkayButton, tempControl;	PicHandle	tempPict;	Rect		tempRect;	UseResFile(theFileStuff.GWDataFileNumber);	tempPict = GetPicture(907);	if (tempPict == nil) FatalError("Error loading PICT 907", true);	tempRect.left = 0;	tempRect.right = (**tempPict).picFrame.right;	tempRect.top = 0;	tempRect.bottom = (**tempPict).picFrame.bottom;		CenterRectInRect(&tempRect, &(**theUserInterface.gameGDevice).gdRect);		dialogWindow  = NewCWindow(nil, &tempRect, "\p", true,						plainDBox, (WindowPtr)-1L, false, (long)nil);	SetPort(dialogWindow);	DrawPicture(tempPict, &(**tempPict).picFrame);	ReleaseResource((Handle)tempPict);	UseResFile(theFileStuff.applicationFile);	SetRect(&tempRect, 110, 195, 170, 215); 	dialogOkayButton = NewControl( dialogWindow, &tempRect, "\pOkay", true, 0, 0, 0, 0, 0 );	PlayASound(sndSerialWrong);	tempControl =	MarksCustomDialog(dialogWindow);	if (tempControl == nil)	{		HiliteControl(dialogOkayButton, 10);		GWDelay(0, 10, false);		HiliteControl(dialogOkayButton, 0);	}								DisposeWindow(dialogWindow);	SetGWorld(&theUserInterface.myCWindowRecord.port, GetMainDevice());	// update the window behind the control	theRegion = theUserInterface.theWindow->visRgn;		tempRect.left = (**theRegion).rgnBBox.left;	tempRect.right = (**theRegion).rgnBBox.right;	tempRect.top = (**theRegion).rgnBBox.top;	tempRect.bottom = (**theRegion).rgnBBox.bottom;		CopyBits(  &((GrafPtr)theGWorlds.screenGWorld)->portBits, &((GrafPtr)(&theUserInterface.myCWindowRecord.port))->portBits,		&tempRect, &tempRect,  srcCopy, ( RgnHandle )0L );			UpdateControls(theUserInterface.theWindow, theRegion);}#endifvoid HackedCopy( void ){// if it gets here, then either the game or the world file has been hacked in// unregistered copy// put up a dialog and then quit.	WindowPtr	dialogWindow;	ControlHandle	dialogOkayButton, tempControl;	PicHandle	tempPict;	Rect		tempRect;	UseResFile(theFileStuff.GWDataFileNumber);	tempPict = GetPicture(908);	if (tempPict == nil) FatalError("Error loading PICT 908", true);	tempRect.left = 0;	tempRect.right = (**tempPict).picFrame.right;	tempRect.top = 0;	tempRect.bottom = (**tempPict).picFrame.bottom;		CenterRectInRect(&tempRect, &(**theUserInterface.gameGDevice).gdRect);		dialogWindow  = NewCWindow(nil, &tempRect, "\p", true, plainDBox, (WindowPtr)-1L, false, (long)nil);	SetPort(dialogWindow);	DrawPicture(tempPict, &(**tempPict).picFrame);	ReleaseResource((Handle)tempPict);	UseResFile(theFileStuff.applicationFile);	SetRect(&tempRect, 110, 195, 170, 215); 	dialogOkayButton = NewControl( dialogWindow, &tempRect, "\pQuit", true, 0, 0, 0, 0, 0 );	CustomShowCursor();	tempControl = MarksCustomDialog(dialogWindow);	if (tempControl == nil)	{		HiliteControl(dialogOkayButton, 10);		GWDelay(0, 10, false);		HiliteControl(dialogOkayButton, 0);	}	DisposeWindow(dialogWindow);	SetGWorld(&theUserInterface.myCWindowRecord.port, GetMainDevice());	CleanUpAndQuit();	ExitToShell();}