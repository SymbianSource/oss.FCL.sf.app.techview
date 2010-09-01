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

void CResourceAppUi::ConstructL()
    {
	BaseConstructL();
	iListBoxView = CResourceView::NewL(ClientRect(), this);
	//construct a default view
	HandleCommandL(EResourceMenu1Item0);
	}

CResourceAppUi::~CResourceAppUi()
	{
	delete iListBoxView;
	}

void CResourceAppUi::HandleCommandL(TInt aCommand)
	{
	// Increase this threads priority for a proper snapshot
	RThread thisThread;
	thisThread.SetPriority(EPriorityAbsoluteHigh);
	thisThread.Close();

	TObjectsViewed aObjectViewed = EObjectNone;

	switch (aCommand)
		{

	// Menu 1

	case EResourceMenu1Item0:
		aObjectViewed = EObjectProcess;
		break;

	case EResourceMenu1Item1:	
		aObjectViewed = EObjectThread;
		break;
	
	case EResourceMenu1Item2:	
		aObjectViewed = EObjectChunk;
		break;
               
	case EResourceMenu1Item3:
		aObjectViewed = EObjectLibrary;
		break;

	case EResourceMenu1Item4:
		aObjectViewed = EObjectServer;
		break;

	case EResourceMenu1Item5:
		aObjectViewed = EObjectSemaphore;
		break;

	case EResourceMenu1Item6:
		aObjectViewed = EObjectMutex;
		break;

	case EResourceMenu1Item7:
		aObjectViewed = EObjectLogicalDevice;
		break;

	case EResourceMenu1Item8:
		aObjectViewed = EObjectPhysicalDevice;
		break;

	case EResourceMenu1Item9:
		aObjectViewed = EObjectLogicalChannel;
		break;

	// Menu 2

	case EResourceMenu2Item0:
		aObjectViewed = EObjectWindow;
		break;
	
	case EResourceMenu2Item1:
		aObjectViewed = EObjectDrive;
		break;

	case EResourceMenu2Item2:
		aObjectViewed = EObjectHal;
		break;
		
	// Menu 3
	case EResourceMenu3Item0:
		SetDebug();
		break;
	case EResourceMenu3Item1:
		SetThread();
		break;
	case EResourceMenu3Item2:
		SetProcess();
		break;
	case EResourceMenu3Item3:
		SetRam();
		break;
	case EResourceMenu3Item4:
		SetDrive();
		break;
	case EEikCmdExit: // necessary for uninstalling
		Exit();
		break;
	default:
		break;
		}

	// Set view data
	if (aObjectViewed != EObjectNone)
		{
		iListBoxView->SetCurrentObjects(aObjectViewed);
		iListBoxView->UpdateView();
		}

	// ResetPriority
	RThread thisThread2;
	thisThread2.SetPriority(EPriorityNormal);
	thisThread2.Close();

	}

void CResourceAppUi::SetProcess()
	{
	CProcessNumberDialog* dialog = new(ELeave) CProcessNumberDialog();

	dialog->ExecuteLD(R_PROCESS_NUMBER_DIALOG);
	
	}

void CResourceAppUi::SetThread()
	{
	CThreadNumberDialog* dialog = new(ELeave) CThreadNumberDialog();

	dialog->ExecuteLD(R_THREAD_DIALOG);
		
	}

void CResourceAppUi::SetDebug()
	{
	CDebugDialog* dialog = new(ELeave) CDebugDialog();

	dialog->ExecuteLD(R_DEBUG_DIALOG);
		
	}

void CResourceAppUi::SetRam()
	{
	CRamDialog* dialog = new(ELeave) CRamDialog();

	dialog->ExecuteLD(R_RAM_DIALOG);
		
	}

void CResourceAppUi::SetDrive()
	{
	CDriveDialog* dialog = new(ELeave) CDriveDialog();

	dialog->ExecuteLD(R_DRIVE_DIALOG);
		
	}

void CResourceAppUi::AppendDesc(TDes& root, TDes& add)
	{
	add.Trim();
	root.Append(add);
	root.Append(_L(",  ")); 
	}


