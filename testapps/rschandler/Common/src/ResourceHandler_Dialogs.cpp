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

#include <e32svr.h>
#include "ResourceHandler.h"

//
// Thread Dialog
//

CThreadNumberDialog::CThreadNumberDialog()
	{
	}

void CThreadNumberDialog::PreLayoutDynInitL()
	{	
	// to start use the thread ID of this thread
	iThreadID = RThread().Id();
	UpdateDialogData();
	}

void CThreadNumberDialog::PostLayoutDynInitL()
	{	

	}

void CThreadNumberDialog::UpdateDialogData()
	{
	if (iThreadID > 0) UpdateDialogDataReal(); // Real thread
	else UpdateDialogDataAll(); // 0 for all unprotected threads 
	}

void CThreadNumberDialog::UpdateDialogDataAll()
//update dialog - set for all threads
	{
	// match controls and set values
	CXIKNUMBEREDITOR *thisNumberEditor;
	thisNumberEditor = STATIC_CAST(CXIKNUMBEREDITOR*, Control(EThreadNumberCtl));
	thisNumberEditor->SETNUMBERVALUEL(0);

	CEikCheckBox *sysCheckBox, *protCheckBox;
	sysCheckBox = STATIC_CAST(CEikCheckBox*,Control(EThreadSystemCtl));
	protCheckBox = STATIC_CAST(CEikCheckBox*,Control(EThreadProtectedCtl));

	CEikCommandButton *killBut, *okBut, *setBut;
	killBut = STATIC_CAST(CEikCommandButton*, ButtonGroupContainer().CommandButtonOrNull(EThreadKillButtonCtl));
	okBut = STATIC_CAST(CEikCommandButton*, ButtonGroupContainer().CommandButtonOrNull(EEikBidOk));
	setBut = STATIC_CAST(CEikCommandButton*, ButtonGroupContainer().CommandButtonOrNull(EThreadSetButtonCtl));
	CEikLabel *priorityDialogPrompt, *priorityNumberLabel;
	priorityNumberLabel = STATIC_CAST(CEikLabel*, Control(EThreadPriorityText));
	priorityDialogPrompt = STATIC_CAST(CEikLabel*, Control(EThreadPriorityPrompt));
	
	priorityDialogPrompt->SetTextL(_L("Cannot set priority for all threads."));
	
#ifdef UIQ
	CEikLabel *theThreadLabel, *theProcessLabel;
	theThreadLabel = STATIC_CAST(CEikLabel*, Control(EThreadLabelThreadName));
	theProcessLabel = STATIC_CAST(CEikLabel*, Control(EThreadLabelProcessName));
	theProcessLabel->SetTextL(_L("Various"));
	theThreadLabel->SetTextL(_L("All unprotected"));
#endif
#ifdef TV
	CEikLabel *threadAndProcessLabel;
	threadAndProcessLabel = STATIC_CAST(CEikLabel*, Control(EThreadLabelThreadAndProcessName));
	threadAndProcessLabel->SetTextL(_L("All threads/processes"));
#endif
	CEikChoiceList *prioritySelector;
	prioritySelector = STATIC_CAST(CEikChoiceList*, Control(EThreadPriorityButtonCtl));


	// Change the dimming on the component controls as appropriate for all threads
	protCheckBox->SetDimmed(ETrue);
	sysCheckBox->SetDimmed(EFalse);

	prioritySelector->MakeVisible(EFalse);

	priorityNumberLabel->MakeVisible(EFalse);

	killBut->SetDimmed(ETrue);
	okBut->SetDimmed(EFalse);
	setBut->SetDimmed(EFalse);
	DrawNow();

	}

