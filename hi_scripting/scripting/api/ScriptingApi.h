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

#ifndef HI_SCRIPTING_API_H_INCLUDED
#define HI_SCRIPTING_API_H_INCLUDED


/** This class wraps all available functions for the scripting engine provided by a ScriptProcessor.
*	@ingroup scripting
*/
class ScriptingApi
{
public:

	/** All scripting methods related to the midi message that triggered the callback.
	*	@ingroup scriptingApi
	*
	*	Every method must be called on the message like this:
	*
	*		message.delayEvent(200);
	*/
	class Message: public ScriptingObject,
				   public ApiClass
	{
	public:

		Message(ProcessorWithScriptingContent *p):
			ScriptingObject(p),
			ApiClass(0),
			ignored(false),
			wrongNoteOff(false),
			messageHolder(nullptr),
			currentEventId(0),
			eventIdCounter(0)
			
		{
			ADD_API_METHOD_1(setNoteNumber);
			ADD_API_METHOD_1(setVelocity);
			ADD_API_METHOD_1(setControllerNumber);
			ADD_API_METHOD_1(setControllerValue);
			ADD_API_METHOD_0(getNoteNumber);
			ADD_API_METHOD_0(getVelocity);
			ADD_API_METHOD_1(ignoreEvent);
			ADD_API_METHOD_1(delayEvent);
			ADD_API_METHOD_0(getEventId);
			ADD_API_METHOD_0(getChannel);
			ADD_API_METHOD_1(setChannel);
		};

		~Message()
		{
			messageHolder = nullptr;
		}

		Identifier getName() const override { RETURN_STATIC_IDENTIFIER("Message"); }

		static Identifier getClassName() { RETURN_STATIC_IDENTIFIER("Message"); }

		/** Return the note number. This can be called only on midi event callbacks. */
		int getNoteNumber() const;

		/** Delays the event by the sampleAmount. */
		void delayEvent(int samplesToDelay);

		/** returns the controller number or 'undefined', if the message is neither controller nor pitch wheel nor aftertouch.
		*
		*	You can also check for pitch wheel values and aftertouch messages.
		*	Pitchwheel has number 128, Aftertouch has number 129.
		*/
		var getControllerNumber() const;
		
		/** Returns the value of the controller. */
		var getControllerValue() const;
		
		/** Returns the MIDI Channel from 1 to 16. */
		int getChannel() const;

		/** Changes the MIDI channel from 1 to 16. */
		void setChannel(int newChannel);

		/** Changes the note number. */
		void setNoteNumber(int newNoteNumber);

		/** Changes the velocity (range 1 - 127). */
		void setVelocity(int newVelocity);

		/** Changes the ControllerNumber. */
		void setControllerNumber(int newControllerNumber);
		
		/** Changes the controller value (range 0 - 127). */
		void setControllerValue(int newControllerValue);

		/** Returns the Velocity. */
		int getVelocity() const;

		/** Ignores the event. */
		void ignoreEvent(bool shouldBeIgnored=true) { ignored = shouldBeIgnored; };

		/** Returns the event id of the current message. */
		int getEventId() const;

		struct Wrapper
		{
			API_VOID_METHOD_WRAPPER_1(Message, setNoteNumber);
			API_VOID_METHOD_WRAPPER_1(Message, setVelocity);
			API_VOID_METHOD_WRAPPER_1(Message, setControllerNumber);
			API_VOID_METHOD_WRAPPER_1(Message, setControllerValue);
			API_METHOD_WRAPPER_0(Message, getNoteNumber);
			API_METHOD_WRAPPER_0(Message, getVelocity);
			API_VOID_METHOD_WRAPPER_1(Message, ignoreEvent);
			API_VOID_METHOD_WRAPPER_1(Message, delayEvent);
			API_METHOD_WRAPPER_0(Message, getEventId);
			API_METHOD_WRAPPER_0(Message, getChannel);
			API_VOID_METHOD_WRAPPER_1(Message, setChannel);
		};

	private:

		struct MidiMessageWithEventId
		{
			MidiMessageWithEventId() :m(MidiMessage::noteOn(1, 0, 1.0f)), eventId (-1) { };

			MidiMessageWithEventId(MidiMessage &m_, int eventId_):	m(m_),	eventId(eventId_) { };

			inline int getNoteNumber() {return m.getNoteNumber(); };
			bool isVoid() const { return (eventId == -1); }
			void setVoid() {eventId = -1;};

			MidiMessage m;
			int eventId;
			static MidiMessageWithEventId empty;
		};

		// sets the reference to the midi message.
		void setMidiMessage(MidiMessage *m);

		friend class JavascriptMidiProcessor;
		friend class HardcodedScriptProcessor;

		MidiMessage *messageHolder;
		bool wrongNoteOff;
		bool ignored;
		int currentEventId;
		int eventIdCounter;
		MidiMessageWithEventId noteOnMessages[1024];
	};

