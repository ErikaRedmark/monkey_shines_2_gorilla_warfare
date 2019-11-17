#define	kSpriteDone		0// common between editor and game (I hope)#define		maskConveyorLeft	8#define		maskConveyorRight	16#define		maskDissolver		32#define		maskHazard			64#define		maskSpring			128#define		maskMonster			256#define		maskBanana			512#define		maskItem			2048#define		maskTalisman		4096#define		maskDoor			8192#define		maskDoorInactive	16384#define		maskItemInactive	16384#define		sndAlarm				128#define		sndClick				129#define		sndSerialRight			130#define		sndSerialWrong			131#define		sndHighScore			132#define		sndGameOver				133#define		sndDarwinWalk			134#define		sndDarwinRun			135#define		sndDarwinJump			136#define		sndDarwinLand			137#define		sndDarwinTired			138#define		sndDarwinScratch		139#define		sndDarwinThrow			140#define		sndDarwinTeleport		141#define		sndExitTimeMachine		142#define		sndDarwinDie			143#define		sndDarwinDamageFall		144#define		sndDarwinDieFall		145#define		sndDarwinLoseEnergy		146#define		sndObjectFruit			147#define		sndObjectMultiplier		148#define		sndObjectBanana			149#define		sndObjectLife			150#define		sndObjectHealth			151#define		sndObjectShield			152#define		sndObjectKey			153#define		sndObjectTalisman		154#define		sndObjectLastTalisman	155#define		sndBananaHits			156#define		sndPowerBananaHits		157#define		sndMonsterDestroyed		158#define		sndHighScoreApplause	159#define		sndShieldExpire			160#define		sndAboutBox				161#define		sndVolumeFeedback		162#define		sndSpring				163#define		sndJetPackExpire		164#define		sndBeanieExpire			165#define		sndObjectJetPack		166#define		sndObjectBeanie			167#define		sndJetPackUsed			168#define		sndBeanieUsed			169#define		sndDoorOpens			170#define		sndEnterTimeMachine		171#define		sndBananaExplodes		172#define		sndApeHit				173#define		sndApeDie				174#define		sndSmallHazard1Death	175#define		sndSmallHazard2Death	176#define		sndSmallHazard3Death	177#define		sndSmallHazard4Death	178#define		sndSmallHazard5Death	179#define		sndSmallHazard6Death	180#define		sndSmallHazard7Death	181#define		sndSmallHazard8Death	182// other sounds may be loaded from the world file// some additional sounds//#define		sndCompleteGame			133#define		widthInset			32#define		heightInset			32// size of Darwin#define		darwinHeight	72#define		darwinWidth		64// other stuff for Darwin#define	accel			1#define	accelV			1#define	walkSpeed		5#define	runSpeed		10#define	conveyorSpeed	4#define walkRow			0#define	jumpRow			72#define	deathRow		144#define	throwRow		216#define	idleRow			216#define	tiredRow		288#define	scratchRow		360#define	runningRow		432#define teleportRow		432#define	timeMachineRow	504// some defines for InputSprocketenum{	darwinMovement,	darwinJump,	darwinThrow,	darwinRun,	pause,	abortLife,	abortGame,	saveGame,	quitGame,	screenshot,	numInputs};#define	spriteTypeScenery	1#define spriteTypeDrainer	2#define spriteTypeKiller	3#define	spriteMovementStatic			1#define	spriteMovementHorizontal		2#define	spriteMovementHorizontalWalking	3#define	spriteMovementVertical			4#define	spriteMovementWalk				5#define	spriteMovementFly				6#define	spriteLayerBack		1#define	spriteLayerFront	2#define	spriteAggressionWimpy			1#define	spriteAggressionSlightlyWimpy	2#define	spriteAggressionAverage			3#define	spriteAggressionAggressive		4#define	spriteAggressionHardasNails		5#define	spriteSpeed1	1#define	spriteSpeed2	2#define	spriteSpeed3	3#define	spriteSpeed4	4#define	spriteSpeed5	5#define	spriteAnim30fps	1#define	spriteAnim15fps	2#define	spriteAnim10fps	3#define	spriteBananaActionDamage	1#define	spriteBananaActionNothing	2// collectable items#define	itemCherry			0#define	itemDouble			1#define	itemQuad			2#define	itemBeanie			3#define	itemBanana			4#define	itemFastFire		5#define	itemMultipleFire	6#define	itemPowerFire		7#define	itemExtraLife		8#define	itemHealth			9#define	itemShield			10#define	itemRedKey			11#define	itemGreenKey		12#define	itemGoldKey			13#define	itemBlueKey			14#define	itemJetPack			15#define	noPowerUp		0#define	doublePoints	1#define	quadPoints		2#define beanieHat		4#define	fastFire		8#define	multipleFire	16#define	powerFire		32#define	shield			64#define	jetPack			128#define	directionRight	0#define	directionLeft	1#define	directionDown	0#define	directionUp		1// for music#include	"project defines.h"#ifndef DEMO	#define	typeMADH		1	#define	typeAIFF		2#endif#define	noMusic			0#define	gameMusic		1#define	interfaceMusic	2#ifndef DEMO	// different world types		#define	typeTalisman	1024	#define	typeMonster		2048#endif// for the profiler//#define	useProfiler	1