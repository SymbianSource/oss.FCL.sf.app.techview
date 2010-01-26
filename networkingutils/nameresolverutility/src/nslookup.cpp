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
// nslookup.cpp - nslookup client main module
//

#include <coemain.h>
#include <eikenv.h>
#include <eikdef.h>
#include <eikappui.h>
#include <eikapp.h>
#include <techview/eikdialg.h>
#include <eikdoc.h>
#include <techview/eikchkbx.h>
#include <techview/eiktbar.h>
#include <techview/eikedwin.h>

#if EPOC_SDK >= 0x06000000
#   include <techview/eikon.hrh>
#else
#   include <eikdialg.hrh>
#   include <eikedwin.hrh>
#   include <eikcmds.hrh>
#endif

#ifdef MAKE_EXE_APPLICATION
#include <eikstart.h>
#endif

#include <nslookup.rsg>
#include "engine.h"
#include "nslookup.hrh"
#include "nslookup.h"

#ifdef CALYPSO
#include <AknTextSettingPage.h>
#endif

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
    iConsole=new(ELeave) CEikConsoleScreen;
    iConsole->ConstructL(_L("TEST"),aTopLeft,aSize,aFlags,EEikConsWinInPixels);
    iConsole->SetHistorySizeL(200,0);
    iHistory=200;
    }

void CConsoleControl::ActivateL()
    {
    CCoeControl::ActivateL();
    iConsole->SetKeepCursorInSight(TRUE);
    iConsole->DrawCursor();
    iConsole->SetAtt(ATT_NORMAL);
    }


void CConsoleControl::HandleCommandL(TInt /* aCommand */)
	{
	}

void CConsoleControl::FocusChanged(TDrawNow aDrawNow)
	{
    iConsole->ConsoleControl()->SetFocus(IsFocused(), aDrawNow); 
	}


