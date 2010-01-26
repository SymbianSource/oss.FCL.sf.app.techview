/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TestAppServer.h"
#include "TestAppStep.h"

 
_LIT(KServerName,"Smoketest_TestApp_Server");
 
CTestAppServer* CTestAppServer::NewL()
/**
 * @return - Instance of the test server
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
	{
	CTestAppServer * server = new (ELeave) CTestAppServer();
	CleanupStack::PushL(server);
	
	server-> ConstructL(KServerName);
	CleanupStack::Pop(server);
	return server;
	}

 
LOCAL_C void MainL()
	{
	// Leave the hooks in for platform security
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);
#endif
	CActiveScheduler* sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	 
	CTestAppServer* server = NULL;
	TRAPD(err,server = CTestAppServer::NewL());
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	delete sched;
	}

GLDEF_C TInt E32Main()
/**
 * @return - Standard Epoc error code on exit
 */
	{
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAP_IGNORE(MainL());
	delete cleanup;
	return KErrNone;
    }

 
CTestStep* CTestAppServer::CreateTestStep(const TDesC& aStepName)
/**
 * @return - A CTestStep derived instance
 */
	{
	CTestStep* testStep = NULL;
	 
	if(aStepName == KSampleStep1)
		testStep = new CTestAppStep1();
	 
	return testStep;
	}