	/** All scripting methods related to the main engine can be accessed here.
	*	@ingroup scriptingApi
	*/
	class Engine: public ScriptingObject,
				  public ApiClass
	{
	public:

		Engine(ProcessorWithScriptingContent *p):
			ScriptingObject(p),
			ApiClass(0)
		{
			ADD_API_METHOD_0(allNotesOff);
			ADD_API_METHOD_0(getUptime);
			ADD_API_METHOD_0(getHostBpm);
			ADD_API_METHOD_2(setGlobal);
			ADD_API_METHOD_1(getGlobal);
			ADD_API_METHOD_1(getMilliSecondsForTempo);
			ADD_API_METHOD_1(getSamplesForMilliSeconds);
			ADD_API_METHOD_1(getMilliSecondsForSamples);
			ADD_API_METHOD_1(getGainFactorForDecibels);
			ADD_API_METHOD_1(getDecibelsForGainFactor);
			ADD_API_METHOD_1(getFrequencyForMidiNoteNumber);
			ADD_API_METHOD_0(getSampleRate);
			ADD_API_METHOD_1(getMidiNoteName);
			ADD_API_METHOD_1(getMidiNoteFromName);
			ADD_API_METHOD_1(getMacroName);
			ADD_API_METHOD_2(setKeyColour);
			ADD_API_METHOD_1(setLowestKeyToDisplay);
            ADD_API_METHOD_0(createMidiList);
			ADD_API_METHOD_1(openEditor);
			ADD_API_METHOD_0(createLiveCodingVariables);
			ADD_API_METHOD_1(include);
			ADD_API_METHOD_0(getPlayHead);
			ADD_API_METHOD_2(dumpAsJSON);
			ADD_API_METHOD_1(loadFromJSON);
			ADD_API_METHOD_0(getUserPresetDirectoryContent);
			ADD_API_METHOD_1(setCompileProgress);
			ADD_API_METHOD_2(matchesRegex);
            ADD_API_METHOD_2(getRegexMatches);
            ADD_API_METHOD_2(doubleToString);
			ADD_API_METHOD_0(getOS);
			ADD_API_METHOD_0(createTimerObject);

		}

		Identifier getName() const override  { RETURN_STATIC_IDENTIFIER("Engine"); };

		/** Returns the current sample rate. */
		double getSampleRate() const;

		/** Converts milli seconds to samples */
		double getSamplesForMilliSeconds(double milliSeconds) const 
		{ 
			return (milliSeconds / 1000.0) * getSampleRate(); 
		};
		
		/** Converts samples to milli seconds. */
		double getMilliSecondsForSamples(double samples) const { return samples / getSampleRate() * 1000.0; };
		
		/** Converts decibel (-100.0 ... 0.0) to gain factor (0.0 ... 1.0). */
		double getGainFactorForDecibels(double decibels) const { return Decibels::decibelsToGain<double>(decibels); };
		
		/** Converts gain factor (0.0 .. 1.0) to decibel (-100.0 ... 0). */
		double getDecibelsForGainFactor(double gainFactor) const { return Decibels::gainToDecibels<double>(gainFactor); };

		/** Converts midi note number 0 ... 127 to Frequency 20 ... 20.000. */
		double getFrequencyForMidiNoteNumber(int midiNumber) const { return MidiMessage::getMidiNoteInHertz(midiNumber); };

		/** Converts MIDI note number to Midi note name ("C3" for middle C). */
		String getMidiNoteName(int midiNumber) const { return MidiMessage::getMidiNoteName(midiNumber, true, true, 3); };

		/** Converts MIDI note name to MIDI number ("C3" for middle C). */
		int getMidiNoteFromName(String midiNoteName) const;

		/** Sends an allNotesOff message at the next buffer. */
		void allNotesOff();

		/** Saves a variable into the global container. */
		void setGlobal(int index, var valueToSave);
		
		/** returns a variable from the global container. */
		var getGlobal(int index) const;

		/** Returns the uptime of the engine in seconds. */
		double getUptime() const;
		
		/** Sets a key of the global keyboard to the specified colour (using the form 0x00FF00 for eg. of the key to the specified colour. */
		void setKeyColour(int keyNumber, int colourAsHex);
		
		/** Changes the lowest visible key on the on screen keyboard. */
		void setLowestKeyToDisplay(int keyNumber);

		/** Returns the millisecond value for the supplied tempo (HINT: Use "TempoSync" mode from Slider!) */
		double getMilliSecondsForTempo(int tempoIndex) const
		{
			return (double)TempoSyncer::getTempoInMilliSeconds(getHostBpm(), (TempoSyncer::Tempo)tempoIndex);
		};

		/** Returns the Bpm of the host. */
		double getHostBpm() const;
		
		/** Returns the name for the given macro index. */
		String getMacroName(int index);
		
		/** Returns the current operating system ("OSX" or ("WIN"). */
		String getOS();

		/** Opens an editor for the included file. */
		void openEditor(int includedFileIndex);

		/** Includes the file (from the script folder). */
		void include(const String &string);
        
		/** Creates some handy variables for live coding purposes (note names, some chords, etc). */
		void createLiveCodingVariables();

		/** Allows access to the data of the host (playing status, timeline, etc...). */
		DynamicObject *getPlayHead();

		/** Creates a MIDI List object. */
        ScriptingObjects::MidiList *createMidiList(); 

		/** Creates a new timer object. */
		ScriptingObjects::TimerObject* createTimerObject();

		/** Exports an object as JSON. */
		void dumpAsJSON(var object, String fileName);

		/** Imports a JSON file as object. */
		var loadFromJSON(String fileName);

		/** returns an array with all files within the user preset directory content. */
		var getUserPresetDirectoryContent();

		/** Displays the progress (0.0 to 1.0) in the progress bar of the editor. */
		void setCompileProgress(var progress);

		/** Matches the string against the regex token. */
		bool matchesRegex(String stringToMatch, String regex);

        /** Returns an array with all matches. */
        var getRegexMatches(String stringToMatch, String regex);
        
        /** Returns a string of the value with the supplied number of digits. */
        String doubleToString(double value, int digits);
        
		struct Wrapper
		{
			API_VOID_METHOD_WRAPPER_0(Engine, allNotesOff);
			API_METHOD_WRAPPER_0(Engine, getUptime);
			API_METHOD_WRAPPER_0(Engine, getHostBpm);
			API_VOID_METHOD_WRAPPER_2(Engine, setGlobal);
			API_METHOD_WRAPPER_1(Engine, getGlobal);
			API_METHOD_WRAPPER_1(Engine, getMilliSecondsForTempo);
			API_METHOD_WRAPPER_1(Engine, getSamplesForMilliSeconds);
			API_METHOD_WRAPPER_1(Engine, getMilliSecondsForSamples);
			API_METHOD_WRAPPER_1(Engine, getGainFactorForDecibels);
			API_METHOD_WRAPPER_1(Engine, getDecibelsForGainFactor);
			API_METHOD_WRAPPER_1(Engine, getFrequencyForMidiNoteNumber);
			API_METHOD_WRAPPER_0(Engine, getSampleRate);
			API_METHOD_WRAPPER_1(Engine, getMidiNoteName);
			API_METHOD_WRAPPER_1(Engine, getMidiNoteFromName);
			API_METHOD_WRAPPER_1(Engine, getMacroName);
			API_VOID_METHOD_WRAPPER_2(Engine, setKeyColour);
			API_VOID_METHOD_WRAPPER_1(Engine, setLowestKeyToDisplay);
			API_METHOD_WRAPPER_0(Engine, createMidiList);
			API_METHOD_WRAPPER_0(Engine, createTimerObject);
			API_VOID_METHOD_WRAPPER_1(Engine, openEditor);
			API_VOID_METHOD_WRAPPER_0(Engine, createLiveCodingVariables);
			API_VOID_METHOD_WRAPPER_1(Engine, include);
			API_METHOD_WRAPPER_0(Engine, getPlayHead);
			API_VOID_METHOD_WRAPPER_2(Engine, dumpAsJSON);
			API_METHOD_WRAPPER_1(Engine, loadFromJSON);
			API_METHOD_WRAPPER_0(Engine, getUserPresetDirectoryContent);
			API_VOID_METHOD_WRAPPER_1(Engine, setCompileProgress);
			API_METHOD_WRAPPER_2(Engine, matchesRegex);
			API_METHOD_WRAPPER_2(Engine, getRegexMatches);
			API_METHOD_WRAPPER_2(Engine, doubleToString);
			API_METHOD_WRAPPER_0(Engine, getOS);
		};
	};

	/** All scripting functions for sampler specific functionality. */
	class Sampler : public CreatableScriptObject
	{
	public:

		Sampler(ProcessorWithScriptingContent *p, ModulatorSampler *sampler);

		Identifier getObjectName() const override
		{
			return "Sampler";
		}

		/** Enables / Disables the automatic round robin group start logic (works only on samplers). */
		void enableRoundRobin(bool shouldUseRoundRobin);

		/** Enables the group with the given index (one-based). Works only with samplers and `enableRoundRobin(false)`. */
		void setActiveGroup(int activeGroupIndex);

		/** Returns the amount of actual RR groups for the notenumber and velocity*/
		int getRRGroupsForMessage(int noteNumber, int velocity);

		/** Recalculates the RR Map. Call this at compile time if you want to use 'getRRGroupForMessage()'. */
		void refreshRRMap();

		/** Selects samples using the regex string as wildcard and the selectMode ("SELECT", "ADD", "SUBTRACT")*/
		void selectSounds(String regex);

		/** Returns the amount of selected samples. */
		int getNumSelectedSounds();

		/** Sets the property of the sampler sound for the selection. */
		void setSoundPropertyForSelection(int propertyIndex, var newValue);

		/** Returns the property of the sound with the specified index. */
		var getSoundProperty(int propertyIndex, int soundIndex);

		/** Sets the property for the index within the selection. */
		void setSoundProperty(int soundIndex, int propertyIndex, var newValue);

		/** Purges all samples of the given mic (Multimic samples only). */
		void purgeMicPosition(String micName, bool shouldBePurged);

		/** Returns the name of the channel with the given index (Multimic samples only. */
		String getMicPositionName(int channelIndex);

		/** Refreshes the interface. Call this after you changed the properties. */
		void refreshInterface();

		/** Loads a new samplemap into this sampler. */
		void loadSampleMap(const String &fileName);

		bool objectDeleted() const override
		{
			return sampler.get() == nullptr;
		}

		bool objectExists() const override
		{
			return sampler.get() != nullptr;
		}

		struct Wrapper
		{
			static var enableRoundRobin(const var::NativeFunctionArgs& args);
			static var setActiveGroup(const var::NativeFunctionArgs& args);
			static var getRRGroupsForMessage(const var::NativeFunctionArgs& args);
			static var refreshRRMap(const var::NativeFunctionArgs& args);
			static var selectSounds(const var::NativeFunctionArgs& args);
			static var getNumSelectedSounds(const var::NativeFunctionArgs& args);
			static var setSoundPropertyForSelection(const var::NativeFunctionArgs& args);
			static var getSoundProperty(const var::NativeFunctionArgs& args);
			static var setSoundProperty(const var::NativeFunctionArgs& args);
			static var purgeMicPosition(const var::NativeFunctionArgs& args);
			static var getMicPositionName(const var::NativeFunctionArgs& args);
			static var refreshInterface(const var::NativeFunctionArgs& args);
			static var loadSampleMap(const var::NativeFunctionArgs& args);

		};