//updates dialog depending on iThreadID
void CThreadNumberDialog::UpdateDialogDataReal()
	{
	// match controls
	CXIKNUMBEREDITOR *thisNumberEditor;
	thisNumberEditor = STATIC_CAST(CXIKNUMBEREDITOR*, Control(EThreadNumberCtl));
	thisNumberEditor->SETNUMBERVALUEL(0);

	CEikCheckBox *sysCheckBox, *protCheckBox;
	sysCheckBox = STATIC_CAST(CEikCheckBox*,Control(EThreadSystemCtl));
	protCheckBox = STATIC_CAST(CEikCheckBox*,Control(EThreadProtectedCtl));

	CEikCommandButton *killBut, *okBut, *setBut;
	okBut = STATIC_CAST(CEikCommandButton*, ButtonGroupContainer().CommandButtonOrNull(EEikBidOk));
	setBut = STATIC_CAST(CEikCommandButton*, ButtonGroupContainer().CommandButtonOrNull(EThreadSetButtonCtl));
	killBut = STATIC_CAST(CEikCommandButton*, ButtonGroupContainer().CommandButtonOrNull(EThreadKillButtonCtl));
		
#ifdef UIQ
	CEikLabel *theThreadLabel, *theProcessLabel;
	theThreadLabel = STATIC_CAST(CEikLabel*, Control(EThreadLabelThreadName));
	theProcessLabel = STATIC_CAST(CEikLabel*, Control(EThreadLabelProcessName));
#endif
#ifdef TV
	CEikLabel *threadAndProcessLabel;
	threadAndProcessLabel = STATIC_CAST(CEikLabel*, Control(EThreadLabelThreadAndProcessName));
#endif	

	CEikChoiceList *priorityOpButs;
	priorityOpButs = STATIC_CAST(CEikChoiceList*, Control(EThreadPriorityButtonCtl));

	//
	//Update dialog info with thread details
	//

	RThread thread;
	TInt err = KErrNone;
	
	err = thread.Open(iThreadID);
		
	if (err!=KErrNone)
		{ 
		thread.Close();
		TBuf<70> errorMessage(_L("Cannot get thread number "));
		errorMessage.AppendNum(iThreadID);
		errorMessage.Append(_L(". Thread::Open()"));
		DisplayError(errorMessage, err);
		thread.Open(RThread().Id()); // Get this thread
		}

	thisNumberEditor->SETNUMBERVALUEL(thread.Id());

#ifdef UIQ
	theThreadLabel->SetTextL(thread.Name()); 
#endif
#ifdef TV
	TBuf<150> lProcessAndThread(thread.Name());
#endif

	RProcess process;
	thread.Process(process);
#ifdef UIQ
	theProcessLabel->SetTextL(process.Name());
#endif
#ifdef TV
	lProcessAndThread.Append(_L("/"));
	lProcessAndThread.Append(process.Name());
	threadAndProcessLabel->SetTextL(lProcessAndThread); 
#endif
	process.Close();
	
	CEikLabel *priorityDialogPrompt, *priorityNumberLabel;
	
	priorityDialogPrompt = STATIC_CAST(CEikLabel*, Control(EThreadPriorityPrompt));	
	if ( iThreadID == RThread().Id()) priorityDialogPrompt->SetTextL(_L("The priority of the current thread cannot be reset from within this application."));
	else priorityDialogPrompt->SetTextL(_L("Set the priority."));
	
	priorityNumberLabel = STATIC_CAST(CEikLabel*, Control(EThreadPriorityText));
	TBuf<30> priorityText(_L("Priority Value:  "));
	TBuf<4> priorityNumber;
	
	priorityNumber.Format(KFormatDec,thread.Priority());
	priorityText.Append(priorityNumber);
	priorityNumberLabel->SetTextL(priorityText);

	TThreadPriority lThreadPriority = thread.Priority();
	TInt lThreadPrioritySetting;
	switch (lThreadPriority)
	{
		case(EPriorityAbsoluteHigh): lThreadPrioritySetting = EThreadP500_AbsoluteHigh; break;
		case(EPriorityAbsoluteForeground): lThreadPrioritySetting = EThreadP400_AbsoluteForeground; break;
		case(EPriorityAbsoluteBackground): lThreadPrioritySetting = EThreadP300_AbsoluteBackground; break;
		case(EPriorityAbsoluteLow): lThreadPrioritySetting = EThreadP200_AbsoluteLow; break;
		case(EPriorityAbsoluteVeryLow): lThreadPrioritySetting = EThreadP100_AbsoluteVeryLow; break;
		case(EPriorityRealTime): lThreadPrioritySetting = EThreadP30_RealTime; break;
		case(EPriorityMuchMore): lThreadPrioritySetting = EThreadP20_MuchMore; break;
		case(EPriorityMore): lThreadPrioritySetting = EThreadP10_More; break;
		case(EPriorityNormal): lThreadPrioritySetting = EThreadP0_Normal; break;
		case(EPriorityLess): lThreadPrioritySetting = EThreadPm10_Less; break;
		case(EPriorityMuchLess): lThreadPrioritySetting = EThreadPm20_MuchLess; break;
		case(EPriorityNull): lThreadPrioritySetting = EThreadPm30_Null; break;
		default: lThreadPrioritySetting = EThreadP_NotStandard; break;
	}

	priorityOpButs->SetCurrentItem(lThreadPrioritySetting);

	sysCheckBox->SetState(CEikButtonBase::EIndeterminate);
	protCheckBox->SetState(CEikButtonBase::EIndeterminate);

	// Change the dimming on the component controls depending on the thread
	sysCheckBox->SetDimmed(EFalse);

	if ( iThreadID == RThread().Id()) priorityOpButs->SetDimmed(ETrue);
	else priorityOpButs->SetDimmed(EFalse);

	killBut->SetDimmed(EFalse);
	okBut->SetDimmed(EFalse);
	setBut->SetDimmed(EFalse);
	
	if (iThreadID == RThread().Id())
		{
		// can change the protection on this thread
		protCheckBox->SetDimmed(EFalse);
		}
	else
		{
		// cannot change protection
		protCheckBox->SetDimmed(ETrue);
		if (true)	// If we are in secure then we ensure that the
					// user can not change anything.
			{
			// cannot change anything
			sysCheckBox->SetDimmed(ETrue);
			priorityOpButs->SetDimmed(ETrue);
			killBut->SetDimmed(ETrue);
			okBut->SetDimmed(ETrue);
			setBut->SetDimmed(ETrue);
			}

		}

	thread.Close();
	DrawNow();

	}

void CThreadNumberDialog::UpdatePriorityText()
//updates the priority number value to correspond to the priority option selected
	{	

	}

void CThreadNumberDialog::HandleControlStateChangeL(TInt aControlId)
	{
	if (aControlId == EThreadGetButtonCtl)
		{
		CXIKNUMBEREDITOR *thisNumberEditor; 
		thisNumberEditor = STATIC_CAST(CXIKNUMBEREDITOR*, Control(EThreadNumberCtl));
		iThreadID = (TThreadId)thisNumberEditor->NUMBERVALUE();
		// re-evaluate everything
		UpdateDialogData(); 

		// re-draw
		PageChangedL(EThreadPageGeneral); 
		PageChangedL(EThreadPagePriority);
		PageChangedL(EThreadPageOther);
		DrawNow();
		}
	}