void CConsoleControl::SetScrollBarVisibilityL(CEikScrollBarFrame::TScrollBarVisibility aHBarVisibility, CEikScrollBarFrame::TScrollBarVisibility aVBarVisibility)
	{
    iConsole->SetScrollBarVisibilityL(aHBarVisibility,aVBarVisibility);
    iConsole->ConsoleControl()->UpdateArea();
    iConsole->UpdateScrollBars();
    iConsole->ConsoleControl()->UpdateArea();
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


TSize CConsoleControl::ScreenSize() const
	{
    return iConsole->ScreenSize();
	}


CNslookupView::CNslookupView(CNslookup *aModel)
	{
    iModel = aModel;
	}

void CNslookupView::ConstructL(const TRect& aRect)
{
    CreateWindowL();
    Window().SetShadowDisabled(ETrue);
    iContext=this;
    iBrushStyle=CGraphicsContext::ESolidBrush;
    iBrushColor=KRgbWhite;
#if EPOC_SDK >= 0x06000000
    SetRect(aRect);
#else
    SetRectL(aRect);
#endif
    
    // Create its only component, a CNslookupContainer
    
    iContainer = new(ELeave) CNslookupContainer(iModel);
    iContainer->SetContainerWindowL(*this);
    TRect containerRect = Rect();
    iContainer->ConstructL(containerRect);

    ActivateL();
    DrawNow();
}

CNslookupView::~CNslookupView()
    {

    delete iContainer;
    }
    
TInt CNslookupView::CountComponentControls() const
    {   
    return 1; //Just the container
    }

CCoeControl* CNslookupView::ComponentControl(TInt /*aIndex*/) const
    {
    return iContainer;
    }


void CNslookupView::ResetScreen()
{
    iContainer->ResetScreen();
}


TKeyResponse CNslookupView::OfferKeyEventL(const TKeyEvent& /*aKeyEvent*/,TEventCode /*aType*/)
{
    return EKeyWasNotConsumed;
}

void CNslookupView::Draw(const TRect& /*aRect*/) const
    {

    CWindowGc& gc = SystemGc();
    gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.DrawRect(Rect());

    }


//
// CNslookupcontainer
//
const TInt KLabelHeight=25;


 // Construction
CNslookupContainer::CNslookupContainer(CNslookup* aModel)
	{
    iModel = aModel;
	}


// Destruction
CNslookupContainer::~CNslookupContainer()
{
    delete iLabel;
    delete iLabel2;
    delete iConsole;
}

void CNslookupContainer::ConstructL(const TRect& aRect)
{
    iModel->SetConsole(this);

#if EPOC_SDK >= 0x06000000
    SetRect(aRect);
#else
    SetRectL(aRect);
#endif

    TRect rect=Rect();
    rect.iTl.iY += KLabelHeight;    // make way for top label
    rect.iBr.iY -= KLabelHeight;    // make way for bottom label

    CreateConsoleL(rect);

    //Bottom label
    iLabel=new (ELeave) CEikLabel;
    rect=Rect();
    rect.iTl.iY=rect.iBr.iY-KLabelHeight; // make it bottom 20 pixels
    iLabel->SetContainerWindowL(*this);
#if EPOC_SDK >= 0x06000000
    iLabel->SetRect(rect);
#else
    iLabel->SetRectL(rect);
#endif
    iLabel->SetAlignment(EHLeftVCenter); // center text
    iLabel->SetBufferReserveLengthL(500); // nice long buffer
    iLabel->SetTextL(_L("ok"));			// Label needs to have some content to prevent panics..
    //Top label
    iLabel2=new (ELeave) CEikLabel;
    rect=Rect();
    rect.iBr.iY=rect.iTl.iY+KLabelHeight; // make it top 20 pixels
    iLabel2->SetContainerWindowL(*this);
#if EPOC_SDK >= 0x06000000
    iLabel2->SetRect(rect);
#else
    iLabel2->SetRectL(rect);
#endif
    iLabel2->SetAlignment(EHLeftVCenter); // center text
    iLabel2->SetBufferReserveLengthL(500); // nice long buffer
    WriteHostL(_L("<None>"));
    ActivateL();
}


void CNslookupContainer::CreateConsoleL(const TRect& aRect)
{
    iConsole = new(ELeave) CConsoleControl;
#if defined(CRYSTAL)  //This is a patch to put the console in the right place. 
                //For some unknown reason appears displaced.
    aRect.iBr.iX += 92;
    aRect.iTl.iX += 92;
    iConsole->ConstructL(aRect.iTl, aRect.Size(), CEikConsoleScreen::ENoInitialCursor);
#elif defined(CALYPSO)
    TSize size;
    size.iHeight = aRect.iBr.iY - aRect.iTl.iY;
    size.iWidth = aRect.iBr.iX - aRect.iTl.iX - 10; // -10 to leave some space on the right
                                                    // (based on trial and error)
    TPoint position;
    position.iY = 70; // To skip the status pane and host name field (based on trial and error)
    position.iX = 5;  // 5 to leave some space on the left (based on trial and error)

    iConsole->ConstructL(position, size, CEikConsoleScreen::ENoInitialCursor);
#else
    iConsole->ConstructL(aRect.iTl, aRect.Size(), CEikConsoleScreen::ENoInitialCursor);    
#endif
    
    iConsole->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff,CEikScrollBarFrame::EAuto);
}

void CNslookupContainer::ResetScreen()
	{
    iConsole->ClearScreen();
	}

void CNslookupContainer::WriteHostL(const TDesC& aHostname)
{
    TBuf<100> aux;
    
    aux.Append(aHostname);

    iLabel2->SetTextL(aux);
    iLabel2->DrawNow();
}


void CNslookupContainer::WriteLine(const TDesC& abuf)
	{
    iConsole->Write(abuf);
	}

TKeyResponse CNslookupContainer::OfferKeyEventL(const TKeyEvent& /*aKeyEvent*/,TEventCode /*aType*/)
	{
    return EKeyWasNotConsumed;
	}

// Virtual, defined by CCoeControl; replaces the default implementation
// provided by CCoeControl.
void CNslookupContainer::Draw(const TRect& aRect) const
{
    CWindowGc& gc = SystemGc();
    gc.SetClippingRect(aRect);
    gc.DrawRect(Rect());

}
    
// Virtual, defined by CCoeControl; replaces the default implementation
// provided by CCoeControl. 
TInt CNslookupContainer::CountComponentControls() const
{
    return 3;
}

// Virtual, defined by CCoeControl; replaces the default implementation
// provided by CCoeControl.
CCoeControl* CNslookupContainer::ComponentControl(TInt aIndex) const
{
    switch (aIndex)
    {
    case 0: return iLabel;
    case 1: return iLabel2;
    case 2: return iConsole;
    //case 2: return iEdit;
	default: break;
    }

    return NULL;
}

