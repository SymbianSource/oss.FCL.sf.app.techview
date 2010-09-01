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

#ifndef RESOURCEHANDLER_DIALOGS_H
#define RESOURCEHANDLER_DIALOGS_H

#include <techview/eikdialg.h>

class CDialogBase : public CEikDialog
	{
public:
	// constructor
	CDialogBase();
public:
	void DisplayError(TDesC& aMessage, TInt aErrorNumber);
	void DisplayError(TDesC& aMessage);
	};

class CThreadNumberDialog : public CDialogBase
	{
public:
	// constructor
	CThreadNumberDialog();
private:
	void PreLayoutDynInitL();
	void PostLayoutDynInitL();
	TBool OkToExitL(TInt aKeycode);
	void SetThread();
	void UpdateDialogData(void);
	void UpdateDialogDataReal(void);
	void UpdateDialogDataAll(void);
	void UpdatePriorityText(void); //updates the priority number value to correspond to the priority option selected
	void HandleControlStateChangeL(TInt aControlId);

private:
	TThreadId iThreadID;
	};

class CProcessNumberDialog : public CDialogBase
	{
public:
	// constructor
	CProcessNumberDialog();
private:
	void PreLayoutDynInitL();
	void PostLayoutDynInitL();
	TBool OkToExitL(TInt aKeycode);
	void UpdateDialogData(void);
	void UpdatePriorityText(void);
	void UpdateProcessId(void);
	void SetProcess();
	void HandleControlStateChangeL(TInt aControlId);

private:
	TProcessId iProcessID;
	};

class CDebugDialog : public CDialogBase
	{
public:
	// constructor
	CDebugDialog();
private:
	void PreLayoutDynInitL();
	void GetKernelOptions();
	void GetPortNumber();
	TBool OkToExitL(TInt aKeycode);
	void SetDebugOptions();
private:
	TInt iCurrentDebugPort;
	};

class CRamDialog : public CDialogBase
	{
public:
	// constructor
	CRamDialog();

private:
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aKeycode);
	void FillRam();
	void UndoFillRam();
	void UpdateDialogData();
	
private:
	RChunk chunks[10000];
	TUint noChunks;
	};

class CDriveDialog : public CDialogBase
	{
public:
	// constructor
	CDriveDialog();
	~CDriveDialog();
	void UpdateDialogLeaveSpace();
private:
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aKeycode);
private:
	CDesCArrayFlat* iDriveName;
	CArrayFixFlat<TInt>* iDriveLetter;
	};

#endif 
