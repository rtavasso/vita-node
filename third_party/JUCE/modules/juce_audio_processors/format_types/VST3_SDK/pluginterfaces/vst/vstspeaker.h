//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Interfaces
// Filename    : pluginterfaces/vst/vstspeaker.h
// Created by  : Steinberg, 01/2018
// Description : common defines
//
//-----------------------------------------------------------------------------
// This file is part of a Steinberg SDK. It is subject to the license terms
// in the LICENSE file found in the top-level directory of this distribution
// and at www.steinberg.net/sdklicenses. 
// No part of the SDK, including this file, may be copied, modified, propagated,
// or distributed except according to the terms contained in the LICENSE file.
//-----------------------------------------------------------------------------

#pragma once
#include "vsttypes.h"

//#include "pluginterfaces/base/ftype.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
//------------------------------------------------------------------------
/** \defgroup speakerArrangements Speaker Arrangements
\image html "vst3_speaker_types.jpg"
\n
A SpeakerArrangement is a bitset combination of speakers. For example:
\code
const SpeakerArrangement kStereo = kSpeakerL | kSpeakerR; // => hex: 0x03 / binary: 0011.
\endcode*/
//------------------------------------------------------------------------

//------------------------------------------------------------------------
/** Speaker Definitions.
\ingroup speakerArrangements */
//------------------------------------------------------------------------
/**@{*/
const Speaker kSpeakerL    = 1 << 0;		///< Left (L)
const Speaker kSpeakerR    = 1 << 1;		///< Right (R)
const Speaker kSpeakerC    = 1 << 2;		///< Center (C)
const Speaker kSpeakerLfe  = 1 << 3;		///< Subbass (Lfe)
const Speaker kSpeakerLs   = 1 << 4;		///< Left Surround (Ls)
const Speaker kSpeakerRs   = 1 << 5;		///< Right Surround (Rs)
const Speaker kSpeakerLc   = 1 << 6;		///< Left of Center (Lc) - Front Left Center
const Speaker kSpeakerRc   = 1 << 7;		///< Right of Center (Rc) - Front Right Center
const Speaker kSpeakerS    = 1 << 8;		///< Surround (S)
const Speaker kSpeakerCs   = kSpeakerS;		///< Center of Surround (Cs) - Back Center - Surround (S)
const Speaker kSpeakerSl   = 1 << 9;		///< Side Left (Sl)
const Speaker kSpeakerSr   = 1 << 10;		///< Side Right (Sr)
const Speaker kSpeakerTc   = 1 << 11;		///< Top Center Over-head, Top Middle (Tc)
const Speaker kSpeakerTfl  = 1 << 12;		///< Top Front Left (Tfl)
const Speaker kSpeakerTfc  = 1 << 13;		///< Top Front Center (Tfc)
const Speaker kSpeakerTfr  = 1 << 14;		///< Top Front Right (Tfr)
const Speaker kSpeakerTrl  = 1 << 15;		///< Top Rear/Back Left (Trl)
const Speaker kSpeakerTrc  = 1 << 16;		///< Top Rear/Back Center (Trc)
const Speaker kSpeakerTrr  = 1 << 17;		///< Top Rear/Back Right (Trr)
const Speaker kSpeakerLfe2 = 1 << 18;		///< Subbass 2 (Lfe2)
const Speaker kSpeakerM    = 1 << 19;		///< Mono (M)

const Speaker kSpeakerACN0  = (Speaker)1 << 20;	///< Ambisonic ACN 0
const Speaker kSpeakerACN1  = (Speaker)1 << 21;	///< Ambisonic ACN 1
const Speaker kSpeakerACN2  = (Speaker)1 << 22;	///< Ambisonic ACN 2
const Speaker kSpeakerACN3  = (Speaker)1 << 23;	///< Ambisonic ACN 3
const Speaker kSpeakerACN4  = (Speaker)1 << 38;	///< Ambisonic ACN 4
const Speaker kSpeakerACN5  = (Speaker)1 << 39;	///< Ambisonic ACN 5
const Speaker kSpeakerACN6  = (Speaker)1 << 40;	///< Ambisonic ACN 6
const Speaker kSpeakerACN7  = (Speaker)1 << 41;	///< Ambisonic ACN 7
const Speaker kSpeakerACN8  = (Speaker)1 << 42;	///< Ambisonic ACN 8
const Speaker kSpeakerACN9  = (Speaker)1 << 43;	///< Ambisonic ACN 9
const Speaker kSpeakerACN10 = (Speaker)1 << 44;	///< Ambisonic ACN 10
const Speaker kSpeakerACN11 = (Speaker)1 << 45;	///< Ambisonic ACN 11
const Speaker kSpeakerACN12 = (Speaker)1 << 46;	///< Ambisonic ACN 12
const Speaker kSpeakerACN13 = (Speaker)1 << 47;	///< Ambisonic ACN 13
const Speaker kSpeakerACN14 = (Speaker)1 << 48;	///< Ambisonic ACN 14
const Speaker kSpeakerACN15 = (Speaker)1 << 49;	///< Ambisonic ACN 15

const Speaker kSpeakerTsl = 1 << 24;		///< Top Side Left (Tsl)
const Speaker kSpeakerTsr = 1 << 25;		///< Top Side Right (Tsr)
const Speaker kSpeakerLcs = 1 << 26;		///< Left of Center Surround (Lcs) - Back Left Center
const Speaker kSpeakerRcs = 1 << 27;		///< Right of Center Surround (Rcs) - Back Right Center

const Speaker kSpeakerBfl = 1 << 28;		///< Bottom Front Left (Bfl)
const Speaker kSpeakerBfc = 1 << 29;		///< Bottom Front Center (Bfc)
const Speaker kSpeakerBfr = 1 << 30;		///< Bottom Front Right (Bfr)