// Defined as pure virtual by the mixin class MCoeControlObserver 
// inherited by CCoeControl. An empty implementation provided by 
// this class (its containees do not report events).
void CNslookupContainer::HandleControlEventL(CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
{


}


//
// CHostNameDialog
//
#ifdef CALYPSO
CHostNameDialog::CHostNameDialog(TDes& aHostname) : CAknTextQueryDialog(aHostname)
{
}
#else
CHostNameDialog::CHostNameDialog(CNslookup *aModel):CEikDialog()
{
    iModel=aModel;
}
#endif

// Checks if data entered by user is correct
// returns ETrue to exit dialog anf EFalse to not exit it (no name entered)

#ifdef CALYPSO
TBool CHostNameDialog::OkToExitL(TInt aButton)
{   
    return CAknTextQueryDialog::OkToExitL(aButton);
}
#else
TBool CHostNameDialog::OkToExitL(TInt aButton)
{   
	iModel->iQueryType = ChoiceListCurrentItem(ENslookupQueryType);
 	iModel->iShowSource = (CheckBoxState(ENslookupShowSource)== CEikButtonBase::ESet);

    TBuf<KHostNameLimit> hostname(_L(""));
    if (aButton==EEikBidCancel) //CANCEL BUTTON
    {
        iOKPressed=EFalse;
        return ETrue;
    }
    
    GetEdwinText(hostname,ENslookupHostName);
    
    if (hostname.Length()!=0)
    {
        iModel->SetHostName(hostname);
        return ETrue;
    }
    else
        return EFalse;  //If no hostname specified can't continue
    
}
#endif // CALYPSO

//To initialize dialog data
#ifdef CALYPSO
void CHostNameDialog::PreLayoutDynInitL()
{
    CAknTextQueryDialog::PreLayoutDynInitL();
}
#else
void CHostNameDialog::PreLayoutDynInitL()
{
	SetCheckBoxState(ENslookupShowSource, iModel->iShowSource  ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	SetChoiceListCurrentItem(ENslookupQueryType, iModel->iQueryType);
    SetEdwinTextL(ENslookupHostName,iModel->GetHostName());
}
#endif



//
// CNslookupAppUi
//

void CNslookupAppUi::ConstructL()
{
    BaseConstructL();
    
    InitModelL();
    iAppView=new(ELeave) CNslookupView(iModel);
    iAppView->ConstructL(ClientRect());

#if EPOC_SDK < 0x06000000
    //Just to watch the name of the file on the top of the toolbar
    CEikFileNameLabel* pingLabel=(CEikFileNameLabel*)iToolBar->ControlById(ENslookupCmdFileName);
    pingLabel->UpdateL();
#endif

#ifndef CALYPSO
    
    // add app view to stack; enables key event handling.
    AddToStackL(iAppView);
    
#else // if CALYPSO defined
    
    AppViewToStackL();

    iAppView->ActivateL();

    iSettingView = new (ELeave) CSettingView(iModel);
    iSettingView->ConstructL(ClientRect());
    iSettingView->SetComponentsToInheritVisibility( ETrue );

    SettingViewToStackL();

    iSettingView->ActivateL();

    ShowAppViewL();
    
#endif
}

#ifdef CALYPSO

void CNslookupAppUi::ShowAppViewL()
{
    iSettingView->MakeVisible( EFalse );
    SettingViewFromStack();
    AppViewToStackL();
    iAppView->MakeVisible( ETrue );
    iAppView->DrawNow();

    CEikButtonGroupContainer *cba = ((CAknAppUi*)CEikonEnv::Static()->EikAppUi())->Cba();
    cba->SetCommandSetL(R_NSLOOKUP_CBA);
    cba->DrawDeferred();

    // Make sure that setting view's scroll indicators are not shown in the app view
    iSettingView->ListBox()->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EOff);
    iSettingView->ListBox()->UpdateScrollBarsL();
}

void CNslookupAppUi::ShowSettingViewL()
{
    iAppView->MakeVisible( EFalse );
    AppViewFromStack();
    SettingViewToStackL();
    iSettingView->MakeVisible( ETrue );
    iSettingView->DrawNow();

    CEikButtonGroupContainer *cba = ((CAknAppUi*)CEikonEnv::Static()->EikAppUi())->Cba();
    cba->SetCommandSetL(R_SETTINGS_CBA);
    cba->DrawDeferred();

    // Make sure that scroll bars are shown if needed
    iSettingView->ListBox()->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOn, CEikScrollBarFrame::EAuto);
    iSettingView->ListBox()->UpdateScrollBarsL();
}

