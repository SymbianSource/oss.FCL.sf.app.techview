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

#if (!defined __TESTAPP_STEP_H__)
#define __TESTAPP_STEP_H__
#include <test/testexecutestepbase.h>
#include "TestAppServer.h"

 
_LIT(KSampleStep1,"TestAppStep1");

class CTestAppStep1 : public CTestStep
	{
public:
	CTestAppStep1();
	~CTestAppStep1();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();
	virtual TVerdict doTestStepL();
private:
	};

 
 
#endif
