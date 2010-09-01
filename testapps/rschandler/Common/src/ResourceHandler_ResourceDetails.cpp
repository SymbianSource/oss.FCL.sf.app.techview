// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//


#include "ResourceHandler.h"

/**
Gets process details for a process with the given name - assumed to be a valid process
name.
*/
void TResourceDetails::GetProcessData(TDesC& aProcessName)
	{
		RProcess process;
		process.Open(aProcessName);

		// ID
		iProcessData.iIdString.Format(KFormatDec,process.Id().Id());

		// Store Process Name
		iProcessData.iProcessName.Copy(aProcessName);
		//to increase readability of resource table
		AppendDotsToResourceName(iProcessData.iProcessName);
		
		// Priority
		iProcessData.iPriorityString.Format(KFormatDec,process.Priority());
		
		// Protected

		// System

		// Owner
		RProcess owner;
		TInt err = KErrNotSupported;
		if (err == KErrNone)
			{
			iProcessData.iOwnerIdString.Format(KFormatHex,owner.Id().Id());
			}
		else
			{
			iProcessData.iOwnerIdString.Format(KFormatHex,0);
			}
				
		// Exit Type
		TExitType et = process.ExitType();
		switch(et)
			{
			case(0): iProcessData.iExitTypeString.Copy((_L("Kill"))); break;
			case(1): iProcessData.iExitTypeString.Copy((_L("Terminate"))); break;
			case(2): iProcessData.iExitTypeString.Copy((_L("Panic"))); break;
			case(3): iProcessData.iExitTypeString.Copy((_L("Pending"))); break;
			default: iProcessData.iExitTypeString.Copy((_L("Error!"))); break;
			};
		
		// Exit Reason
		TInt er = process.ExitReason();
		if (er==0) iProcessData.iExitReasonString.Copy(_L("n/a"));
		else
			{
			iProcessData.iExitReasonString.Format(KFormatHex,(TInt)er);
			}

		// Exit Category
		TExitCategoryName ec;
		ec = process.ExitCategory();
		if (ec.Length() == 0) iProcessData.iExitCategoryName.Copy(_L("n/a"));
		else iProcessData.iExitCategoryName.Copy(ec);

		// UIDs
		TUidType uids = process.Type();
		iProcessData.iUidType0String.Format(KFormatHex, uids[0].iUid);
		iProcessData.iUidType1String.Format(KFormatHex, uids[1].iUid);
		iProcessData.iUidType2String.Format(KFormatHex, uids[2].iUid);
				
		// File
		iProcessData.iFileName = process.FileName();

		process.Close();	
	}
	
/**
Gets process details for a drive with the properties. These properties are assumed 
to be a valid.
*/
void TResourceDetails::GetDriveData(TInt aValidDriveNum, TDriveUnit aDriveUnit, TVolumeInfo aVolumeInfo)
	{
	iDriveData.iDriveName.Copy(aDriveUnit.Name());
	iDriveData.iVolumeName.Copy(aVolumeInfo.iName);
	iDriveData.iDriveNumberString.Format(KFormatDec, aValidDriveNum);

	TBufC<20> mediaInfo;
	switch (aVolumeInfo.iDrive.iType)
		{
		case EMediaNotPresent:mediaInfo=_L("EMediaNotPresent");break;
		case EMediaUnknown:mediaInfo=_L("EMediaUnknown");break;
		case EMediaFloppy:mediaInfo=_L("EMediaFloppy");break;
		case EMediaHardDisk:mediaInfo=_L("EMediaHardDisk");break;
		case EMediaCdRom:mediaInfo=_L("EMediaCdRom");break;
		case EMediaRam:mediaInfo=_L("EMediaRam");break;
		case EMediaFlash:mediaInfo=_L("EMediaFlash");break;
		case EMediaRom:mediaInfo=_L("EMediaRom");break;
		case EMediaRemote:mediaInfo=_L("EMediaRemote");break;
		default:mediaInfo=_L("Other");break;
		}

	iDriveData.iMediaString.Copy(mediaInfo);
	iDriveData.iAttributesString.Format(KFormatHex,aVolumeInfo.iDrive.iDriveAtt);
	iDriveData.iSpaceString.Format(KFormatDec64,aVolumeInfo.iSize);
	#if defined(WINS)
		iDriveData.iFreeSpaceString.Copy(_L("Unavail."));
	#else
	if (aVolumeInfo.iFree.GetTInt() > aVolumeInfo.iSize.GetTInt()) // this value is odd on wins
		{
		iDriveData.iFreeSpaceString.Format(KFormatDec,aVolumeInfo.iSize.GetTInt());
		}
	else if (aVolumeInfo.iFree.GetTInt() < 0)
		{
		iDriveData.iFreeSpaceString.Copy(_L("InvalidVal"));
		}
	else
		{
		iDriveData.iFreeSpaceString.Format(KFormatDec,aVolumeInfo.iFree.GetTInt());
		}
	#endif
	}
	
