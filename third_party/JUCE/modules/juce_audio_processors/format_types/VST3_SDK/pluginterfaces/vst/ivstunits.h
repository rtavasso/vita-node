//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Interfaces
// Filename    : pluginterfaces/vst/ivstunits.h
// Created by  : Steinberg, 2005
// Description : VST Units Interfaces
//
//-----------------------------------------------------------------------------
// This file is part of a Steinberg SDK. It is subject to the license terms
// in the LICENSE file found in the top-level directory of this distribution
// and at www.steinberg.net/sdklicenses. 
// No part of the SDK, including this file, may be copied, modified, propagated,
// or distributed except according to the terms contained in the LICENSE file.
//-----------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "vsttypes.h"

//------------------------------------------------------------------------
#include "pluginterfaces/base/falignpush.h"
//------------------------------------------------------------------------

namespace Steinberg {
class IBStream;

//------------------------------------------------------------------------
namespace Vst {
//------------------------------------------------------------------------
/** Special UnitIDs for UnitInfo */
static const UnitID kRootUnitId	= 0;        ///< identifier for the top level unit (root)
static const UnitID kNoParentUnitId = -1;	///< used for the root unit which doesn't have a parent.

//------------------------------------------------------------------------
/** Special ProgramListIDs for UnitInfo */
static const ProgramListID kNoProgramListId = -1;	///< no programs are used in the unit.

//------------------------------------------------------------------------
/** Basic Unit Description.
\see IUnitInfo */
//------------------------------------------------------------------------
struct UnitInfo
{
	UnitID id;						///< unit identifier
	UnitID parentUnitId;			///< identifier of parent unit (kNoParentUnitId: does not apply, this unit is the root)
	String128 name;					///< name, optional for the root component, required otherwise
	ProgramListID programListId;	///< id of program list used in unit (kNoProgramListId = no programs used in this unit)
};

//------------------------------------------------------------------------
/** Basic Program List Description.
\see IUnitInfo */
//------------------------------------------------------------------------
struct ProgramListInfo
{
	ProgramListID id;				///< program list identifier
	String128 name;					///< name of program list
	int32 programCount;				///< number of programs in this list
};

//------------------------------------------------------------------------
/** Special programIndex value for IUnitHandler::notifyProgramListChange */
static const int32 kAllProgramInvalid =	-1;		///< all program information is invalid

//------------------------------------------------------------------------
/** Host callback for unit support.
\ingroup vstIHost vst300
- [host imp]
- [extends IComponentHandler]
- [released: 3.0.0]
- [optional]

Host callback interface, used with IUnitInfo.
Retrieve via queryInterface from IComponentHandler.

\see \ref vst3Units, IUnitInfo */
//------------------------------------------------------------------------
class IUnitHandler: public FUnknown
{
public:
//------------------------------------------------------------------------
	/** Notify host when a module is selected in Plug-in GUI. */
	virtual tresult PLUGIN_API notifyUnitSelection (UnitID unitId) = 0;

	/** Tell host that the Plug-in controller changed a program list (rename, load, PitchName changes).
	    \param listId is the specified program list ID to inform.
		\param programIndex : when kAllProgramInvalid, all program information is invalid, otherwise only the program of given index. */
	virtual tresult PLUGIN_API notifyProgramListChange (ProgramListID listId, int32 programIndex) = 0;

//------------------------------------------------------------------------
	static const FUID iid;
};

DECLARE_CLASS_IID (IUnitHandler, 0x4B5147F8, 0x4654486B, 0x8DAB30BA, 0x163A3C56)


//------------------------------------------------------------------------
/** Host callback for extended unit support.
\ingroup vstIHost vst365
- [host imp]
- [extends IUnitHandler]
- [released: 3.6.5]
- [optional]

Host callback interface, used with IUnitInfo.
Retrieve via queryInterface from IComponentHandler.

\see \ref vst3Units, IUnitHandler */
//------------------------------------------------------------------------
class IUnitHandler2 : public FUnknown
{
public:
	//------------------------------------------------------------------------
	/** Tell host that assignment Unit-Bus defined by IUnitInfo::getUnitByBus has changed. */
	virtual tresult PLUGIN_API notifyUnitByBusChange () = 0;

	//------------------------------------------------------------------------
	static const FUID iid;
};

DECLARE_CLASS_IID (IUnitHandler2, 0xF89F8CDF, 0x699E4BA5, 0x96AAC9A4, 0x81452B01)


//------------------------------------------------------------------------
/** Edit controller extension to describe the Plug-in structure.
\ingroup vstIPlug vst300
- [plug imp]
- [extends IEditController]
- [released: 3.0.0]
- [optional]

IUnitInfo describes the internal structure of the Plug-in.
- The root unit is the component itself, so getUnitCount must return 1 at least.
- The root unit id has to be 0 (kRootUnitId).
- Each unit can reference one program list - this reference must not change.
- Each unit using a program list, references one program of the list.

\see \ref vst3Units, IUnitHandler */
//------------------------------------------------------------------------
class IUnitInfo: public FUnknown
{
public:
//------------------------------------------------------------------------
	/** Returns the flat count of units. */
	virtual int32 PLUGIN_API getUnitCount () = 0;

