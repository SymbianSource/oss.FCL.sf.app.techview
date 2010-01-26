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
#ifdef TV
#include <eikstart.h>
#endif

//             The entry point for the application code. It creates
//             an instance of the CApaApplication derived
//             class, CResourceApplication.
//


#ifdef UIQ
EXPORT_C CApaApplication* NewApplication()
	{
	return new CResourceApplication;
	}
	
//             This function is required by all EPOC32 DLLs. In this 
//             Resource, it does nothing.
//
GLDEF_C TInt E32Dll(TDllReason)
	{
	return KErrNone;
	}
#endif

#ifdef TV
LOCAL_C CApaApplication* NewApplication()
	{
	return new CResourceApplication;
	}

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication(NewApplication);
	}

#endif
