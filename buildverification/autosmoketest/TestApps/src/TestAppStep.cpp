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

#include "TestAppStep.h"


CTestAppStep1::~CTestAppStep1()
/**
 * Destructor
 */
	{
	}

CTestAppStep1::CTestAppStep1()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KSampleStep1);
	}

TVerdict CTestAppStep1::doTestStepPreambleL()
/**
 * @return - TVerdict code
 */
	{
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CTestAppStep1::doTestStepL()
/**
 * @return - TVerdict code
 * Smoke test step for a techview application.
 * Retrieves an exe name and corresponding uid from the config file.
 * Runs the exe. Delays for 20 secs. Checks that the exe is still running.
 */
	{
	INFO_PRINTF1(_L("In Test Step CTestAppStep1"));
	SetTestStepResult(EFail);

  	// Get the name of the app to run from the .ini configuration file.	
	TPtrC exeToRun;
  	_LIT(KExeName, "exename");
  	GetStringFromConfig(ConfigSection(),KExeName,exeToRun);
  	INFO_PRINTF2(_L("The exe to run is %S"), &exeToRun);
  	  
  	// Create a new process to run the executable in
  	RProcess testProcess;	 
  	if(testProcess.Create(exeToRun, KNullDesC()) == KErrNone)
  		{testProcess.Resume();}
  	else 
  		{return TestStepResult();}

  	// Retrieve the UID of the app in order to check that it is running.
  	TInt uidValue;
  	_LIT(KUidKey, "uid");
  	if (!GetHexFromConfig(ConfigSection(), KUidKey, uidValue))
  		{
  	  	ERR_PRINTF1(_L("Cannot read UID from config section."));	  
  	  	return TestStepResult();
  	  	}
  	TUid exeUid;
  	exeUid.iUid = uidValue;	   
  	  	 
  	// In order to test this test code the process can be forced to panic
  	// _LIT(KTestPanic, "test panic");
  	// testProcess.Panic(KTestPanic, 123);
  	
  	// Make sure app has stayed up for at least 20 secs
  	User::After(2000000);
  	INFO_PRINTF2(_L("Check that %S is still running after at least 20 seconds"), &exeToRun); 
  	 
    // Loop through each process (match against default of *)
  	TFindProcess findProcess;
  	TFullName fullName;
  	
   
  	while((findProcess.Next(fullName) == KErrNone)) //&& (TestStepResult() == EFail))
  		{
  		RProcess process;
  		TInt ret=KErrNone;
  		TInt count=0;
  		
  		// Open a handle to the process
  		do
  			{
  			//INFO_PRINTF2(_L("process running is %S"), &fullName); 
  			count++;
  	 		ret=process.Open(findProcess);
  			if (ret!=KErrNone)
  				{
  				User::After(200000);
  				}
  			}while (ret!=KErrNone && count<=10);
  			
  		if (ret!=KErrNone)
  			{
  			User::Leave(ret);
  			}
  		
  		// Extract any panic characteristics
  		TUidType uid = process.Type();
  		TExitType exitType = process.ExitType();
  		TExitCategoryName exitCategory = process.ExitCategory();
  		TInt exitReason = process.ExitReason();
  		process.Close();
  		
  		// If the uid being searched for is found and is running set the test step result to
  		// pass.
  		if(uid[2] == exeUid)
  			{
  			if (exitType == EExitPending)
  				{
  				INFO_PRINTF2(_L("A running process has been found with UID 0x%x"), exeUid );
  				SetTestStepResult(EPass);
  				}
  			else {
  				if (exitType == EExitPanic) //print for debugging purposes
  					{
  					INFO_PRINTF2(_L("The process with UID 0x%x has exited due to a panic"), exeUid );
  					INFO_PRINTF3(_L("The exit Reason is %d, The exit category is \"%S\""), exitReason, &exitCategory);				 
  					}
  				 }
  			} // uid[2] == exeUid				 	 
  	} // while	  	  
  	 
  	// clean up 
  	testProcess.Kill(KErrNone);
  	testProcess.Close();
  		
	return TestStepResult();
	}

TVerdict CTestAppStep1::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}
