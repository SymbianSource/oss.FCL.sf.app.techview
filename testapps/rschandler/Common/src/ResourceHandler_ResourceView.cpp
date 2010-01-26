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
// Source file for the implementation of the 
// application view class - CResourceView
// 
//

#include "ResourceHandler.h"

CResourceView::CResourceView()
	{
	}

CResourceView* CResourceView::NewL(const TRect& aRect, CResourceAppUi *aAppUi)
	{
	CResourceView* self = new(ELeave) CResourceView();
	CleanupStack::PushL(self);
	self->ConstructL(aRect, aAppUi);
	CleanupStack::Pop();
	return self;
	}

CResourceView::~CResourceView()
	{
	delete iCommandButton;
	}

void CResourceView::ConstructL(const TRect& aRect, CResourceAppUi* aAppUi)
    {
	// Initialisation
	iAppUi = aAppUi;
	iObjectsViewed = EObjectProcess;

	CreateWindowL();
	           
	// Whole extent of view
	SetRect(aRect);

	// Border around components in pixels
	iBorder = 1;

	// Place the refresh button	
	iCommandButton = new(ELeave) CEikCommandButton;
	iCommandButton->SetTextL(_L("Refresh"));
	iCommandButton->SetSize(iCommandButton->MinimumSize());
	iCommandButton->SetPosition(TPoint(Rect().Width() - iCommandButton->Size().iWidth - iBorder, iBorder));
	iCommandButton->SetContainerWindowL(*this);
	iCommandButton->SetObserver(this);
	TInt buttonHeight = iCommandButton->Size().iHeight;	

	// Get the title font height
	TInt titleHeight = iEikonEnv->TitleFont()->HeightInPixels();
	TInt tablePosition = 0;
	if (titleHeight>=buttonHeight) 
		tablePosition=titleHeight;
	else
		tablePosition=buttonHeight;

	CResourceList::ConstructL(tablePosition);
// Activate the main window control - this will also activate the contained controls
	
	ActivateL();
	DrawNow();
	}


void CResourceView::Draw(const TRect& /*aRect*/) const
	{
	CWindowGc& gc = SystemGc();                     

	gc.Clear();
		
	// Put title on view
	const CFont* fontUsed = iEikonEnv->TitleFont();
	gc.UseFont(fontUsed);
	gc.SetPenColor(KRgbBlack);
	TPoint pos(iBorder,fontUsed->AscentInPixels()+iBorder);
	gc.DrawText(iTitle, pos);

	gc.DiscardFont();
	}



TInt CResourceView::CountComponentControls() const
	{
	return 2;
	}


CCoeControl* CResourceView::ComponentControl(TInt aIndex) const
	{
	if (aIndex == 0)
		return iCommandButton;
	else 
		return CResourceList::GetListBox();
	}


// for use by refresh button control
void CResourceView::HandleControlEventL(CCoeControl* aControl,TCoeEvent /*aEventType*/)
	{	
	if (aControl == iCommandButton)
		{
		UpdateView();
		}
	}

void CResourceView::InvalidateView(void) const
	{
	Window().Invalidate(Rect()); //calls Draw()
	}

void CResourceView::UpdateView(void)
	{
	CResourceList::UpdateListData();
	UpdateTitle();
	InvalidateView();
	}

void CResourceView::UpdateTitle(void)
	{
	// assume the table data, iResourceCount and iObjectViewed data are up to date
	TBufC<20> title;

	// Set the title appropriately
	switch (iObjectsViewed)
		{
		case EObjectProcess: 
			title = _L("Processes");
			break;
		case EObjectThread: 
			title = _L("Threads");
			break;
		case EObjectChunk: 
			title = _L("Chunks");  
			break;
		case EObjectLibrary: 
			title = _L("Libraries"); 
			break;
		case EObjectServer: 
			title = _L("Servers");  
			break;
		case EObjectSemaphore: 
			title = _L("Semaphores"); 
			break;
		case EObjectMutex: 
			title = _L("Mutexes");  
			break;
		case EObjectLogicalDevice: 
			title = _L("Log. Devices"); 
			break;
		case EObjectPhysicalDevice: 
			title = _L("Phy. Devices"); 
			break;
		case EObjectLogicalChannel: 
			title = _L("Log. Channels"); 
			break;
		case EObjectWindow: 
			title = _L("Window Grps"); 
			break;
		case EObjectDrive: 
			title = _L("Drives");  
			break;
		case EObjectHal: 
			title = _L("HALs");  
			break;
		default: break;
		}
		
	//set title and resource count
	TBuf<2> openingBracket(_L(" ("));
	TBuf<1> closingBracket(_L(")"));
	iTitle.Format(_L("%S%S%u%S"), &title, &openingBracket, iResourceCount, &closingBracket);
	}
