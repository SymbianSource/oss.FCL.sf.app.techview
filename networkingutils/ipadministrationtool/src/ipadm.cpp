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
// ipadm.cpp - IP administration tool main module
//

#include <techview/eikfnlab.h>
#ifndef CALYPSO
#include <techview/eikfontd.h>
#endif

#if EPOC_SDK < 0x06000000
#include <e32keys.h>
#include <coemain.h>

#include <eikenv.h>
#include <eikdef.h>
#include <eikcmds.hrh>
#include <techview/eikon.rsg>
#include <techview/eiklabel.h>

#include <techview/eiktbar.h>
#include <techview/eikchlst.h>
#include <techview/eikpgsel.h>

#include <techview/eiklabel.h>
#endif

#ifdef MAKE_EXE_APPLICATION
#include <eikstart.h>
#endif

#include "ipadm.h"
#include "engine.h"

// 
// CConsoleControl
//
CConsoleControl::~CConsoleControl()
	{
	delete iConsole;
	}

void CConsoleControl::ConstructL(const TPoint& aTopLeft, const TSize& aSize, TInt aFlags)
	{
	TRect rect(aTopLeft,aTopLeft + aSize.AsPoint());
#if EPOC_SDK >= 0x06000000
	SetRect(rect);
#else
	SetRectL(rect);
#endif
    iConsole= new (ELeave) CEikConsoleScreen;
	iConsole->ConstructL(_L("TEST"),aTopLeft,aSize,aFlags,EEikConsWinInPixels);
	iConsole->SetHistorySizeL(200,0);
	//iConsole->SetAllPrintable(ETrue);
	iHistory=200;
	}

void CConsoleControl::ActivateL()
	{
	CCoeControl::ActivateL();
	iConsole->SetKeepCursorInSight(TRUE);
	iConsole->DrawCursor();
	iConsole->SetAtt(ATT_NORMAL);
	}

void CConsoleControl::HandleCommandL(TInt aCommand)
	{
    switch (aCommand)
		{
	case EConsolFontDialog:
#if EPOC_SDK >= 0x06000000
		{
#ifndef CALYPSO
		TCharFormat charFormat;
		charFormat.iFontSpec = iConsole->Font();
		TCharFormatMask dummy;
		if (CEikFontDialog::RunDlgLD(charFormat, dummy))
			{
			//charFormat.iFontSpec.iTypeface.SetIsProportional(EFalse);
			iConsole->SetFontL(charFormat.iFontSpec);
			}
#endif
		}
#else
		{
		TCharFormat charFormat;
		charFormat.iFontSpec = iConsole->Font();
		TCharFormatMask dummy;
		CEikFontDialog* dialog=new(ELeave) CEikFontDialog(charFormat,dummy);
		if (dialog->ExecuteLD(R_EIK_DIALOG_FONT))
			{
			//charFormat.iFontSpec.iTypeface.SetIsProportional(EFalse);
			iConsole->SetFontL(charFormat.iFontSpec);
			}
		}
#endif
        break;
#if 0
	case EConsolHistory:
		{
		CHistoryDialog* dialog2 = new(ELeave) CHistoryDialog(&iHistory);
		if (dialog2->ExecuteLD(R_KMD_HISTORY_DIALOG))
			iConsole->SetHistorySizeL(iHistory,0);
		}
        break;
#endif
    case EConsolScrollNone:
		iConsole->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff,CEikScrollBarFrame::EOff);
        break;
    case EConsolScrollHor:
		iConsole->SetScrollBarVisibilityL(CEikScrollBarFrame::EAuto,CEikScrollBarFrame::EOff);
        break;
    case EConsolScrollVert:
		iConsole->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff,CEikScrollBarFrame::EAuto);
        break;
   	case EConsolScrollBoth:
		iConsole->SetScrollBarVisibilityL(CEikScrollBarFrame::EAuto,CEikScrollBarFrame::EAuto);
        break;
	default:
		break;
		}
    }

void CConsoleControl::FocusChanged(TDrawNow aDrawNow)
	{
	iConsole->ConsoleControl()->SetFocus(IsFocused(), aDrawNow); 
	}