void CThreadNumberDialog::SetThread()
	{
	//change thread parameters
		if (iThreadID > 0)
			{
			// only get here if thread is not protected		


			RThread thread;
			TInt err = thread.Open(iThreadID);

			if (err == KErrNone)
				{
				// This is the current thread
				if ( iThreadID == RThread().Id())
					{
					}
				
					

				CEikChoiceList *opButs;
				opButs = STATIC_CAST(CEikChoiceList*, Control(EThreadPriorityButtonCtl));

				TThreadPriority threadPrioritySet = EPriorityNull;
				TInt currentItem = opButs->CurrentItem();
				
				switch (currentItem) 
					{
					case (EThreadP500_AbsoluteHigh): threadPrioritySet=EPriorityAbsoluteHigh; break;
					case (EThreadP400_AbsoluteForeground): threadPrioritySet=EPriorityAbsoluteForeground; break;
					case (EThreadP300_AbsoluteBackground): threadPrioritySet=EPriorityAbsoluteBackground; break;
					case (EThreadP200_AbsoluteLow): threadPrioritySet=EPriorityAbsoluteLow; break;
					case (EThreadP100_AbsoluteVeryLow): threadPrioritySet=EPriorityAbsoluteVeryLow; break;
					case (EThreadP30_RealTime): threadPrioritySet=EPriorityRealTime; break;
					case (EThreadP20_MuchMore): threadPrioritySet=EPriorityMuchMore; break;
					case (EThreadP10_More): threadPrioritySet=EPriorityMore; break;
					case (EThreadP0_Normal): threadPrioritySet=EPriorityNormal; break;
					case (EThreadPm10_Less): threadPrioritySet=EPriorityLess; break;
					case (EThreadPm20_MuchLess): threadPrioritySet=EPriorityMuchLess; break;
					case (EThreadPm30_Null): threadPrioritySet=EPriorityNull; 
					iEikonEnv->InfoWinL(_L("Error"), _L("Cannot change thread priority to Null"));
					break;
					default: iEikonEnv->InfoWinL(_L("Error"), _L("Invalid priority: cannot change thread priority to Not Standard"));	
						
					};

				if (threadPrioritySet != EPriorityNull) 
				{
					thread.SetPriority(threadPrioritySet);
						
					TThreadPriority threadPriorityGot = thread.Priority();
					if (threadPrioritySet!=threadPriorityGot)
						{		
						iEikonEnv->InfoWinL(_L("Error"), _L("Thread priority not set: RThread::SetPriority() unsuccessful!"));	
						}
					thread.Close();
					}
				}
			}
		else //all threads selected - only changes system setting
			{

			TName prefix(KAsterisk);
			TFindThread findHb;
			findHb.Find(prefix);
			TFullName name; // this is a TBuf
			RThread thread;

			while (findHb.Next(name)==KErrNone)
				{
				TInt err = thread.Open(name);
				if (err != KErrNone)
					{	
					TBuf<70> errorMessage(_L("Cannot get thread "));
					errorMessage.Append(name);
					errorMessage.Append(_L(". RThread::Open()"));
					DisplayError(errorMessage, err);
					}	
					
				if (err == KErrNone)
					{


					thread.Close();
					}
				}
			}	
	}

TBool CThreadNumberDialog::OkToExitL(TInt aButtonId)
	{
	if (aButtonId == EEikBidCancel) 
		{
		return ETrue;
		}
	else if (aButtonId == EThreadSetButtonCtl)
		{
		SetThread();
		UpdateDialogData();
		iEikonEnv->InfoMsg(_L("Attribute(s) set as displayed"));
		return EFalse;
		}
	else if (aButtonId == EEikBidOk)
		{
		SetThread();
		return ETrue;
	}
	else if (aButtonId == EThreadKillButtonCtl)
		{
		RThread thread;
		TInt err = thread.Open(iThreadID);
		if (err == KErrNone)
			{
			thread.Kill(KErrNone);
			iEikonEnv->InfoMsg(_L("Thread Killed"));
			}
		else iEikonEnv->InfoWinL(_L("Error"), _L("Attempt to open thread for modification unsuccessful."));
		thread.Close();
		return EFalse;
		}
	return ETrue;
	}

//
//
// Process Dialog
//
//

CProcessNumberDialog::CProcessNumberDialog()
	{
	}

void CProcessNumberDialog::PreLayoutDynInitL()
	{	
	// to start use the thread ID of this Process
	iProcessID = RProcess().Id();
	UpdateDialogData();
	}

void CProcessNumberDialog::PostLayoutDynInitL()
	{	
	}

