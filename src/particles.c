#include "globals extern.h"GWParticleWrapperPtr	AllocateParticleWrapper(void){	GWParticleWrapperPtr	tempParticle;		tempParticle = (GWParticleWrapperPtr)NewPtrClear(sizeof(GWParticleWrapper));	if (tempParticle == nil) FatalError("Particle Allocation Failed", true);		if (firstParticle == nil)	{		// no particles have been allocated yet			firstParticle = tempParticle;		lastParticle = tempParticle;		tempParticle->prevParticle = nil;		tempParticle->nextParticle = nil;	}	else	{		tempParticle->prevParticle = lastParticle;		lastParticle->nextParticle = tempParticle;		lastParticle = tempParticle;		}		return tempParticle;}void	AllocateParticle(GWParticlePtr	tempParticle, short vert, short horiz, short color){	short			direction, seed;		tempParticle->active = true;	tempParticle->vert = vert;	tempParticle->horiz = horiz;	tempParticle->color = color;		seed = Random();		tempParticle->duration = 1 + (seed & 63); // 1 - 64, ca 2 seconds	// TODO - particle speed		direction = seed & 7;		switch (direction)	{		case 0:			tempParticle->speedh = 0;			tempParticle->speedv = -1;			break;		case 1:			tempParticle->speedh = 0.7;			tempParticle->speedv = -0.7;			break;		case 2:			tempParticle->speedh = 1;			tempParticle->speedv = 0;			break;		case 3:			tempParticle->speedh = 0.7;			tempParticle->speedv = 0.7;			break;		case 4:			tempParticle->speedh = 0;			tempParticle->speedv = 1;			break;		case 5:			tempParticle->speedh = -0.7;			tempParticle->speedv = 0.7;			break;		case 6:			tempParticle->speedh = -1;			tempParticle->speedv = 0;			break;		case 7:			tempParticle->speedh = -0.7;			tempParticle->speedv = -0.7;			break;		default:			FatalError("Particle Speed Error", true);			break;	}}/* Routine to dispose of a particle list - make the list pass over the sprite *//* then release the memory occupied by the sprite */GWParticleWrapperPtr DisposeParticle(GWParticleWrapperPtr theParticle){	GWParticleWrapperPtr	returnValue;	GWParticleWrapperPtr	tempParticle = theParticle;	if (theParticle == nil) FatalError("Trying to dispose of zero particle", true);		returnValue = theParticle->nextParticle;	// may be nil	if (theParticle == lastParticle)	{		if (theParticle == firstParticle)		{			// disposing of the first and only particle						lastParticle = nil;			firstParticle = nil;		}		else lastParticle = theParticle->prevParticle; // TODO - shouldn't really be needed should it ?	}	else if (theParticle == firstParticle)	{		firstParticle = theParticle->nextParticle;	}	if (tempParticle->nextParticle) (tempParticle->nextParticle)->prevParticle = tempParticle->prevParticle;	if (tempParticle->prevParticle) (tempParticle->prevParticle)->nextParticle = tempParticle->nextParticle;	DisposePtr( (Ptr)theParticle->particleList);	DisposePtr( (Ptr)theParticle);	return returnValue;}Boolean	HandleParticle(GWParticlePtr theParticle, Rect *drawingRect){	Point	thePoint;	short	h, v;		if (theParticle->active == false) return false;	theParticle->vert += theParticle->speedv;	theParticle->horiz += theParticle->speedh;		// TODO - check moving out of playing area - should be handled okay below	theParticle->duration--;	// frame countdown to disposal		if (theParticle->duration == 0)	{		theParticle->active = false;		theParticle->needToDraw = false;		return true;		}			thePoint.v = theParticle->vert;	thePoint.h = theParticle->horiz;	// TODO - quicker to check myself ?	if (PtInRect(thePoint, drawingRect)) theParticle->needToDraw = true;	else theParticle->needToDraw = false;			// if we don't need to draw it, nothing more to do		if (theParticle->needToDraw == false) return false;	h = theParticle->horiz - (24 * minActualHoriz);	v = theParticle->vert - (24 * minActualVert);	theParticle->dest = (short *)(theGWorlds.safeMemPtr + (v * theGWorlds.safeRowOffSet) + 2 * h);	return false;}// a sprite has exploded, so turn it into particlesvoid	CreateParticlesFromMonster(MonsterSpritePtr	theMonster){	short i, j;	short temp;	short count;	GWParticleWrapperPtr	theWrapper;	GWParticlePtr			theParticleList;	theWrapper = AllocateParticleWrapper();	// first count how many particles we will need		count = 0;	for (i = 0; i < (theMonster->theRect.bottom - theMonster->theRect.top); i++)	{		for (j = 0; j < (theMonster->theRect.right - theMonster->theRect.left); j++)		{			// check if the sprite has a pixel here						temp = theMonster->source[j +  i * theMonster->sourceRowBytes/2];			if (temp != 32767) count++;		}	}		theWrapper->numberOfParticles = count;	theWrapper->numberOfActiveParticles = count;	theParticleList = (GWParticlePtr)NewPtrClear(count * sizeof(GWParticle));	if (theParticleList == nil) FatalError("Particle List allocation failed", true);	theWrapper->particleList = theParticleList;	count = 0;	for (i = 0; i < (theMonster->theRect.bottom - theMonster->theRect.top); i++)	{		for (j = 0; j < (theMonster->theRect.right - theMonster->theRect.left); j++)		{			// check if the sprite has a pixel here						temp = theMonster->source[j +  i * theMonster->sourceRowBytes/2];			if (temp != 32767)			{				AllocateParticle(&theParticleList[count], theMonster->theRect.top + i,												theMonster->theRect.left + j, temp);				count++;			}		}	}}#ifdef DEMOvoid WorldCheckSix(void){	long	size, j;	long	count = 0;	size = GetResourceSizeOnDisk((Handle)theWorldPlatforms)/2;		for (j = 0; j < size; j++)	{		count += (*theWorldPlatforms)[j] * (j & 14);	}		if ((count != 793978) && (count != 912254) && (count != 6457826)) hackedSix = true;}#endifvoid	CreateParticlesFromBanana(BananaSpritePtr	theBanana){	short i, j;	short temp;	short count;	GWParticleWrapperPtr	theWrapper;	GWParticlePtr			theParticleList;	theWrapper = AllocateParticleWrapper();	// first count how many particles we will need		count = 0;	for (i = 0; i < (theBanana->theRect.bottom - theBanana->theRect.top); i++)	{		for (j = 0; j < (theBanana->theRect.right - theBanana->theRect.left); j++)		{			// check if the sprite has a pixel here						temp = theBanana->source[j +  i * theBanana->sourceRowBytes/2];			if (temp != 32767) count++;		}	}		theWrapper->numberOfParticles = count;	theWrapper->numberOfActiveParticles = count;	theParticleList = (GWParticlePtr)NewPtrClear(count * sizeof(GWParticle));	if (theParticleList == nil) FatalError("Particle List allocation failed", true);	theWrapper->particleList = theParticleList;	count = 0;		#ifdef DEMO		if (hackedSeven) FatalError("Error exploding banana", true);	#endif		for (i = 0; i < (theBanana->theRect.bottom - theBanana->theRect.top); i++)	{		for (j = 0; j < (theBanana->theRect.right - theBanana->theRect.left); j++)		{			// check if the sprite has a pixel here						temp = theBanana->source[j +  i * theBanana->sourceRowBytes/2];			if (temp != 32767)			{				AllocateParticle(&theParticleList[count], theBanana->theRect.top + i,												theBanana->theRect.left + j, temp);				count++;			}		}	}}