void CNslookupAppUi::AppViewToStackL()
{
    if (!iAppViewOnStack)
    {
        AddToStackL(iAppView);
        iAppViewOnStack = ETrue;
    }
}

void CNslookupAppUi::SettingViewToStackL()
{
    if (!iSettingViewOnStack)
    {
        AddToStackL(iSettingView);
        iSettingViewOnStack = ETrue;
    }
}

void CNslookupAppUi::AppViewFromStack()
{
    if (iAppViewOnStack) 
    {
        RemoveFromStack(iAppView);
        iAppViewOnStack = EFalse;
    }
}

void CNslookupAppUi::SettingViewFromStack()
{
    if (iSettingViewOnStack)
    {
        RemoveFromStack(iSettingView);
        iSettingViewOnStack = EFalse;
    }
}

#endif // #ifdef CALYPSO

void CNslookupAppUi::InitModelL()
{
    TPreferences param;

    RestorePreferencesL(param); //Reads the .ini file

    iModel= new (ELeave) CNslookup();
    iModel->ConstructL(param);
}

void CNslookupAppUi::HandleCommandL(TInt aCommand)
{
    TPreferences param;
    
    switch (aCommand)
    {
    case ENslookupStart:
        if (!(iModel->IsRunning()))
            if (CreateHostNameDialogL())
                iModel->BeginL();
        break;

    case ENslookupStop: //Stop ping
        if (iModel->IsRunning())
            iModel->EndNslookup();
        break;

    case ENslookupReset:
        iAppView->ResetScreen();
        break;

    case ENslookupAbout:
        CreateAboutDialogL();
        break;

    case EEikCmdExit:
        if (iModel->IsRunning())
            iModel->EndNslookup();
        iModel->GetPreferences(param);
        StorePreferencesL(param);
#ifdef CALYPSO
        AppViewFromStack();
        SettingViewFromStack();
#endif        
        Exit();
        return;

#ifdef CALYPSO
        
    case ENslookupSave:
        // Try to save settings data
        if ( iSettingView->SaveSettingsL() )
        {
            // Switch back to the normal view
            ShowAppViewL();
        }
        break;

    case ENslookupCancel:
        // Simply switch back to the normal view
        ShowAppViewL();
        break;

#endif // #ifdef CALYPSO

	default:
		break;
    }
}

    
// Launches a dialog to show an about box
void CNslookupAppUi::CreateAboutDialogL() const
    {
    CEikDialog* dialog = new (ELeave) CEikDialog();
    dialog->ExecuteLD(R_NSLOOKUP_ABOUT);    //Final D means the dialog is destructed by itself
    }


//Launches a dialog to specify a hostname and returns if Ok has been pressed to
//
#ifdef CALYPSO
TBool CNslookupAppUi::CreateHostNameDialogL()
{
    TBuf<KHostNameLimit> hostname;

    hostname.Copy(*(iModel->GetHostName()));

    CAknTextQueryDialog* dialog = new (ELeave) CHostNameDialog(hostname);

    TInt button = dialog->ExecuteLD(R_NSLOOKUP_HOSTNAME_AVKON);

    if ((button == EAknSoftkeyOk) && (hostname.Length() != 0))
    {
        iModel->SetHostName(hostname);
        return ETrue;
    }
    else
    {
        return EFalse;
    }
}
#else
TBool CNslookupAppUi::CreateHostNameDialogL()
    {
    CEikDialog* dialog = new (ELeave) CHostNameDialog(iModel);
    TInt button=dialog->ExecuteLD(R_NSLOOKUP_HOSTNAME); //Final D means the dialog is destructed by itself
    return (button==EEikBidOk); // If button is CANCEL then the ping is not executed
    }
#endif // CALYPSO

void CNslookupAppUi::RestorePreferencesL(TPreferences& aPreferences) const
{
	CNslookup::DefaultPreferences(aPreferences);
}


void CNslookupAppUi::StorePreferencesL(const TPreferences & /*aPreferences*/) const
{
	// not supported
}



CNslookupAppUi::~CNslookupAppUi()
    {
#ifndef CALYPSO
	RemoveFromStack(iAppView);
#endif
    delete iAppView;
#ifdef CALYPSO
    delete iSettingView;
#endif
    delete iModel;
    }

//
// CNslookupDocument
//

CNslookupDocument::CNslookupDocument(CEikApplication& aApp)
        : CEikDocument(aApp)
    {
    }