void CProcessNumberDialog::UpdateDialogData()
//Updates GUI data
	{
	// match the controls
	CXIKNUMBEREDITOR *thisNumberEditor;
	thisNumberEditor = STATIC_CAST(CXIKNUMBEREDITOR*, Control(EProcessNumberCtl));
	CEikLabel *priorityTextLabel;
	priorityTextLabel = STATIC_CAST(CEikLabel*, Control(EProcessPriorityText));

	CEikCheckBox *sysCheckBox, *protCheckBox;
	sysCheckBox = STATIC_CAST(CEikCheckBox*,Control(EProcessSystemCtl));
	protCheckBox = STATIC_CAST(CEikCheckBox*,Control(EProcessProtectedCtl));

	CEikCommandButton *killBut, *okBut, *setBut;
	killBut = STATIC_CAST(CEikCommandButton*, ButtonGroupContainer().CommandButtonOrNull(EProcessKillButtonCtl));
	okBut = STATIC_CAST(CEikCommandButton*, ButtonGroupContainer().CommandButtonOrNull(EEikBidOk));
	setBut = STATIC_CAST(CEikCommandButton*, ButtonGroupContainer().CommandButtonOrNull(EProcessSetButtonCtl));

	CEikLabel *theProcessLabel;
	theProcessLabel = STATIC_CAST(CEikLabel*, Control(EProcessLabelProcessName));

	CEikChoiceList *opButs;
	opButs = STATIC_CAST(CEikChoiceList*, Control(EProcessPriorityButtonCtl));

	//open a process and update GUI data
	RProcess process;
	TInt err = KErrNone;
	
	err = process.Open(iProcessID); // Opens a handle to a process
	
	if (err!=KErrNone)
		{ 
		TBuf<70> errorMessage(_L("Cannot get process number "));
		errorMessage.AppendNum(iProcessID);
		errorMessage.Append(_L(". System error number "));
		errorMessage.AppendNum(err);

		iEikonEnv->InfoWinL(_L("Error"), errorMessage);
		iProcessID = RProcess().Id();
		process.Open(iProcessID); // Opens a handle to this Process
		}

	thisNumberEditor->SETNUMBERVALUEL(process.Id());
	theProcessLabel->SetTextL(process.Name());
	
	TBuf<30> priorityText(_L("Priority Value:  "));
	TBuf<10> priorityNumber;
	priorityNumber.Format(KFormatDec,process.Priority());
	priorityText.Append(priorityNumber);
	priorityTextLabel->SetTextL(priorityText);
	if (process.Priority()==EPriorityLow) opButs->SetCurrentItem(EProcessP150_PriorityLow);
	else if (process.Priority()==EPriorityBackground) opButs->SetCurrentItem(EProcessP250_PriorityBackground);
	else if (process.Priority()==EPriorityForeground) opButs->SetCurrentItem(EProcessP350_PriorityForeground);
	else if (process.Priority()==EPriorityHigh) opButs->SetCurrentItem(EProcessP450_PriorityHigh);
	else if (process.Priority()==EPriorityWindowServer) opButs->SetCurrentItem(EProcessP650_PriorityWindowServer);
	else if (process.Priority()==EPriorityFileServer) opButs->SetCurrentItem(EProcessP750_PriorityFileServer);
	else if (process.Priority()==EPriorityRealTimeServer) opButs->SetCurrentItem(EProcessP850_PriorityRealTimeServer);
	else if (process.Priority()==EPrioritySupervisor) opButs->SetCurrentItem(EProcessP950_PrioritySupervisor);
	else (opButs->SetCurrentItem(EProcessPriorityNotStandard));
	

	sysCheckBox->SetState(CEikButtonBase::EIndeterminate);
	protCheckBox->SetState(CEikButtonBase::EIndeterminate);
 

	// Change the dimming on the component controls depending on the Process
	sysCheckBox->SetDimmed(EFalse);
	opButs->SetDimmed(EFalse);
	#ifdef WINS
	opButs->SetDimmed(ETrue);
	#endif

	killBut->SetDimmed(EFalse);
	okBut->SetDimmed(EFalse);
	setBut->SetDimmed(EFalse);
	if (iProcessID == RProcess().Id())
		{
		// can change the protection on this Process
		protCheckBox->SetDimmed(EFalse);
		}
	else
		{
		// cannot change protection
		protCheckBox->SetDimmed(ETrue);
		if (true)	// If we are in secure then we ensure that the
					// user can not change anything.
			{
			// cannot change anything
			sysCheckBox->SetDimmed(ETrue);
			opButs->SetDimmed(ETrue);
			killBut->SetDimmed(ETrue);
			okBut->SetDimmed(ETrue);
			setBut->SetDimmed(ETrue);
			}
		}
	process.Close(); // Closes the handle
	DrawNow();

	}

void CProcessNumberDialog::SetProcess()
{
		// only get here if Process is not protected		


		RProcess process;
		TInt err = process.Open(iProcessID);

		if (err == KErrNone)
			{
			// This is the current Process
			if ( iProcessID == RProcess().Id())
				{
				}
			
				
			
			CEikChoiceList *opButs;
			TProcessPriority processPrioritySelected = EPriorityLow;

			opButs = STATIC_CAST(CEikChoiceList*, Control(EProcessPriorityButtonCtl));

			TInt currentItem = opButs->CurrentItem();
			TBool validPrioritySelected = ETrue;
				
			switch (currentItem) 
				{
				case (EProcessP150_PriorityLow): processPrioritySelected=EPriorityLow; break;
				case (EProcessP250_PriorityBackground): processPrioritySelected=EPriorityBackground; break;
				case (EProcessP350_PriorityForeground): processPrioritySelected=EPriorityForeground; break;
				case (EProcessP450_PriorityHigh): processPrioritySelected=EPriorityHigh; break;
				case (EProcessP650_PriorityWindowServer): 
				case (EProcessP750_PriorityFileServer): 
				case (EProcessP850_PriorityRealTimeServer): 
				case (EProcessP950_PrioritySupervisor): 
				default: iEikonEnv->InfoWinL(_L("Error"), _L("This is an internal priority and cannot be explicitly set"));
						 validPrioritySelected = EFalse;
						 break;
				}
			if (validPrioritySelected == 1) process.SetPriority(processPrioritySelected);
		
		}
		process.Close();
	}

void CProcessNumberDialog::HandleControlStateChangeL(TInt aControlId)
	{
	if (aControlId == EProcessGetButtonCtl)
		{
		CXIKNUMBEREDITOR *thisNumberEditor; 
		thisNumberEditor = STATIC_CAST(CXIKNUMBEREDITOR*, Control(EProcessNumberCtl));
		iProcessID = (TProcessId)thisNumberEditor->NUMBERVALUE();
		// re-evaluate everything
		UpdateDialogData(); 

		// re-draw
		PageChangedL(EProcessPageGeneral); 
		PageChangedL(EProcessPagePriority);
		PageChangedL(EProcessPageOther);
		DrawNow();
		}
	}

TBool CProcessNumberDialog::OkToExitL(TInt aButtonId)
	{
	if (aButtonId == EEikBidCancel) // do nothing
		{
		return ETrue;
		}
	else if (aButtonId == EProcessSetButtonCtl)
		{
		SetProcess();
		UpdateDialogData();
		iEikonEnv->InfoMsg(_L("Attribute(s) set as displayed"));
		return EFalse;
		}
	else if (aButtonId == EProcessKillButtonCtl) 
		{
		RProcess Process;
		TInt err = Process.Open(iProcessID);
		if (err == KErrNone)
			{
			iEikonEnv->InfoMsg(_L("Killing Process"));
			Process.Kill(KErrNone);
			}
		Process.Close();
		iEikonEnv->InfoMsg(_L("Process killed"));
		return EFalse;
		}

	else if (aButtonId == EEikBidOk)
		{
		SetProcess();
		return ETrue;
		}
	else return ETrue;
	}


//
//
// Debug Dialog
//
//

CDebugDialog::CDebugDialog()
	{
	}

