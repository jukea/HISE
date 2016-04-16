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

#ifndef BACKENDAPPLICATIONCOMMANDS_H_INCLUDED
#define BACKENDAPPLICATIONCOMMANDS_H_INCLUDED

class BackendProcessorEditor;

class BackendCommandTarget: public ApplicationCommandTarget,
							  public MenuBarModel
{
public:

	BackendCommandTarget(BackendProcessor *owner);

	enum MenuNames
	{
		FileMenu = 0,
		EditMenu,
		ToolsMenu,
		ViewMenu,
		HelpMenu,
		numMenuNames
	};

	enum ColumnMode
	{
		OneColumn,
		TwoColumns,
		ThreeColumns
	};

	/** All application commands are collected here. */
	enum MainToolbarCommands
	{
		ModulatorList = 0x10000,

		CustomInterface,
		DebugPanel,
		ViewPanel,
		Mixer,
		Macros,
		Keyboard,
		Settings,
		numToolbarButtons,
		MenuFileOffset = 0x20000,
		MenuNewFile,
		MenuOpenFile,
		MenuOpenFileFromProjectOffset,
		MenuSaveFile = 0x23000,
		MenuSaveFileAsXmlBackup,
		MenuOpenXmlBackup,
        MenuFileXmlBackupMenuOffset,
		MenuProjectNew = 0x24000,
		MenuProjectLoad,
		MenuCloseProject,
		MenuProjectShowInFinder,
		MenuProjectRecentOffset,
		MenuRevertFile = 0x26000,
        MenuFileSaveUserPreset,
        MenuFileUserPresetMenuOffset,
		MenuFileSettingsProject = 0x28000,
		MenuFileSettingsPreset,
		MenuFileSettings,
		MenuFileSettingsCompiler,
		MenuFileSettingsUser,
		MenuFileSettingCheckSanity,
		MenuReplaceWithClipboardContent,
		MenuExportFileAsPlugin,
		MenuExportFileAsPlayerLibrary,
		MenuFileQuit,
		MenuEditOffset = 0x30000,
		MenuEditCopy,
		MenuEditPaste,
		MenuViewShowSelectedProcessorInPopup,
        MenuEditPlotModulator,
		MenuEditCreateScriptVariable,
        MenuEditCloseAllChains,
		MenuViewOffset = 0x40000,
        MenuViewFullscreen,
		MenuViewBack,
		MenuViewForward,
		MenuViewSetMainContainerAsRoot,
		MenuOneColumn,
		MenuTwoColumns,
		MenuThreeColumns,
		MenuViewShowPool,
		MenuViewShowInspector,
		MenuViewShowPluginPopupPreview,
		MenuAddView,
		MenuDeleteView,
		MenuRenameView,
        MenuViewSaveCurrentView,
        MenuViewRemoveAllSoloProcessors,
        MenuViewShowAllHiddenProcessors,
        MenuViewListOffset = 0x70000,
		MenuViewProcessorListOffset = 0x80000,
		MenuToolsRecompile = 0x50000,
        MenuToolsCheckDuplicate,
        MenuToolsClearConsole,
		MenuToolsSetCompileTimeOut,
		MenuToolsUseBackgroundThreadForCompile,
		MenuToolsRecompileScriptsOnReload,
		MenuToolsExternalScriptFileOffset,
		MenuToolsResolveMissingSamples = 0x60000,
		MenuToolsDeleteMissingSamples,
		MenuToolsUseRelativePaths,
		MenuToolsCollectExternalFiles,
        MenuToolsRedirectSampleFolder,
		MenuToolsCreateRSAKeys,
		MenuToolsCreateDummyLicenceFile,
		MenuHelpShowAboutPage,
        MenuHelpCheckVersion,
		numCommands
	};

	virtual ~BackendCommandTarget()
	{
		mainCommandManager->setFirstCommandTarget(nullptr);
	};

	ApplicationCommandTarget* getNextCommandTarget() override
	{
		return findFirstTargetParentComponent();
	};

	void setCommandTarget(ApplicationCommandInfo &result, const String &name, bool active, bool ticked, char shortcut, bool useShortCut=true, ModifierKeys mod=ModifierKeys::commandModifier) {
		result.setInfo(name, name, "Target", 0);
		result.setActive(active); 
		result.setTicked(ticked);

		if (useShortCut) result.addDefaultKeypress(shortcut, mod);
	};

	bool clipBoardNotEmpty() const { return SystemClipboard::getTextFromClipboard().isNotEmpty(); }
    
    bool viewActive() const
    {
        return owner->synthChain->getCurrentViewInfo() != nullptr;
    }

	
	void setEditor(BackendProcessorEditor *editor);

	void getAllCommands(Array<CommandID>& commands) override;;

	void getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) override;;

	bool perform(const InvocationInfo &info) override;
	
	void updateCommands()
	{
		mainCommandManager->commandStatusChanged();
		createMenuBarNames();

		menuItemsChanged();
	}

	void setCopyPasteTarget(CopyPasteTarget *newTarget)
	{
        
        
		if (currentCopyPasteTarget.get() != nullptr)
		{
			currentCopyPasteTarget->deselect();
		}
		else
		{
			mainCommandManager->setFirstCommandTarget(this);
		}
        
		currentCopyPasteTarget = newTarget;

		updateCommands();
	}


	void createMenuBarNames();

	StringArray getMenuBarNames() override { return menuNames; };
	PopupMenu getMenuForIndex(int topLevelMenuIndex, const String &menuName) override;
	void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;;

	ColumnMode getColumnMode() const noexcept { return currentColumnMode; }