CEikAppUi* CNslookupDocument::CreateAppUiL()
    {
    return(new(ELeave) CNslookupAppUi);
    }

//
// CNslookupApplication
//

TUid CNslookupApplication::AppDllUid() const
    {
    return KUidNslookupApp;
    }


CApaDocument* CNslookupApplication::CreateDocumentL()
    {
    return new(ELeave) CNslookupDocument(*this);
    }

//
// EXPORTed functions
//

EXPORT_C CApaApplication* NewApplication()
    {
    return new CNslookupApplication;
    }


#ifdef MAKE_EXE_APPLICATION

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication(NewApplication);
	}

#else


#endif // MAKE_EXE_APPLICATION

#ifdef CALYPSO

//------------------------------------------------------------------
// CNslookupBinaryPopupSettingItem
//
// NOTE. This class is implemented only to circumvent
// a bug (?) in CAknBinaryPopupSettingItem::Load
//------------------------------------------------------------------

CNslookupBinaryPopupSettingItem::CNslookupBinaryPopupSettingItem( TInt aIdentifier, TBool& aBinaryValue ) :
   CAknBinaryPopupSettingItem( aIdentifier, aBinaryValue )
	{
	}

void CNslookupBinaryPopupSettingItem::LoadL()
	{
    // Call CAknEnumeratedTextPopupSettingItem::LoadL to copy
    // the external boolean value to an internal value
    // NOTE. CAknBinaryPopupSettingItem::LoadL must NOT be called
    CAknEnumeratedTextPopupSettingItem::LoadL();
	}

//-----------------------------------------------------------------
// CNslookupTextSettingItem
//
// NOTE. This is a re-implementation/copy of CAknTextSettingItem
// (the only difference is in the LoadL method) due to a bug/
// feature in CAknTextSettingItem::LoadL()
//-----------------------------------------------------------------

CNslookupTextSettingItem::CNslookupTextSettingItem( TInt aIdentifier, TDes& aText ) : 
   CAknSettingItem(aIdentifier), iExternalText( aText ), iInternalTextPtr(0,0)
	{
	}

CNslookupTextSettingItem::~CNslookupTextSettingItem()
	{
    delete iInternalText;
	}

void CNslookupTextSettingItem::StoreL()
	{
    // Copy the internal to the external
    iExternalText.Copy(iInternalText->Des() );
	}

// NOTE. This is the only method that has changed from CAknTextSettingItem
void CNslookupTextSettingItem::LoadL()
	{
    delete iInternalText;
    iInternalText = 0;
    // NOTE. The difference is below
    // OLD CODE:
    // iInternalText = iExternalText.AllocL();
    // END OF OLD CODE
    // NEW CODE:
    iInternalText = HBufC::NewL( iExternalText.MaxLength() );
    TPtr ptrText = iInternalText->Des();
    ptrText.Copy( iExternalText );
    // END OF NEW CODE
    // Use Set so that the whole TPtr is just overwritten - and gets a new buffer 
    iInternalTextPtr.Set(iInternalText->Des() );
	}

const TDesC& CNslookupTextSettingItem::SettingTextL()
{
    return *iInternalText;
}

void CNslookupTextSettingItem::EditItemL( TBool /* aCalledFromMenu */)
	{
    CAknSettingPage* dlg = new( ELeave )CAknTextSettingPage( SettingPageResourceId(), iInternalTextPtr, 0 );
    SetSettingPage( dlg );
    SettingPage()->SetSettingPageObserver(this);
    SetUpStandardSettingPageL();
    SettingPage()->ExecuteLD(CAknSettingPage::EUpdateWhenChanged);
	}

TPtr& CNslookupTextSettingItem::InternalTextPtr()
{
    return iInternalTextPtr;
}

TPtrC CNslookupTextSettingItem::ExternalText()
{
    return (TPtrC)(iExternalText);
}

void CNslookupTextSettingItem::SetExternalText( TDesC& aNewExternalText )
{
    iExternalText.Copy( aNewExternalText );
}

//-----------------------------------------------------------------
// CSettingView
//-----------------------------------------------------------------

