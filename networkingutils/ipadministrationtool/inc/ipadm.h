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
// ipadm.h - IP administration tool main module
//



/**
 @internalComponent
*/

#ifndef __IPADM_H
#define __IPADM_H

//#include <coecntrl.h>
#include <coeccntx.h>

#include <eikenv.h>
#include <eikappui.h>
#include <eikapp.h>
#include <eikdoc.h>
#include <techview/eiktbar.h>
#include <techview/eikconso.h>

#include <ipadm.rsg>
#include "ipadm.hrh"

const TUid KUidIpAdmApp = {0x10000887}; 
// 
// CSimpleConsole
//
enum TMessageControlFontStyle
    {
    EStyleElementBold=EMenuCommandBold,
    EStyleElementItalic=EMenuCommandItalic,
    EStyleElementInverse=EMenuCommandInverse,
    EStyleElementUnderline=EMenuCommandUnderline,
    EStyleElementColor=EMenuCommandColor
    };

class CConsoleControl : public CCoeControl
	{
public:
	CConsoleControl() {}
	~CConsoleControl();
	void ConstructL(const TPoint& aLeftTop, const TSize& aSize, TInt aFlags);
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
protected:
    void FocusChanged(TDrawNow aDrawNow);
private:
    void ToggleFontStyleAndRedrawL(TMessageControlFontStyle aStyleElement);
	
private:
	CEikConsoleScreen* iConsole;
	TInt iHistory;
	};


//
// class CIpAdmView
//
class CIpAdmEngine;
class CIpAdmView : public CCoeControl, public MCoeControlBrushContext
    {
public:
	~CIpAdmView();
    void ConstructL(const TRect& aRect);
	
	void StartL();
	void StopL();
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl(TInt aIndex) const;
	void Write(const TDesC &aDes);
	void ClearScreenL();
	void ShowError(TInt aId);

	void HandleCommandL(TInt aCommand);
private: 
	void Draw(const TRect& /*aRect*/) const;

	//Component Controls
	void CreateBigConsoleL(TInt aFlags);

	void ShowError(const TDes &msg);
	void ShowError(const TDes &msg, TInt aErr);

private:
	CConsoleControl* iConsole;
	
	CIpAdmEngine *iModel;
	TBool iRunning;
    };


//
//	CIpAdmAppUi
//
class CIpAdmAppUi : public CEikAppUi
    {
public:
    void ConstructL();
	~CIpAdmAppUi();

private:
	void HandleCommandL(TInt aCommand);
	//	TBool LaunchOptionsDialog(CUDPSendEngine* aModel);
	// void LaunchAboutDialog();
private:
    CIpAdmView* iAppView;
//	CHelpTask *iHelp;
    };

//
// CIpAdmDocument
//
class CIpAdmDocument : public CEikDocument
	{
public:
	CIpAdmDocument(CEikApplication& aApp);
private:
	CEikAppUi* CreateAppUiL();
	};

//
// CIpAdmAppUi
//
class CIpAdmApplication : public CEikApplication
	{
private: // from CApaApplication
	CApaDocument* CreateDocumentL();
	TUid AppDllUid() const;
	};


#endif
