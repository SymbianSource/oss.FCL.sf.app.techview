/**
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



/**
 @file SmoketestContactsServer.h
*/
#if (!defined __SMOKETESTCONTACTS_SERVER_H__)
#define __SMOKTETESTCONTACTS_SERVER_H__
#include <test/testexecuteserverbase.h>

class CSmoketestContactsServer : public CTestServer
	{
public:
	static CSmoketestContactsServer* NewL();
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);

private:
	void ConstructL();
	};
#endif