private:


	class Actions
	{
	public:

		static bool hasProcessorInClipboard();
		static void openFile(BackendProcessorEditor *bpe);
		static void saveFile(BackendProcessorEditor *bpe);
		static void replaceWithClipboardContent(BackendProcessorEditor *bpe);
		static void createScriptVariableDeclaration(CopyPasteTarget *currentCopyPasteTarget);
		static void recompileAllScripts(BackendProcessorEditor * bpe);
        static void toggleFullscreen(BackendProcessorEditor * bpe);
        static void addView(BackendProcessorEditor *bpe);
        static void deleteView(BackendProcessorEditor *bpe);
        static void saveView(BackendProcessorEditor *bpe);
        static void renameView(BackendProcessorEditor *bpe);
        static void closeAllChains(BackendProcessorEditor *bpe);
        static void checkDuplicateIds(BackendProcessorEditor *bpe);
		static void showAboutPage(BackendProcessorEditor * bpe);
        static void checkVersion(BackendProcessorEditor *bpe);
		static void setColumns(BackendProcessorEditor * bpe, BackendCommandTarget* target, ColumnMode columns);
		static void showProcessorInPopup(BackendProcessorEditor * bpe, BetterProcessorEditor* editor);
   		static void plotModulator(CopyPasteTarget *currentCopyPasteTarget);
		static void resolveMissingSamples(BackendProcessorEditor *bpe);
		static void deleteMissingSamples(BackendProcessorEditor *bpe);
		static void setCompileTimeOut(BackendProcessorEditor * bpe);
		static void toggleUseBackgroundThreadsForCompiling(BackendProcessorEditor * bpe);
		static void toggleCompileScriptsOnPresetLoad(BackendProcessorEditor * bpe);
		static void createNewProject(BackendProcessorEditor *bpe);
		static void loadProject(BackendProcessorEditor *bpe);
		static void closeProject(BackendProcessorEditor *bpe);
		static void showProjectInFinder(BackendProcessorEditor *bpe);
        static void saveUserPreset(BackendProcessorEditor *bpe);
        static void loadUserPreset(BackendProcessorEditor *bpe, const File &fileToLoad);
		static void toggleRelativePath(BackendProcessorEditor * bpe);
		static void collectExternalFiles(BackendProcessorEditor * bpe);
		static void saveFileAsXml(BackendProcessorEditor * bpe);
		static void openFileFromXml(BackendProcessorEditor * bpe, const File &fileToLoad);
        static void redirectSampleFolder(BackendProcessorEditor *bpe);
		static void showFilePresetSettings(BackendProcessorEditor * bpe);
		static void showFileProjectSettings(BackendProcessorEditor * bpe);
		static void showFileUserSettings(BackendProcessorEditor * bpe);
		static void showFileCompilerSettings(BackendProcessorEditor * bpe);
		static void checkSettingSanity(BackendProcessorEditor * bpe);
		static void togglePluginPopupWindow(BackendProcessorEditor * bpe);
		static void createRSAKeys(BackendProcessorEditor * bpe);
		static void createDummyLicenceFile(BackendProcessorEditor * bpe);
	};

	ColumnMode currentColumnMode;

	BackendProcessor *owner;

	StringArray menuNames;

	WeakReference<CopyPasteTarget> currentCopyPasteTarget;

	BackendProcessorEditor *bpe;

	ApplicationCommandManager *mainCommandManager;

	int rootY;

	Array<File> recentFileList;

};


#endif  // BACKENDAPPLICATIONCOMMANDS_H_INCLUDED