void CDebugDialog::PreLayoutDynInitL()
	{
	GetPortNumber();
	GetKernelOptions();
	}

void CDebugDialog::GetPortNumber()
	{
	//map the control
	CXIKNUMBEREDITOR *thisNumberEditor;
	thisNumberEditor = static_cast<CXIKNUMBEREDITOR*>(Control(EDebugPortCtl));
	//set control value
	TInt debugPort, err;
	err = HAL::Get(HAL::EDebugPort, debugPort);
	if (err == KErrNone)
			{
			thisNumberEditor->SETNUMBERVALUEL(debugPort);
			iCurrentDebugPort = debugPort;
			}
		else
			{
			thisNumberEditor->SETNUMBERVALUEL(-1);
			iCurrentDebugPort = -1;
			}

	}

void CDebugDialog::GetKernelOptions()
	{
	// Kernel debug mask

	TUint32 debugMask = UserSvr::DebugMask();
	//map the controls (check boxes) and set the control values
	
	CEikCheckBox *aCheckBox;
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKHARDWARE));
	aCheckBox->SetState(debugMask & KHARDWARE ? CEikButtonBase::ESet : CEikButtonBase::EClear);

	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKBOOT));
	aCheckBox->SetState(debugMask & KBOOT ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKSERVER));
	aCheckBox->SetState(debugMask & KSERVER ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKMMU));
	aCheckBox->SetState(debugMask & KMMU ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKSEMAPHORE));
	aCheckBox->SetState(debugMask & KSEMAPHORE ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKSCHED));
	aCheckBox->SetState(debugMask & KSCHED ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKPROC));
	aCheckBox->SetState(debugMask & KPROC ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKEXEC));
	aCheckBox->SetState(debugMask & KEXEC ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKEXECFAST));
	aCheckBox->SetState(debugMask & KEXECFAST ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKTHREAD));
	aCheckBox->SetState(debugMask & KTHREAD ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKDLL));
	aCheckBox->SetState(debugMask & KDLL ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKLOGON));
	aCheckBox->SetState(debugMask & KLOGON ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKPBUS1));
	aCheckBox->SetState(debugMask & KPBUS1 ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKPBUS2));
	aCheckBox->SetState(debugMask & KPBUS2 ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKPBUSDRV));
	aCheckBox->SetState(debugMask & KPBUSDRV ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKPOWER));
	aCheckBox->SetState(debugMask & KPOWER ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKTIMING));
	aCheckBox->SetState(debugMask & KTIMING ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKEVENT));
	aCheckBox->SetState(debugMask & KEVENT ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKMEMTRACE));
	aCheckBox->SetState(debugMask & KMEMTRACE ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKIPC));
	aCheckBox->SetState(debugMask & KIPC ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKDEBUGGER));
	aCheckBox->SetState(debugMask & KDEBUGGER ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKVIDEO));
	aCheckBox->SetState(debugMask & KVIDEO ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKUSB));
	aCheckBox->SetState(debugMask & KUSB ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	
	aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKPANIC));
	aCheckBox->SetState(debugMask & KPANIC ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	}

void CDebugDialog::SetDebugOptions()
	{	
		TBuf<800> errorMssg; //to show where updates unsuccessful
		//set new debug port
		CXIKNUMBEREDITOR *thisNumberEditor;
		thisNumberEditor = static_cast<CXIKNUMBEREDITOR*>(Control(EDebugPortCtl));
		TInt newDebugPort = thisNumberEditor->NUMBERVALUE();

		TInt err;
		if (newDebugPort!=-1)
			{
				err = HAL::Set(HAL::EDebugPort, newDebugPort);
				if (err==KErrNone) iCurrentDebugPort = newDebugPort;
				else
					{
					TBuf<3> errorCode;
					errorCode.Format(KFormatDec,err);
					errorMssg.Append(_L("Cannot set port: System error "));
					errorMssg.Append(errorCode);
					errorMssg.Append('\n');
					thisNumberEditor->SETNUMBERVALUEL(iCurrentDebugPort);
					}
			}
		//set new Kernel debug values
		CEikCheckBox *aCheckBox;
		TUint32 debugMask = 0;	
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKHARDWARE));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KHARDWARE;

		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKBOOT));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KBOOT;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKSERVER));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KSERVER;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKMMU));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KMMU;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKSEMAPHORE));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KSEMAPHORE;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKSCHED));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KSCHED;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKPROC));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KPROC;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKEXEC));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KEXEC;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKEXECFAST));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KEXECFAST;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKTHREAD));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KTHREAD;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKDLL));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KDLL;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKLOGON));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KLOGON;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKPBUS1));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KPBUS1;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKPBUS2));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KPBUS2;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKPBUSDRV));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KPBUSDRV;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKPOWER));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KPOWER;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKTIMING));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KTIMING;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKEVENT));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KEVENT;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKMEMTRACE));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KMEMTRACE;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKIPC));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KIPC;
		
		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKDEBUGGER));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KDEBUGGER;

		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKVIDEO));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KVIDEO;

		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKUSB));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KUSB;

		aCheckBox = static_cast<CEikCheckBox*>(Control(EDebugKernelKPANIC));
		if (aCheckBox->State()==CEikButtonBase::ESet) debugMask|=KPANIC;

		User::SetDebugMask(debugMask);
		TUint32 updatedDebugMask = UserSvr::DebugMask();
		if (debugMask != updatedDebugMask)
			{
			errorMssg.Append(_L("Unable to set kernel options selected. Note that KPanic must be selected if any other options are selected. View the \nupdated dialog to see which Kernel options are set"));
			GetKernelOptions();
			}


		CEikCheckBox *thisCheckBox, *thisCheckBox2, *thisCheckBox3;
		thisCheckBox = static_cast<CEikCheckBox*>(Control(EDebugFileServerCtl));
		thisCheckBox2 = static_cast<CEikCheckBox*>(Control(EDebugFATCtl));
		thisCheckBox3 = static_cast<CEikCheckBox*>(Control(EDebugLoaderCtl));

		TInt fsDebugMask = 0;
		if (thisCheckBox->State()==CEikButtonBase::ESet) fsDebugMask|=KFSERV;
		if (thisCheckBox2->State()==CEikButtonBase::ESet) fsDebugMask|=KFSYS;
		if (thisCheckBox3->State()==CEikButtonBase::ESet) fsDebugMask|=KFLDR;

		RFs aRFs;
		err = aRFs.Connect();
		if (err == KErrNone)
			{
			err = aRFs.SetDebugRegister(fsDebugMask);
			if (err != KErrNone)
				errorMssg.Append(_L("Unable to set FS options: RFs::SetDebugRegister() fails with  System error number "));
			TBuf<3> errorCode;
			errorCode.Format(KFormatDec, err);
			errorMssg.Append(errorCode);
			}
		else
			{
			errorMssg.Append(_L("\nRFs::Connect() fails with System error number "));
			TBuf<3> errorCode;
			errorCode.Format(KFormatDec, err);
			errorMssg.Append(errorCode);
			thisCheckBox->SetState(CEikButtonBase::EClear);
			thisCheckBox2->SetState(CEikButtonBase::EClear);
			thisCheckBox3->SetState(CEikButtonBase::EClear);
			}
		aRFs.Close();

		if (errorMssg.Length() != 0)
			{
			_LIT(KDialogTitle, "Error");
			iEikonEnv->InfoWinL(KDialogTitle, errorMssg);
			}
	}

