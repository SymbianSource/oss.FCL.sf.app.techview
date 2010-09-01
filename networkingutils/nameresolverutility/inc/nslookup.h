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
// nslookup.h - nslookup client main module
//



/**
 @internalComponent
*/
#ifndef __NSLOOKUP_H
#define __NSLOOKUP_H

#include <coecntrl.h>
#include <coeccntx.h>
#include <techview/eiklabel.h>
#include <txtrich.h>
#include <eikrted.h>
#include <eikappui.h>
#include <eikapp.h>
#include <techview/eikdialg.h>
#include <eikdoc.h>
#include <eikconso.h>
#ifdef CALYPSO
#include <AknQueryDialog.h>
#include <AknSettingItemList.h>
#include <AknAppUi.h>
#endif


// UID of app


const TUid KUidNslookupApp = { 0x10000892 } ;
const TUid KUidNslookupVersionUid = { 0x10000892 } ;

#ifdef CALYPSO

class CNslookupBinaryPopupSettingItem : public CAknBinaryPopupSettingItem
	{
    public:
        CNslookupBinaryPopupSettingItem( TInt aIdentifier, TBool& aBinaryValue );

        virtual void LoadL();
	};

class CNslookupTextSettingItem : public CAknSettingItem
	{
    public:
        CNslookupTextSettingItem( TInt aIdentifier, TDes& aText );
        CNslookupTextSettingItem::~CNslookupTextSettingItem();
        virtual void StoreL();
        virtual void LoadL(); // Called from 2nd stage constructors
        virtual const TDesC& SettingTextL();
        virtual void EditItemL( TBool aCalledFromMenu );

    protected:
        TPtr& InternalTextPtr();
        TPtrC ExternalText();
        void SetExternalText( TDesC& aNewExternalText );

    private:
        TDes& iExternalText;
        HBufC* iInternalText;
        TPtr iInternalTextPtr;
	};

class CSettingView : public CAknSettingItemList, public MEikCommandObserver
	{
    public:
        CSettingView(CNslookup *aModel);
        ~CSettingView();
        void ConstructL(const TRect& aRect);
        void ProcessCommandL(TInt aCommand);
        CAknSettingItem* CreateSettingItemL( TInt identifier );
        TBool SaveSettingsL();

    private:
        TBool    iLimitPacketCount;        // Indicates if unlimited packet number
        TBuf<5>  iMaxPacketCount;          // Number of ICMP Echo Request packets to send
        TBuf<5>  iPacketDataSize;          // Default data size (not including ICMP header)
        TBuf<5>  iWaitTime;                // Time between sent packets (default 1 second)
        TBuf<5>  iLastWaitTime;            // Time to wait for the last packet (default 2 seconds)
        TBuf<MAX_PATTERN_LENGTH> iPattern; // Pattern to fill the packet (default 0xFF)
        TBool    iQuiet;                   // No packet info, just statistics
        TBool    iVerbose;                 // Verbose Output. All ICMP packets, not only Echo reply
        TBool    iDebug;                   // Set the SO_DEBUG flag in the socket

        CNslookup *iModel;
	};

#endif // #ifdef CALYPSO

// 
// CSimpleConsole
//
/*
enum TMessageControlFontStyle
{
    EStyleElementBold=EMenuCommandBold,
    EStyleElementItalic=EMenuCommandItalic,
    EStyleElementInverse=EMenuCommandInverse,
    EStyleElementUnderline=EMenuCommandUnderline,
    EStyleElementColor=EMenuCommandColor
};
*/
class CConsoleControl : public CCoeControl
{
public:
	CConsoleControl() {}
	~CConsoleControl();
	//void ConstructL(TInt aFlags);
	//void ConstructL(TPoint aLeftTop,const TSize& aSize,TInt aFlags,CCoeControl *aParent);
	void ConstructL(const TPoint& aTopLeft,const TSize& aSize,TInt aFlags);
    //TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
    //void HandlePointerEventL(const TPointerEvent& aPointerEvent);
	//void DynInitMenuPaneL(TInt aMenuId,CEikMenuPane* aMenuPane);
    void HandleCommandL(TInt aCommand);
    void ActivateL();
	void SetScrollBarVisibilityL(CEikScrollBarFrame::TScrollBarVisibility aHBarVisibility, CEikScrollBarFrame::TScrollBarVisibility aVBarVisibility);
	void DrawCursor();
	void Write(const TDesC &aDes);
	CEikConsoleControl *ConsoleControl() const;
	TBool UpdateScrollBars();
	void ClearScreen();
	void Redraw(const TRect &aRect);
	void Lf();
	TSize ScreenSize() const;
protected:
    void FocusChanged(TDrawNow aDrawNow);
private:
    //void ToggleFontStyleAndRedrawL(TMessageControlFontStyle aStyleElement);
	
private:
	CEikConsoleScreen* iConsole;
	TInt iHistory;
	//TInt iAllPrintable,iScrollLock,iIgnoreCursor,iHideCursor;
	//TDesC* iSelBufPtr;
	//TInt iSmallScreen;
	//TInt iHighCursor;
};


