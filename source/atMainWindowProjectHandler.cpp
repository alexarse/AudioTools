//
//  atMainWindowProjectHandler.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-22.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "atMainWindowProjectHandler.h"
#include "PyoAudio.h"
#include "editor/atEditorMainWindow.hpp"

#include "project/atArchive.hpp"
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
			ax::console::Error("No project is currently open.");
			return;
		}

		PyoAudio::GetInstance()->StopServer();
		_main_window->_gridWindow->SaveLayout(
			_main_window->_project.GetLayoutPath(), _main_window->_project.GetScriptPath());
		_main_window->_bottom_section->SaveFile(_main_window->_project.GetScriptPath());

		_main_window->_project.Save();
	}

	void MainWindowProjectHandler::OnSaveProject(const ax::event::StringMsg& msg)
	{
		SaveCurrentProject();
	}

	void MainWindowProjectHandler::OnSaveAsProject(const ax::event::StringMsg& msg)
	{
		std::string project_path(msg.GetMsg());
		boost::filesystem::path filepath(project_path);

		// Check file extension.
		std::string ext = filepath.extension().string();

		if (ext.empty()) {
			ax::console::Print("Empty extension");
			// project_path;// += ".atproj";
		}
		else if (ext == ".atproj") {
			/// @todo Remove extension.
			ax::console::Print("atproj extension");
			return;
		}
		else {
			ax::console::Print("extension :", ext);
			ax::console::Error("incorrect file extension :", ext);
			return;
		}

		PyoAudio::GetInstance()->StopServer();

		filepath = boost::filesystem::path(project_path);

		// Check if file exist.
		if (boost::filesystem::exists(filepath)) {
			/// @todo Manage this case with message box.
			ax::console::Error("File", filepath.string(), "already exist.");
			return;
		}

		// Check is a project is already open.
		if (!_main_window->_project.IsProjectOpen()) {
			ax::console::Error("No project is currently open.");
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

	bool MainWindowProjectHandler::IsProjectPathValid(const std::string& project_path)
	{
		boost::filesystem::path filepath(project_path);

		// Check is empty.
		if (project_path.empty()) {
			ax::console::Error("Project path is empty.");
			return false;
		}

		// Check if file exist.
		if (!boost::filesystem::exists(filepath)) {
			ax::console::Error("File", filepath.string(), "doesn't exist.");
			return false;
		}

		// Check file extension.
		if (filepath.extension() != ".atproj") {
			ax::console::Error("Wrong project file format.");
			return false;
		}

		return true;
	}

	bool MainWindowProjectHandler::IsNewProjectPathValid(const std::string& project_path)
	{
		boost::filesystem::path filepath(project_path);

		// Check file extension.
		std::string ext = filepath.extension().string();

		if (ext.empty()) {
			//			ax::console::Print("Empty extension");
		}
		else if (ext == ".atproj") {
			/// @todo Remove extension.
			ax::console::Print("atproj extension");
			return false;
		}
		else {
			ax::console::Print("extension :", ext);
			ax::console::Error("incorrect file extension :", ext);
			return false;
		}

		// filepath = boost::filesystem::path(project_path);

		// Check if file exist.
		if (boost::filesystem::exists(filepath)) {
			/// @todo Manage this case with message box.
			ax::console::Error("File", filepath.string(), "already exist.");
			return false;
		}

		return true;
	}

	bool MainWindowProjectHandler::OpenProject(const std::string& project_path)
	{
		if (!IsProjectPathValid(project_path)) {
			return false;
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
			ax::console::Error("Can't open project :", project_path);

			/// @todo Load empty project.
			return false;
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

		// Check if layout has a MainWindow panel.ed
		if (_main_window->_gridWindow->GetMainWindow() == nullptr) {
			_main_window->_left_menu->SetOnlyMainWindowWidgetSelectable();
			_main_window->GetWindow()->PushEvent(
				MainWindow::HAS_WIDGET_ON_GRID, new ax::event::SimpleMsg<bool>(false));
		}
		else {
			_main_window->_left_menu->SetAllSelectable();
			_main_window->GetWindow()->PushEvent(
				MainWindow::HAS_WIDGET_ON_GRID, new ax::event::SimpleMsg<bool>(true));
		}

		return true;
	}

	bool MainWindowProjectHandler::CreateProject(const std::string& project_path)
	{
		if (!IsNewProjectPathValid(project_path)) {
			return false;
		}

		PyoAudio::GetInstance()->StopServer();

		boost::filesystem::path filepath = boost::filesystem::path(project_path);

		// Check is a project is already open.
		if (_main_window->_project.IsProjectOpen()) {
			_main_window->_project.Close();
		}

		at::FileArchive archive;
		archive.Open(filepath.string() + ".atproj");

		std::string name = filepath.filename().string();
		archive.AddDirectory(name);

		std::vector<std::pair<std::string, std::vector<char>>> data;
		std::vector<std::string> proj_files = { "layouts/empty.xml", "scripts/empty.py" };
		std::vector<std::string> p_saved_name = { "layout.xml", "script.py" };

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
			ax::console::Error("Can't open project :", project_path);

			/// @todo Load empty project.
			return false;
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
			_main_window->GetWindow()->PushEvent(
				MainWindow::HAS_WIDGET_ON_GRID, new ax::event::SimpleMsg<bool>(false));
		}
		else {
			_main_window->_left_menu->SetAllSelectable();
			_main_window->GetWindow()->PushEvent(
				MainWindow::HAS_WIDGET_ON_GRID, new ax::event::SimpleMsg<bool>(true));
		}

		return true;
	}

	void MainWindowProjectHandler::OnOpenProject(const ax::event::StringMsg& msg)
	{
		OpenProject(msg.GetMsg());
	}

	void MainWindowProjectHandler::OnCreateNewProject(const ax::event::StringMsg& msg)
	{
		const std::string project_path(msg.GetMsg());
		CreateProject(project_path);
	}
}
}