TBool CDebugDialog::OkToExitL(TInt aButtonId)
	{
	if (aButtonId == EDebugSetButtonCtl)
		{
		SetDebugOptions();
		DrawNow();
		return EFalse;
		}
	if (aButtonId == EEikBidOk)
		{
		SetDebugOptions();
		}
	else if (aButtonId == EEikBidCancel)
		{
		// Do Nothing
		}
	return ETrue;
	}

//
//
// Ram Dialog
//
//

CRamDialog::CRamDialog()
	{
	noChunks = 0;
	}

void CRamDialog::PreLayoutDynInitL()
	{
	UpdateDialogData();
	}

void CRamDialog::UpdateDialogData()
	{
	CEikLabel *theTotalLabel, *theFreeLabel;
	theTotalLabel = STATIC_CAST(CEikLabel*, Control(ERamTotal));
	theFreeLabel = STATIC_CAST(CEikLabel*, Control(ERamFree));

	TMemoryInfoV1Buf memorybuf;
	UserHal::MemoryInfo(memorybuf);
	TInt total=memorybuf().iTotalRamInBytes;
	TInt free=memorybuf().iFreeRamInBytes;

	TBuf<40> totalTxt;
	totalTxt.Format(_L("Total RAM (bytes) %d"),total);
	TBuf<40> freeTxt;
	freeTxt.Format(_L("Free RAM (bytes) %d"),free);

	theTotalLabel->SetTextL(totalTxt);
	theFreeLabel->SetTextL(freeTxt);

	CXIKOPTIONBUTTONLIST *fillSpaceOpButs;
	fillSpaceOpButs = STATIC_CAST(CXIKOPTIONBUTTONLIST*, Control(ERamToLeaveButtonCtl));

	// All buttons invisible
	fillSpaceOpButs->ComponentControl(0)->MakeVisible(ETrue);
	fillSpaceOpButs->ComponentControl(1)->MakeVisible(ETrue);
	fillSpaceOpButs->ComponentControl(2)->MakeVisible(ETrue);
	fillSpaceOpButs->ComponentControl(3)->MakeVisible(ETrue);
	fillSpaceOpButs->ComponentControl(4)->MakeVisible(ETrue);

	if (free<K4k) 
		{
		// All buttons visible
		fillSpaceOpButs->ComponentControl(0)->MakeVisible(EFalse);
		fillSpaceOpButs->ComponentControl(1)->MakeVisible(EFalse);
		fillSpaceOpButs->ComponentControl(2)->MakeVisible(EFalse);
		fillSpaceOpButs->ComponentControl(3)->MakeVisible(EFalse);
		fillSpaceOpButs->ComponentControl(4)->MakeVisible(EFalse);
		}
	else if (free>=K4k && free<K8k) 
		{
		fillSpaceOpButs->ComponentControl(1)->MakeVisible(EFalse);
		fillSpaceOpButs->ComponentControl(2)->MakeVisible(EFalse);
		fillSpaceOpButs->ComponentControl(3)->MakeVisible(EFalse);
		fillSpaceOpButs->ComponentControl(4)->MakeVisible(EFalse);
		fillSpaceOpButs->SetButtonById(ERamLeave4K);
		}
	else if (free>=K8k && free<K16k) 
		{
		fillSpaceOpButs->ComponentControl(2)->MakeVisible(EFalse);
		fillSpaceOpButs->ComponentControl(3)->MakeVisible(EFalse);
		fillSpaceOpButs->ComponentControl(4)->MakeVisible(EFalse);
		fillSpaceOpButs->SetButtonById(ERamLeave8K);
		}
	else if (free>=K16k && free<K64k)
		{
		fillSpaceOpButs->ComponentControl(3)->MakeVisible(EFalse);
		fillSpaceOpButs->ComponentControl(4)->MakeVisible(EFalse);
		fillSpaceOpButs->SetButtonById(ERamLeave16K);
		}
	else if (free>= K64k && free<K256k)
		{
		fillSpaceOpButs->ComponentControl(4)->MakeVisible(EFalse);
		fillSpaceOpButs->SetButtonById(ERamLeave64K);
		}
	else 
		{
		fillSpaceOpButs->SetButtonById(ERamLeave256K);
		}
	}

