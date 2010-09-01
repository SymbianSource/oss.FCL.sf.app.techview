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
// uniload.h - Universal file load (work for narrow or wide builds)
//



/**
 @internalComponent
*/

#ifndef __UNILOAD_H__
#define __UNILOAD_H__

#include <e32std.h>
#include <f32file.h>

class UnicodeLoad
	{
public:
	static HBufC *LoadL(RFs &aFs, const TDesC &aName);
	};

#endif

