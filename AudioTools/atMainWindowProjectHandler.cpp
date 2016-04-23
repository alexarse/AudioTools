//
//  atMainWindowProjectHandler.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-22.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "PyoAudio.h"
#include "atEditorMainWindow.h"
#include "atMainWindowProjectHandler.h"

#include "atArchive.h"
#include <boost/filesystem.hpp>

namespace at {
namespace editor {
	MainWindowProjectHandler::MainWindowProjectHandler(MainWindow* main_window)
		: _main_window(main_window)
	{
	}

	void MainWindowProjectHandler::SaveCurrentProject()
	{
		if (!_main_window->_project.IsProjectOpen()) {
			ax::Error("No project is currently open.");
			return;
		}

		PyoAudio::GetInstance()->StopServer();
		_main_window->_gridWindow->SaveLayout(
			_main_window->_project.GetLayoutPath(), _main_window->_project.GetScriptPath());
		_main_window->_bottom_section->SaveFile(_main_window->_project.GetScriptPath());

		_main_window->_project.Save();
	}

	void MainWindowProjectHandler::OnSaveProject(const ax::Event::StringMsg& msg)
	{
		SaveCurrentProject();
	}

	void MainWindowProjectHandler::OnSaveAsProject(const ax::Event::StringMsg& msg)
	{
		std::string project_path(msg.GetMsg());
		boost::filesystem::path filepath(project_path);

		// Check file extension.
		std::string ext = filepath.extension().string();

		if (ext.empty()) {
			ax::Print("Empty extension");
			// project_path;// += ".atproj";
		}
		else if (ext == ".atproj") {
			/// @todo Remove extension.
			ax::Print("atproj extension");
			return;
		}
		else {
			ax::Print("extension :", ext);
			ax::Error("incorrect file extension :", ext);
			return;
		}

		PyoAudio::GetInstance()->StopServer();

		filepath = boost::filesystem::path(project_path);

		// Check if file exist.
		if (boost::filesystem::exists(filepath)) {
			/// @todo Manage this case with message box.
			ax::Error("File", filepath.string(), "already exist.");
			return;
		}

		// Check is a project is already open.
		if (!_main_window->_project.IsProjectOpen()) {
			ax::Error("No project is currently open.");
			return;
		}

		// Save layout to temporary file.
		_main_window->_gridWindow->SaveLayout(
			_main_window->_project.GetLayoutPath(), _main_window->_project.GetScriptPath());

		// Save script to temporary file.
		_main_window->_bottom_section->SaveFile(_main_window->_project.GetScriptPath());

		// Save as new project.
		_main_window->_project.SaveAs(project_path);

		// Close current project.
		_main_window->_project.Close();

		// Open newly saved project.
		_main_window->_project.Open(project_path + ".atproj");

		// Assign new name to status bar.
		_main_window->_statusBar->SetLayoutFilePath(_main_window->_project.GetProjectName());
	}

	void MainWindowProjectHandler::OnOpenProject(const ax::Event::StringMsg& msg)
	{
		const std::string project_path(msg.GetMsg());
		boost::filesystem::path filepath(project_path);

		// Check is empty.
		if (project_path.empty()) {
			ax::Error("Project path is empty.");
		}

		// Check if file exist.
		if (!boost::filesystem::exists(filepath)) {
			ax::Error("File", filepath.string(), "doesn't exist.");
			return;
		}

		// Check file extension.
		if (filepath.extension() != ".atproj") {
			ax::Error("Wrong project file format.");
			return;
		}

		// Stop audio server.
		PyoAudio::GetInstance()->StopServer();

		// Close current project.
		if (_main_window->_project.IsProjectOpen()) {
			_main_window->_project.Close();
		}

		// Open new project.
		_main_window->_project.Open(project_path);

		// Check is project is valid.
		if (!_main_window->_project.IsProjectOpen()) {
			ax::Error("Can't open project :", project_path);

			/// @todo Load empty project.
			return;
		}

		// Remove selected widget from right side menu.
		_main_window->_selected_windows.clear();
		_main_window->_right_menu->SetInspectorHandle(nullptr);

		// Open project layout.
		_main_window->_gridWindow->OpenLayout(_main_window->_project.GetLayoutPath());

		// Assign project label to status bar.
		_main_window->_statusBar->SetLayoutFilePath(_main_window->_project.GetProjectName());

		// Assign script content to text editor.
		_main_window->_bottom_section->OpenFile(_main_window->_project.GetScriptPath());

		// Check if layout has a MainWindow panel.
		if (_main_window->_gridWindow->GetMainWindow() == nullptr) {
			_main_window->_left_menu->SetOnlyMainWindowWidgetSelectable();
		}
		else {
			_main_window->_left_menu->SetAllSelectable();
		}
	}