	private:

		WeakReference<Processor> sampler;

		SelectedItemSet<WeakReference<ModulatorSamplerSound>> soundSelection;
	};
	

	/** Provides access to the synth where the script processor resides.
	*	@ingroup scriptingApi
	*
	*	There are special methods for SynthGroups which only work with SynthGroups
	*/
	class Synth: public ScriptingObject,
				 public ApiClass
	{
	public:
		Synth(ProcessorWithScriptingContent *p, ModulatorSynth *ownerSynth);
		
		Identifier getName() const override { RETURN_STATIC_IDENTIFIER("Synth"); };

		/** Adds the interface to the Container's body (or the frontend interface if compiled) */
		void addToFront(bool addToFront);

		/** Defers all callbacks to the message thread (midi callbacks become read-only). */
		void deferCallbacks(bool makeAsynchronous);

		/**	Changes the allowed state of one of the child synths. Works only with SynthGroups. */
		void allowChildSynth(int synthIndex, bool shouldBeAllowed); 

		/** Sends a note off message. The envelopes will tail off. */
		void noteOff(int noteNumber);
		
		/** Plays a note. Be careful or you get stuck notes! */
		void playNote(int noteNumber, int velocity);
		
		/** Starts the timer of the synth. */
		void startTimer(double milliseconds);
		
		/** Sets an attribute of the parent synth. */
		void setAttribute(int attributeIndex, float newAttribute);

		/** Returns the attribute of the parent synth. */
		float getAttribute(int attributeIndex) const;

		/** Adds a note on to the buffer. */
		void addNoteOn(int channel, int noteNumber, int velocity, int timeStampSamples);

		/** Adds a note off to the buffer. */
		void addNoteOff(int channel, int noteNumber, int timeStampSamples);

		/** Adds a controller to the buffer. */
		void addController(int channel, int number, int value, int timeStampSamples);

		/** Sets the internal clock speed. */
		void setClockSpeed(int clockSpeed);

		/** Stops the timer of the synth. You can call this also in the timer callback. */
		void stopTimer();

		/** Sets one of the eight macro controllers to the newValue.
		*
		*	@param macroIndex the index of the macro from 1 - 8
		*	@param newValue The range for the newValue is 0.0 - 127.0. 
		*/
		void setMacroControl(int macroIndex, float newValue);
		

		/** Sends a controller event to the synth.
		*
		*	The message will be only sent to the internal ModulatorChains (the MidiProcessorChain will be bypassed)
		*/
		void sendController(int controllerNumber, int controllerValue);

		/** Sends a controller event to all Child synths. Works only if the script sits in a ModulatorSynthChain. */
		void sendControllerToChildSynths(int controllerNumber, int controllerValue);

		/** Returns the number of child synths. Works with SynthGroups and SynthChains. */
		int getNumChildSynths() const;

		/** Sets a ModulatorAttribute.
		*
		*	@param chainId the chain where the Modulator is. GainModulation = 1, PitchModulation = 0
		*	@param modulatorIndex the index of the Modulator starting with 0.
		*	@param attributeIndex the index of the Modulator starting with 0. Intensity is '-12', Bypassed is '-13'
		*	@param newValue the value. The range for Gain is 0.0 - 1.0, the Range for Pitch is -12.0 ... 12.0
		*
		*/
		void setModulatorAttribute(int chainId, int modulatorIndex, int attributeIndex, float newValue);

		/** Returns the number of pressed keys (!= the number of playing voices!). */
		int getNumPressedKeys() const {return numPressedKeys; };

		/** Checks if any key is pressed. */
		bool isLegatoInterval() const { return numPressedKeys != 1; };

		/** Adds a Modulator to the synth's chain. If it already exists, it returns the index. */
		int addModulator(int chainId, const String &type, const String &id) const;

		typedef ScriptingObjects::ScriptingModulator ScriptModulator;

		/** Returns the Modulator with the supplied name. Can be only called in onInit. It looks also in all child processors. */
		ScriptModulator *getModulator(const String &name);

		typedef ScriptingObjects::ScriptingEffect ScriptEffect;

		/** Returns the Effect with the supplied name. Can only be called in onInit(). It looks also in all child processors. */
		ScriptEffect *getEffect(const String &name);

		typedef ScriptingObjects::ScriptingMidiProcessor ScriptMidiProcessor;

		/** Returns the MidiProcessor with the supplied name. Can not be the own name! */
		ScriptMidiProcessor * getMidiProcessor(const String &name);

		typedef ScriptingObjects::ScriptingSynth ScriptSynth;

		/** Returns the child synth with the supplied name. */
		ScriptSynth * getChildSynth(const String &name);

		typedef ScriptingObjects::ScriptingAudioSampleProcessor ScriptAudioSampleProcessor;

		/** Returns the child synth with the supplied name. */
		ScriptAudioSampleProcessor * getAudioSampleProcessor(const String &name);

		typedef ScriptingObjects::ScriptingTableProcessor ScriptTableProcessor;

		/** Returns the table processor with the given name. */
		ScriptTableProcessor *getTableProcessor(const String &name);

		/** Returns the sampler with the supplied name. */
		Sampler *getSampler(const String &name);

		/** Returns the index of the Modulator in the chain with the supplied chainId */
		int getModulatorIndex(int chainId, const String &id) const;


		/** Returns true if the sustain pedal is pressed. */
		bool isSustainPedalDown() const { return sustainState; }

		struct Wrapper
		{
			API_VOID_METHOD_WRAPPER_2(Synth, allowChildSynth);
			API_METHOD_WRAPPER_0(Synth, getNumChildSynths);
			API_VOID_METHOD_WRAPPER_1(Synth, addToFront);
			API_VOID_METHOD_WRAPPER_1(Synth, deferCallbacks);
			API_VOID_METHOD_WRAPPER_1(Synth, noteOff);
			API_VOID_METHOD_WRAPPER_2(Synth, playNote);
			API_VOID_METHOD_WRAPPER_2(Synth, setAttribute);
			API_METHOD_WRAPPER_1(Synth, getAttribute);
			API_VOID_METHOD_WRAPPER_4(Synth, addNoteOn);
			API_VOID_METHOD_WRAPPER_3(Synth, addNoteOff);
			API_VOID_METHOD_WRAPPER_4(Synth, addController);
			API_VOID_METHOD_WRAPPER_1(Synth, startTimer);
			API_VOID_METHOD_WRAPPER_0(Synth, stopTimer);
			API_VOID_METHOD_WRAPPER_2(Synth, setMacroControl);
			API_VOID_METHOD_WRAPPER_2(Synth, sendController);
			API_VOID_METHOD_WRAPPER_2(Synth, sendControllerToChildSynths);
			API_VOID_METHOD_WRAPPER_4(Synth, setModulatorAttribute);
			API_VOID_METHOD_WRAPPER_3(Synth, addModulator);
			API_METHOD_WRAPPER_1(Synth, getModulator);
			API_METHOD_WRAPPER_1(Synth, getAudioSampleProcessor);
			API_METHOD_WRAPPER_1(Synth, getTableProcessor);
			API_METHOD_WRAPPER_1(Synth, getSampler);
			API_METHOD_WRAPPER_1(Synth, getEffect);
			API_METHOD_WRAPPER_1(Synth, getMidiProcessor);
			API_METHOD_WRAPPER_1(Synth, getChildSynth);
			API_METHOD_WRAPPER_2(Synth, getModulatorIndex);
			API_METHOD_WRAPPER_0(Synth, getNumPressedKeys);
			API_METHOD_WRAPPER_0(Synth, isLegatoInterval);
			API_METHOD_WRAPPER_0(Synth, isSustainPedalDown);
			API_VOID_METHOD_WRAPPER_1(Synth, setClockSpeed);

#if 0
			static var allowChildSynth(const var::NativeFunctionArgs& args);
			static var getNumChildSynths(const var::NativeFunctionArgs& args);
			static var noteOff(const var::NativeFunctionArgs& args);
			static var deferCallbacks(const var::NativeFunctionArgs& args);
			static var addToFront(const var::NativeFunctionArgs& args);
			static var sendController(const var::NativeFunctionArgs& args);
			static var sendControllerToChildSynths(const var::NativeFunctionArgs& args);
			static var startTimer(const var::NativeFunctionArgs& args);
			static var stopTimer(const var::NativeFunctionArgs& args);
			static var getAttribute(const var::NativeFunctionArgs& args);
			static var setAttribute(const var::NativeFunctionArgs& args);
			static var addNoteOn(const var::NativeFunctionArgs& args);
			static var addNoteOff(const var::NativeFunctionArgs& args);
			static var addController(const var::NativeFunctionArgs& args);
			static var playNote(const var::NativeFunctionArgs& args);
			static var setMacroControl(const var::NativeFunctionArgs& args);
			static var getModulator(const var::NativeFunctionArgs& args);
			static var getEffect(const var::NativeFunctionArgs& args);
			static var getChildSynth(const var::NativeFunctionArgs& args);
			static var getMidiProcessor(const var::NativeFunctionArgs& args);
			static var setModulatorAttribute(const var::NativeFunctionArgs& args);
			static var addModulator(const var::NativeFunctionArgs& args);
			static var getModulatorIndex(const var::NativeFunctionArgs& args);
			static var getNumPressedKeys(const var::NativeFunctionArgs& args);
			static var isLegatoInterval(const var::NativeFunctionArgs& args);
			static var isSustainPedalDown(const var::NativeFunctionArgs& args);
			static var getAudioSampleProcessor(const var::NativeFunctionArgs& args);
			static var getTableProcessor(const var::NativeFunctionArgs& args);
			static var getSampler(const var::NativeFunctionArgs& args);
			static var setClockSpeed(const var::NativeFunctionArgs& args);
#endif
			
		};

		void increaseNoteCounter()
		{
			numPressedKeys++;
		}

		void decreaseNoteCounter()
		{
			numPressedKeys--;
			if(numPressedKeys < 0) numPressedKeys = 0;
		}

		void setSustainPedal(bool shouldBeDown)
		{
			sustainState = shouldBeDown;
		};

	private:

		OwnedArray<Message> artificialNoteOns;
		ModulatorSynth * const owner;
		int numPressedKeys;

		SelectedItemSet<WeakReference<ModulatorSamplerSound>> soundSelection;

		bool sustainState;
	};

