#include "atProjectManager.h"

namespace at {
ProjectManager::ProjectManager()
	: _p_file(nullptr)
{
}

ProjectManager::~ProjectManager()
{
	if (_p_file != nullptr) {
		_p_file->DeleteTempFolder();
		delete _p_file;
		_p_file = nullptr;
	}
}

bool ProjectManager::Open(const std::string& path)
{
	if (_p_file != nullptr) {
		delete _p_file;
		_p_file = nullptr;
	}

	_p_file = new at::ProjectFile(path);

	at::ProjectFile::ProjectError p_err = _p_file->CreateTempFolder("tmp/");

	if (p_err == at::ProjectFile::DIRECTORY_ALREADY_EXIST) {
		ax::console::Error("Directory already exist.");
		return -1;
	}
	else if (p_err != at::ProjectFile::NO_ERROR) {
		ax::console::Error("Can't create directory");
		return -1;
	}

	_p_file->ExtractArchive("tmp/");

	return true;
}

bool ProjectManager::SaveAs(const std::string& path)
{
	_p_file->SaveAsProject(path);
	return true;
}

bool ProjectManager::Save()
{
	_p_file->SaveProject();
	return true;
}

bool ProjectManager::CreateNewProject(const std::string& path)
{
	return false;
}

void ProjectManager::Close()
{
	if (_p_file != nullptr) {
		_p_file->DeleteTempFolder();
		delete _p_file;
		_p_file = nullptr;
	}
}

void ProjectManager::SaveAndClose()
{
	if (_p_file != nullptr) {
		_p_file->SaveProject();
		_p_file->DeleteTempFolder();
		delete _p_file;
		_p_file = nullptr;
	}
}
}
