/****************************************************//*													*//*					globals							*//*													*//****************************************************/#ifdef DEMOextern	Boolean hackedOne, hackedTwo, hackedThree, hackedFour, hackedFive, hackedSix, hackedSeven;#endif// world data#ifdef DEMO	extern	short	**theWorldScenery, **theWorldPlatforms, **theWorldOther;#else	extern	short	**theRoomData;#endifextern	short		*platformAddresses[257];extern	short		*sceneryAddresses[385];#ifndef DEMO	extern	short		theWorldType, theUniqueMonsterNumber, theProtectedMonsterNumber;	extern	Boolean		invincible;#endif// offsets due to scrolling inertiaextern	float		scrollOffsetH, scrollOffsetV;extern	short		parallaxRatioH, parallaxRatioV;extern	short		pWidth, pHeight;// how wide is the room (in terms of number of platforms) ?extern	short		minActualVert, minActualHoriz, maxActualVert, maxActualHoriz;extern	short		minRequiredHoriz, maxRequiredHoriz, minRequiredVert, maxRequiredVert;extern	short		roomLoaded;extern	short		roomWidthTiles, roomHeightTiles;extern	short		roomWidthPixels, roomHeightPixels;// sprite stuffextern	PlayerSprite		player;extern	MonsterSpritePtr	firstMonster;extern	DoorSpritePtr		firstDoor;extern	ItemSpritePtr		firstItem, firstKeyCopy;extern	SpringSpritePtr		firstSpring;extern	HazardSpritePtr		firstHazard;extern	ConveyorSpritePtr	firstConveyor;extern	DissolverSpritePtr	firstDissolver;extern	TalismanSpritePtr	firstTalisman;extern	BananaSpritePtr		firstBanana;extern	GWParticleWrapperPtr	firstParticle, lastParticle;// game loop stuffextern	GameGlobals	theGlobals;extern	Boolean		canAbort;extern	short		saveMessageCounter, saveMessageTwoCounter;extern	Boolean		justSaved;// sprite definitionsextern	MonsterDefHandle	spriteDefinitions[50];extern	DoorDefHandle		doorDefinitions[8];extern	SCRTHandle			spriteScripts[50];extern	short			frameCount, talismanBitField;extern	ISpElementReference	gInputElements[numInputs];// registration stuff#ifndef DEMO	extern	SerialNumHnd		serialNumberHandle;	extern	Boolean				cracksInstalled;	extern	short				gRegistered;	extern	short				globalDifficulty;#else	extern	long				fileLength, fileDate, fileCount;#endifextern	scoreHandle			theScoreHandle;	// high scoresextern	PrefsHnd			gPrefs;// user interface stuffextern	UserInterfaceStuff	theUserInterface;extern	GameGWorlds			theGWorlds;extern	FileStuff	theFileStuff;extern	AudioStuff	theAudioStuff;