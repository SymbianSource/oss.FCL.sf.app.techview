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
// uniload.cpp - universal file load (work for narrow or wide builds)
//

#include "uniload.h"

HBufC *UnicodeLoad::LoadL(RFs &aFs, const TDesC &aName)
	{
	RFile pf;
	TInt size;
	User::LeaveIfError(pf.Open(aFs, aName, EFileShareAny));
	pf.Size(size);
	//
	// For narrow build, this allocates exactly right amount
	// of space, for Unicode build, this allocates the size
	// doubled in bytes, which is exactly what we want, if
	// we need to widen the narrow Ascii control file (this
	// is assumed to be the default case). For UNICODE file
	// in UNICODE environment, this allocates too much space.
	//
	HBufC *data = HBufC::New(size);

	if (data == NULL)
		{
		pf.Close();
		User::Leave(KErrNoMemory);
		}
	//
	// buf is "native" pointer, either 16 or 8 wide
	//
	TPtr buf(data->Des());
#ifdef __WINS__
	buf.Set(data->Des());		// VC6 kludge
#endif
	//
	// ptr is always 8 wide
	//
	TPtr8 ptr((TUint8 *)buf.Ptr(), size, size);
	pf.Read(ptr);
	pf.Close();

	if (size >= 2 && ptr[0] == 0xFF && ptr[1] == 0xFE)
		{
		//
		// The file is presented as UNICODE text
		// (for now, only one byte order supported)
		//
		size -= 2;
#ifdef _UNICODE
		Mem::Copy((TUint8 *)ptr.Ptr(), ptr.Ptr() + 2, size);
		buf.SetLength(size / 2);	// True number of Unicode characters.
#else
		// Trying to load a Unicode file for Narrow build
		size = size / 2;
		for (TInt i = 1; i < size; ++i)
			{
			if (ptr[2*i] != 0)
				ptr[i] = 0x01;	// Substitute too wide stuff with 0x1
			else
				ptr[i] = ptr[2*i + 1];
			}
		buf.SetLength(size); 
#endif
		}
	else
		{
		//
		// The file is narrow, 8 bit characters
		//
		buf.SetLength(size); 
#ifdef _UNICODE
		// Must "widen" the characters for unicode.
		for (TInt i = size; --i >= 0; )
			buf[i] = ptr[i];
#endif
		}
	return data;
	}
