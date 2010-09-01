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

#include <techview/eiktxlbm.h> //CTextListBoxModel

void CResourceList::ConstructL(TInt aTablePosition)
	{
		// Create the basic list box        
	iListBox = new(ELeave) CEikColumnListBox;
	iListBox->ConstructL( this ); //This view is parent. No flags. Also constructs the associated list box model and item drawer for the list box.

	iListBox->SetContainerWindowL( *this );
	iListBox->CreateScrollBarFrameL( );
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EAuto, CEikScrollBarFrame::EAuto);  
  
	iListBox->SetPosition(TPoint(iBorder, aTablePosition + iBorder));

	iListBox->SetSize(Rect().Size()-TSize(0, aTablePosition) -TSize(iBorder*2,iBorder*2));

	iListBox->ActivateL();
	
	iModel = iListBox->Model();
	
	iTextArray = iModel->ItemTextArray();
	iListBoxItems = static_cast<CDesCArray*>(iTextArray);
	iListBoxData = STATIC_CAST(CColumnListBoxItemDrawer*,iListBox->View()->ItemDrawer())->ColumnData();
	
	}

CResourceList::~CResourceList()
	{
	delete iListBox;
    iModel = NULL;
	iTextArray = NULL;
	iListBoxItems = NULL;
	iListBoxData = NULL;
	}

CCoeControl* CResourceList::GetListBox() const
	{
	return iListBox;
	}

void CResourceList::EmptyList()
	{
	iListBoxItems->Reset();
	iListBox->Reset();
	}

#ifdef UIQ
#define CW_Id 40
#define CW_HalId 35
#define CW_DriveAtt 35
#define CW_Short 45
#define CW_UID 75
#define CW_ExT_R 50
#define CW_Medium 60
#define CW_Long 90
#define CW_Priority 60
#define CW_ProcessId 70
#define CW_Read 50
#define CW_Writeable 70
#define CW_Dynamic 60
#define CW_List 60
#define CW_DriveSize 70
#define CW_Count 80
#define CW_MemSize 90 
#define CW_HalName 200
#define CW_MutexName 280
#define CW_Media 120
#define CW_Name 135
#define CW_ProcessName 220
#define CW_ExitCat 80
#define CW_ChunkName 380
#define CW_SemName 300
#define CW_ThreadName 370
#define CW_LongName 250
#define CW_Message 250
#define CW_FileName 400
#define GapWidth 10
#endif
#ifdef TV
#define CW_Id 25
#define CW_HalId 45
#define CW_DriveAtt 60
#define CW_Short 60
#define CW_UID 100
#define CW_Medium 90
#define CW_Long 115
#define CW_Priority 85
#define CW_ProcessId 95
#define CW_Writeable 90
#define CW_Read 70
#define CW_Name 195
#define CW_ProcessName 195
#define CW_ExitCat 185
#define CW_ChunkName 350
#define CW_SemName 305
#define CW_HalName 260
#define CW_ThreadName 380
#define CW_LongName 380
#define CW_Message 380
#define CW_ExT_R 70
#define CW_MutexName 305
#define CW_FileName 500
#define CW_Media 140
#define CW_List 150
#define CW_Dynamic 100
#define CW_Count 115
#define CW_MemSize 130
#define GapWidth 25
#endif