const Speaker kSpeakerPl  = (Speaker)1 << 31;	///< Proximity Left (Pl)
const Speaker kSpeakerPr  = (Speaker)1 << 32;	///< Proximity Right (Pr)

const Speaker kSpeakerBsl = (Speaker)1 << 33;	///< Bottom Side Left (Bsl)
const Speaker kSpeakerBsr = (Speaker)1 << 34;	///< Bottom Side Right (Bsr)
const Speaker kSpeakerBrl = (Speaker)1 << 35;	///< Bottom Rear Left (Brl)
const Speaker kSpeakerBrc = (Speaker)1 << 36;	///< Bottom Rear Center (Brc)
const Speaker kSpeakerBrr = (Speaker)1 << 37;	///< Bottom Rear Right (Brr)
//------------------------------------------------------------------------
/** @}*/

//------------------------------------------------------------------------
/** Speaker Arrangement Definitions (SpeakerArrangement)*/
//------------------------------------------------------------------------
namespace SpeakerArr
{
//------------------------------------------------------------------------
/** Speaker Arrangement Definitions.
\ingroup speakerArrangements */
/*@{*/
const SpeakerArrangement kEmpty			 = 0;          ///< empty arrangement
const SpeakerArrangement kMono			 = kSpeakerM;  ///< M
const SpeakerArrangement kStereo		 = kSpeakerL   | kSpeakerR;    ///< L R
const SpeakerArrangement kStereoSurround = kSpeakerLs  | kSpeakerRs;   ///< Ls Rs
const SpeakerArrangement kStereoCenter	 = kSpeakerLc  | kSpeakerRc;   ///< Lc Rc
const SpeakerArrangement kStereoSide	 = kSpeakerSl  | kSpeakerSr;   ///< Sl Sr
const SpeakerArrangement kStereoCLfe	 = kSpeakerC   | kSpeakerLfe;  ///< C Lfe
const SpeakerArrangement kStereoTF		 = kSpeakerTfl | kSpeakerTfr;  ///< Tfl Tfr
const SpeakerArrangement kStereoTS		 = kSpeakerTsl | kSpeakerTsr;  ///< Tsl Tsr
const SpeakerArrangement kStereoTR		 = kSpeakerTrl | kSpeakerTrr;  ///< Trl Trr
const SpeakerArrangement kStereoBF		 = kSpeakerBfl | kSpeakerBfr;  ///< Bfl Bfr

/** L R C */
const SpeakerArrangement k30Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC;
/** L R S */
const SpeakerArrangement k30Music		 = kSpeakerL  | kSpeakerR | kSpeakerS;
/** L R C   Lfe */
const SpeakerArrangement k31Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe;
/** L R Lfe S */
const SpeakerArrangement k31Music		 = kSpeakerL  | kSpeakerR | kSpeakerLfe | kSpeakerS;
/** L R C   S (LCRS) */
const SpeakerArrangement k40Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerS;
/** L R Ls  Rs (Quadro) */
const SpeakerArrangement k40Music		 = kSpeakerL  | kSpeakerR | kSpeakerLs  | kSpeakerRs;
/** L R C   Lfe S (LCRS+Lfe) */
const SpeakerArrangement k41Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerS;
/** L R Lfe Ls Rs (Quadro+Lfe) */
const SpeakerArrangement k41Music		 = kSpeakerL  | kSpeakerR | kSpeakerLfe | kSpeakerLs  | kSpeakerRs;
/** L R C   Ls Rs */
const SpeakerArrangement k50			 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLs  | kSpeakerRs;
/** L R C  Lfe Ls Rs */
const SpeakerArrangement k51			 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerLs | kSpeakerRs;
/** L R C  Ls  Rs Cs */
const SpeakerArrangement k60Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLs  | kSpeakerRs | kSpeakerCs;
/** L R Ls Rs  Sl Sr */
const SpeakerArrangement k60Music		 = kSpeakerL  | kSpeakerR | kSpeakerLs  | kSpeakerRs  | kSpeakerSl | kSpeakerSr;
/** L R C  Lfe Ls Rs Cs */
const SpeakerArrangement k61Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerCs;
/** L R Lfe Ls  Rs Sl Sr */
const SpeakerArrangement k61Music		 = kSpeakerL  | kSpeakerR | kSpeakerLfe | kSpeakerLs  | kSpeakerRs | kSpeakerSl | kSpeakerSr;
/** L R C   Ls  Rs Lc Rc */
const SpeakerArrangement k70Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLs  | kSpeakerRs | kSpeakerLc | kSpeakerRc;
/** L R C   Ls  Rs Sl Sr */
const SpeakerArrangement k70Music		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLs  | kSpeakerRs | kSpeakerSl | kSpeakerSr;
/** L R C Lfe Ls Rs Lc Rc */
const SpeakerArrangement k71Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerLc | kSpeakerRc;
const SpeakerArrangement k71CineFullFront= k71Cine;
/** L R C Lfe Ls Rs Lcs Rcs */
const SpeakerArrangement k71CineFullRear = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerLcs | kSpeakerRcs;
/** L R C Lfe Ls Rs Sl Sr */
const SpeakerArrangement k71Music		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerSl | kSpeakerSr;
const SpeakerArrangement k71CineSideFill = k71Music;
/** L R C Lfe Ls Rs Pl Pr */
const SpeakerArrangement k71Proximity	 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerPl | kSpeakerPr;
/** L R C Ls  Rs Lc Rc Cs */
const SpeakerArrangement k80Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLs  | kSpeakerRs | kSpeakerLc | kSpeakerRc | kSpeakerCs;
/** L R C Ls  Rs Cs Sl Sr */
const SpeakerArrangement k80Music		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLs  | kSpeakerRs | kSpeakerCs | kSpeakerSl | kSpeakerSr;
/** L R C Lfe Ls Rs Lc Rc Cs */
const SpeakerArrangement k81Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerLc | kSpeakerRc | kSpeakerCs;
/** L R C Lfe Ls Rs Cs Sl Sr */
const SpeakerArrangement k81Music		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerCs | kSpeakerSl | kSpeakerSr;


/** First-Order with Ambisonic Channel Number (ACN) ordering and SN3D normalization */
const SpeakerArrangement kAmbi1stOrderACN = kSpeakerACN0 | kSpeakerACN1 | kSpeakerACN2 | kSpeakerACN3;
/** Second-Order with Ambisonic Channel Number (ACN) ordering and SN3D normalization */
const SpeakerArrangement kAmbi2cdOrderACN = kAmbi1stOrderACN | kSpeakerACN4 | kSpeakerACN5 | kSpeakerACN6 | kSpeakerACN7 | kSpeakerACN8;
/** Third-Order with Ambisonic Channel Number (ACN) ordering and SN3D normalization */
const SpeakerArrangement kAmbi3rdOrderACN = kAmbi2cdOrderACN | kSpeakerACN9 | kSpeakerACN10 | kSpeakerACN11 | kSpeakerACN12 | kSpeakerACN13 | kSpeakerACN14 | kSpeakerACN15;


/*-----------*/
/* 3D formats */
/*-----------*/
/** L R Ls Rs Tfl Tfr Trl Trr */
const SpeakerArrangement k80Cube		   = kSpeakerL | kSpeakerR | kSpeakerLs | kSpeakerRs  | kSpeakerTfl| kSpeakerTfr| kSpeakerTrl | kSpeakerTrr;
/** L R C Lfe Ls Rs Cs Tc */
const SpeakerArrangement k71CineTopCenter  = kSpeakerL | kSpeakerR | kSpeakerC  | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerCs  | kSpeakerTc; 
/** L R C Lfe Ls Rs Cs Tfc */
const SpeakerArrangement k71CineCenterHigh = kSpeakerL | kSpeakerR | kSpeakerC  | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerCs  | kSpeakerTfc; 
/** L R C Lfe Ls Rs Tfl Tfr */
const SpeakerArrangement k71CineFrontHigh  = kSpeakerL | kSpeakerR | kSpeakerC  | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerTfl | kSpeakerTfr; 
const SpeakerArrangement k71MPEG3D		   = k71CineFrontHigh;
/** L R C Lfe Ls Rs Tsl Tsr */
const SpeakerArrangement k71CineSideHigh   = kSpeakerL | kSpeakerR | kSpeakerC  | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerTsl | kSpeakerTsr; 

/** L R Lfe Ls Rs Tfl Tfc Tfr Bfc */
const SpeakerArrangement k81MPEG3D		 = kSpeakerL | kSpeakerR | kSpeakerLfe | kSpeakerLs | kSpeakerRs |
										   kSpeakerTfl | kSpeakerTfc | kSpeakerTfr | kSpeakerBfc;

/** L R C Ls Rs Tfl Tfr Trl Trr */
const SpeakerArrangement k90			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLs  | kSpeakerRs |
										   kSpeakerTfl| kSpeakerTfr | kSpeakerTrl | kSpeakerTrr;
const SpeakerArrangement k50_4			 = k90;
/** L R C Lfe Ls Rs Tfl Tfr Trl Trr */
const SpeakerArrangement k91			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLfe | kSpeakerLs | kSpeakerRs  |
										   kSpeakerTfl| kSpeakerTfr | kSpeakerTrl | kSpeakerTrr;
const SpeakerArrangement k51_4			 = k91;

/** L R C Lfe Ls Rs Sl Sr Tsl Tsr */
const SpeakerArrangement k71_2			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLfe | kSpeakerLs |
										   kSpeakerRs | kSpeakerSl | kSpeakerSr | kSpeakerTsl | kSpeakerTsr;
const SpeakerArrangement k91Atmos		 = k71_2;

/** L R C Ls Rs Tc Tfl Tfr Trl Trr */
const SpeakerArrangement k100			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLs  | kSpeakerRs | 
										   kSpeakerTc | kSpeakerTfl | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr;
/** L R C Lfe Ls Rs Tc Tfl Tfr Trl Trr */
const SpeakerArrangement k101			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLfe | kSpeakerLs | kSpeakerRs  | 
										   kSpeakerTc | kSpeakerTfl | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr;
const SpeakerArrangement k101MPEG3D		 = k101;

/** L R C Lfe Ls Rs Tfl Tfc Tfr Trl Trr Lfe2 */
const SpeakerArrangement k102			 = kSpeakerL  | kSpeakerR | kSpeakerC  | kSpeakerLfe | kSpeakerLs | kSpeakerRs  |
										   kSpeakerTfl| kSpeakerTfc | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr | kSpeakerLfe2;

/** L R C Ls Rs Tc Tfl Tfc Tfr Trl Trr */
const SpeakerArrangement k110			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLs  | kSpeakerRs |
										   kSpeakerTc | kSpeakerTfl | kSpeakerTfc | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr;
/** L R C Lfe Ls Rs Tc Tfl Tfc Tfr Trl Trr */
const SpeakerArrangement k111			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLfe | kSpeakerLs | kSpeakerRs | 
										   kSpeakerTc | kSpeakerTfl | kSpeakerTfc | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr;

/** L R C Ls Rs Sl Sr Tfl Tfr Trl Trr */
const SpeakerArrangement k70_4			 = kSpeakerL | kSpeakerR | kSpeakerC | kSpeakerLs | kSpeakerRs | kSpeakerSl | kSpeakerSr |
										   kSpeakerTfl | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr;

/** L R C Lfe Ls Rs Sl Sr Tfl Tfr Trl Trr */
const SpeakerArrangement k71_4			 = kSpeakerL | kSpeakerR | kSpeakerC | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerSl | kSpeakerSr |
										   kSpeakerTfl | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr;
const SpeakerArrangement k111MPEG3D		 = k71_4;

/** L R C Lfe Ls Rs Lc Rc Tfl Tfc Tfr Trl Trr Lfe2 */
const SpeakerArrangement k122			 = kSpeakerL  | kSpeakerR | kSpeakerC  | kSpeakerLfe | kSpeakerLs | kSpeakerRs	| kSpeakerLc  | kSpeakerRc |
										   kSpeakerTfl| kSpeakerTfc | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr | kSpeakerLfe2;
/** L R C Ls Rs Sl Sr Tc Tfl Tfc Tfr Trl Trr */
const SpeakerArrangement k130			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLs  | kSpeakerRs | kSpeakerSl | kSpeakerSr |
										   kSpeakerTc | kSpeakerTfl | kSpeakerTfc | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr;
/** L R C Lfe Ls Rs Sl Sr Tc Tfl Tfc Tfr Trl Trr */
const SpeakerArrangement k131			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerSl | kSpeakerSr |
										   kSpeakerTc | kSpeakerTfl | kSpeakerTfc | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr;

/** L R Ls Rs Sl Sr Tfl Tfr Trl Trr Bfl Bfr Brl Brr  */
const SpeakerArrangement k140			 = kSpeakerL | kSpeakerR | kSpeakerLs | kSpeakerRs | kSpeakerSl | kSpeakerSr |
										   kSpeakerTfl | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr |
										   kSpeakerBfl | kSpeakerBfr | kSpeakerBrl | kSpeakerBrr;

/** L R C Lfe Ls Rs Lc Rc Cs Sl Sr Tc Tfl Tfc Tfr Trl Trc Trr Lfe2 Tsl Tsr Bfl Bfc Bfr */
const SpeakerArrangement k222			 = kSpeakerL  | kSpeakerR | kSpeakerC  | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerLc | kSpeakerRc | kSpeakerCs | kSpeakerSl | kSpeakerSr | 
										   kSpeakerTc | kSpeakerTfl | kSpeakerTfc | kSpeakerTfr | kSpeakerTrl | kSpeakerTrc | kSpeakerTrr | kSpeakerLfe2 | kSpeakerTsl | kSpeakerTsr | 
										   kSpeakerBfl| kSpeakerBfc | kSpeakerBfr;


//------------------------------------------------------------------------
/** Speaker Arrangement String Representation.
\ingroup speakerArrangements */
/*@{*/
const CString kStringEmpty		= "";
const CString kStringMono		= "Mono";
const CString kStringStereo		= "Stereo";
const CString kStringStereoR	= "Stereo (Ls Rs)";
const CString kStringStereoC	= "Stereo (Lc Rc)";
const CString kStringStereoSide	= "Stereo (Sl Sr)";
const CString kStringStereoCLfe	= "Stereo (C LFE)";
const CString kStringStereoTF	= "Stereo (Tfl Tfr)";
const CString kStringStereoTS	= "Stereo (Tsl Tsr)";
const CString kStringStereoTR	= "Stereo (Trl Trr)";
const CString kStringStereoBF	= "Stereo (Bfl Bfr)";

const CString kString30Cine		= "LRC";
const CString kString30Music	= "LRS";
const CString kString31Cine		= "LRC+LFE";
const CString kString31Music	= "LRS+LFE";
const CString kString40Cine		= "LRCS";
const CString kString40Music	= "Quadro";
const CString kString41Cine		= "LRCS+LFE";
const CString kString41Music	= "Quadro+LFE";
const CString kString50			= "5.0";
const CString kString51			= "5.1";
const CString kString60Cine		= "6.0 Cine";
const CString kString60Music	= "6.0 Music";
const CString kString61Cine		= "6.1 Cine";
const CString kString61Music	= "6.1 Music";
const CString kString70Cine		= "7.0 Cine (SDDS)";
const CString kString70Music	= "7.0 Music (Dolby)";
const CString kString71Cine		= "7.1 Cine (SDDS)";
const CString kString71Music	= "7.1 Music (Dolby)";
const CString kString71CineTopCenter	= "7.1 Cine Top Center";
const CString kString71CineCenterHigh	= "7.1 Cine Center High";
const CString kString71CineFrontHigh	= "7.1 Cine Front High";
const CString kString71CineSideHigh		= "7.1 Cine Side High";
const CString kString71CineFullRear		= "7.1 Cine Full Rear";
const CString kString71Proximity		= "7.1 Proximity";
const CString kString80Cine		= "8.0 Cine";
const CString kString80Music	= "8.0 Music";
const CString kString80Cube		= "8.0 Cube";
const CString kString81Cine		= "8.1 Cine";
const CString kString81Music	= "8.1 Music";
const CString kString102		= "10.2 Experimental";
const CString kString122 = "12.2";
const CString kString50_4 = "5.0.4";
const CString kString51_4 = "5.1.4";
const CString kString91Atmos	= "9.1 Dolby Atmos";
const CString kString100		= "10.0 Auro-3D";
const CString kString101		= "10.1 Auro-3D";
const CString kString110		= "11.0 Auro-3D";
const CString kString111		= "11.1 Auro-3D";
const CString kString130		= "13.0 Auro-3D";
const CString kString131		= "13.1 Auro-3D";
const CString kString81MPEG		= "8.1 MPEG";
const CString kString70_4		= "7.0.4";
const CString kString71_4		= "7.1.4";
const CString kString140		= "14.0";
const CString kString222		= "22.2";
const CString kStringAmbi1stOrder = "1st Order Ambisonics";
const CString kStringAmbi2cdOrder = "2nd Order Ambisonics";
const CString kStringAmbi3rdOrder = "3rd Order Ambisonics";

/*@}*/

//------------------------------------------------------------------------
/** Speaker Arrangement String Representation with Speakers Name.
\ingroup speakerArrangements */
/*@{*/
const CString kStringMonoS		= "M";
const CString kStringStereoS	= "L R";
const CString kStringStereoRS	= "Ls Rs";
const CString kStringStereoCS	= "Lc Rc";
const CString kStringStereoSS	= "Sl Sr";
const CString kStringStereoCLfeS= "C LFE";
const CString kStringStereoTFS	= "Tfl Tfr";
const CString kStringStereoTSS	= "Tsl Tsr";
const CString kStringStereoTRS	= "Trl Trr";
const CString kStringStereoBFS	= "Bfl Bfr";
const CString kString30CineS	= "L R C";
const CString kString30MusicS	= "L R S";
const CString kString31CineS	= "L R C LFE";
const CString kString31MusicS	= "L R LFE S";
const CString kString40CineS	= "L R C S";
const CString kString40MusicS	= "L R Ls Rs";
const CString kString41CineS	= "L R C LFE S";
const CString kString41MusicS	= "L R LFE Ls Rs";
const CString kString50S		= "L R C Ls Rs";
const CString kString51S		= "L R C LFE Ls Rs";
const CString kString60CineS	= "L R C Ls Rs Cs";
const CString kString60MusicS	= "L R Ls Rs Sl Sr";
const CString kString61CineS	= "L R C LFE Ls Rs Cs";
const CString kString61MusicS	= "L R LFE Ls Rs Sl Sr";
const CString kString70CineS	= "L R C Ls Rs Lc Rc";
const CString kString70MusicS	= "L R C Ls Rs Sl Sr";
const CString kString71CineS	= "L R C LFE Ls Rs Lc Rc";
const CString kString71MusicS	= "L R C LFE Ls Rs Sl Sr";
const CString kString80CineS	= "L R C Ls Rs Lc Rc Cs";
const CString kString80MusicS	= "L R C Ls Rs Cs Sl Sr";
const CString kString81CineS	= "L R C LFE Ls Rs Lc Rc Cs";
const CString kString81MusicS	= "L R C LFE Ls Rs Cs Sl Sr";
const CString kString80CubeS	= "L R Ls Rs Tfl Tfr Trl Trr";
const CString kString71CineTopCenterS	= "L R C LFE Ls Rs Cs Tc";
const CString kString71CineCenterHighS	= "L R C LFE Ls Rs Cs Tfc";
const CString kString71CineFrontHighS	= "L R C LFE Ls Rs Tfl Tfl";
const CString kString71CineSideHighS	= "L R C LFE Ls Rs Tsl Tsl";
const CString kString71CineFullRearS	= "L R C LFE Ls Rs Lcs Rcs";
const CString kString71ProximityS		= "L R C LFE Ls Rs Pl Pr";
const CString kString50_4S		= "L R C Ls Rs Tfl Tfr Trl Trr";
const CString kString51_4S		= "L R C LFE Ls Rs Tfl Tfr Trl Trr"; 
const CString kString91AtmosS	= "L R C LFE Ls Rs Sl Sr Tsl Tsr";
const CString kString100S		= "L R C Ls Rs Tc Tfl Tfr Trl Trr";
const CString kString101S		= "L R C LFE Ls Rs Tc Tfl Tfr Trl Trr";
const CString kString110S		= "L R C Ls Rs Tc Tfl Tfc Tfr Trl Trr";
const CString kString111S		= "L R C LFE Ls Rs Tc Tfl Tfc Tfr Trl Trr";
const CString kStringk70_4S		= "L R C Ls Rs Sl Sr Tfl Tfr Trl Trr";
const CString kStringk71_4S		= "L R C LFE Ls Rs Sl Sr Tfl Tfr Trl Trr";
const CString kString130S		= "L R C Ls Rs Sl Sr Tc Tfl Tfc Tfr Trl Trr";
const CString kString131S		= "L R C LFE Ls Rs Sl Sr Tc Tfl Tfc Tfr Trl Trr";
const CString kString102S		= "L R C LFE Ls Rs Tfl Tfc Tfr Trl Trr LFE2";
const CString kString122S		= "L R C LFE Ls Rs Lc Rc Tfl Tfc Tfr Trl Trr LFE2";
const CString kString81MPEGS	= "L R LFE Ls Rs Tfl Tfc Tfr Bfc";
const CString kString140S		= "L R Ls Rs Sl Sr Tfl Tfr Trl Trr Bfl Bfr Brl Brr";
const CString kString222S		= "L R C LFE Ls Rs Lc Rc Cs Sl Sr Tc Tfl Tfc Tfr Trl Trc Trr LFE2 Tsl Tsr Bfl Bfc Bfr";

const CString kStringAmbi1stOrderS	= "0 1 2 3";
const CString kStringAmbi2cdOrderS	= "0 1 2 3 4 5 6 7 8";
const CString kStringAmbi3rdOrderS	= "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15";
/*@}*/

//------------------------------------------------------------------------
/** Returns number of channels used in speaker arrangement.
\ingroup speakerArrangements */
/*@{*/
inline int32 getChannelCount (SpeakerArrangement arr)
{
	int32 count = 0;
	while (arr)
	{
		if (arr & (SpeakerArrangement)1)
			++count;
		arr >>= 1;
	}
	return count;
}

//------------------------------------------------------------------------
/** Returns the index of a given speaker in a speaker arrangement (-1 if speaker is not part of the
 * arrangement). */
inline int32 getSpeakerIndex (Speaker speaker, SpeakerArrangement arrangement)
{
	// check if speaker is present in arrangement
	if ((arrangement & speaker) == 0)
		return -1;

	int32 result = 0;
	Speaker i = 1;
	while (i < speaker)
	{
		if (arrangement & i)
			result++;
		i <<= 1;
	}

	return result;
}

//------------------------------------------------------------------------
/** Returns the speaker for a given index in a speaker arrangement (return 0 when out of range). */
inline Speaker getSpeaker (const SpeakerArrangement& arr, int32 index)
{
	SpeakerArrangement arrTmp = arr;

	int32 index2 = -1;
	int32 pos = -1;
	while (arrTmp)
	{
		if (arrTmp & 0x1)
			index2++;
		pos++;
		if (index2 == index)
			return (Speaker)1 << pos;
		
		arrTmp = arrTmp >> 1;
	}
	return 0;
}

//------------------------------------------------------------------------
/** Returns true if arrSubSet is a subset speaker of arr (means each speaker of arrSubSet is
 * included in arr). */
inline bool isSubsetOf (const SpeakerArrangement& arrSubSet, const SpeakerArrangement& arr)
{
	return (arrSubSet == (arrSubSet & arr));
}

//------------------------------------------------------------------------
/** Returns true if arrangement is a Auro configuration. */
inline bool isAuro (const SpeakerArrangement& arr)
{
	if (arr == k90 || arr == k91 || arr == k100 || arr == k101 || arr == k110 || arr == k111 ||
	    arr == k130 || arr == k131)
	{
		return true;
	}
	return false;
}

//------------------------------------------------------------------------
/** Returns true if arrangement is a Auro configuration. */
inline bool isAmbisonics (const SpeakerArrangement& arr)
{
	if (arr == kAmbi1stOrderACN || arr == kAmbi2cdOrderACN || arr == kAmbi3rdOrderACN)
	{
		return true;
	}
	return false;
}

//------------------------------------------------------------------------
/** Returns the speaker arrangement associated to a string representation.
    Returns kEmpty if no associated arrangement is known.
*/
inline SpeakerArrangement getSpeakerArrangementFromString (CString arrStr)
{
	if (!strcmp8 (arrStr, kStringMono))
		return kMono;
	if (!strcmp8 (arrStr, kStringStereo))
		return kStereo;
	if (!strcmp8 (arrStr, kStringStereoR))
		return kStereoSurround;
	if (!strcmp8 (arrStr, kStringStereoC))
		return kStereoCenter;
	if (!strcmp8 (arrStr, kStringStereoSide))
		return kStereoSide;
	if (!strcmp8 (arrStr, kStringStereoCLfe))
		return kStereoCLfe;
	if (!strcmp8 (arrStr, kStringStereoTF))
		return kStereoTF;
	if (!strcmp8 (arrStr, kStringStereoTS))
		return kStereoTS;
	if (!strcmp8 (arrStr, kStringStereoTR))
		return kStereoTR;
	if (!strcmp8 (arrStr, kStringStereoBF))
		return kStereoBF;
	if (!strcmp8 (arrStr, kString30Cine))
		return k30Cine;
	if (!strcmp8 (arrStr, kString30Music))
		return k30Music;
	if (!strcmp8 (arrStr, kString31Cine))
		return k31Cine;
	if (!strcmp8 (arrStr, kString31Music))
		return k31Music;
	if (!strcmp8 (arrStr, kString40Cine))
		return k40Cine;
	if (!strcmp8 (arrStr, kString40Music))
		return k40Music;
	if (!strcmp8 (arrStr, kString41Cine))
		return k41Cine;
	if (!strcmp8 (arrStr, kString41Music))
		return k41Music;
	if (!strcmp8 (arrStr, kString50))
		return k50;
	if (!strcmp8 (arrStr, kString51))
		return k51;
	if (!strcmp8 (arrStr, kString60Cine))
		return k60Cine;
	if (!strcmp8 (arrStr, kString60Music))
		return k60Music;
	if (!strcmp8 (arrStr, kString61Cine))
		return k61Cine;
	if (!strcmp8 (arrStr, kString61Music))
		return k61Music;
	if (!strcmp8 (arrStr, kString70Cine))
		return k70Cine;
	if (!strcmp8 (arrStr, kString70Music))
		return k70Music;
	if (!strcmp8 (arrStr, kString71Cine))
		return k71Cine;
	if (!strcmp8 (arrStr, kString71Music))
		return k71Music;
	if (!strcmp8 (arrStr, kString71Proximity))
		return k71Proximity;
	if (!strcmp8 (arrStr, kString80Cine))
		return k80Cine;
	if (!strcmp8 (arrStr, kString80Music))
		return k80Music;
	if (!strcmp8 (arrStr, kString81Cine))
		return k81Cine;
	if (!strcmp8 (arrStr, kString81Music))
		return k81Music;
	if (!strcmp8 (arrStr, kString102))
		return k102;
	if (!strcmp8 (arrStr, kString122))
		return k122;
	if (!strcmp8 (arrStr, kString80Cube))
		return k80Cube;
	if (!strcmp8 (arrStr, kString71CineTopCenter))
		return k71CineTopCenter;
	if (!strcmp8 (arrStr, kString71CineCenterHigh))
		return k71CineCenterHigh;
	if (!strcmp8 (arrStr, kString71CineFrontHigh))
		return k71CineFrontHigh;
	if (!strcmp8 (arrStr, kString71CineSideHigh))
		return k71CineSideHigh;
	if (!strcmp8 (arrStr, kString71CineFullRear))
		return k71CineFullRear;
	if (!strcmp8 (arrStr, kString50_4))
		return k50_4;
	if (!strcmp8 (arrStr, kString51_4))
		return k51_4;
	if (!strcmp8 (arrStr, kString81MPEG))
		return k81MPEG3D;
	if (!strcmp8 (arrStr, kString91Atmos))
		return k91Atmos;
	if (!strcmp8 (arrStr, kString100))
		return k100;
	if (!strcmp8 (arrStr, kString101))
		return k101;
	if (!strcmp8 (arrStr, kString110))
		return k110;
	if (!strcmp8 (arrStr, kString111))
		return k111;
	if (!strcmp8 (arrStr, kString70_4))
		return k70_4;
	if (!strcmp8 (arrStr, kString71_4))
		return k71_4;
	if (!strcmp8 (arrStr, kString130))
		return k130;
	if (!strcmp8 (arrStr, kString131))
		return k131;
	if (!strcmp8 (arrStr, kString140))
		return k140;
	if (!strcmp8 (arrStr, kString222))
		return k222;
	if (!strcmp8 (arrStr, kStringAmbi1stOrder))
		return kAmbi1stOrderACN;
	if (!strcmp8 (arrStr, kStringAmbi2cdOrder))
		return kAmbi2cdOrderACN;
	if (!strcmp8 (arrStr, kStringAmbi3rdOrder))
		return kAmbi3rdOrderACN;
	return kEmpty;
}

//------------------------------------------------------------------------
/** Returns the string representation of a given speaker arrangement.
	Returns kStringEmpty if arr is unknown.
*/
inline CString getSpeakerArrangementString (SpeakerArrangement arr, bool withSpeakersName)
{
	switch (arr)
	{
		case kMono:				return withSpeakersName ? kStringMonoS		: kStringMono;
		case kStereo:			return withSpeakersName ? kStringStereoS	: kStringStereo;
		case kStereoSurround:	return withSpeakersName ? kStringStereoRS	: kStringStereoR;
		case kStereoCenter:		return withSpeakersName ? kStringStereoCS	: kStringStereoC;
		case kStereoSide:		return withSpeakersName ? kStringStereoSS	: kStringStereoSide;
		case kStereoCLfe:		return withSpeakersName ? kStringStereoCLfeS: kStringStereoCLfe;
		case kStereoTF:			return withSpeakersName ? kStringStereoTFS	: kStringStereoTF;
		case kStereoTS:			return withSpeakersName ? kStringStereoTSS	: kStringStereoTS;
		case kStereoTR:			return withSpeakersName ? kStringStereoTRS	: kStringStereoTR;
		case kStereoBF:			return withSpeakersName ? kStringStereoBFS	: kStringStereoBF;
		case k30Cine:			return withSpeakersName ? kString30CineS	: kString30Cine;
		case k30Music:			return withSpeakersName ? kString30MusicS	: kString30Music;
		case k31Cine:			return withSpeakersName ? kString31CineS	: kString31Cine;
		case k31Music:			return withSpeakersName ? kString31MusicS	: kString31Music;
		case k40Cine:			return withSpeakersName ? kString40CineS	: kString40Cine;
		case k40Music:			return withSpeakersName ? kString40MusicS	: kString40Music;
		case k41Cine:			return withSpeakersName ? kString41CineS	: kString41Cine;
		case k41Music:			return withSpeakersName ? kString41MusicS	: kString41Music;
		case k50:				return withSpeakersName ? kString50S		: kString50;
		case k51:				return withSpeakersName ? kString51S		: kString51;
		case k60Cine:			return withSpeakersName ? kString60CineS	: kString60Cine;
		case k60Music:			return withSpeakersName ? kString60MusicS	: kString60Music;
		case k61Cine:			return withSpeakersName ? kString61CineS	: kString61Cine;
		case k61Music:			return withSpeakersName ? kString61MusicS	: kString61Music;
		case k70Cine:			return withSpeakersName ? kString70CineS	: kString70Cine;
		case k70Music:			return withSpeakersName ? kString70MusicS	: kString70Music;
		case k71Cine:			return withSpeakersName ? kString71CineS	: kString71Cine;
		case k71Music:			return withSpeakersName ? kString71MusicS	: kString71Music;
		case k71Proximity:		return withSpeakersName ? kString71ProximityS : kString71Proximity;
		case k80Cine:			return withSpeakersName ? kString80CineS	: kString80Cine;
		case k80Music:			return withSpeakersName ? kString80MusicS	: kString80Music;
		case k81Cine:			return withSpeakersName ? kString81CineS	: kString81Cine;
		case k81Music:			return withSpeakersName ? kString81MusicS	: kString81Music;
		case k81MPEG3D:			return withSpeakersName ? kString81MPEGS	: kString81MPEG;
		case k102:				return withSpeakersName ? kString102S		: kString102;
		case k122:				return withSpeakersName ? kString122S		: kString122;
		case k80Cube:			return withSpeakersName ? kString80CubeS	: kString80Cube;
		case k71CineTopCenter:	return withSpeakersName ? kString71CineTopCenterS	: kString71CineTopCenter;
		case k71CineCenterHigh:	return withSpeakersName ? kString71CineCenterHighS	: kString71CineCenterHigh;
		case k71CineFrontHigh:	return withSpeakersName ? kString71CineFrontHighS	: kString71CineFrontHigh;
		case k71CineSideHigh:	return withSpeakersName ? kString71CineSideHighS	: kString71CineSideHigh;
		case k71CineFullRear:	return withSpeakersName ? kString71CineFullRearS	: kString71CineFullRear;
		case k91Atmos:			return withSpeakersName ? kString91AtmosS	: kString91Atmos;
		case k100:				return withSpeakersName ? kString100S		: kString100;
		case k101:				return withSpeakersName ? kString101S		: kString101;
		case k110:				return withSpeakersName ? kString110S		: kString110;
		case k111:				return withSpeakersName ? kString111S		: kString111;

		case k50_4:				return withSpeakersName ? kString50_4S		: kString50_4;
		case k51_4:				return withSpeakersName ? kString51_4S		: kString51_4;
		case k70_4:				return withSpeakersName ? kStringk70_4S		: kString70_4;
		case k71_4:				return withSpeakersName ? kStringk71_4S		: kString71_4;
			
		case k130:				return withSpeakersName ? kString130S		: kString130;
		case k131:				return withSpeakersName ? kString131S		: kString131;
		case k140:				return withSpeakersName ? kString140S		: kString140;
		case k222:				return withSpeakersName ? kString222S		: kString222;
			break;
	}

	if (arr == kAmbi1stOrderACN)
		return withSpeakersName ? kStringAmbi1stOrderS : kStringAmbi1stOrder;
	if (arr == kAmbi2cdOrderACN)
		return withSpeakersName ? kStringAmbi2cdOrderS : kStringAmbi2cdOrder;
	if (arr == kAmbi3rdOrderACN)
		return withSpeakersName ? kStringAmbi3rdOrderS : kStringAmbi3rdOrder;

	return kStringEmpty;
}

//------------------------------------------------------------------------
/** Returns a CString representation of a given speaker in a given arrangement
 */
inline CString getSpeakerShortName (const SpeakerArrangement& arr, int32 index)
{
	SpeakerArrangement arrTmp = arr;

	bool found = false;
	int32 index2 = -1;
	int32 pos = -1;
	while (arrTmp)
	{
		if (arrTmp & 0x1)
			index2++;
		pos++;
		if (index2 == index)
		{
			found = true;
			break;
		}
		arrTmp = arrTmp >> 1;
	}

	if (!found)
		return "";

	Speaker speaker = (Speaker)1 << pos;
	if (speaker == kSpeakerL)
		return "L";
	if (speaker == kSpeakerR)
		return "R";
	if (speaker == kSpeakerC)
		return "C";
	if (speaker == kSpeakerLfe)
		return "LFE";
	if (speaker == kSpeakerLs)
		return "Ls";
	if (speaker == kSpeakerRs)
		return "Rs";
	if (speaker == kSpeakerLc)
		return "Lc";
	if (speaker == kSpeakerRc)
		return "Rc";
	if (speaker == kSpeakerS)
		return "S";
	if (speaker == kSpeakerSl)
		return "Sl";
	if (speaker == kSpeakerSr)
		return "Sr";
	if (speaker == kSpeakerTc)
		return "Tc";
	if (speaker == kSpeakerTfl)
		return "Tfl";
	if (speaker == kSpeakerTfc)
		return "Tfc";
	if (speaker == kSpeakerTfr)
		return "Tfr";
	if (speaker == kSpeakerTrl)
		return "Trl";
	if (speaker == kSpeakerTrc)
		return "Trc";
	if (speaker == kSpeakerTrr)
		return "Trr";
	if (speaker == kSpeakerLfe2)
		return "LFE2";
	if (speaker == kSpeakerM)
		return "M";

	if (speaker == kSpeakerACN0)
		return "0";
	if (speaker == kSpeakerACN1)
		return "1";
	if (speaker == kSpeakerACN2)
		return "2";
	if (speaker == kSpeakerACN3)
		return "3";
	if (speaker == kSpeakerACN4)
		return "4";
	if (speaker == kSpeakerACN5)
		return "5";
	if (speaker == kSpeakerACN6)
		return "6";
	if (speaker == kSpeakerACN7)
		return "7";
	if (speaker == kSpeakerACN8)
		return "8";
	if (speaker == kSpeakerACN9)
		return "9";
	if (speaker == kSpeakerACN10)
		return "10";
	if (speaker == kSpeakerACN11)
		return "11";
	if (speaker == kSpeakerACN12)
		return "12";
	if (speaker == kSpeakerACN13)
		return "13";
	if (speaker == kSpeakerACN14)
		return "14";
	if (speaker == kSpeakerACN15)
		return "15";

	if (speaker == kSpeakerTsl)
		return "Tsl";
	if (speaker == kSpeakerTsr)
		return "Tsr";
	if (speaker == kSpeakerLcs)
		return "Lcs";
	if (speaker == kSpeakerRcs)
		return "Rcs";

	if (speaker == kSpeakerBfl)
		return "Bfl";
	if (speaker == kSpeakerBfc)
		return "Bfc";
	if (speaker == kSpeakerBfr)
		return "Bfr";
	if (speaker == kSpeakerPl)
		return "Pl";
	if (speaker == kSpeakerPr)
		return "Pr";
	if (speaker == kSpeakerBsl)
		return "Bsl";
	if (speaker == kSpeakerBsr)
		return "Bsr";
	if (speaker == kSpeakerBrl)
		return "Brl";
	if (speaker == kSpeakerBrc)
		return "Brc";
	if (speaker == kSpeakerBrr)
		return "Brr";

	return "";
}

/*@}*/

//------------------------------------------------------------------------
} // namespace SpeakerArr
} // namespace Vst
} // namespace Steinberg