/**
Gets thread details for a thread with the given name - assumed to be a valid name.
*/
void TResourceDetails::GetThreadData(TDesC& aThreadName)
	{
	RThread thread;
	TInt err = thread.Open(aThreadName);
	if (err == KErrNone)
		{
		// ID
		iThreadData.iIdString.Format(KFormatDec,thread.Id().Id());

		// Name
		iThreadData.iThreadName.Copy(aThreadName);
		
		//to increase readability of resource table
		AppendDotsToResourceName(iThreadData.iThreadName);

		// Priority
		iThreadData.iPriorityString.Format(KFormatDec,thread.Priority());

		// Owner Process
		RProcess owner;
		thread.Process(owner);
		iThreadData.iOwnerIdString.Format(KFormatDec,owner.Id().Id());

		owner.Close();
				
		// Stack Size

		TInt aHeapSize = 0, aStackSize = 0;
		iThreadData.iStackSizeString.Format(KFormatHex,aStackSize);
		iThreadData.iHeapSizeString.Format(KFormatHex,aHeapSize);
		
		// Protected

		// System
		
		// Exit Type
		TExitType et = thread.ExitType();
		switch(et)
			{
			case(0): iThreadData.iExitTypeString.Copy((_L("Kill"))); break;
			case(1): iThreadData.iExitTypeString.Copy((_L("Terminate"))); break;
			case(2): iThreadData.iExitTypeString.Copy((_L("Panic"))); break;
			case(3): iThreadData.iExitTypeString.Copy((_L("Pending"))); break;
			default: iThreadData.iExitTypeString.Copy((_L("Error!"))); break;
			};

		// Exit Reason
		
		TInt er = thread.ExitReason();
		if (er==0) iThreadData.iExitReasonString.Copy(_L("n/a"));
		else iThreadData.iExitReasonString.Format(KFormatHex,(TInt)er);

		// Exit Category

		TExitCategoryName ec;
		ec = thread.ExitCategory();
		if (ec.Length() == 0) iThreadData.iExitCategoryName.Copy(_L("n/a"));
		else iThreadData.iExitCategoryName.Copy(ec);

		thread.Close();
		}
	}
	
/**
Gets details for a chunk using the given TFindChunk and name.
*/
void TResourceDetails::GetChunkData(TFindChunk findHb, TDesC& aChunkName)
	{	
	RChunk chunk;
	chunk.Open(findHb);

	// Name
	iChunkData.iChunkName.Copy(aChunkName);
	
	//to increase readability of resource table
	AppendDotsToResourceName(iChunkData.iChunkName);

	// Size
	TInt chunkSize = 0;
	iChunkData.iSizeString.Format(KFormatHex,chunkSize);

	// Max Size

	TInt chunkMaxSize = 0;
	iChunkData.iMaxSizeString.Format(KFormatHex,chunkMaxSize);
		
	// Readable
	if (chunk.IsReadable()) iChunkData.iReadableIndicator.Copy(_L("R"));
	else iChunkData.iReadableIndicator.Copy(_L("-"));

	// Writeable
	if (chunk.IsWritable()) iChunkData.iWritableIndicator.Copy(_L("W"));
	else iChunkData.iWritableIndicator.Copy(_L("-"));
			
	chunk.Close();
	}

/**
Gets details for a mutex using the given TFindMutex and name.
*/
void TResourceDetails::GetMutexData(TFindMutex& findHb, TDesC& aMutexName)
	{	
	RMutex mutex;
	
	// Name
	iMutexData.iMutexName.Copy(aMutexName);	
	
	//to increase readability of resource table
	AppendDotsToResourceName(iMutexData.iMutexName);
		
	// Count
	mutex.Open(findHb);
	TInt count = 0;
	iMutexData.iHexCountString.Format(KFormatHex, count);
	iMutexData.iDecCountString.Format(KFormatDec, count);
	mutex.Close();
	}
	
/**
Gets details for a mutex using the given TFindSemaphore and name.
*/
void TResourceDetails::GetSemaphoreData(TFindSemaphore& findHb, TDesC& aSemaphoreName)
	{			
		RSemaphore sem;
		// Name
		iSemaphoreData.iSemaphoreName.Copy(aSemaphoreName);
		//to increase readability of resource table
		AppendDotsToResourceName(iSemaphoreData.iSemaphoreName);
		iSemaphoreData.iSemaphoreName.AppendFill('.', KMaxFullName - iSemaphoreData.iSemaphoreName.Length());
		
		// Count
		sem.Open(findHb);
		TInt count = 0;
		iSemaphoreData.iHexCountString.Format(KFormatHex,count);
	
		iSemaphoreData.iDecCountString.Format(KFormatDec,count);
		
		sem.Close();
		}