void CResourceList::SetUpProcessTable()
{
		// deal with the columns
	iListBoxData->SetColumnWidthPixelL( 0, CW_Id );
	iListBoxData->SetColumnWidthPixelL( 1, CW_ProcessName );
	iListBoxData->SetColumnAlignmentL( 1, CGraphicsContext::ELeft);
	iListBoxData->SetColumnWidthPixelL( 2, CW_Medium ); //Priority
	iListBoxData->SetColumnAlignmentL( 2, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 3, CW_Short ); //Prot?
	iListBoxData->SetColumnAlignmentL( 3, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 4, CW_Short ); //Sys?
	iListBoxData->SetColumnAlignmentL( 4, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 5, CW_Short ); //Owner Id
	iListBoxData->SetColumnAlignmentL( 5, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 6, CW_Medium ); //Exit T.
	iListBoxData->SetColumnAlignmentL( 6, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 7, CW_Short ); // Exit R
	iListBoxData->SetColumnAlignmentL( 7, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 8, CW_Short ); // Exit C
	iListBoxData->SetColumnAlignmentL( 8, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 9, CW_UID ); // Uid 0
	iListBoxData->SetColumnAlignmentL( 9, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 10, CW_UID ); //Uid 1
	iListBoxData->SetColumnAlignmentL( 10, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 11, CW_UID + GapWidth ); // Uid 2
	iListBoxData->SetColumnAlignmentL( 11, CGraphicsContext::ERight);
	iListBoxData->SetColumnHorizontalGapL(11, GapWidth);
	iListBoxData->SetColumnWidthPixelL( 12, CW_FileName ); // File Name
	iListBoxData->SetColumnHorizontalGapL(12, GapWidth);

	TBuf<1000> titles(_L("Id\tName\tPriority\tProt?\tSys?\tOwner\tExit T.\tExit R.\tExit C.\tUid 0\tUid 1\tUid 2\tExecutable File"));
	iListBoxItems->AppendL(titles);
}

void CResourceList::SetUpThreadTable()
{
	iListBoxData->SetColumnWidthPixelL( 0, CW_Id );
	iListBoxData->SetColumnWidthPixelL( 1, CW_ThreadName );
	iListBoxData->SetColumnAlignmentL( 1, CGraphicsContext::ELeft);
	iListBoxData->SetColumnWidthPixelL( 2, CW_Priority ); //Priority
	iListBoxData->SetColumnAlignmentL( 2, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 3, CW_ProcessId ); //ProcessId
	iListBoxData->SetColumnAlignmentL( 3, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 4, CW_Medium ); //Stack Sz
	iListBoxData->SetColumnAlignmentL( 4, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 5, CW_Medium ); //Heap Sz
	iListBoxData->SetColumnAlignmentL( 5, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 6, CW_Short ); //Prot?
	iListBoxData->SetColumnAlignmentL( 6, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 7, CW_Short ); //Sys?
	iListBoxData->SetColumnAlignmentL( 7, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 8, CW_Medium ); //Exit T.
	iListBoxData->SetColumnAlignmentL( 8, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 9, CW_Short ); //Exit R.
	iListBoxData->SetColumnAlignmentL( 9, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 10, CW_Short ); //Exit C.
	iListBoxData->SetColumnAlignmentL( 10, CGraphicsContext::ERight);

	TBuf<1000> titles(_L("Id\tName\tPriority\tProcess\tStack Sz\tHeap Sz\tProt?\tSys?\tExit T.\tExit R.\tExit C."));
	iListBoxItems->AppendL(titles);
}

void CResourceList::SetUpChunkTable()
{
	iListBoxData->SetColumnWidthPixelL( 0, CW_ChunkName );
	iListBoxData->SetColumnWidthPixelL( 1, CW_Medium );
	iListBoxData->SetColumnAlignmentL(1, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 2, CW_Medium + GapWidth );
	iListBoxData->SetColumnAlignmentL(2, CGraphicsContext::ERight);	
	iListBoxData->SetColumnHorizontalGapL( 2, GapWidth);
	iListBoxData->SetColumnWidthPixelL( 3, CW_Read );
	iListBoxData->SetColumnAlignmentL( 3, CGraphicsContext::ELeft);
	iListBoxData->SetColumnWidthPixelL( 4, CW_Writeable );
	iListBoxData->SetColumnAlignmentL( 4, CGraphicsContext::ELeft);

	TBuf<1000> titles(_L("Name\tSize\tMax Size\tRead-/\tWrite-able"));
	iListBoxItems->AppendL(titles);
}

void CResourceList::SetUpSemaphoreTable()
{
	iListBoxData->SetColumnWidthPixelL( 0, CW_SemName );
	iListBoxData->SetColumnWidthPixelL( 1, CW_Count );
	iListBoxData->SetColumnAlignmentL(1, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 2, CW_Count );
	iListBoxData->SetColumnAlignmentL(2, CGraphicsContext::ERight);

	TBuf<1000> titles(_L("Name\tHex Count\tDec Count"));
	iListBoxItems->AppendL(titles);
}

