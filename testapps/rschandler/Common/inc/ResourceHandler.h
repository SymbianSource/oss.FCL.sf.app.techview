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

#ifndef __ResourceHandler_H
#define __ResourceHandler_H


#include "ResourceHandler.hrh"
//for storing resource data
#include "ResourceHandler_DataStructs.h"

#include <apgwgnam.h>
#include <apgtask.h>
 
#include <coeccntx.h>

#include <eikenv.h>
#include <eikappui.h>
#include <eikapp.h>
#include <eikdoc.h>
#include <techview/eikmenup.h>

#include "ResourceHandler_Dialogs.h" // may not need for S60

#include <resourcehandler.rsg>
#include <techview/eikchkbx.h>
#include <techview/eikchlst.h>
#include <techview/eiklbbut.h>

#include <techview/eiklabel.h>
#include <techview/eikcmbut.h>

#ifdef UIQ
#include <qiknumbereditor.h>
#include <qikvertoptionbuttonlist.h>
#endif


#ifdef TV

#include <techview/eikhopbt.h>
// for CEikNumberEditor
#include <techview/eikmfne.h>
// for CEikButtonGroupContainer
#include <techview/eikbtgpc.h>
// for CEikNumberEditor
#include <techview/eikmfne.h>
#endif

#include <techview/eikclb.h> //for column list box
#include <techview/eikclbd.h> // for CColumnListBoxData in TV/UIQ

#include <resourcehandler.rsg>
// for CEikButtonGroupContainer
#include <techview/eikbtgpc.h>

#include <techview/eikon.hrh>

#include <e32base.h>
#include <e32cons.h>
#include <e32hal.h>
#include <hal.h>
#include <txtfmlyr.h>
#include <txtrich.h>
#include <miutset.h>
#include <mtmuids.h>
#include <msvreg.h> 
#include <msvuids.h>
#include <techview/eikedwin.h>
#include <smut.h>
#include <f32file.h>
#include <f32dbg.h>


// copy from k32std.h
#define KHARDWARE		0x00000001
#define KBOOT			0x00000002
#define KSERVER			0x00000004
#define KMMU			0x00000008
#define KSEMAPHORE		0x00000010
#define KSCHED			0x00000020
#define KPROC			0x00000040
#define KEXEC			0x00000080
#define KEXECFAST		0x00000100
#define KTHREAD			0x00000200
#define KDLL			0x00000400
#define KLOGON			0x00000800
#define KPBUS1			0x00001000
#define KPBUS2			0x00002000
#define KPBUSDRV		0x00004000
#define KPOWER			0x00008000
#define KTIMING			0x00010000
#define KEVENT			0x00020000
#define KMEMTRACE		0x00080000  
#define KIPC			0x00100000	
#define KDEBUGGER		0x00200000  
#define	KVIDEO			0x00800000	
#define KUSB            0x01000000  
#define KPANIC			0x80000000

_LIT(KFormatDec, "%d");
_LIT(KFormatDec64, "%ld");
_LIT(KFormatUDec,"%u");
_LIT(KFormatHex, "%x");
_LIT(KAsterisk, "*");
_LIT(KFillName, "ResMan");

enum TMemorySizes
{
	K4k		=	0x00001000, //   4,096
	K8k		=	0x00002000, //   8,192
	K16k	=	0x00004000, //  16,384
	K32k	=	0x00008000, 
	K64k	=	0x00010000, //  65,536
	K128k	=	0x00020000,
	K256k	=	0x00040000, // 262,144
	K512k	=	0x00080000,
	K1M		=	0x00100000,
	K2M		=	0x00200000,
	K4M		=	0x00400000,
	K8M		=	0x00800000, //8,388,608
	K16M	=	0x01000000	
};

enum TObjectsViewed
	{
	EObjectNone,
	EObjectProcess,
	EObjectThread,
	EObjectChunk,
	EObjectLibrary,
	EObjectServer,
	EObjectSemaphore,
	EObjectMutex,
	EObjectLogicalDevice,
	EObjectPhysicalDevice,
	EObjectLogicalChannel,
	EObjectWindow,
	EObjectDrive,
	EObjectHal
	};

// Forward declarations

class CResourceAppUi;
class CResourceAppView;
class CResourceListView;
class CResourceView;

//
//
// CResourceApplication
//
//

class CResourceApplication :  public CXKNAPPLICATION
	{
private: 
	           // Inherited from class CApaApplication
	CApaDocument* CreateDocumentL();
	TUid AppDllUid() const;
	};
//
//
// CResourceDocument
//
//
class CResourceDocument : public CXKNDOCUMENT
	{
public:
	static CResourceDocument* NewL(CEikApplication& aApp);
	CResourceDocument(CEikApplication& aApp);
	void ConstructL();
private: 
	           // Inherited from CXKNDOCUMENT
	CEikAppUi* CreateAppUiL();
	};