	/** Gets UnitInfo for a given index in the flat list of unit. */
	virtual tresult PLUGIN_API getUnitInfo (int32 unitIndex, UnitInfo& info /*out*/) = 0;

	/** Component intern program structure. */
	/** Gets the count of Program List. */
	virtual int32 PLUGIN_API getProgramListCount () = 0;

	/** Gets for a given index the Program List Info. */
	virtual tresult PLUGIN_API getProgramListInfo (int32 listIndex, ProgramListInfo& info /*out*/) = 0;

	/** Gets for a given program list ID and program index its program name. */
	virtual tresult PLUGIN_API getProgramName (ProgramListID listId, int32 programIndex, String128 name /*out*/) = 0;

	/** Gets for a given program list ID, program index and attributeId the associated attribute value. */
	virtual tresult PLUGIN_API getProgramInfo (ProgramListID listId, int32 programIndex,
		CString attributeId /*in*/, String128 attributeValue /*out*/) = 0;

	/** Returns kResultTrue if the given program index of a given program list ID supports PitchNames. */
	virtual tresult PLUGIN_API hasProgramPitchNames (ProgramListID listId, int32 programIndex) = 0;

	/** Gets the PitchName for a given program list ID, program index and pitch.
		If PitchNames are changed the Plug-in should inform the host with IUnitHandler::notifyProgramListChange. */
	virtual tresult PLUGIN_API getProgramPitchName (ProgramListID listId, int32 programIndex,
		int16 midiPitch, String128 name /*out*/) = 0;

	// units selection --------------------
	/** Gets the current selected unit. */
	virtual UnitID PLUGIN_API getSelectedUnit () = 0;

	/** Sets a new selected unit. */
	virtual tresult PLUGIN_API selectUnit (UnitID unitId) = 0;

	/** Gets the according unit if there is an unambiguous relation between a channel or a bus and a unit.
	    This method mainly is intended to find out which unit is related to a given MIDI input channel. */
	virtual tresult PLUGIN_API getUnitByBus (MediaType type, BusDirection dir, int32 busIndex,
		int32 channel, UnitID& unitId /*out*/) = 0;

	/** Receives a preset data stream.
	    - If the component supports program list data (IProgramListData), the destination of the data
		  stream is the program specified by list-Id and program index (first and second parameter)
		- If the component supports unit data (IUnitData), the destination is the unit specified by the first
		  parameter - in this case parameter programIndex is < 0). */
	virtual tresult PLUGIN_API setUnitProgramData (int32 listOrUnitId, int32 programIndex, IBStream* data) = 0;

//------------------------------------------------------------------------
	static const FUID iid;
};

DECLARE_CLASS_IID (IUnitInfo, 0x3D4BD6B5, 0x913A4FD2, 0xA886E768, 0xA5EB92C1)

//------------------------------------------------------------------------
/** Component extension to access program list data.
\ingroup vstIPlug vst300
- [plug imp]
- [extends IComponent]
- [released: 3.0.0]
- [optional]

A component can either support program list data via this interface or
unit preset data (IUnitData), but not both!

\see \ref vst3UnitPrograms */
//------------------------------------------------------------------------
class IProgramListData: public FUnknown
{
public:
//------------------------------------------------------------------------
	/** Returns kResultTrue if the given Program List ID supports Program Data. */
	virtual tresult PLUGIN_API programDataSupported (ProgramListID listId) = 0;

	/** Gets for a given program list ID and program index the program Data. */
	virtual tresult PLUGIN_API getProgramData (ProgramListID listId, int32 programIndex, IBStream* data) = 0;

	/** Sets for a given program list ID and program index a program Data. */
	virtual tresult PLUGIN_API setProgramData (ProgramListID listId, int32 programIndex, IBStream* data) = 0;

//------------------------------------------------------------------------
	static const FUID iid;
};

DECLARE_CLASS_IID (IProgramListData, 0x8683B01F, 0x7B354F70, 0xA2651DEC, 0x353AF4FF)

//------------------------------------------------------------------------
/** Component extension to access unit data.
\ingroup vstIPlug vst300
- [plug imp]
- [extends IComponent]
- [released: 3.0.0]
- [optional]

A component can either support unit preset data via this interface or
program list data (IProgramListData), but not both!

\see \ref vst3UnitPrograms */
//------------------------------------------------------------------------
class IUnitData: public FUnknown
{
public:
//------------------------------------------------------------------------
	/** Returns kResultTrue if the specified unit supports export and import of preset data. */
	virtual tresult PLUGIN_API unitDataSupported (UnitID unitID) = 0;

	/** Gets the preset data for the specified unit. */
	virtual tresult PLUGIN_API getUnitData (UnitID unitId, IBStream* data) = 0;

	/** Sets the preset data for the specified unit. */
	virtual tresult PLUGIN_API setUnitData (UnitID unitId, IBStream* data) = 0;

//------------------------------------------------------------------------
	static const FUID iid;
};

DECLARE_CLASS_IID (IUnitData, 0x6C389611, 0xD391455D, 0xB870B833, 0x94A0EFDD)

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg

//------------------------------------------------------------------------
#include "pluginterfaces/base/falignpop.h"
//------------------------------------------------------------------------