void CResourceList::SetUpMutexTable()
{
	iListBoxData->SetColumnWidthPixelL( 0, CW_MutexName );
	iListBoxData->SetColumnWidthPixelL( 1, CW_Count );
	iListBoxData->SetColumnAlignmentL(1, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 2, CW_Count );
	iListBoxData->SetColumnAlignmentL(2, CGraphicsContext::ERight);

	TBuf<1000> titles(_L("Name\tHex Count\tDec Count"));
	iListBoxItems->AppendL(titles);
}


void CResourceList::SetUpDriveTable()
{
	iListBoxData->SetColumnWidthPixelL( 0, CW_Short );
	iListBoxData->SetColumnWidthPixelL( 1, CW_Long );
	iListBoxData->SetColumnAlignmentL( 1, CGraphicsContext::ELeft);	
	iListBoxData->SetColumnHorizontalGapL( 1, GapWidth);
	iListBoxData->SetColumnWidthPixelL( 2, CW_Short + GapWidth );
	iListBoxData->SetColumnAlignmentL( 2, CGraphicsContext::ELeft);	
	iListBoxData->SetColumnHorizontalGapL(2, GapWidth);
	iListBoxData->SetColumnWidthPixelL( 3, CW_Media ); //Media
	iListBoxData->SetColumnAlignmentL( 3, CGraphicsContext::ELeft);
	iListBoxData->SetColumnWidthPixelL( 4, CW_DriveAtt ); //Attr.
	iListBoxData->SetColumnAlignmentL( 4, CGraphicsContext::ELeft);
	iListBoxData->SetColumnWidthPixelL( 5, CW_MemSize );
	iListBoxData->SetColumnAlignmentL(5, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 6, CW_MemSize );
	iListBoxData->SetColumnAlignmentL(6, CGraphicsContext::ERight);
	#if defined(WINS)
	iListBoxData->SetColumnWidthPixelL( 7, CW_Message );
	iListBoxData->SetColumnAlignmentL(7, CGraphicsContext::ELeft);
	#endif

	TBuf<1000> titles(_L("Drive\tVol. name\tNo.\tMedia\tAtt.\tSize (bytes)\tFree (bytes)"));
	#if defined(WINS)
	titles.Append(_L("\t(not retrievable in WINS)"));
	#endif
	iListBoxItems->AppendL(titles);
}

void CResourceList::SetUpHalTable()
{
	iListBoxData->SetColumnWidthPixelL( 0, CW_HalId );
	iListBoxData->SetColumnWidthPixelL( 1, CW_HalName );
	iListBoxData->SetColumnAlignmentL(1, CGraphicsContext::ELeft);
	iListBoxData->SetColumnWidthPixelL( 2, CW_Dynamic );
	iListBoxData->SetColumnAlignmentL( 2, CGraphicsContext::ECenter);
	iListBoxData->SetColumnWidthPixelL( 3, CW_MemSize );
	iListBoxData->SetColumnAlignmentL(3, CGraphicsContext::ERight);
	iListBoxData->SetColumnWidthPixelL( 4, CW_MemSize );
	iListBoxData->SetColumnAlignmentL(4, CGraphicsContext::ERight);

	TBuf<1000> titles(_L("No.\tName\tDynamic\tDec Value\tHex Value"));
	iListBoxItems->AppendL(titles);
}

void CResourceList::SetUpList()
{
	iListBoxData->SetColumnWidthPixelL( 0, 300 );
}
	
void CResourceList::UpdateListData()
	{
	CResourceList::EmptyList();

	switch (iObjectsViewed)
		{
		case EObjectProcess: 
			SetUpProcessTable();
			break;
		case EObjectThread: 
			SetUpThreadTable();
			break;
		case EObjectChunk: 
			SetUpChunkTable();
			break;
		case EObjectLibrary:  
			SetUpList();
			break;
		case EObjectServer: 
			SetUpList();
			break;
		case EObjectSemaphore:  
			SetUpSemaphoreTable(); 
			break;
		case EObjectMutex: 
			SetUpMutexTable();
			break;
		case EObjectLogicalDevice: 
			SetUpList();
			break;
		case EObjectPhysicalDevice: 
			SetUpList();
			break;
		case EObjectLogicalChannel: 
			SetUpList();
			break;
		case EObjectWindow: 
			SetUpList();
			break;
		case EObjectDrive:  
			SetUpDriveTable();
			break;
		case EObjectHal: 
			SetUpHalTable();
			break;
		default: break;
		}
	GetResourceList();
	}