void CRamDialog::UndoFillRam()
	{	
	if (noChunks == 0) iEikonEnv->InfoMsg(_L("Undo fill invalid"));
	else
	{
		while (noChunks>0)
			{
			chunks[--noChunks].Close();
			}
	}
}
void CRamDialog::FillRam()
	{
		iEikonEnv->BusyMsgL(_L("Filling RAM"));

		CXIKOPTIONBUTTONLIST *opButs;
		opButs = STATIC_CAST(CXIKOPTIONBUTTONLIST*, Control(ERamToLeaveButtonCtl));

		TUint ramToLeave=K4k;
		if (opButs->LabeledButtonId()==ERamLeave4K) ramToLeave=K4k;
		else if (opButs->LabeledButtonId()==ERamLeave8K) ramToLeave=K8k;
		else if (opButs->LabeledButtonId()==ERamLeave16K) ramToLeave=K16k;
		else if (opButs->LabeledButtonId()==ERamLeave64K) ramToLeave=K64k;
		else if (opButs->LabeledButtonId()==ERamLeave256K) ramToLeave=K256k; 


		TUint chunkSize;
		TInt err = KErrNone;
		TBuf<20> aName;
		TTime time;

		TMemoryInfoV1Buf memorybuf;
		UserHal::MemoryInfo(memorybuf);

		// Fill the RAM up with various chunks
		for (chunkSize = K8M; chunkSize>=ramToLeave; chunkSize>>=1) 
			{
			TUint KMaxChunks = 10000;
			for (err=KErrNone; err==KErrNone && noChunks<KMaxChunks;)	
				{
				// create a unique chunk name
				time.UniversalTime();
				aName.Format(_L("%S%02d-%08x"),&KFillName, noChunks,*(reinterpret_cast<const TInt32*>(&(time.Int64()))));

				err = chunks[noChunks].CreateGlobal(aName, chunkSize, chunkSize);
				// ensure RAM is committed by filling it
				if (err==KErrNone)
					{
					Mem::Fill(chunks[noChunks++].Base(),chunkSize,'A');
					}
				User::CompressAllHeaps();
				} ;
			}
		iEikonEnv->BusyMsgCancel();
		if (err == KErrNone) iEikonEnv->InfoMsg(_L("Fill Complete"));
	}

TBool CRamDialog::OkToExitL(TInt aButtonId)
	{
	if (aButtonId == EEikBidCancel)
		{
		return ETrue;
		}
	else if (aButtonId == ERamSetButtonCtl)
		{
		FillRam();
		UpdateDialogData();
		DrawNow();
		return EFalse;
		}
	else if (aButtonId == ERamUndoFillButtonCtl)
		{
		UndoFillRam();
		UpdateDialogData();
		DrawNow();
		return EFalse;
		}
	return ETrue;
	}

//
//
// Drive Dialog
//
//

CDriveDialog::CDriveDialog() : iDriveName(NULL), iDriveLetter(NULL)
	{
	}

CDriveDialog::~CDriveDialog()
	{
		delete iDriveLetter;
		delete iDriveName;
	}

void CDriveDialog::PreLayoutDynInitL()
	{
	iDriveName = new (ELeave) CDesCArrayFlat(KMaxLocalDrives);
	iDriveLetter = new  CArrayFixFlat<TInt>(KMaxLocalDrives);

	TBuf<40> string; // displayed in choice box
	RFs rfs;	
	User::LeaveIfError(rfs.Connect());
	TDriveInfo driveInfo;
	TVolumeInfo volumeInfo;
	TDriveUnit driveUnit(EDriveA);
	TInt err1,err2,drive=0,number=0;

	do {
	
		driveUnit = drive;
		err1 = rfs.Volume(volumeInfo,driveUnit);
		err2 = rfs.Drive(driveInfo, driveUnit);
		if (err1==KErrNone && err2==KErrNone)
			{

			// Only interested if not ROM and local

			if (volumeInfo.iDrive.iType != EMediaRom &&
				(volumeInfo.iDrive.iDriveAtt | KDriveAttLocal))
				{
				number++;

				string.Delete(0,string.MaxLength());

				string=driveUnit.Name();

				string.Append(_L(" "));
				string.Append(volumeInfo.iName);

				iDriveName->AppendL(string);
				iDriveLetter->AppendL(drive);
				}
			}

		drive++;

		} while (drive <= EDriveZ && number < KMaxLocalDrives);

	rfs.Close();

	CEikChoiceList *driveChoice;
	driveChoice = STATIC_CAST(CEikChoiceList*, Control(EDriveChoiceList));
	driveChoice->SetArrayL(iDriveName);
	driveChoice->SetArrayExternalOwnership(ETrue);

	
	CXIKOPTIONBUTTONLIST *fillSpaceOpButs;
	fillSpaceOpButs = STATIC_CAST(CXIKOPTIONBUTTONLIST*, Control(EDriveToLeaveButtonCtl));
	fillSpaceOpButs->SetButtonById(EDriveLeave256K);
	}


void UpdateDialogLeaveSpace()
	{	
	}

