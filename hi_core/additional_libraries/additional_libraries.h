/*  ===========================================================================
*
*   This file is part of HISE.
*   Copyright 2016 Christoph Hart
*
*   HISE is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   HISE is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with HISE.  If not, see <http://www.gnu.org/licenses/>.
*
*   Commercial licences for using HISE in an closed source project are
*   available on request. Please visit the project's website to get more
*   information about commercial licencing:
*
*   http://www.hartinstruments.net/hise/
*
*   HISE is based on the JUCE library,
*   which also must be licenced for commercial applications:
*
*   http://www.juce.com
*
*   ===========================================================================
*/

#ifndef ADDITIONAL_LIBRARIES_H_INCLUDED
#define ADDITIONAL_LIBRARIES_H_INCLUDED

/** The ICST DSP library v1.2.
*
*	Released under the 2-clause BSD license. Copyright (c) 2008-2010, Zurich
*   University of the Arts, Beat Frei. All rights reserved.
*
*/

#ifndef DONT_INCLUDE_HEADERS_IN_CPP
#define DONT_INCLUDE_HEADERS_IN_CPP 1
#endif

#if USE_IPP
#define ICSTLIB_USE_IPP 1
#endif


#include "icst/MathDefs.h"
#include "icst/Common.h"  
#include "icst/fftooura.h"
#include "icst/AudioAnalysis.h"
//#include "icst/AudioFile.h"
#include "icst/AudioSynth.h"
#include "icst/BlkDsp.h"

#include "icst/Neuro.h"
#include "icst/SpecMath.h"


#endif  // ADDITIONAL_LIBRARIES_H_INCLUDED