void CResourceList::GetResourceList()
	{
	switch(iObjectsViewed)	
		{
		case EObjectProcess: 
			GetProcessList();
			break;
		case EObjectThread:
			GetThreadList();
			break;
		case EObjectChunk:
			GetChunkList();
			break;
		case EObjectLibrary:
			GetLibraryList();
			break;
		case EObjectServer:
			GetServerList();
			break;
		case EObjectSemaphore:
			GetSemaphoreList();
			break;
		case EObjectMutex:
			GetMutexList();
			break;
		case EObjectLogicalDevice:
			GetLogicalDeviceList();
			break;
		case EObjectPhysicalDevice:
			GetPhysicalDeviceList();
			break;
		case EObjectLogicalChannel:
			GetLogicalChannelList();
			break;
		case EObjectWindow:
			GetWindowList();
			break;
		case EObjectDrive:
			GetDriveList();
			break;
		case EObjectHal:
			GetHalList();
			break;
		default:
			break;
		}
	}

void CResourceList::GetProcessList(void)
	{
	TName prefix(KAsterisk);
	TFindProcess findHb;
	findHb.Find(prefix);
	TFullName name;
	
	TInt number=0;
	while (findHb.Next(name)==KErrNone)
		{
		number++;
		GetProcessData(name);
		AddRow( iProcessData);
		}
	UpdateResourceCount(number);
	}

void CResourceList::GetThreadList(void)
	{
	TName prefix(KAsterisk);
	TFindThread findHb;
	findHb.Find(prefix);
	TFullName name;
	TInt number=0;
	while (findHb.Next(name)==KErrNone)
		{
		number++;
		GetThreadData(name);
		AddRow( iThreadData);
		}
	UpdateResourceCount(number);
	}


void CResourceList::GetChunkList(void)
	{
	TName prefix(KAsterisk);
	TFindChunk findHb;
	findHb.Find(prefix);
	TFullName name;

	TInt number=0;
	while (findHb.Next(name)==KErrNone)
		{
		number++;
		GetChunkData(findHb, name);

		AddRow(iChunkData);
		}

	UpdateResourceCount(number);
	}

void CResourceList::GetLibraryList(void)
	{
	TName prefix(KAsterisk);
	TFindLibrary findHb;
	findHb.Find(prefix);
	TFullName name;
	TInt number=0;
	while (findHb.Next(name)==KErrNone)
		{
		number++;
		// Name
		iListData.iName.Copy(name);
		AddRow(iListData);
		}

	UpdateResourceCount(number);
	}

void CResourceList::GetServerList(void)
	{
	TName prefix(KAsterisk);
	TFindServer findHb;
	findHb.Find(prefix);
	TFullName name;
	TInt number=0;
	while (findHb.Next(name)==KErrNone)
		{
		number++;
		iListData.iName.Copy(name);
		AddRow(iListData);

		}

	UpdateResourceCount(number);
	}

void CResourceList::GetSemaphoreList(void)
	{
	TName prefix(KAsterisk);
	TFindSemaphore findHb;
	findHb.Find(prefix);
	TFullName name;

	TInt number=0;
	while (findHb.Next(name)==KErrNone)
		{
		number++;
		GetSemaphoreData(findHb, name);
		AddRow(iSemaphoreData);
		}

	UpdateResourceCount(number);
	}

void CResourceList::GetMutexList(void)
	{
	TName prefix(KAsterisk);
	TFindMutex findHb;
	findHb.Find(prefix);
	TFullName name;

	TInt number=0;
	while (findHb.Next(name)==KErrNone)
		{
		number++;
		GetMutexData(findHb, name);
		AddRow(iMutexData);
		}

	UpdateResourceCount(number);
	}