TBool CDriveDialog::OkToExitL(TInt aButtonId)
	{
	if (aButtonId == EEikBidCancel) 
		{
		delete iDriveName;
		delete iDriveLetter;
		return ETrue;
		}
	else
		{
		CEikChoiceList *driveChoice;
		driveChoice = STATIC_CAST(CEikChoiceList*, Control(EDriveChoiceList));
		TInt choice = driveChoice->CurrentItem();
		TDriveUnit driveUnit;
		driveUnit = (*iDriveLetter)[choice];

		RFs aRfs;
		User::LeaveIfError(aRfs.Connect());

		CXIKOPTIONBUTTONLIST *fillSpaceOpButs;
		fillSpaceOpButs = STATIC_CAST(CXIKOPTIONBUTTONLIST*, Control(EDriveToLeaveButtonCtl));

		if (aButtonId==EDriveFillButtonCtl)
			{ 
			iEikonEnv->BusyMsgL(_L("Filling Drive"));
			// need a unique file name
			TInt i=0,err = KErrNone;
			TFileName fileName;
			TTime time;

			TInt fileSize;

			CXIKOPTIONBUTTONLIST *fillSpaceOpButs;
			fillSpaceOpButs = STATIC_CAST(CXIKOPTIONBUTTONLIST*, Control(EDriveToLeaveButtonCtl));

			TInt spaceToLeave=K4k;
			if (fillSpaceOpButs->LabeledButtonId()==EDriveLeave4K) spaceToLeave=K4k;
			else if (fillSpaceOpButs->LabeledButtonId()==EDriveLeave8K) spaceToLeave=K8k;
			else if (fillSpaceOpButs->LabeledButtonId()==EDriveLeave16K) spaceToLeave=K16k;
			else if (fillSpaceOpButs->LabeledButtonId()==EDriveLeave64K) spaceToLeave=K64k;
			else if (fillSpaceOpButs->LabeledButtonId()==EDriveLeave256K) spaceToLeave=K256k;

			for (fileSize = K1M; fileSize>=spaceToLeave; fileSize>>=1)
				{

				do { 
					time.UniversalTime();
					TDriveName driveName = driveUnit.Name();
					fileName.Format(_L("%S\\%S%06d-%08x"),&driveName,&KFillName,++i,*(reinterpret_cast<const TInt32*>(&(time.Int64()))));

					// create a file of this file size
					RFile file;
					err=file.Create(aRfs, fileName, EFileRead|EFileWrite);
					if (err!=KErrNone)
						{	
						TBuf<70> errorMessage(_L("Cannot complete fill. RFile::Create() failed with error no. "));
						errorMessage.AppendNum(err);
						iEikonEnv->InfoWinL(_L("Error"), errorMessage);
						}
							
						// fill file up to fileSize
					if (err==KErrNone)
						{
						const TInt MaxBufSize=512;
						TBuf8<MaxBufSize> buf;
						buf.SetLength(MaxBufSize);
						Mem::Fill((void*)buf.Ptr(),MaxBufSize,0xef);
						TInt rem=fileSize;
						while(rem)
							{
							TInt s=Min(MaxBufSize,rem);
							err=file.Write(buf,s);
							if (err!=KErrNone) break;
							rem-=s;
							}	
						}

					file.Close();
					if(err!=KErrNone) aRfs.Delete(fileName);
					} while (err==KErrNone && i<100);
				} 
				iEikonEnv->BusyMsgCancel();
				iEikonEnv->InfoMsg(_L("Drive filled"));
			}
		else if (aButtonId==EDriveUndoFillButtonCtl)
			{
			iEikonEnv->BusyMsgL(_L("Undo Drive Fill"));
			TFindFile fileFinder(aRfs);
			CDir* fileList;

			TBuf<10> name;
			name=KFillName;
			name.Append(KAsterisk);

			TBuf<3> driveName;
			driveName=driveUnit.Name();
			driveName.Append(_L("\\"));

			TInt findErr = fileFinder.FindWildByPath(name,&driveName,fileList);
			TInt deleteErr = KErrNone;

			if (findErr == KErrNone)
				{
				for (TInt i=0;i<fileList->Count();++i)
					{
					TBuf<100> filePath(driveName);
					filePath.Append((*fileList)[i].iName);

					deleteErr = aRfs.Delete(filePath);
					}
				}
			iEikonEnv->BusyMsgCancel();

			if (deleteErr !=KErrNone)
				{
				TBufC<15> function(_L("RFs::Delete()"));
				DisplayError(function, deleteErr);
				}
			if (findErr == KErrNone && deleteErr == KErrNone)
			{
				fillSpaceOpButs->ComponentControl(0)->MakeVisible(ETrue);
				fillSpaceOpButs->ComponentControl(1)->MakeVisible(ETrue);
				fillSpaceOpButs->ComponentControl(2)->MakeVisible(ETrue);
				fillSpaceOpButs->ComponentControl(3)->MakeVisible(ETrue);
				fillSpaceOpButs->ComponentControl(4)->MakeVisible(ETrue);
				iEikonEnv->InfoMsg(_L("Undo Fill Complete"));
			}

			else iEikonEnv->InfoMsg(_L("Undo Fill Invalid"));
			delete fileList;
			}
		else if (aButtonId==EDriveFormatButtonCtl)
			{
			TInt count;
			RFormat format;
			TInt err=format.Open(aRfs,driveUnit.Name(),EHighDensity,count);
			if (err==KErrNone)
				{
				iEikonEnv->BusyMsgL(_L("Formatting Drive"));
				while (count && err==KErrNone)
					{
					err = format.Next(count);
					if (err != KErrNone)
						{
						TBufC<15> function(_L("RFormat::Next()"));
						DisplayError(function, err);
						}
					}
				iEikonEnv->BusyMsgCancel();
				}
			else
				{						
				TBuf<70> errorMessage(_L("RFormat::Open() failed with system error no."));
				errorMessage.AppendNum(err);
				iEikonEnv->InfoWinL(_L("Error"), errorMessage);
				}
			format.Close();
			}

		aRfs.Close();
		}
	return EFalse;
	}

CDialogBase::CDialogBase()
	{
	}

void CDialogBase::DisplayError(TDesC& aMessage, TInt aErrorNumber)
	{
	//could check doesn't go out of bounds
	TBuf<250> errorMssge(aMessage);
	errorMssge.Append(_L(" fails with system error no. "));
	TBuf<3> errorNum;
	_LIT(KFormatDec, "%d");
	errorNum.Format(KFormatDec, aErrorNumber);
	errorMssge.Append(errorNum);
	iEikonEnv->InfoWinL(_L("Error"), errorMssge);	
	}

void CDialogBase::DisplayError(TDesC& aMessage)
	{
	//could check don't go out of bounds
	TBuf<250> errorMssge(aMessage);
	iEikonEnv->InfoWinL(_L("Error"), errorMssge);	
	}
	