//
//
// CResourceAppUi
//
//
class CResourceAppUi : public CXKNAPPUI
    {
public:
    void ConstructL();
	~CResourceAppUi();

private:
	// Inherited from class CXKNAPPUI
	void HandleCommandL(TInt aCommand);
	
private:
	CResourceView* iListBoxView;
	void SetProcess(void);
	void SetThread(void);
	void SetDebug(void);
	void SetRam(void);
	void SetDrive(void);

	void AppendDesc(TDes& root, TDes& add);

	};

//
//
// CResourceDetails - stores resource names and details.
// Used for resources for which not just the name but additional
// details are displayed.
//
//
class TResourceDetails
{
protected:
	TProcessData iProcessData;
	TThreadData iThreadData;
	TChunkData iChunkData;
	TSemaphoreData iSemaphoreData;
	TMutexData iMutexData;
	TDriveData iDriveData;
	THalData iHalData;
	TListData iListData;
	void GetProcessData(TDesC& aProcessName);
	void GetThreadData(TDesC& aThreadName);
	void GetChunkData(TFindChunk findHb, TDesC& aChunkName);
	void GetSemaphoreData(TFindSemaphore& findHb, TDesC& aSemaphoreName);
	void GetMutexData(TFindMutex& findHb, TDesC& aMutexName);
	void GetDriveData(TInt aValidDriveNum, TDriveUnit aDriveUnit, TVolumeInfo aVolumeInfo);
	void GetHalData(TInt aEntryNum, HAL::SEntry*);
private:
	void AppendDotsToResourceName(TFullName& aResourceName);

};
//
//
// CResourceList
//
//
class CResourceList: public CCoeControl, public MCoeControlObserver, public TResourceDetails
{
public:
	void ConstructL(TInt aTablePosition);
	~CResourceList();
protected:
	void UpdateResourceCount(TUint aNumberOfResource) {iResourceCount = aNumberOfResource;}
	void UpdateListData();

	CCoeControl* GetListBox() const;

private:
	void SetUpProcessTable();
	void SetUpThreadTable();
	void SetUpChunkTable();
	void SetUpSemaphoreTable();
	void SetUpMutexTable();
	void SetUpDriveTable();
	void SetUpHalTable();
	void SetUpList(); //uses single column tables

	void GetResourceList();
	void AddRow(const TProcessData& aProcessData);
	void AddRow(const TThreadData& aThreadData);
	void AddRow(const TChunkData& aChunkData);
	void AddRow(const TSemaphoreData& aSemaphoreData);
	void AddRow(const TMutexData& aMutexData);
	void AddRow(const TDriveData& aDriveData);
	void AddRow(const THalData& aHalData);
	void AddRow(const TListData& aListData);

	void GetThreadList(void);
	void GetProcessList(void);
	void GetChunkList(void);
	void GetLibraryList(void);
	void GetServerList(void);
	void GetSemaphoreList(void);
	void GetMutexList(void);
	void GetLogicalDeviceList(void);
	void GetPhysicalDeviceList(void);
	void GetLogicalChannelList(void);
	void GetWindowList(void);
	void GetDriveList(void);
	void GetHalList(void);

	void EmptyList();
	
	CEikColumnListBox* iListBox;
	CTextListBoxModel* iModel;
	MDesCArray* iTextArray;
	CDesCArray* iListBoxItems;
	CColumnListBoxData* iListBoxData;
protected:
	TInt iBorder;
	TUint iResourceCount; // Count of number of items in table
	TObjectsViewed iObjectsViewed; // What view is showing

};
 
//
//
// CResourceView
//
//
class CResourceView : public CResourceList
    {
public:
	static CResourceView* NewL(const TRect& aRect, CResourceAppUi* aAppUi);
	
	~CResourceView();
	
	void SetCurrentObjects(TObjectsViewed aViewed)
		{iObjectsViewed = aViewed;};

	void UpdateView();

private:
	// Private constructor to force NewL
	CResourceView();
	void ConstructL(const TRect& aRect, CResourceAppUi* aAppUi);
	void InvalidateView(void) const;
	void UpdateTitle(void);
protected:

	// Inherited from CCoeControl
	virtual TInt CountComponentControls() const;				
	virtual CCoeControl* ComponentControl(TInt aIndex) const;
	virtual void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);

private:
	
	// Inherited from CCoeControl
	void Draw(const TRect& /*aRect*/) const;

private:

	CResourceAppUi* iAppUi;

	// State Data
	TBuf<27> iTitle; // Title above table
	// Controls
	CEikCommandButton* iCommandButton;	
    };

#endif