	/** A set of handy function to debug the script. 
	*	@ingroup scriptingApi
	*	
	*
	*/
	class Console: public ApiClass,
				   public ScriptingObject
	{
	public:

		Console(ProcessorWithScriptingContent *p):
			ScriptingObject(p),
			ApiClass(0),
			startTime(0.0),
			benchmarkTitle(String::empty)
		{
			ADD_API_METHOD_1(print);
			ADD_API_METHOD_1(start);
			ADD_API_METHOD_0(stop);
		};

		Identifier getName() const override { RETURN_STATIC_IDENTIFIER("Console"); }
		static Identifier getClassName()   { RETURN_STATIC_IDENTIFIER("Console"); };

		/** Prints a message to the console. */
		void print(var debug);

		/** Starts the benchmark. You can give it a name that will be displayed with the result if desired. */
		void start(String title = String::empty) { startTime = Time::highResolutionTicksToSeconds(Time::getHighResolutionTicks()); };

		/** Stops the benchmark and prints the result. */
		void stop();
		
		struct Wrapper
		{
			API_VOID_METHOD_WRAPPER_1(Console, print);
			API_VOID_METHOD_WRAPPER_1(Console, start);
			API_VOID_METHOD_WRAPPER_0(Console, stop);
		};

		double startTime;
		String benchmarkTitle;
	};

	