void CResourceList::GetLogicalDeviceList(void)
	{
	TName prefix(KAsterisk);
	TFindLogicalDevice findHb;
	findHb.Find(prefix);
	TFullName name;

	TInt number=0;
	while (findHb.Next(name)==KErrNone)
		{
		number++;
		iListData.iName.Copy(name);
		AddRow(iListData);
		}

	UpdateResourceCount(number);
	}

void CResourceList::GetPhysicalDeviceList(void)
	{
	TName prefix(KAsterisk);
	TFindPhysicalDevice findHb;
	findHb.Find(prefix);
	TFullName name;

	TInt number=0;
	while (findHb.Next(name)==KErrNone)
		{
		number++;
		iListData.iName.Copy(name);
		AddRow(iListData);

		}

	UpdateResourceCount(number);
	}

void CResourceList::GetLogicalChannelList(void)
	{
	UpdateResourceCount(0);
	}

void CResourceList::GetWindowList(void)
	{
	RWsSession ws;
	TInt err = ws.Connect();
	if (err == KErrNone)
		{

		TInt number = ws.NumWindowGroups(0);

		CArrayFixFlat<TInt>* wgIds = new(ELeave) CArrayFixFlat<TInt>(number);

		ws.WindowGroupList(0,wgIds);

		CApaWindowGroupName* wgName = CApaWindowGroupName::NewLC(ws);

		for (TInt i=0; i<number; ++i)
			{
			TInt wgId=(*wgIds)[i];

			wgName->ConstructFromWgIdL(wgId);
			iListData.iName.Copy(wgName->Caption());
			AddRow(iListData);

			}
		CleanupStack::PopAndDestroy(wgName);

		delete wgIds;
		wgIds = NULL;

		UpdateResourceCount(number);
		
		ws.Close();
		}
	}

void CResourceList::GetDriveList()
	{
	RFs fileServer;	
	User::LeaveIfError(fileServer.Connect());
	TInt err1,err2;
	TInt number=0;
	TInt driveNum=0;

	TDriveInfo driveInfo;
	TVolumeInfo volumeInfo;
	TDriveUnit driveUnit(EDriveA);


	do {
		driveUnit = driveNum;
		err1 = fileServer.Volume(volumeInfo,driveUnit);
		err2 = fileServer.Drive(driveInfo, driveUnit);
		if (err1==KErrNone && err2==KErrNone)
			{
			GetDriveData(driveNum, driveUnit, volumeInfo); //sets drive data to iDriveData
			AddRow(iDriveData);
			number++;
			}
		driveNum++;
		} while (driveNum <= EDriveZ);
	fileServer.Close();
	UpdateResourceCount(number);
	}

void CResourceList::GetHalList()
	{
	TInt number=0;

	HAL::SEntry* aSEntry = NULL;
	TInt numOfEntries, error;
	error = HAL::GetAll(numOfEntries, aSEntry);
		
	if (error ==KErrNone)
		{
		for (TInt iEntryNum=0; iEntryNum<numOfEntries; ++iEntryNum)
			{
			if (aSEntry[iEntryNum].iProperties & HAL::EEntryValid)
				{
				number++;
				GetHalData(iEntryNum, aSEntry);

				AddRow(iHalData);

				}
			}
		}

	UpdateResourceCount(number);
	User::Free(aSEntry);
	}

void CResourceList::AddRow(const TProcessData& aProcessData)
{
	// The actual item text format depends on the listbox type, see tables with listbox types
	TBuf<1000> item;
	item.Format(_L("%S\t%S\t%S\t%S\t%S\t%S\t%S\t%S\t%S\t%S\t%S\t%S\t%S\t"), &aProcessData.iIdString,
																				&aProcessData.iProcessName,
																				&aProcessData.iPriorityString,
																				&aProcessData.iProtectedIndicator,
																				&aProcessData.iSystemIndicator,
																				&aProcessData.iOwnerIdString,
																				&aProcessData.iExitTypeString,
																				&aProcessData.iExitReasonString,
																				&aProcessData.iExitCategoryName,
																				&aProcessData.iUidType0String,
																				&aProcessData.iUidType1String,
																				&aProcessData.iUidType2String,
																				&aProcessData.iFileName);

	iListBoxItems->AppendL(item);
	iListBox->HandleItemAdditionL();
	iListBox->DrawDeferred();
}

