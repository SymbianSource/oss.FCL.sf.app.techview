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

#ifndef __ResourceHandler_DataStructs_H
#define __ResourceHandler_DataStructs_H


#include <e32std.h> //descriptors, TUidType etc

struct TListData
/** Stores library, server, device and logical channel data */
{
	TFullName iName;
};

struct TProcessData
/** Stores process details */
	{
	TBuf<8> iIdString;
	TFullName iProcessName; // this is a TBuf (big)
	TBuf<8> iPriorityString;
	TBuf<1> iProtectedIndicator;
	TBuf<1> iSystemIndicator;
	TBuf<8> iOwnerIdString;
	TBuf<15> iExitTypeString;
	TBuf<8> iExitReasonString;
	TExitCategoryName iExitCategoryName; // this is a TBuf
	TBuf<KMaxUidName> iUidType0String;	//Note: buffer could be shorter as UID is converted to hexadecimal format
	TBuf<KMaxUidName> iUidType1String;
	TBuf<KMaxUidName> iUidType2String;
	TFileName iFileName; // this is a TBuf
	};

struct TThreadData
/** Stores thread details */
	{
	TBuf<8> iIdString;
	TFullName iThreadName; // this is a TBuf (big)
	TBuf<10> iPriorityString;
	TBuf<8> iOwnerIdString;
	TBuf<8> iStackSizeString;
	TBuf<8> iHeapSizeString;
	TBuf<1> iProtectedIndicator;
	TBuf<1> iSystemIndicator;
	TBuf<15> iExitTypeString;
	TBuf<8> iExitReasonString;
	TExitCategoryName iExitCategoryName; // this is a TBuf
	};



struct TChunkData
/** Stores chunk details */
	{
	TFullName iChunkName; // this is a TBuf (big)
	TBuf<8> iSizeString;
	TBuf<8> iMaxSizeString;
	TBuf<1> iReadableIndicator;
	TBuf<1> iWritableIndicator;
	};

struct TSemaphoreData
/** Stores semaphore details */
	{
	TFullName iSemaphoreName; // this is a TBuf (big)
	TBuf<8> iHexCountString;
	TBuf<8> iDecCountString;
	};

struct TMutexData
/** Stores mutex details */
	{
	TFullName iMutexName; // this is a TBuf (big)
	TBuf<8> iHexCountString;
	TBuf<8> iDecCountString;
	};

struct TDriveData
/** Stores drive details */
	{
	TFullName iDriveName; // this is a TBuf (big)
	TFullName iVolumeName; // this is a TBuf (big) //volume mounted on drive
	TBuf<2> iDriveNumberString;
	TBuf<20> iMediaString; //Media mounted on drive
	TBuf<12> iAttributesString;
	TBuf<12> iSpaceString;
	TBuf<12> iFreeSpaceString;
	};

struct THalData
/**Stores HAL attributes data*/
{
	TBuf<4> iNumberString; //sequential number for each item
	TBuf<30> iName;
	TBuf<1> iDynamicIndicator;
	TBuf<12> iDecValString; //Decimal attribute value - not sure of purpose of this
	TBuf<12> iHexValString; //Hex attribute value - not sure of purpose of this
};

#endif
