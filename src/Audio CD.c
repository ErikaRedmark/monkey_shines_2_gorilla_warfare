/*	Author:		Pierre-Olivier Latour				pol@french-touch.net				http://www.french-touch.net/CodeWareHouse					API Description:				� OSErr AudioCD_Init_UseAppleDriver()					Inits the audio CD driver (Apple official CD/DVD readers only!)				� OSErr AudioCD_SetVolume(UInt8 left, UInt8 right)					Sets the volume of the CD player (0-255).				� OSErr AudioCD_GetVolume(UInt8* left, UInt8* right)					Gets the volume of the CD player (0-255).				� OSErr AudioCD_Eject()					Ejects the CD inserted.				� short AudioCD_GetNumTracks()					Returns the number of tracks (data + audio) on the inserted CD.				� short AudioCD_GetCurrentTrack()					Returns the number of the current audio track being played.				� OSErr AudioCD_Stop()					Stops playing the audio CD.				� OSErr AudioCD_Pause()					Pauses playing the audio CD.				� OSErr AudioCD_Resume()					Pauses playing the audio CD.				� Boolean AudioCD_IsPlaying()					Returns "true" if an audio CD is being played.				� Boolean AudioCD_IsCDInserted()					Returns "true" if a CD is inserted.				� OSErr AudioCD_PlayTrack(UInt8 trackNumber)					Plays and loops a CD audio track.	License:	This sample code is free of use: you may incorporate it into your applications without				restriction.		Disclaimer:	You expressly acknowledge and agree that use of this source code is at your sole risk.				This source code is provided AS IS and without warranty of any kind and the Author.		Copyright:	Parts of this source code come from Apple's sample code.		Note:		Your feedback is appreciated!		History:	� 11/10/00: initial release*/#include						<Devices.h>#include						<Errors.h>#include						"Audio CD.h"//CONSTANTES:#define kCDDriverName			"\p.AppleCD"//Standard Driver Callsenum {	csEjectTheDisc		= 7,	csDriveStatus};//Audio track Control Callsenum {	csReadTOC			= 100,	csReadTheQSubcode,	csReadHeader,	csAudioTrackSearch,	csAudioPlay,	csAudioPause,	csAudioStop,	csAudioStatus,	csAudioScan,	csAudioControl,	csReadMCN,	csReadISRC,	csReadAudioVolume,	csGetSpindleSpeed,	csSetSpindleSpeed,	csReadAudio,	csReadAllSubcodes,	csSetTrackList = 122,	csGetTrackList,	csGetTrackIndex,	csSetPlayMode,	csGetPlayMode};//Special System Control Calls enum {	csGoodBye			= -1};enum {kNormalMode = 0, kShuffleMode, kProgMode};#define kAudioCDPlayMode_Mute				0x00#define kAudioCDPlayMode_RightOnly			0x05#define kAudioCDPlayMode_Stereo				0x09#define kAudioCDPlayMode_LeftOnly			0x0A#define kAudioCDPlayMode_Mono				0x0F#define	kRepeatFlag							(1 << 8)#define	kAddressModeTrack					2#define	kAddressModePlayListIndex			3//STRUCTURES:#pragma pack(2)typedef struct{	UInt16		track;	UInt8		writeProtect;	UInt8		discInPlace;	UInt8		installed;	UInt8		side;	UInt32		qLink;	UInt16		qType;	UInt16		dQDrive;	UInt16		dQRefNum;	UInt16		dQFSID;	UInt8		twoSideFormat;	UInt8		needsFlush;	UInt8		discErrs;}csParamDiscStatusRec;#pragma pack()//LOCAL VARIABLES:static SInt16		gCDDriverRef = 0;//ROUTINES:static void ClearMemory(void* dest, unsigned long size){	long			i;	unsigned char*	ptr = (unsigned char*) dest;		for(i = 0; i < size; ++i) {		*ptr = 0x00;		++ptr;	}}static UInt32 NumToBCD(UInt16 theNum){	UInt32	theBCD;	theBCD = 0x0000;		//�	Get the digits	theBCD |= theNum % 10;	theNum /= 10;	//�	Get the tens	theBCD |= (theNum % 10) << 4;	theNum /= 10;		//�	Get the hundreds	theBCD |= (theNum % 10) << 8;	theNum /= 10;		//�	Get the thousands	theBCD |= (theNum % 10) << 12;		return theBCD;}static SInt16 BCDToNum(UInt8 bcd){	UInt8	place;	SInt16	num = 0;		place = bcd & 0x0F;									//�	Get the first 10 bits	num += place;		place = (bcd >> 4) & 0x0F;	num += (place * 10);		return num;}//PUBLIC ROUTINES:OSErr AudioCD_Init_UseAppleDriver(){	OSErr theError;		theError = MacOpenDriver(kCDDriverName, &gCDDriverRef);	if (theError == noErr) return theError;	// try for the intech drivers	theError = MacOpenDriver("\p.Driver2", &gCDDriverRef);	return theError;}OSErr AudioCD_SetVolume(UInt8 left, UInt8 right){	ParamBlockRec	thePB;		if(!gCDDriverRef)	return paramErr;		ClearMemory(&thePB, sizeof(ParamBlockRec));	thePB.cntrlParam.ioVRefNum = 1;	thePB.cntrlParam.ioCRefNum = gCDDriverRef;	thePB.cntrlParam.csCode = csAudioControl;	thePB.cntrlParam.csParam[0] = (left << 8) | right;		return PBControlSync(&thePB);}OSErr AudioCD_GetVolume(UInt8* left, UInt8* right){	ParamBlockRec	thePB;	OSErr			theError;		if(!gCDDriverRef)	return paramErr;		ClearMemory(&thePB, sizeof(ParamBlockRec));	thePB.cntrlParam.ioVRefNum = 1;	thePB.cntrlParam.ioCRefNum = gCDDriverRef;	thePB.cntrlParam.csCode = csReadAudioVolume;	theError = PBControlSync(&thePB);	if(theError)	return theError;		*left = (thePB.cntrlParam.csParam[0] >> 8) & 0xFF;	*right = thePB.cntrlParam.csParam[0] & 0xFF;		return noErr;}OSErr AudioCD_Eject(){	ParamBlockRec	thePB;	if(!gCDDriverRef)	return paramErr;		ClearMemory(&thePB, sizeof(ParamBlockRec));	thePB.cntrlParam.ioVRefNum = 1;	thePB.cntrlParam.ioCRefNum = gCDDriverRef;	thePB.cntrlParam.csCode = csEjectTheDisc;	return PBControlSync(&thePB);}short AudioCD_GetNumTracks(){	ParamBlockRec	thePB;		if(!gCDDriverRef)	return paramErr;		ClearMemory(&thePB, sizeof(ParamBlockRec));	thePB.cntrlParam.ioVRefNum = 1;	thePB.cntrlParam.ioCRefNum = gCDDriverRef;	thePB.cntrlParam.csCode = csReadTOC;	thePB.cntrlParam.csParam[0] = 1;	if(PBControlSync(&thePB) != noErr)	return 0;		return BCDToNum(thePB.cntrlParam.csParam[0] & 0xFF);}short AudioCD_GetCurrentTrack(){	ParamBlockRec	thePB;		if(!gCDDriverRef)	return paramErr;		ClearMemory(&thePB, sizeof(ParamBlockRec));	thePB.cntrlParam.ioVRefNum = 1;	thePB.cntrlParam.ioCRefNum = gCDDriverRef;	thePB.cntrlParam.csCode = csReadTheQSubcode;	thePB.cntrlParam.csParam[0] = 1;	if(PBControlSync(&thePB) != noErr)	return 0;		return BCDToNum(thePB.cntrlParam.csParam[0] & 0xFF);}OSErr AudioCD_Stop(){	ParamBlockRec		thePB;	if(!gCDDriverRef)	return paramErr;		ClearMemory(&thePB, sizeof(ParamBlockRec));	thePB.cntrlParam.ioVRefNum = 1;	thePB.cntrlParam.ioCRefNum = gCDDriverRef;	thePB.cntrlParam.csCode = csAudioStop;		return PBControlSync(&thePB);}OSErr AudioCD_Pause(){	ParamBlockRec		thePB;	if(!gCDDriverRef)	return paramErr;		ClearMemory(&thePB, sizeof(ParamBlockRec));	thePB.cntrlParam.ioVRefNum = 1;	thePB.cntrlParam.ioCRefNum = gCDDriverRef;	thePB.cntrlParam.csCode = csAudioPause;	thePB.cntrlParam.csParam[0] = 1;	thePB.cntrlParam.csParam[1] = 1;		return PBControlSync(&thePB);}OSErr AudioCD_Resume(){	ParamBlockRec		thePB;	if(!gCDDriverRef)	return paramErr;		ClearMemory(&thePB, sizeof(ParamBlockRec));	thePB.cntrlParam.ioVRefNum = 1;	thePB.cntrlParam.ioCRefNum = gCDDriverRef;	thePB.cntrlParam.csCode = csAudioPause;	thePB.cntrlParam.csParam[0] = 0;		return PBControlSync(&thePB);}Boolean AudioCD_IsPlaying(){	ParamBlockRec	thePB;		if(!gCDDriverRef)	return false;		ClearMemory(&thePB, sizeof(ParamBlockRec));	thePB.cntrlParam.ioVRefNum = 1;	thePB.cntrlParam.ioCRefNum = gCDDriverRef;	thePB.cntrlParam.csCode = csAudioStatus;	if(PBControlSync(&thePB) != noErr)	return false;		if((thePB.cntrlParam.csParam[0] >> 8) == 0)	return true;	return false;}Boolean AudioCD_IsCDInserted(){	ParamBlockRec			thePB;	csParamDiscStatusRec	*statusRec;		if(!gCDDriverRef)	return paramErr;		ClearMemory(&thePB, sizeof(ParamBlockRec));	thePB.cntrlParam.ioVRefNum = 1;	thePB.cntrlParam.ioCRefNum = gCDDriverRef;	thePB.cntrlParam.csCode = csDriveStatus;	if(PBStatusSync(&thePB) != noErr)	return false;		//Check if CD is inserted	statusRec = (csParamDiscStatusRec*) &(thePB.cntrlParam.csParam[0]);	return (statusRec->discInPlace == 1);}OSErr AudioCD_PlayTrack(UInt8 trackNumber){	ParamBlockRec	thePB;	OSErr			theError;	UInt8			buffer[1];		if(!gCDDriverRef)	return paramErr;		//Set mode to "program + repeat"	ClearMemory(&thePB, sizeof(ParamBlockRec));	thePB.cntrlParam.ioVRefNum = 1;	thePB.cntrlParam.ioCRefNum = gCDDriverRef;	thePB.cntrlParam.csCode = csSetPlayMode;	thePB.cntrlParam.csParam[0] = kProgMode | kRepeatFlag;	theError = PBControlSync(&thePB);	if(theError)	return theError;		//Position optical pick up	ClearMemory(&thePB, sizeof(ParamBlockRec));	thePB.cntrlParam.ioVRefNum = 1;	thePB.cntrlParam.ioCRefNum = gCDDriverRef;	thePB.cntrlParam.csCode = csAudioTrackSearch;	thePB.cntrlParam.csParam[0] = kAddressModeTrack;	thePB.cntrlParam.csParam[1] = 0;	thePB.cntrlParam.csParam[2] = LoWord(NumToBCD(trackNumber));	thePB.cntrlParam.csParam[3] = 0;	thePB.cntrlParam.csParam[4] = kAudioCDPlayMode_Stereo;	theError = PBControlSync(&thePB);	if(theError)	return theError;		//Set track list	buffer[0] = trackNumber;	ClearMemory(&thePB, sizeof(ParamBlockRec));	thePB.cntrlParam.ioVRefNum = 1;	thePB.cntrlParam.ioCRefNum = gCDDriverRef;	thePB.cntrlParam.csCode = csSetTrackList;	thePB.cntrlParam.csParam[0] = 1;	*((long*) &thePB.cntrlParam.csParam[1]) = (long) buffer;	theError = PBControlSync(&thePB);	if(theError)	return theError;		//Mark stop address	ClearMemory(&thePB, sizeof(ParamBlockRec));	thePB.cntrlParam.ioVRefNum = 1;	thePB.cntrlParam.ioCRefNum = gCDDriverRef;	thePB.cntrlParam.csCode = csAudioPlay;	thePB.cntrlParam.csParam[0] = kAddressModePlayListIndex;	thePB.cntrlParam.csParam[1] = 0;	thePB.cntrlParam.csParam[2] = 0;	thePB.cntrlParam.csParam[3] = 1;	thePB.cntrlParam.csParam[4] = kAudioCDPlayMode_Stereo;	theError = PBControlSync(&thePB);	if(theError)	return theError;		//Mark start address	ClearMemory(&thePB, sizeof(ParamBlockRec));	thePB.cntrlParam.ioVRefNum = 1;	thePB.cntrlParam.ioCRefNum = gCDDriverRef;	thePB.cntrlParam.csCode = csAudioPlay;	thePB.cntrlParam.csParam[0] = kAddressModePlayListIndex;	thePB.cntrlParam.csParam[1] = 0;	thePB.cntrlParam.csParam[2] = 0;	thePB.cntrlParam.csParam[3] = 0;	thePB.cntrlParam.csParam[4] = kAudioCDPlayMode_Stereo;	theError = PBControlSync(&thePB);	if(theError)	return theError;		return noErr;}