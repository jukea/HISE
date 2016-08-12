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


void multiply(float* dst, const float* src, int numValues)
{

	FloatVectorOperations::multiply(dst, src, numValues);
}

void multiplyScalar(float* dst, double scalar, int numValues)
{
	FloatVectorOperations::multiply(dst, (float)scalar, numValues);
}

TccDspObject::TccDspObject(const String &code) :
compiledOk(false),
pb(nullptr),
pp(nullptr),
sp(nullptr),
gp(nullptr),
gnp(nullptr)
{
	context = new TccContext();
	context->openContext();


	context->addFunction((void*)multiply, "vec_multiply");
	context->addFunction((void*)multiplyScalar, "vec_mul_scalar");

	if (code.isNotEmpty() && context->compile(code.getCharPointer()) == 0)
	{
		pb = (Signatures::processBlock)context->getFunction("processBlock");
		pp = (Signatures::prepareToPlay)context->getFunction("prepareToPlay");
		sp = (Signatures::setParameter)context->getFunction("setParameter");
		gp = (Signatures::getParameter)context->getFunction("getParameter");
		gnp = (Signatures::getNumParameters)context->getFunction("getNumParameters");


		compiledOk = true;
	}

	context->closeContext();
}


var TccDspFactory::createModule(const String &module) const
{
	DspInstance* p = new DspInstance(this, module);

	try
	{
		p->initialise();
	}
	catch (String errorMessage)
	{
		DBG(errorMessage);
		return var::undefined();
	}

	return var(p);
}

DspBaseObject * TccDspFactory::createDspBaseObject(const String &module) const
{
	File f(module);

	if (f.existsAsFile())
	{
		const String code = f.loadFileAsString();

		return new TccDspObject(code);
	}
	else
	{
		return nullptr;
	}
}

void TccDspFactory::destroyDspBaseObject(DspBaseObject *object) const
{
	if (object != nullptr) delete object;
}