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
// This contains CTestTimewCase which is the base class for all the Timew TestCase DLL
// 
//

// EPOC includes
#include <e32base.h>
#include <uri16.h>
#include <uriutils.h>
#include <test/testexecutelog.h>
#include "TestTimewbase.h"

CTestTimewBase::CTestTimewBase()
: CTestStep()
, iSchedular(NULL)
, iModel(NULL)
	{
	}

// destructor
CTestTimewBase::~CTestTimewBase()
	{
	delete iSchedular;
	iSchedular=NULL;
	}

enum TVerdict CTestTimewBase::doTestStepPreambleL()
	{
	iSchedular=new CActiveScheduler();
	CActiveScheduler::Install(iSchedular);

	return CTestStep::doTestStepPreambleL();
	}

enum TVerdict CTestTimewBase::doTestStepPostambleL()
	{
	delete iSchedular;
	iSchedular=NULL;
	CActiveScheduler::Install(NULL);

	return CTestStep::doTestStepPostambleL();
	}

CAlmModel*	CTestTimewBase::Model(void)
/**
returns a pointer to an instance of CAgnModel
*/
	{
  	return iModel;
	}