void CSettingView::ConstructL(const TRect& /*aRect*/)
{
    // Copy data from the model to member variables

    iLimitPacketCount = iModel->iPackLimit ? ETrue : EFalse;
    iMaxPacketCount.Num( iModel->iTotalPackets );
    iPacketDataSize.Num( iModel->iPacketDataSize );
    iWaitTime.Num( iModel->iSecWait );
    iLastWaitTime.Num( iModel->iLastSecWait );

    iPattern.Copy( iModel->iPattern );
    iQuiet = iModel->iQuiet ? ETrue : EFalse;
    iVerbose = iModel->iVerbose ? ETrue : EFalse;
    iDebug = iModel->iDebug ? ETrue : EFalse;

    CAknSettingItemList::ConstructFromResourceL( R_NSLOOKUP_SETTINGS );
}

CSettingView::CSettingView(CNslookup *aModel)
	{
    iModel = aModel;
	}

CSettingView::~CSettingView()
	{
	}

void CSettingView::ProcessCommandL(TInt /*aCommand*/)
	{
	}

CAknSettingItem* CSettingView::CreateSettingItemL( TInt identifier )
	{
    CAknSettingItem* settingItem;

    switch (identifier)
    {
        case 1:
            settingItem = new (ELeave) CNslookupBinaryPopupSettingItem( identifier, iLimitPacketCount );
            return settingItem;
            break;
        case 2:
            settingItem = new (ELeave) CNslookupTextSettingItem( identifier, iMaxPacketCount );
            return settingItem;
            break;
        case 3:
            settingItem = new (ELeave) CNslookupTextSettingItem(identifier, iPacketDataSize);
            return settingItem;
            break;
        case 4:
            settingItem = new (ELeave) CNslookupTextSettingItem(identifier, iWaitTime);
            return settingItem;
            break;
        case 5:
            settingItem = new (ELeave) CNslookupTextSettingItem(identifier, iLastWaitTime);
            return settingItem;
            break;
        case 6:
            settingItem = new (ELeave) CNslookupTextSettingItem(identifier, iPattern);
            return settingItem;
            break;
        case 7:
            settingItem = new (ELeave) CNslookupBinaryPopupSettingItem( identifier, iQuiet );
            return settingItem;
            break;
        case 8:
            settingItem = new (ELeave) CNslookupBinaryPopupSettingItem( identifier, iVerbose );
            return settingItem;
            break;
        case 9:
            settingItem = new (ELeave) CNslookupBinaryPopupSettingItem( identifier, iDebug );
            return settingItem;
            break;
        default:
            settingItem = new (ELeave) CAknSettingItem(identifier);
            return settingItem;
            break;
    }
}

TBool CSettingView::SaveSettingsL()
	{
    // Save settings to the member variables

    StoreSettingsL();

    // Validate input

    TLex lex;

    TInt maxPacketCount;
    lex.Assign( iMaxPacketCount );
    if ( lex.Val( maxPacketCount ) != KErrNone ) 
    {
        CEikonEnv::Static()->InfoMsg(_L("Packet count must be numeric"));
        return EFalse;
    }

    TInt packetDataSize;
    lex.Assign( iPacketDataSize );
    if ( lex.Val( packetDataSize ) != KErrNone ) 
    {
        CEikonEnv::Static()->InfoMsg(_L("Packet size must be numeric"));
        return EFalse;
    }

    TInt waitTime;
    lex.Assign( iWaitTime );
    if ( lex.Val( waitTime ) != KErrNone ) 
    {
        CEikonEnv::Static()->InfoMsg(_L("Wait time must be numeric"));
        return EFalse;
    }

    TInt lastWaitTime;
    lex.Assign( iLastWaitTime );
    if ( lex.Val( lastWaitTime ) != KErrNone ) 
    {
        CEikonEnv::Static()->InfoMsg(_L("Last wait time must be numeric"));
        return EFalse;
    }

    lex.Assign( iPattern );
    while (!lex.Eos())
    {
        if (!lex.Get().IsHexDigit())
        {
            CEikonEnv::Static()->InfoMsg(_L("Pattern must be hexadecimal"));
            return EFalse;
        }
    }

    // Validation OK, so save settings to the model

    iModel->iPackLimit = iLimitPacketCount;
    iModel->iTotalPackets = maxPacketCount;
    iModel->iPacketDataSize = packetDataSize;
    iModel->iSecWait = waitTime;
    iModel->iLastSecWait = lastWaitTime;
    iModel->iPattern.Copy(iPattern);
    iModel->iQuiet = iQuiet;
    iModel->iVerbose = iVerbose;
    iModel->iDebug = iDebug;

    return ETrue;
}
    
#endif // #ifdef CALYPSO