/**
Gets details for a HAL entry. Assumes that the parameters are valid.
*/
void TResourceDetails::GetHalData(TInt aValidEntryNum, HAL::SEntry* aSEntry)
	{
	iHalData.iNumberString.Format(KFormatDec,aValidEntryNum);

	TBuf<100> halName;
	switch (aValidEntryNum)
		{
		case HALData::EManufacturer:	halName=_L("EManufacturer");break;
		case HALData::EManufacturerHardwareRev:	halName=_L("EManufacturerHardwareRev");break;
		case HALData::EManufacturerSoftwareRev:	halName=_L("EManufacturerSoftwareRev");break;
		case HALData::EManufacturerSoftwareBuild:	halName=_L("EManufacturerSoftwareBuild");break;
		case HALData::EModel:	halName=_L("EModel");break;
		case HALData::EMachineUid:	halName=_L("EMachineUid");break;
		case HALData::EDeviceFamily:	halName=_L("EDeviceFamily");break;
		case HALData::EDeviceFamilyRev:	halName=_L("EDeviceFamilyRev");break;
		case HALData::ECPU:	halName=_L("ECPU");break;
		case HALData::ECPUArch:	halName=_L("ECPUArch");break;
		case HALData::ECPUABI:	halName=_L("ECPUABI");break;
		case HALData::ECPUSpeed:	halName=_L("ECPUSpeed");break;
		case HALData::ESystemStartupReason:	halName=_L("ESystemStartupReason");break;
		case HALData::ESystemException:	halName=_L("ESystemException");break;
		case HALData::ESystemTickPeriod:	halName=_L("ESystemTickPeriod");break;
		case HALData::EMemoryRAM:	halName=_L("EMemoryRAM");break;
		case HALData::EMemoryRAMFree:	halName=_L("EMemoryRAMFree");break;	
		case HALData::EMemoryROM:	halName=_L("EMemoryROM");break;
		case HALData::EMemoryPageSize:	halName=_L("EMemoryPageSize");break;
		case HALData::EPowerGood:	halName=_L("EPowerGood");break;
		case HALData::EPowerBatteryStatus:	halName=_L("EPowerBatteryStatus");break;
		case HALData::EPowerBackup:	halName=_L("EPowerBackup");break;	
		case HALData::EPowerBackupStatus:	halName=_L("EPowerBackupStatus");break;
		case HALData::EPowerExternal:	halName=_L("EPowerExternal");break;
		case HALData::EKeyboard:	halName=_L("EKeyboard");break;
		case HALData::EKeyboardDeviceKeys:	halName=_L("EKeyboardDeviceKeys");break;
		case HALData::EKeyboardAppKeys:	halName=_L("EKeyboardAppKeys");break;
		case HALData::EKeyboardClick:	halName=_L("EKeyboardClick");break;
		case HALData::EKeyboardClickState:	halName=_L("EKeyboardClickState");break;
		case HALData::EKeyboardClickVolume:	halName=_L("EKeyboardClickVolume");break;
		case HALData::EKeyboardClickVolumeMax:	halName=_L("EKeyboardClickVolumeMax");break;
		case HALData::EDisplayXPixels:	halName=_L("EDisplayXPixels");break;
		case HALData::EDisplayYPixels:	halName=_L("EDisplayYPixels");break;
		case HALData::EDisplayXTwips:	halName=_L("EDisplayXTwips");break;
		case HALData::EDisplayYTwips:	halName=_L("EDisplayYTwips");break;
		case HALData::EDisplayColors:	halName=_L("EDisplayColors");break;
		case HALData::EDisplayState:	halName=_L("EDisplayState");break;
		case HALData::EDisplayContrast:	halName=_L("EDisplayContrast");break;
		case HALData::EDisplayContrastMax:	halName=_L("EDisplayContrastMax");break;
		case HALData::EBacklight:	halName=_L("EBacklight");break;
		case HALData::EBacklightState:	halName=_L("EBacklightState");break;
		case HALData::EPen:	halName=_L("EPen");break;
		case HALData::EPenX:	halName=_L("EPenX");break;
		case HALData::EPenY:	halName=_L("EPenY");break;
		case HALData::EPenDisplayOn:	halName=_L("EPenDisplayOn");break;
		case HALData::EPenClick:	halName=_L("EPenClick");break;
		case HALData::EPenClickState:	halName=_L("EPenClickState");break;
		case HALData::EPenClickVolume:	halName=_L("EPenClickVolume");break;
		case HALData::EPenClickVolumeMax:	halName=_L("EPenClickVolumeMax");break;
		case HALData::EMouse:	halName=_L("EMouse");break;
		case HALData::EMouseX:	halName=_L("EMouseX");break;
		case HALData::EMouseY:	halName=_L("EMouseY");break;
		case HALData::EMouseState:	halName=_L("EMouseState");break;
		case HALData::EMouseSpeed:	halName=_L("EMouseSpeed");break;
		case HALData::EMouseAcceleration:	halName=_L("EMouseAcceleration");break;
		case HALData::EMouseButtons:	halName=_L("EMouseButtons");break;
		case HALData::EMouseButtonState:	halName=_L("EMouseButtonState");break;
		case HALData::ECaseState:	halName=_L("ECaseState");break;
		case HALData::ECaseSwitch:	halName=_L("ECaseSwitch");break;
		case HALData::ECaseSwitchDisplayOn:	halName=_L("ECaseSwitchDisplayOn");break;
		case HALData::ECaseSwitchDisplayOff:	halName=_L("ECaseSwitchDisplayOff");break;
		case HALData::ELEDs:	halName=_L("ELEDs");break;
		case HALData::ELEDmask:	halName=_L("ELEDmask");break;
		case HALData::EIntegratedPhone:	halName=_L("EIntegratedPhone");break;
		case HALData::EDisplayBrightness:	halName=_L("EDisplayBrightness");break;
		case HALData::EDisplayBrightnessMax:	halName=_L("EDisplayBrightnessMax");break;
		case HALData::EKeyboardBacklightState:	halName=_L("EKeyboardBacklightState");break;
		case HALData::EAccessoryPower:	halName=_L("EAccessoryPower");break;
		case HALData::ELanguageIndex:	halName=_L("ELanguageIndex");break;
		case HALData::EKeyboardIndex:	halName=_L("EKeyboardIndex");break;
		case HALData::EMaxRAMDriveSize:	halName=_L("EMaxRAMDriveSize");break;
		case HALData::EKeyboardState:	halName=_L("EKeyboardState");break;
		case HALData::ESystemDrive:	halName=_L("ESystemDrive");break;
		case HALData::EPenState:	halName=_L("EPenState");break;
		case HALData::EDisplayIsMono:	halName=_L("EDisplayIsMono");break;
		case HALData::EDisplayIsPalettized:	halName=_L("EDisplayIsPalettized");break;
		case HALData::EDisplayBitsPerPixel:	halName=_L("EDisplayBitsPerPixel");break;
		case HALData::EDisplayNumModes:	halName=_L("EDisplayNumModes");break;
		case HALData::EDisplayMemoryAddress:	halName=_L("EDisplayMemoryAddress");break;
		case HALData::EDisplayOffsetToFirstPixel:	halName=_L("EDisplayOffsetToFirstPixel");break;
		case HALData::EDisplayOffsetBetweenLines:	halName=_L("EDisplayOffsetBetweenLines");break;
		case HALData::EDisplayPaletteEntry:	halName=_L("EDisplayPaletteEntry");break;
		case HALData::EDisplayIsPixelOrderRGB:	halName=_L("EDisplayIsPixelOrderRGB");break;
		case HALData::EDisplayIsPixelOrderLandscape:	halName=_L("EDisplayIsPixelOrderLandscape");break;
		case HALData::EDisplayMode:	halName=_L("EDisplayMode");break;
		case HALData::ESwitches:	halName=_L("ESwitches");break;
		case HALData::EDebugPort:	halName=_L("EDebugPort");break;
		case HALData::ELocaleLoaded:	halName=_L("ELocaleLoaded");break;
		case HALData::ENumHalAttributes:	halName=_L("ENumHalAttributes");break;
		default:halName=_L("Unknown");break;
		}

		iHalData.iName.Copy(halName);		

		if (aSEntry[aValidEntryNum].iProperties & HAL::EEntryDynamic)
			iHalData.iDynamicIndicator=_L("D");
		else iHalData.iDynamicIndicator=_L("-");
		
		iHalData.iDecValString.Format(KFormatDec,aSEntry[aValidEntryNum].iValue);
			
		iHalData.iHexValString.Format(KFormatHex,aSEntry[aValidEntryNum].iValue);
		}

/**
Appends dots (after 3 spaces) to a resource name.
Used for names of resources in a table - so you can follow the dots to read the
resource details.
*/
void TResourceDetails::AppendDotsToResourceName(TFullName& aResourceName)
	{
	if (aResourceName.Length() < KMaxFullName - 3)
			aResourceName.AppendFill(' ', 3);
		aResourceName.AppendFill('.', KMaxFullName - aResourceName.Length());
	}
	