	void MainWindowProjectHandler::OnCreateNewProject(const ax::Event::StringMsg& msg)
	{
		std::string project_path(msg.GetMsg());
		boost::filesystem::path filepath(project_path);

		// Check file extension.
		std::string ext = filepath.extension().string();

		if (ext.empty()) {
			//			ax::Print("Empty extension");
		}
		else if (ext == ".atproj") {
			/// @todo Remove extension.
			ax::Print("atproj extension");
			return;
		}
		else {
			ax::Print("extension :", ext);
			ax::Error("incorrect file extension :", ext);
			return;
		}

		PyoAudio::GetInstance()->StopServer();

		filepath = boost::filesystem::path(project_path);

		// Check if file exist.
		if (boost::filesystem::exists(filepath)) {
			/// @todo Manage this case with message box.
			ax::Error("File", filepath.string(), "already exist.");
			return;
		}

		// Check is a project is already open.
		if (_main_window->_project.IsProjectOpen()) {
			_main_window->_project.Close();
			//			ax::Error("No project is currently open.");
			//			return;
		}

		at::FileArchive archive;
		archive.Open(filepath.string() + ".atproj");

		std::string name = filepath.filename().string();
		archive.AddDirectory(name);

		std::vector<std::pair<std::string, std::vector<char>>> data;
		ax::StringVector proj_files = { "layouts/empty.xml", "scripts/empty.py" };
		ax::StringVector p_saved_name = { "layout.xml", "script.py" };

		for (int i = 0; i < proj_files.size(); i++) {
			std::ifstream f_path(proj_files[i], std::ios::binary | std::ios::ate);
			std::ifstream::pos_type pos = f_path.tellg();

			std::vector<char> buffer(pos);

			f_path.seekg(0, std::ios::beg);
			f_path.read(&buffer[0], pos);

			data.push_back(std::pair<std::string, std::vector<char>>(name + "/" + p_saved_name[i], buffer));
		}

		for (auto& n : data) {
			archive.AddFileContent(n.first, (void*)n.second.data(), (unsigned int)n.second.size());
		}

		archive.Close();
		_main_window->_project.Open(filepath.string() + ".atproj");

		// Check is project is valid.
		if (!_main_window->_project.IsProjectOpen()) {
			ax::Error("Can't open project :", project_path);

			/// @todo Load empty project.
			return;
		}

		// Remove selected widget from right side menu.
		_main_window->_selected_windows.clear();
		_main_window->_right_menu->SetInspectorHandle(nullptr);

		// Open project layout.
		_main_window->_gridWindow->OpenLayout(_main_window->_project.GetLayoutPath());

		// Assign project label to status bar.
		_main_window->_statusBar->SetLayoutFilePath(_main_window->_project.GetProjectName());

		// Assign script content to text editor.
		_main_window->_bottom_section->OpenFile(_main_window->_project.GetScriptPath());

		// Check if layout has a MainWindow panel.
		if (_main_window->_gridWindow->GetMainWindow() == nullptr) {
			_main_window->_left_menu->SetOnlyMainWindowWidgetSelectable();
		}
		else {
			_main_window->_left_menu->SetAllSelectable();
		}
	}
}
}