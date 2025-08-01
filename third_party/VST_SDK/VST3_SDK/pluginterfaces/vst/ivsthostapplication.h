//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Interfaces
// Filename    : pluginterfaces/vst/ivsthostapplication.h
// Created by  : Steinberg, 04/2006
// Description : VST Host Interface
//
//-----------------------------------------------------------------------------
// This file is part of a Steinberg SDK. It is subject to the license terms
// in the LICENSE file found in the top-level directory of this distribution
// and at www.steinberg.net/sdklicenses. 
// No part of the SDK, including this file, may be copied, modified, propagated,
// or distributed except according to the terms contained in the LICENSE file.
//-----------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/vst/ivstmessage.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
//------------------------------------------------------------------------
/** Basic Host Callback Interface.
\ingroup vstIHost vst300
- [host imp]
- [passed as 'context' in to IPluginBase::initialize () ]
- [released: 3.0.0]

Basic VST host application interface. */
//------------------------------------------------------------------------
class IHostApplication: public FUnknown
{
public:
//------------------------------------------------------------------------
	/** Gets host application name. */
	virtual tresult PLUGIN_API getName (String128 name) = 0;

	/** Creates host object (e.g. Vst::IMessage). */
	virtual tresult PLUGIN_API createInstance (TUID cid, TUID _iid, void** obj) = 0;

//------------------------------------------------------------------------
	static const FUID iid;
};

DECLARE_CLASS_IID (IHostApplication, 0x58E595CC, 0xDB2D4969, 0x8B6AAF8C, 0x36A664E5)

//------------------------------------------------------------------------
inline IMessage* allocateMessage (IHostApplication* host)
{
	TUID iid;
	IMessage::iid.toTUID (iid);
	IMessage* m = 0;
	if (host->createInstance (iid, iid, (void**)&m) == kResultOk)
		return m;
	return 0;
}

//------------------------------------------------------------------------
/** VST 3 to VST 2 Wrapper Interface.
\ingroup vstIHost vst310
- [host imp]
- [passed as 'context' to IPluginBase::initialize () ]
- [released: 3.1.0]

Informs the Plug-in that a VST 3 to VST 2 wrapper is used between the Plug-in and the real host.
Implemented by the VST 2 Wrapper. */
//------------------------------------------------------------------------
class IVst3ToVst2Wrapper: public FUnknown
{
public:
	//------------------------------------------------------------------------
	static const FUID iid;
};

DECLARE_CLASS_IID (IVst3ToVst2Wrapper, 0x29633AEC, 0x1D1C47E2, 0xBB85B97B, 0xD36EAC61)

//------------------------------------------------------------------------
/** VST 3 to AU Wrapper Interface.
\ingroup vstIHost vst310
- [host imp]
- [passed as 'context' to IPluginBase::initialize () ]
- [released: 3.1.0]

Informs the Plug-in that a VST 3 to AU wrapper is used between the Plug-in and the real host.
Implemented by the AU Wrapper. */
//------------------------------------------------------------------------
class IVst3ToAUWrapper: public FUnknown
{
public:
	//------------------------------------------------------------------------
	static const FUID iid;
};

DECLARE_CLASS_IID (IVst3ToAUWrapper, 0xA3B8C6C5, 0xC0954688, 0xB0916F0B, 0xB697AA44)

//------------------------------------------------------------------------
/** VST 3 to AAX Wrapper Interface.
\ingroup vstIHost vst368
- [host imp]
- [passed as 'context' to IPluginBase::initialize () ]
- [released: 3.6.8]

Informs the Plug-in that a VST 3 to AAX wrapper is used between the Plug-in and the real host.
Implemented by the AAX Wrapper. */
//------------------------------------------------------------------------
class IVst3ToAAXWrapper : public FUnknown
{
public:
	//------------------------------------------------------------------------
	static const FUID iid;
};
DECLARE_CLASS_IID (IVst3ToAAXWrapper, 0x6D319DC6, 0x60C56242, 0xB32C951B, 0x93BEF4C6)

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
