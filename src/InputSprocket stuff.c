#include "InputSprocket.h"#include "globals extern.h"void InitialiseInputSprocket( void ){	OSErr theError;//�	This structure defines the input needs that we'll be requesting from InputSprocketISpNeed		myNeeds[numInputs] ={	{		"\pDarwin Movement",		200,		0,0,		kISpElementKind_Axis,		kISpElementLabel_Axis_XAxis,		0,		0,		0,		0	},	{		"\pDarwin Jump",		201,		0,0,		kISpElementKind_Button,		kISpElementLabel_Btn_Jump,		0,		0,		0,		0	},	{		"\pThrow Banana",		202,		0,0,		kISpElementKind_Button,		kISpElementLabel_Btn_Fire,		0,		0,		0,		0	},	{		"\pRun",		203,		0,0,		kISpElementKind_Button,		kISpElementLabel_Btn_SecondaryFire,		0,		0,		0,		0	},	{		"\pPause Game",		204,		0,0,		kISpElementKind_Button,		kISpElementLabel_Btn_StartPause,		0,		0,		0,		0	},	{		"\pAbort Life",		205,		0,0,		kISpElementKind_Button,		kISpElementLabel_None,		0,		0,		0,		0	},	{		"\pAbort Game",		206,		0,0,		kISpElementKind_Button,		kISpElementLabel_None,		0,		0,		0,		0	},	{		"\pSave Game",		207,		0,0,		kISpElementKind_Button,		kISpElementLabel_None,		0,		0,		0,		0	},	{		"\pQuit Game",		208,		0,0,		kISpElementKind_Button,		kISpElementLabel_None,		0,		0,		0,		0	},	{		"\pScreenshot",		209,		0,0,		kISpElementKind_Button,		kISpElementLabel_None,		0,		0,		0,		0	}};#ifndef DEMO	(**serialNumberHandle).serialNumber *= 11;#endif	theError = ISpElement_NewVirtualFromNeeds(numInputs, myNeeds, gInputElements, 0);	if (theError) FatalError("Could not create ISp virtual controls from needs.", false);			//�	Init InputSprocket and tell it our needs#ifdef DEMO	theError = ISpInit(numInputs, myNeeds, gInputElements, 'bNzD', '0002', 0, 2805, 0);#else	theError = ISpInit(numInputs, myNeeds, gInputElements, 'bNz2', '0002', 0, 2805, 0);#endif	if (theError) FatalError("Could not initialize ISp.", false);	//�	Turn on the keyboard and mouse handlers	ISpDevices_ActivateClass (kISpDeviceClass_Keyboard);	ISpDevices_ActivateClass (kISpDeviceClass_Mouse);	ISpSuspend();}// handle pause/resumevoid Input_Activate(Boolean	pause){	short i; 	if (!pause) 	{ 		ISpResume(); 		for (i = 0; i < numInputs; i++) ISpElement_Flush(gInputElements[i]); 	} 	else ISpSuspend();}//�	--------------------	WasButtonPressedBoolean WasButtonPressed(ISpElementReference inElement){	OSStatus	error;	Boolean		wasEvent;	Boolean		fire = false;	do	{	ISpElementEvent	event;		error = ISpElement_GetNextEvent(inElement, sizeof(ISpElementEvent), &event, &wasEvent);		if (! error && wasEvent && (event.data == kISpButtonDown))		{			fire = true; 			break;		}	} while (wasEvent && !error);		//�	flush the queue	ISpElement_Flush(inElement);		return (fire);}//�	--------------------	IsButtonHeldBoolean IsButtonHeld(ISpElementReference inElement){OSStatus	error;UInt32		input;Boolean		wasEvent;Boolean		fire = false;		//�	poll		error = ISpElement_GetSimpleState(inElement, &input);	if (! error && (input == kISpButtonDown)) 		fire = true;	//�	but don't miss fast clicks or macros	do	{	ISpElementEvent	event;		error = ISpElement_GetNextEvent(inElement, sizeof(ISpElementEvent), &event, &wasEvent);		if (! error && wasEvent && (event.data == kISpButtonDown))		{			fire = true; 			break;		}	} while (wasEvent && !error);		//�	flush the queue	ISpElement_Flush(inElement);		return (fire);}