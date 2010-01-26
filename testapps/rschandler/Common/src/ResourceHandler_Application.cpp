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
 

TUid CResourceApplication::AppDllUid() const
	{
	TUid KUidResourceHandler = { 0x101FD147 };
	return KUidResourceHandler;
	}

CApaDocument* CResourceApplication::CreateDocumentL()
	{
	return new (ELeave) CResourceDocument(*this);
	}
	