	/** This is the interface area that can be filled with buttons, knobs, etc. 
	*	@ingroup scriptingApi
	*
	*/
	class Content: public ScriptingObject,
				   public DynamicObject,
				   public SafeChangeBroadcaster,
				   public RestorableObject
	{
	public:
		Content(ProcessorWithScriptingContent *p):
			ScriptingObject(p),
			height(50),
			width(-1),
			name(String::empty),
			allowGuiCreation(true),
			colour(Colour(0xff777777))
		{
			setMethod("addButton", Wrapper::addButton);
			setMethod("addKnob", Wrapper::addKnob);
			setMethod("addLabel", Wrapper::addLabel);
			setMethod("addComboBox", Wrapper::addComboBox);
			setMethod("addTable", Wrapper::addTable);
			setMethod("addImage", Wrapper::addImage);
			setMethod("addModulatorMeter", Wrapper::addModulatorMeter);
			setMethod("addPlotter", Wrapper::addPlotter);
			setMethod("addPanel", Wrapper::addPanel);
			setMethod("addAudioWaveform", Wrapper::addAudioWaveform);
			setMethod("addSliderPack", Wrapper::addSliderPack);
			setMethod("addPluginEditor", Wrapper::addPluginEditor);
			setMethod("setContentTooltip", Wrapper::setContentTooltip);
			setMethod("setToolbarProperties", Wrapper::setToolbarProperties);
			setMethod("setHeight", Wrapper::setHeight);
			setMethod("setWidth", Wrapper::setWidth);
			setMethod("setName", Wrapper::setName);
			setMethod("setPropertiesFromJSON", Wrapper::setPropertiesFromJSON);
			setMethod("storeAllControlsAsPreset", Wrapper::storeAllControlsAsPreset);
			setMethod("restoreAllControlsFromPreset", Wrapper::restoreAllControlsFromPreset);

			setMethod("setColour", Wrapper::setColour);
			
			setMethod("clear", Wrapper::clear);
		};

		~Content()
		{
			

			masterReference.clear();
			removeAllChangeListeners();
		}

		static Identifier getClassName()   { return "Content"; };

		class PluginParameterConnector
		{
		public:

			PluginParameterConnector():
				parameter(nullptr),
				nextUpdateIsDeactivated(false)
			{}

			virtual ~PluginParameterConnector() {};

			bool isConnected() const { return parameter != nullptr; };

			void setConnected(ScriptedControlAudioParameter *controllingParameter);

			void sendParameterChangeNotification(float newValue);

			void deactivateNextUpdate() { nextUpdateIsDeactivated = true; }

		private:

			ScriptedControlAudioParameter *parameter;

			bool nextUpdateIsDeactivated;
		};

		struct ScriptComponent : public RestorableObject,
								 public CreatableScriptObject,
								 public SafeChangeBroadcaster,
								 public AssignableObject,
								 public DebugableObject
		{
			enum Properties
			{
				text = 0,
				visible,
				enabled,
				x,
				y,
				width,
				height,
				min,
				max,
				tooltip,
				bgColour,
				itemColour,
				itemColour2,
				textColour,
				macroControl,
				zOrder,
				saveInPreset,
				isPluginParameter,
				numProperties
			};

			File getExternalFile(var newValue);

			ScriptComponent(ProcessorWithScriptingContent *base, Content *parentContent, Identifier name_, int x, int y, int width, int height);

			bool isPropertyDeactivated(Identifier &id) const
			{
				return deactivatedProperties.contains(id);
			}

			virtual ~ScriptComponent() {};

			virtual StringArray getOptionsFor(const Identifier &id);

			virtual ScriptCreatedComponentWrapper *createComponentWrapper(ScriptContentComponent *content, int index) = 0;

			virtual ValueTree exportAsValueTree() const override;;

			
			String getDebugValue() const override { return getValue().toString(); };

			String getDebugName() const override { return name.toString(); };

			String getDebugDataType() const override { return getObjectName().toString(); }

			/** This will be called if the user double clicks on the row. */
			virtual void doubleClickCallback(Component *componentToNotify) override;

			var getAssignedValue(int index) const override
			{
				return getScriptObjectProperty(index);
			}

			void assign(const int index, var newValue) override
			{
				setScriptObjectProperty(index, newValue);
			}

			int getCachedIndex(const var &indexExpression) const override
			{
				Identifier id(indexExpression.toString());

				for (int i = 0; i < getNumIds(); i++)
				{
					if (deactivatedProperties.contains(getIdFor(i))) continue;

					if (getIdFor(i) == id) return i;
				}

				return -1;
			}

			Identifier getObjectName() const override
			{
				jassertfalse;
				return Identifier();
			}

			virtual void setScriptObjectPropertyWithChangeMessage(const Identifier &id, var newValue, NotificationType notifyEditor=sendNotification);

			virtual bool isAutomatable() const { return false; }

			const Identifier getIdFor(int p) const
			{
				jassert(p < getNumIds());

				return propertyIds[p];
			}

			int getNumIds() const
			{
				return propertyIds.size();
			}

			const var getScriptObjectProperty(int p) const;

			String getScriptObjectPropertiesAsJSON() const;

			DynamicObject *getScriptObjectProperties() const { return componentProperties.get(); }

			Rectangle<int> getPosition() const;

			// API Methods =====================================================================================================

			/** returns the value of the property. */
			var get(String propertyName) const
			{
				return componentProperties->getProperty(Identifier(propertyName));
			}

			/** Sets the property. */
			void set(String propertyName, var value);;

			/** Returns the current value. */
			virtual var getValue() const { return value; }

			/** Sets the current value
			*
			*	It is safe to call this on the message callbacks, the control will be updated asynchronously.
			*	If you call this method within the onInit callback, it will not restore the value after compilation.
			*
			*	@param componentIndex the index of the component that was returned at the creation.
			*	@param newValue the new value.
			*/
			virtual void setValue(var newValue);

			/** Sets the current value from a range 0.0 ... 1.0. */
			virtual void setValueNormalized(double normalizedValue) { setValue(normalizedValue); };

			virtual double getValueNormalized() const { return getValue(); };

			/** sets the colour of the component (BG, IT1, IT2, TXT). */
			void setColour(int colourId, int colourAs32bitHex);

			/** Restores all properties from a JSON object. */
			void setPropertiesFromJSON(const var &jsonData);

			/** Sets the position of the component. */
			void setPosition(int x, int y, int w, int h);;

			/** Hides / Shows the control. */
			void showControl(bool shouldBeVisible);

			/** Shows a informative text on mouse hover. */
			void setTooltip(const String &tooltip);

			/** Adds the knob / button to a macro controller (from 0 to 7). */
			void addToMacroControl(int macroIndex);

			// End of API Methods ============================================================================================

			void setChanged(bool isChanged=true) noexcept { changed = isChanged; }

			bool isChanged() const noexcept { return changed; };

			Identifier getName() const
			{
				return name;
			}
			
			bool objectExists () const override { return true; };

			
			bool objectDeleted() const override 
			{ 
				return false;	// returns always false, since it should not be accessed after its lifetime.
			}

			virtual void restoreFromValueTree(const ValueTree &v) override 
			{ 
				value = v.getProperty("value", var::undefined()); 
			};

			
			var value;

			Identifier name;
			
			Content *parent;

			bool skipRestoring;

		protected:

			void setDefaultValue(int p, const var &defaultValue)
			{
				defaultValues.set(getIdFor(p), defaultValue);
				setScriptObjectProperty(p, defaultValue);
			}

			void setScriptObjectProperty(int p, var value) { componentProperties->setProperty(getIdFor(p), value); }
			
			Array<Identifier> propertyIds;

			Array<Identifier> deactivatedProperties;

			Array<Identifier> priorityProperties;

			ReferenceCountedObjectPtr<DynamicObject> componentProperties;

		private:

			NamedValueSet defaultValues;

			bool changed;
		};

		struct ScriptSlider: public ScriptComponent,
							 public PluginParameterConnector
		{
		public:

			enum Properties
			{
				Mode = ScriptComponent::Properties::numProperties,
				Style,
				stepSize,
				middlePosition,
				defaultValue,
				suffix,
				filmstripImage,
				numStrips,
				isVertical,
                scaleFactor,
				numProperties
			};

			ScriptSlider(ProcessorWithScriptingContent *base, Content *parentContent, Identifier name_, int x, int y, int, int);

			~ScriptSlider();

			Identifier 	getObjectName () const override { return "ScriptSlider"; }

			virtual bool isAutomatable() const { return true; }

			ScriptCreatedComponentWrapper *createComponentWrapper(ScriptContentComponent *content, int index) override;

			void setScriptObjectPropertyWithChangeMessage(const Identifier &id, var newValue, NotificationType notifyEditor=sendNotification) override;

			ValueTree exportAsValueTree() const override
			{
				ValueTree v = ScriptComponent::exportAsValueTree();

				v.setProperty("rangeMin", minimum, nullptr);
				v.setProperty("rangeMax", maximum, nullptr);

				return v;
			}

			void restoreFromValueTree(const ValueTree &v) override
			{
				ScriptComponent::restoreFromValueTree(v);

				minimum = v.getProperty("rangeMin", 0.0f);
				maximum = v.getProperty("rangeMax", 1.0f);

			}

			StringArray getOptionsFor(const Identifier &id) override;


			/** Set the value from a 0.0 to 1.0 range */
			void setValueNormalized(double normalizedValue) override;

			double getValueNormalized() const override;

			/** Sets the range and the step size of the knob. */
			void setRange(double min, double max, double stepSize);

			/** Sets the knob to the specified mode. */
			void setMode(String mode);

			/** Sets the value that is shown in the middle position. */
			void setMidPoint(double valueForMidPoint);

			/** Sets the style Knob, Horizontal, Vertical. */
			void setStyle(String style);;

			/** Sets the lower range end to the given value. */
			void setMinValue(double min) noexcept;

			/** Sets the upper range end to the given value. */
			void setMaxValue(double max) noexcept;

			/** Returns the lower range end. */
			double getMinValue() const;

			/** Returns the upper range end. */
			double getMaxValue() const;

			/** Checks if the given value is within the range. */
			bool contains(double value);
			
			HiSlider::Mode m;
			
			Slider::SliderStyle styleId;

			const Image *getImage() const { return image; };


		private:

			double minimum, maximum;

			Image const *image;

		};

		struct ScriptButton: public ScriptComponent
		{
			enum Properties
			{
				filmstripImage = ScriptComponent::Properties::numProperties,
				isVertical,
                scaleFactor,
				radioGroup,
				isPluginParameter,
				numProperties
			};

			~ScriptButton();

			ScriptButton(ProcessorWithScriptingContent *base, Content *parentContent, Identifier name, int x, int y, int, int);

			bool isAutomatable() const override { return true; }

			Identifier 	getObjectName () const override { return "ScriptButton"; }

			ScriptCreatedComponentWrapper *createComponentWrapper(ScriptContentComponent *content, int index) override;

			const Image *getImage() const { return image; };

			void setScriptObjectPropertyWithChangeMessage(const Identifier &id, var newValue, NotificationType notifyEditor = sendNotification) override;

			StringArray getOptionsFor(const Identifier &id) override;

		private:

			Image const *image;
		};

		struct ScriptComboBox: public ScriptComponent
		{
			enum Properties
			{
				Items = ScriptComponent::numProperties,
				isPluginParameter,
				numProperties
			};

			ScriptComboBox(ProcessorWithScriptingContent *base, Content *parentContent, Identifier name, int x, int y, int width, int);

			bool isAutomatable() const override { return true; }

			virtual Identifier 	getObjectName () const override { return "ScriptComboBox"; }

			ScriptCreatedComponentWrapper *createComponentWrapper(ScriptContentComponent *content, int index) override;

			void setScriptObjectPropertyWithChangeMessage(const Identifier &id, var newValue, NotificationType notifyEditor=sendNotification)
			{
				if(id == getIdFor(Items))
				{
					setScriptObjectProperty(Items, newValue);
					setScriptObjectProperty(max, getItemList().size());
				}

				ScriptComponent::setScriptObjectPropertyWithChangeMessage(id, newValue, notifyEditor);
			}

			StringArray getItemList() const
			{
				String items = getScriptObjectProperty(Items).toString();

				if(items.isEmpty()) return StringArray();

				StringArray sa;

				sa.addTokens(items, "\n", "");

				sa.removeEmptyStrings();

				return sa;
			}

			/** Returns the currently selected item text. */
			String getItemText() const;

			/** Adds an item to a combo box. */
			void addItem(const String &newName);

		private:
		};


		struct ScriptLabel: public ScriptComponent
		{
			enum Properties
			{
				FontName = ScriptComponent::Properties::numProperties,
				FontSize,
				FontStyle,
				Alignment,
				Editable,
				Multiline,
				numProperties
			};


			ScriptLabel(ProcessorWithScriptingContent *base, Content *parentContent, Identifier name, int x, int y, int width, int );

			virtual Identifier 	getObjectName () const override { return "ScriptLabel"; }

			ScriptCreatedComponentWrapper *createComponentWrapper(ScriptContentComponent *content, int index) override;

			StringArray getOptionsFor(const Identifier &id) override
			{
				StringArray sa;

				int index = propertyIds.indexOf(id);

				Font f("Arial", 13.0f, Font::plain);

				switch(index) 
				{
				case FontStyle:	sa.addArray(f.getAvailableStyles());
								break;
				case FontName:	sa.add("Default");
								sa.add("Oxygen");
								sa.add("Source Code Pro");
								sa.addArray(Font::findAllTypefaceNames());
								break;
				case Alignment: sa.add("left");
								sa.add("right");
								sa.add("top");
								sa.add("bottom");
								sa.add("centred");
								sa.add("centredTop");
								sa.add("centredBottom");
								sa.add("topLeft");
								sa.add("topRight");
								sa.add("bottomLeft");
								sa.add("bottomRight");
								break;
				default:		sa = ScriptComponent::getOptionsFor(id);
				}

				return sa;
			}

			Justification getJustification()
			{
				StringArray options = getOptionsFor(getIdFor(Alignment));

				String justAsString = getScriptObjectProperty(Alignment);
				int index = options.indexOf(justAsString);

				if(index == -1)
				{
					return Justification(Justification::centredLeft);
				}

				Array<Justification::Flags> justifications;
				justifications.ensureStorageAllocated(options.size());

				justifications.add(Justification::left);
				justifications.add(Justification::right);
				justifications.add(Justification::top);
				justifications.add(Justification::bottom);
				justifications.add(Justification::centred);
				justifications.add(Justification::centredTop);
				justifications.add(Justification::centredBottom);
				justifications.add(Justification::topLeft);
				justifications.add(Justification::topRight);
				justifications.add(Justification::bottomLeft);
				justifications.add(Justification::bottomRight);

				return justifications[index];
			}

			/** makes a label `editable`. 
			> This is a test.
			*/
			void setEditable(bool shouldBeEditable);

		};


		struct ScriptTable: public ScriptComponent
		{
			enum Properties
			{
				TableIndex = ScriptComponent::Properties::numProperties,
				ProcessorId,
				numProperties
			};

			ScriptTable(ProcessorWithScriptingContent *base, Content *parentContent,  Identifier name, int x, int y, int width, int height);

			virtual Identifier 	getObjectName () const override { return "ScriptTable"; }

			ScriptCreatedComponentWrapper *createComponentWrapper(ScriptContentComponent *content, int index) override;

			/** Returns the table value from 0.0 to 1.0 according to the input value from 0 to 127. */
			float getTableValue(int inputValue);

			void setScriptObjectPropertyWithChangeMessage(const Identifier &id, var newValue, NotificationType notifyEditor=sendNotification) override;

			StringArray getOptionsFor(const Identifier &id) override;

			/** Connects the table to an existing Processor. */
			void connectToOtherTable(const String &id, int index);

			ValueTree exportAsValueTree() const override
			{
				ValueTree v = ScriptComponent::exportAsValueTree();

				if (getTable() != nullptr) v.setProperty("data", getTable()->exportData(), nullptr);
				else jassertfalse;
				
				return v;
			}

			void restoreFromValueTree(const ValueTree &v) override
			{
				ScriptComponent::restoreFromValueTree(v);

				getTable()->restoreData(v.getProperty("data", String::empty));

				getTable()->sendChangeMessage();
				
			}

			Table *getTable()
			{
				return useOtherTable ? referencedTable.get() : ownedTable;
			}
			
			const Table *getTable() const
			{
				return useOtherTable ? referencedTable.get() : ownedTable;
			}


			LookupTableProcessor * getTableProcessor() const;

		private:

			ScopedPointer<MidiTable> ownedTable;

			WeakReference<Table> referencedTable;
			WeakReference<Processor> connectedProcessor;


			bool useOtherTable;

			int lookupTableIndex;
			
		};

		struct ScriptPanel: public ScriptComponent
		{
			enum Properties
			{
				borderSize = ScriptComponent::numProperties,
				borderRadius,
				allowCallbacks,
				numProperties
			};

			ScriptPanel(ProcessorWithScriptingContent *base, Content *parentContent, Identifier panelName, int x, int y, int width, int height);;

			virtual Identifier 	getObjectName () const override { return "ScriptPanel"; }

			StringArray getOptionsFor(const Identifier &id) override;

            var getValue() const override
            {
                
				return var::undefined();
            }
            
            void setValue(var newValue) override
            {
                
            }
            
			ScriptCreatedComponentWrapper *createComponentWrapper(ScriptContentComponent *content, int index) override;

		};

		struct ScriptImage: public ScriptComponent
		{
			enum Properties
			{
				Alpha = ScriptComponent::numProperties,
				FileName,
                Offset,
                Scale,
                AllowCallbacks,
				PopupMenuItems,
				PopupOnRightClick,
				numProperties
			};

			ScriptImage(ProcessorWithScriptingContent *base, Content *parentContent, Identifier imageName, int x, int y, int width, int height);;

			~ScriptImage();

			virtual Identifier 	getObjectName () const override { return "ScriptImage"; }

			virtual String getDebugValue() const override { return getScriptObjectProperty(Properties::FileName); }

			ScriptCreatedComponentWrapper *createComponentWrapper(ScriptContentComponent *content, int index) override;

			/** Sets the transparency (0.0 = full transparency, 1.0 = full opacity). */
			void setAlpha(float newAlphaValue) 
			{
				setScriptObjectPropertyWithChangeMessage(getIdFor(Alpha), newAlphaValue);
			};

			void setScriptObjectPropertyWithChangeMessage(const Identifier &id, var newValue, NotificationType notifyEditor=sendNotification) override;

			/** Sets the image file that will be displayed. If forceUseRealFile is true, then it will reload the file from disk instead of caching it (Disable this for finished interfaces!) */
			void setImageFile(const String &absoluteFileName, bool forceUseRealFile);

			StringArray getOptionsFor(const Identifier &id) override;

			const Image *getImage() const
			{
				return image;
			}
            
			StringArray getItemList() const
			{
				String items = getScriptObjectProperty(PopupMenuItems).toString();

				if (items.isEmpty()) return StringArray();

				StringArray sa;
				sa.addTokens(items, "\n", "");
				sa.removeEmptyStrings();

				return sa;
			}

			void setScriptProcessor(ProcessorWithScriptingContent *sb);

		private:

			Image const *image;

		};

		struct ScriptSliderPack : public ScriptComponent
		{
			enum Properties
			{
				SliderAmount = ScriptComponent::Properties::numProperties,
				StepSize,
				FlashActive,
				ShowValueOverlay,
                ProcessorId,
				numProperties
			};

			ScriptSliderPack(ProcessorWithScriptingContent *base, Content *parentContent, Identifier imageName, int x, int y, int width, int height);;

			~ScriptSliderPack()
			{

			};

			virtual Identifier 	getObjectName() const override { return "ScriptSliderPack"; }

			ScriptCreatedComponentWrapper *createComponentWrapper(ScriptContentComponent *content, int index) override;

			/** sets the slider value at the given index.*/
			void setSliderAtIndex(int index, double value);

			/** Returns the value at the given index. */
			double getSliderValueAt(int index);

			/** Sets all slider values to the given value. */
			void setAllValues(double value);

			/** Returns the number of sliders. */
			int getNumSliders() const;

            StringArray getOptionsFor(const Identifier &id) override;

			ValueTree exportAsValueTree() const override
			{
				ValueTree v = ScriptComponent::exportAsValueTree();

				v.setProperty("data", getSliderPackData()->toBase64(), nullptr);

				return v;
			}

			void restoreFromValueTree(const ValueTree &v) override
			{
				ScriptComponent::restoreFromValueTree(v);

				getSliderPackData()->fromBase64(v.getProperty("data", String::empty));
				getSliderPackData()->sendChangeMessage();
			}

			void setScriptObjectPropertyWithChangeMessage(const Identifier &id, var newValue, NotificationType notifyEditor = sendNotification) override;

			void setScriptProcessor(ProcessorWithScriptingContent *sb);

            SliderPackData *getSliderPackData() { return (existingData != nullptr) ? existingData.get() : packData.get(); };

            const SliderPackData *getSliderPackData() const { return (existingData != nullptr) ? existingData.get() : packData.get(); };
            
		private:

            void connectToOtherSliderPack(const String &otherPackId);
            
            ScopedPointer<SliderPackData> packData;
            
            WeakReference<SliderPackData> existingData;
		};

		struct ModulatorMeter: public ScriptComponent
		{
			enum Properties
			{
				ModulatorId = ScriptComponent::numProperties,
				numProperties
			};

			ModulatorMeter(ProcessorWithScriptingContent *base, Content *parentContent, Identifier modulatorName, int x, int y, int width, int height);;

			virtual Identifier 	getObjectName () const override { return "ModulatorMeter"; }

			ScriptCreatedComponentWrapper *createComponentWrapper(ScriptContentComponent *content, int index) override;

			void setScriptObjectPropertyWithChangeMessage(const Identifier &id, var newValue, NotificationType notifyEditor=sendNotification) override;

			StringArray getOptionsFor(const Identifier &id) override;

			void setScriptProcessor(ProcessorWithScriptingContent *sb);

			WeakReference<Modulator> targetMod;
		};


		struct ScriptedPlotter: public ScriptComponent
		{
		public:

			ScriptedPlotter(ProcessorWithScriptingContent *base, Content *parentContent, Identifier plotterName, int x, int y, int width, int height):
				ScriptComponent(base, parentContent, plotterName, x,y,width, height)
			{
				setMethod("addModulatorToPlotter", Wrapper::addModulatorToPlotter);
				setMethod("clearModulatorPlotter", Wrapper::clearModulatorToPlotter);
			};

			virtual Identifier 	getObjectName () const override { return "ScriptedPlotter"; }

			ScriptCreatedComponentWrapper *createComponentWrapper(ScriptContentComponent *content, int index) override;

			void addModulator(Modulator *m) {mods.add(m);};

			void clearModulators() {mods.clear();};

			/** Searches a processor and adds the modulator to the plotter. */
			void addModulatorToPlotter(String processorName, String modulatorName);

			/** Removes all modulators from the plotter. */
			void clearModulatorPlotter();

			Array<WeakReference<Modulator>> mods;

		};

		struct ScriptAudioWaveform: public ScriptComponent
		{
			enum Properties
			{
				processorId = ScriptComponent::numProperties,
				numProperties
			};

			ScriptAudioWaveform(ProcessorWithScriptingContent *base, Content *parentContent, Identifier plotterName, int x, int y, int width, int height);;

			virtual Identifier 	getObjectName() const override { return "ScriptAudioWaveform"; };
			
			ScriptCreatedComponentWrapper *createComponentWrapper(ScriptContentComponent *content, int index) override;

			void connectToAudioSampleProcessor(String processorId);

			void setScriptObjectPropertyWithChangeMessage(const Identifier &id, var newValue, NotificationType notifyEditor = sendNotification) override;

			ValueTree exportAsValueTree() const override;

			void restoreFromValueTree(const ValueTree &v) override;

			StringArray getOptionsFor(const Identifier &id) override;
			
			AudioSampleProcessor * getAudioProcessor()
			{
				return dynamic_cast<AudioSampleProcessor*>(connectedProcessor.get());
			};

		private:

			WeakReference<Processor> connectedProcessor;

		};


		struct ScriptPluginEditor : public ScriptComponent
		{
			enum Properties
			{
				processorId = ScriptComponent::numProperties,
				numProperties
			};

			ScriptPluginEditor(ProcessorWithScriptingContent *base, Content*parentContent, Identifier name, int x, int y, int width, int height);

			virtual Identifier getObjectName() const override 
			{ 
				static Identifier id("ScriptPluginEditor");
				return id;
			};

			void connectToAudioProcessorWrapper(String processorId);

			ScriptCreatedComponentWrapper *createComponentWrapper(ScriptContentComponent *content, int index) override;

			void setScriptObjectPropertyWithChangeMessage(const Identifier &id, var newValue, NotificationType notifyEditor = sendNotification) override;

			ValueTree exportAsValueTree() const override;

			void restoreFromValueTree(const ValueTree &v) override;

			StringArray getOptionsFor(const Identifier &id) override;

			AudioProcessorWrapper *getProcessor();

		private:

			WeakReference<Processor> connectedProcessor;
		};

		/** Adds a toggle button to the Content and returns the component index.
		*
		*	@param knobName the name for the knob. It should contain no whitespace
		*	@param x the x position
		*	@param y the y position
		*/
		ScriptButton *addButton(Identifier buttonName, int x, int y);

		/** Adds a knob to the Content and returns the component index.
		*
		*	@param knobName the name for the knob. It should contain no whitespace
		*	@param x the x position
		*	@param y the y position
		*/
		ScriptSlider *addKnob(Identifier knobName, int x, int y);

		/** Adds a table editor to the Content and returns the component index. */
		ScriptTable *addTable(Identifier tableName, int x, int y);

		/** Adds a comboBox to the Content and returns the component index. */
		ScriptComboBox *addComboBox(Identifier boxName, int x, int y);
		
		/** Adds a text input label. */
		ScriptLabel *addLabel(Identifier label, int x, int y);

		/** Adds a peak meter that displays the modulator's output. */
		ModulatorMeter *addModulatorMeter(Identifier modulatorName, int x, int y);

		/** Adds a plotter that plots multiple modulators. */
		ScriptedPlotter *addPlotter(Identifier plotterName, int x, int y);

		/** Adds a image to the script interface. */
		ScriptImage *addImage(Identifier imageName, int x, int y);

		/** Adds a panel (rectangle with border and gradient). */
		ScriptPanel *addPanel(Identifier panelName, int x, int y);

		/** Adds a audio waveform display. */
		ScriptAudioWaveform *addAudioWaveform(Identifier audioWaveformName, int x, int y);

		/** Adds a slider pack. */
		ScriptSliderPack *addSliderPack(Identifier sliderPackName, int x, int y);

		/** Adds a plugin editor window. */
		ScriptPluginEditor *addPluginEditor(Identifier pluginEditorName, int x, int y);

		void setPropertiesFromJSON(const Identifier &name, const var &jsonData)
		{
			for(int i = 0; i < components.size(); i++)
			{
				if(components[i]->getName() == name)
				{
					components[i]->setPropertiesFromJSON(jsonData);
				}
			}
		}

		/** Sets the colour for the panel. */
		void setColour(int red, int green, int blue) { colour = Colour((uint8)red, (uint8)green, (uint8)blue); };

		/** Sets the height of the content. */
		void setHeight(int newHeight) noexcept;

		/** Sets the height of the content. */
		void setWidth(int newWidth) noexcept;

		/** sets the Tooltip that will be shown if the mouse hovers over the script's tab button. */
		void setContentTooltip(const String &tooltipToShow)
		{
			tooltip = tooltipToShow;
		}

		/** Sets the main toolbar properties from a JSON object. */
		void setToolbarProperties(const var &toolbarProperties);

		/** Sets the name that will be displayed in big fat Impact. */
		void setName(const String &newName)	{ name = newName; };

		/** Saves all controls that should be saved into a XML data file. */
		void storeAllControlsAsPreset(const String &fileName);

		/** Restores all controls from a previously saved XML data file. */
		void restoreAllControlsFromPreset(const String &fileName);

		void restoreAllControlsFromPreset(const ValueTree &preset);

		Colour getColour() const {return colour;};

		void endInitialization();

		ValueTree exportAsValueTree() const override;

		void restoreFromValueTree(const ValueTree &v) override;

		bool isEmpty();

		int getNumComponents() const noexcept { return components.size(); };

		ScriptComponent *getComponent(int index)
		{
			if(index == -1) return nullptr;

			return components[index];
		};

		const ScriptComponent *getComponent(int index) const {return components[index];};

		ScriptComponent * getComponentWithName(const Identifier &componentName);

		const ScriptComponent * getComponentWithName(const Identifier &componentName) const;

	private:

		struct Wrapper
		{
			static var addButton (const var::NativeFunctionArgs& args);
			static var addKnob (const var::NativeFunctionArgs& args);
			static var addLabel (const var::NativeFunctionArgs& args);
			static var addComboBox (const var::NativeFunctionArgs& args);
			static var addTable (const var::NativeFunctionArgs& args);
			static var addImage (const var::NativeFunctionArgs& args);
			static var addModulatorMeter (const var::NativeFunctionArgs& args);
			static var addPlotter (const var::NativeFunctionArgs& args);
			static var addModulatorToPlotter (const var::NativeFunctionArgs& args);
			static var addPanel (const var::NativeFunctionArgs& args);
			static var addAudioWaveform(const var::NativeFunctionArgs& args);
			static var addSliderPack(const var::NativeFunctionArgs& args);
			static var addPluginEditor(const var::NativeFunctionArgs& args);
			static var set (const var::NativeFunctionArgs& args);
			static var get (const var::NativeFunctionArgs& args);
			static var clearModulatorToPlotter (const var::NativeFunctionArgs& args);
			static var addToMacroControl (const var::NativeFunctionArgs& args);
			static var setRange (const var::NativeFunctionArgs& args);
			static var setMode (const var::NativeFunctionArgs& args);
			static var setStyle (const var::NativeFunctionArgs& args);
			static var setPropertiesFromJSON (const var::NativeFunctionArgs& args);
			static var storeAllControlsAsPreset(const var::NativeFunctionArgs& args);
			static var restoreAllControlsFromPreset(const var::NativeFunctionArgs& args);
			static var setMidPoint (const var::NativeFunctionArgs& args);
			static var setValue (const var::NativeFunctionArgs& args);
			static var setPosition (const var::NativeFunctionArgs& args);
			static var setHeight (const var::NativeFunctionArgs& args);
			static var setWidth (const var::NativeFunctionArgs& args);
			static var setName (const var::NativeFunctionArgs& args);
			static var addItem (const var::NativeFunctionArgs& args);
			static var setColour (const var::NativeFunctionArgs& args);
			static var setTooltip (const var::NativeFunctionArgs& args);
			static var setContentTooltip (const var::NativeFunctionArgs& args);
			static var setToolbarProperties(const var::NativeFunctionArgs& args);
			static var setImageFile (const var::NativeFunctionArgs& args);
			static var setImageAlpha (const var::NativeFunctionArgs& args);
			static var showControl (const var::NativeFunctionArgs& args);
			static var getValue (const var::NativeFunctionArgs& args);
			static var getItemText (const var::NativeFunctionArgs& args);
			static var getTableValue(const var::NativeFunctionArgs& args);
			static var connectToOtherTable(const var::NativeFunctionArgs& args);
			static var connectToAudioSampleProcessor(const var::NativeFunctionArgs& args);
			static var setEditable (const var::NativeFunctionArgs& args);
			static var clear (const var::NativeFunctionArgs& args);
			static var setValueNormalized(const var::NativeFunctionArgs& args);;
			static var getValueNormalized(const var::NativeFunctionArgs& args);;
			static var setSliderAtIndex(const var::NativeFunctionArgs& args);
			static var getSliderValueAt(const var::NativeFunctionArgs& args);
			static var setAllValues(const var::NativeFunctionArgs& args);
			static var getNumSliders(const var::NativeFunctionArgs& args);
			static var setMinValue (const var::NativeFunctionArgs& args);
			static var setMaxValue (const var::NativeFunctionArgs& args);
			static var getMinValue (const var::NativeFunctionArgs& args);
			static var getMaxValue (const var::NativeFunctionArgs& args);
			static var contains (const var::NativeFunctionArgs& args);
		};


		template<class Subtype> Subtype *addComponent(Identifier name, int x, int y, int width=-1, int height=-1);
		


		friend class ScriptContentComponent;
		friend class WeakReference<ScriptingApi::Content>;
		WeakReference<ScriptingApi::Content>::Master masterReference;

		
		CriticalSection lock;
		bool allowGuiCreation;
		int width, height;
		ReferenceCountedArray<ScriptComponent> components; // This is ref counted to allow anonymous controls
		Colour colour;
		String name;
		String tooltip;
	};

#if 0
	/** This is only for the autocomplete documentation. */
	class Math : public ApiClass,
				 public ScriptingObject
	{
	public:
		Math() :
		ApiClass(2)
		{
		}

		Identifier getName() const override { static const Identifier i("Math"); return i; }

		/** Returns a random number between 0.0 and 1.0. */
		var random()
		{
			return Random::getSystemRandom().nextDouble();
		}

		/** Returns a random integer between the low and the high values. */
		var randInt(var low, var high)
		{
			return Random::getSystemRandom().nextInt(Range<int>((int)low, (int)high));
		}

		/** Returns the absolute (unsigned) value. */
		var abs(var value)
		{
			return value.isInt() ? var(std::abs((int)value)) :
				var(std::abs((double)value));
		}

		/** Rounds the value to the next integer. */
		var round(var value)
		{
			return value.isInt() ? var(roundToInt((int)value)) :
				var(roundToInt((double)value));
		}

		/** Returns the sign of the value. */
		var sign(var value)
		{
			return var::undefined();
		}

		/** Limits the value to the given range. */
		var range(var value, var lowerLimit, var upperLimit)
		{
			return value.isInt() ? var(jlimit<int>(lowerLimit, upperLimit, value)) :
				var(jlimit<double>(lowerLimit, upperLimit, value));
		}

		/** Returns the smaller number. */
		var min(var first, var second)
		{
			return (first.isInt() && second.isInt()) ? var(jmin((int)first, (int)second)) :
				var(jmin((double)first, (double)second));
		}

		/** Returns the bigger number. */
		var max(var first, var second)
		{
			return (first.isInt() && second.isInt()) ? var(jmax((int)first, (int)second)) :
				var(jmax((double)first, (double)second));
		}

		/** Converts radian (0...2PI) to degree (0...360). */
		var toDegrees(var value) { return radiansToDegrees((double)value); }

		/** Converts degree  (0...360) to radian (0...2PI). */
		var toRadians(var value) { return degreesToRadians((double)value); }

		/** Calculates the sine value (radian based). */
		var sin(var value) { return std::sin((double)value); }

		/** Calculates the asine value (radian based). */
		var asin(var value) { return std::asin((double)value); }

		/** Calculates the cosine value (radian based). */
		var cos(var value) { return std::cos((double)value); }

		/** Calculates the acosine value (radian based). */
		var acos(var value) { return std::acos((double)value); }

		/** Calculates the sinh value (radian based). */
		var sinh(var value) { return std::sinh((double)value); }

		/** Calculates the asinh value (radian based). */
		var asinh(var value) { return std::asinh((double)value); }

		/** Calculates the cosh value (radian based). */
		var cosh(var value) { return std::cosh((double)value); }

		/** Calculates the acosh value (radian based). */
		var acosh(var value) { return std::acosh((double)value); }

		/** Calculates the tan value (radian based). */
		var tan(var value) { return std::tan((double)value); }

		/** Calculates the tanh value (radian based). */
		var tanh(var value) { return std::tanh((double)value); }

		/** Calculates the atan value (radian based). */
		var atan(var value) { return std::atan((double)value); }

		/** Calculates the atanh value (radian based). */
		var atanh(var value) { return std::atanh((double)value); }

		/** Calculates the log value (with base E). */
		var log(var value) { return std::log((double)value); }

		/** Calculates the log value (with base 10). */
		var log10(var value) { return std::log10((double)value); }

		/** Calculates the exp value. */
		var exp(var value) { return std::exp((double)value); }

		/** Calculates the power of base and exponent. */
		var pow(var base, var exp) { return std::pow((double)base, (double)exp); }

		/** Calculates the square (xx) of the value. */
		var sqr(var value) { double x = (double)value; return x * x; }

		/** Calculates the square root of the value. */
		var sqrt(var value) { return std::sqrt((double)value); }

		/** Rounds up the value. */
		var ceil(var value) { return std::ceil((double)value); }

		/** Rounds down the value. */
		var floor(var value) { return std::floor((double)value); }

		
	};

#endif
};


#endif