void CConsoleControl::ToggleFontStyleAndRedrawL(TMessageControlFontStyle aStyleElement)
    {
    switch (aStyleElement)
        {
    case EStyleElementColor:
		if ( iConsole->Att() & ATT_COLORMASK )	// color?
			iConsole->SetAtt(ATT_NORMAL);	// then set normal
		else								// else
			iConsole->SetAtt(4,11);			// set 4 (darkgray) on 11 (lightgray)
        break;
    case EStyleElementBold:
		// clear color flag (just to be sure) and switch bold flag
		iConsole->SetAtt( (iConsole->Att()&(~ATT_COLORMASK)) ^ ATT_BOLD );
        break;
    case EStyleElementItalic:
		// clear color flag (just to be sure) and switch italic flag
		iConsole->SetAtt( (iConsole->Att()&(~ATT_COLORMASK)) ^ ATT_ITALIC );
        break;
    case EStyleElementInverse:
		// clear color flag (just to be sure) and switch inverse flag
		iConsole->SetAtt( (iConsole->Att()&(~ATT_COLORMASK)) ^ ATT_INVERSE );
        break;
    case EStyleElementUnderline:
		// clear color flag (just to be sure) and switch underline flag
		iConsole->SetAtt( (iConsole->Att()&(~ATT_COLORMASK)) ^ ATT_UNDERLINE );
        break;
        }
    }

void CConsoleControl::SetScrollBarVisibilityL(CEikScrollBarFrame::TScrollBarVisibility aHBarVisibility, CEikScrollBarFrame::TScrollBarVisibility aVBarVisibility)
	{
	iConsole->SetScrollBarVisibilityL(aHBarVisibility,aVBarVisibility);
	iConsole->ConsoleControl()->UpdateArea();
	iConsole->UpdateScrollBars();
	iConsole->ConsoleControl()->UpdateArea();
	//TBool b=iConsole->RecalculateSize();
	}


void CConsoleControl::DrawCursor()
	{
	iConsole->DrawCursor();
	}


void CConsoleControl::Write(const TDesC &aMsg)
	{
	iConsole->Write(aMsg);
	iConsole->FlushChars();
	}

CEikConsoleControl *CConsoleControl::ConsoleControl() const
	{
	return iConsole->ConsoleControl();
	}

TBool CConsoleControl::UpdateScrollBars()
	{
	return iConsole->UpdateScrollBars();
	}

void CConsoleControl::ClearScreen()
	{
	iConsole->ClearScreen();
	}


void CConsoleControl::Redraw(const TRect &aRect)
	{
	iConsole->Redraw(aRect);
	}

void CConsoleControl::Lf()
	{
	iConsole->Lf();
	}



// ****************
// APPLICATION VIEW
// ****************
//
void CIpAdmView::ConstructL(const TRect& aRect)
    {
	CreateWindowL();
#if EPOC_SDK >= 0x06000000
    SetRect(aRect);
#else
    SetRectL(aRect);
#endif
    iContext = this;
	iBrushStyle = CGraphicsContext::ESolidBrush;
    iBrushColor = KRgbWhite;
	CreateBigConsoleL(CEikConsoleScreen::ENoInitialCursor);

	StartL();
	
    ActivateL();
    }

void CIpAdmView::CreateBigConsoleL(TInt aFlags)
	{
	iConsole =new(ELeave) CConsoleControl;
//	TRect rect=Rect();
//	rect.Shrink(3,3);
	iConsole->ConstructL(Position(),Rect().Size(),aFlags);
	iConsole->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff,CEikScrollBarFrame::EAuto);
	
	}

CIpAdmView::~CIpAdmView()
	{
	delete iConsole;
	delete iModel;
	}
    
TInt CIpAdmView::CountComponentControls() const
	{
	return 1;
	}

CCoeControl* CIpAdmView::ComponentControl(TInt aIndex) const
	{
	switch (aIndex)
		{
	case 0:
		return iConsole;
	default:
		return 0;
		}
	}

void CIpAdmView::Draw(const TRect& /*aRect*/) const
	{
#if 0
	CWindowGc& gc = SystemGc();
	
	gc.SetPenStyle(CGraphicsContext::ESolidPen);
	TSize penSizeBold(3,3);
	gc.SetPenSize(penSizeBold);
	gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
	
	TRect rect=iConsole->Rect();	//Lines around the Console
	rect.Grow(3,3);
	gc.DrawRect(rect);
#endif
	}