class CNslookupContainer: public CCoeControl, public MCoeControlObserver
{
public:
	  // Construction
	CNslookupContainer(CNslookup* aModel);
	void ConstructL(const TRect& aRect);

	  // Destruction
	~CNslookupContainer();
	//void ConstructFromResourceL(TResourceReader& aReader);
	//void CSmileyContainer::PrepareForFocusLossL();
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	void WriteHostL(const TDesC& aHostname);
	void WriteLine(const TDesC& abuf);
	void ResetScreen();

//protected:
	//void FocusChanged(TDrawNow aDrawNow);
private:
	  // Virtual, defined by CCoeControl; replaces the default implementation
	  // provided by CCoeControl.
	void         Draw(const TRect& aRect) const;
	
	  // Virtual, defined by CCoeControl; replaces the default implementation
	  // provided by CCoeControl. 
    TInt         CountComponentControls() const;

	  // Virtual, defined by CCoeControl; replaces the default implementation
	  // provided by CCoeControl.
	CCoeControl* ComponentControl(TInt aIndex) const;

	  // Defined as pure virtual by the mixin class MCoeControlObserver 
	  // inherited by CCoeControl. An empty implementation provided by 
	  // this class (its containees do not report events).
	void         HandleControlEventL(CCoeControl* aControl,
		                             TCoeEvent aEventType);

	void CreateConsoleL(const TRect& aRect);
private:
	  // Member functions defined and used by this class
private:
      // Data members defined and used by this class.
	CNslookup *iModel;
	CEikLabel* iLabel; // label for status messages
	CEikLabel* iLabel2; // label for status messages
	
	CConsoleControl* iConsole;
	};






//
// class CNslookupView
//

class CNslookupView : public CCoeControl, public MCoeControlBrushContext
    {
public:
	CNslookupView(CNslookup *aModel);
    void ConstructL(const TRect& aRect);
    ~CNslookupView();
	// changing view
	void ConstructViewL();
	void ResetScreen();
	// various types of update


private: // from CCoeControl
	void Draw(const TRect& /*aRect*/) const;
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
    TInt CountComponentControls() const;
    CCoeControl* ComponentControl(TInt aIndex) const;
private: // new function
	void CreateLabelL();
private: // data
	CNslookup *iModel;
	CNslookupContainer* iContainer;
    };

#ifdef CALYPSO
class CHostNameDialog : public CAknTextQueryDialog
#else
class CHostNameDialog : public CEikDialog
#endif
	{
public:
#ifdef CALYPSO
    CHostNameDialog(TDes& aHostname);
#else    
    CHostNameDialog(CNslookup *aModel);
#endif
	
private:
	TBool OkToExitL(TInt aButton);
	void PreLayoutDynInitL();
private:
	TBool iOKPressed;
	CNslookup *iModel;
	};

//
// CNslookupAppUi
//

#ifdef CALYPSO
class CNslookupAppUi : public CAknAppUi
#else
class CNslookupAppUi : public CEikAppUi
#endif
    {
public:
    void ConstructL();
	~CNslookupAppUi();

#ifdef CALYPSO

private:
    TBool iAppViewOnStack;
    TBool iSettingViewOnStack;
    void AppViewToStackL();
    void AppViewFromStack();
    void SettingViewToStackL();
    void SettingViewFromStack();
    void ShowAppViewL();
    void ShowSettingViewL();
    CSettingView* iSettingView;

#endif // #ifdef CALYPSO
    
private: // from CEikAppUi
	static TInt Launcher(TAny* x);
	void InitModelL();
	void HandleCommandL(TInt aCommand);
	void CreateOptionsDialog();
	TBool CreateHostNameDialogL();
	void CreateAboutDialogL() const;
	void RestorePreferencesL(TPreferences& aPreferences) const;
	void StorePreferencesL(const TPreferences &aPreferences) const;
	//void UnDimStop();
	//void DimStop();

private:
    CNslookupView* iAppView;
	CNslookup *iModel;	//contains all related Data
	//TBossPuzzle* iModel;
    };

//
// CExampleShellDocument
//

class CNslookupDocument : public CEikDocument
	{
public:
	CNslookupDocument(CEikApplication& aApp);
	//CNslookupDocument(CEikApplication& aApp): CEikDocument(aApp) { }
	//TBossPuzzle* Model() { return(&iModel); }
private: // from CEikDocument
	CEikAppUi* CreateAppUiL();
private:
	//TBossPuzzle iModel;
	};

//
// CNslookupApplication
//

class CNslookupApplication : public CEikApplication
	{
private: // from CApaApplication
	CApaDocument* CreateDocumentL();
	TUid AppDllUid() const;
	};

#endif