void CResourceList::AddRow(const TChunkData& aChunkData)
{
	// The actual item text format depends on the listbox type, see tables with listbox types
	TBuf<1000> item;
	item.Format(_L("%S\t%S\t%S\t%S\t%S"), &aChunkData.iChunkName,
										  &aChunkData.iSizeString,
										  &aChunkData.iMaxSizeString,
										  &aChunkData.iReadableIndicator,
										  &aChunkData.iWritableIndicator);

	iListBoxItems->AppendL(item);
	iListBox->HandleItemAdditionL();
	iListBox->DrawDeferred();
}

void CResourceList::AddRow(const TThreadData& aThreadData)
{
	// The actual item text format depends on the listbox type, see tables with listbox types
	TBuf<1000> item;
	item.Format(_L("%S\t%S\t%S\t%S\t%S\t%S\t%S\t%S\t%S\t%S\t%S\t"), &aThreadData.iIdString,
																	&aThreadData.iThreadName,
																	&aThreadData.iPriorityString,
																	&aThreadData.iOwnerIdString,
																	&aThreadData.iStackSizeString,
																	&aThreadData.iHeapSizeString,
																	&aThreadData.iProtectedIndicator,
																	&aThreadData.iSystemIndicator,
																	&aThreadData.iExitTypeString,
																	&aThreadData.iExitReasonString,
																	&aThreadData.iExitCategoryName);

	iListBoxItems->AppendL(item);
	iListBox->HandleItemAdditionL(); 
	iListBox->DrawDeferred();
}

void CResourceList::AddRow(const TSemaphoreData& aSemaphoreData)
{
	// The actual item text format depends on the listbox type, see tables with listbox types
	TBuf<1000> item;
	item.Format(_L("%S\t%S\t%S\t"), &aSemaphoreData.iSemaphoreName,
									&aSemaphoreData.iHexCountString,
									&aSemaphoreData.iDecCountString);

	iListBoxItems->AppendL(item);
	iListBox->HandleItemAdditionL();
	iListBox->DrawDeferred();
}

void CResourceList::AddRow(const TMutexData& aMutexData)
{
	// The actual item text format depends on the listbox type, see tables with listbox types
	TBuf<1000> item;
	item.Format(_L("%S\t%S\t%S\t"), &aMutexData.iMutexName,
									&aMutexData.iHexCountString,
									&aMutexData.iDecCountString);

	iListBoxItems->AppendL(item);
	iListBox->HandleItemAdditionL();
	iListBox->DrawDeferred();
}

void CResourceList::AddRow(const TDriveData& aDriveData)
{
	// The actual item text format depends on the listbox type, see tables with listbox types
	TBuf<1000> item;
	item.Format(_L("%S\t%S\t%S\t%S\t%S\t%S\t%S\t"), &aDriveData.iDriveName,
												&aDriveData.iVolumeName,
												&aDriveData.iDriveNumberString,
												&aDriveData.iMediaString,
												&aDriveData.iAttributesString,
												&aDriveData.iSpaceString,
												&aDriveData.iFreeSpaceString);

	iListBoxItems->AppendL(item);
	iListBox->HandleItemAdditionL();
	iListBox->DrawDeferred();
}

void CResourceList::AddRow(const THalData& aHalData)
{
	// The actual item text format depends on the listbox type, see tables with listbox types
	TBuf<1000> item;
	item.Format(_L("%S\t%S\t%S\t%S\t%S\t"), &aHalData.iNumberString,
												&aHalData.iName,
												&aHalData.iDynamicIndicator,
												&aHalData.iDecValString,
												&aHalData.iHexValString);

	iListBoxItems->AppendL(item);
	iListBox->HandleItemAdditionL();
	iListBox->DrawDeferred();
}

void CResourceList::AddRow(const TListData& aListData)
{
	// The actual item text format depends on the listbox type, see tables with listbox types
	TBuf<1000> item;
	item.Format(_L("%S"), &aListData.iName);

	iListBoxItems->AppendL(item);
	iListBox->HandleItemAdditionL();
	iListBox->DrawDeferred();
}