void CIpAdmView::StartL()
	{
	TInt err;

	if (!iModel)
		{
		iModel = new CIpAdmEngine(this);
		CEikonEnv::Static()->BusyMsgL(R_BUSY);
		TRAP(err, iModel->ConstructL());
		CEikonEnv::Static()->BusyMsgCancel();
		if (err!=KErrNone)
			{
			TBuf<50> msg;
			msg.Format(_L("Error initializing: %d"), err);
			ShowError(msg);
			StopL();
			}
		}
	else
		CEikonEnv::Static()->InfoMsg(_L("Already Running!"));
	}


void CIpAdmView::Write(const TDesC &aMsg)
	{
	iConsole->Write(aMsg);
	iConsole->Write(_L("\n"));
	}

void CIpAdmView::ShowError(TInt aId)
	{
	iEikonEnv->InfoMsg(aId);
	}

void CIpAdmView::ShowError(const TDes &msg)
	{
	iEikonEnv->InfoMsg(msg);
	}

void CIpAdmView::ShowError(const TDes &msg, TInt aErr)
	{
	TBuf<100> txt;
	TBuf<100> txt2;

	txt.Format(msg);
	iEikonEnv->GetErrorText(txt2,aErr);
	txt.AppendFormat(txt2);
	iEikonEnv->InfoMsg(txt);
	}

void CIpAdmView::StopL()
	{
	if (iModel)
		{
		CEikonEnv::Static()->BusyMsgL(R_BUSY);
		delete iModel;	
		CEikonEnv::Static()->BusyMsgCancel();
		Write(_L("*Stopped*"));
		}
	else
		CEikonEnv::Static()->InfoMsg(_L("Not running!"));
	iModel = NULL;
	}


void CIpAdmView::ClearScreenL()
	{
	delete iConsole;
	iConsole = NULL;
	CreateBigConsoleL(CEikConsoleScreen::ENoInitialCursor);	
	}

void CIpAdmView::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
	case EIpAdmClearScreen:
		ClearScreenL();
		break;

	default:
		ASSERT(iConsole != NULL);
		iConsole->HandleCommandL(aCommand);
		if (iModel)
			iModel->HandleCommandL(aCommand);
		break;
		}
	}

// **************
// APPLICATION UI
// **************
//
void CIpAdmAppUi::ConstructL()
    {
    BaseConstructL();

    iAppView= new (ELeave) CIpAdmView;
    iAppView->ConstructL(ClientRect());

#if EPOC_SDK < 0x06000000
	CEikFileNameLabel* filenameLabel=STATIC_CAST(CEikFileNameLabel*, iToolBar->ControlById(EIpAdmFilename));
	filenameLabel->UpdateL();
#endif
    }

void CIpAdmAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
	case EEikCmdExit:
		Exit();
		return;
	default:
		iAppView->HandleCommandL(aCommand);
		}
	}

CIpAdmAppUi::~CIpAdmAppUi()
	{
#ifndef CALYPSO
	RemoveFromStack(iAppView);
#endif
    delete iAppView;
	}

// ********
// DOCUMENT
// ********
CIpAdmDocument::CIpAdmDocument(CEikApplication& aApp)
		: CEikDocument(aApp)
	{
	}

CEikAppUi* CIpAdmDocument::CreateAppUiL()
	{
    return new (ELeave) CIpAdmAppUi;
	}

// ***********
// APPLICATION
// ***********
//
TUid CIpAdmApplication::AppDllUid() const
	{
	return KUidIpAdmApp;
	}

CApaDocument* CIpAdmApplication::CreateDocumentL()
	{
	return new(ELeave) CIpAdmDocument(*this);
	}

// ****
// MAIN
// ****
//
EXPORT_C CApaApplication* NewApplication()
	{
	return new CIpAdmApplication;
	}


#ifdef MAKE_EXE_APPLICATION

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication(NewApplication);
	}

#else

#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return KErrNone;
	}
#endif

#endif // MAKE_EXE_APPLICATION
