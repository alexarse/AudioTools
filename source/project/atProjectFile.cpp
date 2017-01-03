/*
 * Copyright (c) 2016 AudioTools - All Rights Reserved
 *
 * This Software may not be distributed in parts or its entirety
 * without prior written agreement by AudioTools.
 *
 * Neither the name of the AudioTools nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY AUDIOTOOLS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL AUDIOTOOLS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Written by Alexandre Arsenault <alx.arsenault@gmail.com>
 */

#include "project/atProjectFile.hpp"
#include <axlib/FileSystem.hpp>
#include <axlib/Util.hpp>
#include <boost/filesystem.hpp>

namespace at {
ProjectFile::ProjectFile(const std::string& filename)
	: _project_file_path(filename)
	, _is_valid(false)
{
	boost::filesystem::path f_path(filename);

	if (!boost::filesystem::exists(f_path)) {
		ax::console::Error("Project name :", filename, "doesn't exist.");
		return;
	}

	if (f_path.extension() != ".atproj") {
		ax::console::Error("Project name :", filename, "doesn't proper extension.");
		return;
	}

	_project_name = f_path.stem().string();

	ax::console::Print("Project name :", _project_name);

	if (_archive.Open(filename)) {
		_is_valid = true;
	}
}

std::string ProjectFile::GetLayoutContent()
{
	if (!_is_valid) {
		return "";
	}

	std::vector<char> data = _archive.GetFileContent(_project_name + "/layout.xml");

	if (data.empty()) {
		return "";
	}

	return std::string(data.data());
}

std::string ProjectFile::GetScriptContent()
{
	if (!_is_valid) {
		return "";
	}

	std::vector<char> data = _archive.GetFileContent(_project_name + "/script.py");

	if (data.empty()) {
		return "";
	}

	return std::string(data.data());
}

ProjectFile::ProjectError ProjectFile::CreateTempFolder(const std::string& folder_path)
{
	if (!_is_valid) {
		ax::console::Error("Archive not valid.");
		return ProjectError::ARCHIVE_NOT_VALID;
	}

	if (_project_name.empty()) {
		ax::console::Error("Project name empty.");
		return ProjectError::EMPTY_PROJECT_NAME;
	}

	_tmp_folder_path = folder_path + _project_name;
	boost::filesystem::path tmp_dir(_tmp_folder_path);

	if (boost::filesystem::exists(tmp_dir)) {
		return ProjectError::DIRECTORY_ALREADY_EXIST;
	}

	if (boost::filesystem::create_directory(tmp_dir)) {
		return ProjectError::NO_ERROR;
	}

	return ProjectError::CANT_CREATE_FOLDER;
}

bool ProjectFile::ExtractArchive(const std::string& path)
{
	return _archive.ExtractArchive(path);
}

bool ProjectFile::SaveProject()
{
	boost::filesystem::path tmp_dir(_tmp_folder_path);

	if (boost::filesystem::is_directory(tmp_dir)) {
		std::cout << tmp_dir << " is a directory containing:\n";

		boost::filesystem::recursive_directory_iterator end;

		std::vector<std::string> proj_files;

		for (boost::filesystem::recursive_directory_iterator i(tmp_dir); i != end; ++i) {
			const boost::filesystem::path cp = (*i);
			ax::console::Print(cp.filename().string());
			proj_files.push_back(cp.filename().string());
		}

		std::vector<std::pair<std::string, std::vector<char>>> data;

		for (auto& n : proj_files) {
			std::ifstream f_path(_tmp_folder_path + "/" + n, std::ios::binary | std::ios::ate);
			std::ifstream::pos_type pos = f_path.tellg();

			std::vector<char> buffer(pos);

			f_path.seekg(0, std::ios::beg);
			f_path.read(&buffer[0], pos);

			data.push_back(std::pair<std::string, std::vector<char>>(_project_name + "/" + n, buffer));
		}

		for (auto& n : data) {
			_archive.AddFileContent(n.first, (void*)n.second.data(), (unsigned int)n.second.size());
		}

		_archive.Close();
		_archive.Open(_project_file_path);
	}

	return false;
}

bool ProjectFile::SaveAsProject(const std::string& filepath)
{
	boost::filesystem::path tmp_dir(_tmp_folder_path);

	if (boost::filesystem::is_directory(tmp_dir)) {
		std::cout << tmp_dir << " is a directory containing:\n";

		boost::filesystem::recursive_directory_iterator end;

		std::vector<std::string> proj_files;

		for (boost::filesystem::recursive_directory_iterator i(tmp_dir); i != end; ++i) {
			const boost::filesystem::path cp = (*i);
			ax::console::Print(cp.filename().string());
			proj_files.push_back(cp.filename().string());
		}

		std::vector<std::pair<std::string, std::vector<char>>> data;

		at::FileArchive arch_file;
		arch_file.Open(filepath + ".atproj");

		boost::filesystem::path f_path(filepath);
		std::string name = f_path.filename().string();
		arch_file.AddDirectory(name);

		for (auto& n : proj_files) {
			std::ifstream f_path(_tmp_folder_path + "/" + n, std::ios::binary | std::ios::ate);
			std::ifstream::pos_type pos = f_path.tellg();

			std::vector<char> buffer(pos);

			f_path.seekg(0, std::ios::beg);
			f_path.read(&buffer[0], pos);

			data.push_back(std::pair<std::string, std::vector<char>>(name + "/" + n, buffer));
		}

		for (auto& n : data) {
			arch_file.AddFileContent(n.first, (void*)n.second.data(), (unsigned int)n.second.size());
		}
	}

	return false;
}

bool ProjectFile::DeleteTempFolder()
{
	boost::filesystem::path tmp_dir(_tmp_folder_path);
	boost::filesystem::remove_all(tmp_dir);
	return true;
}
}
