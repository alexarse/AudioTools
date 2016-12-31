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
#pragma once

#include "project/atArchive.hpp"
#include <axlib/Util.hpp>
#include <string>

namespace at {
class ProjectFile {
public:
	enum ProjectError {
		NO_ERROR = 0,
		ARCHIVE_NOT_VALID,
		EMPTY_PROJECT_NAME,
		DIRECTORY_ALREADY_EXIST,
		CANT_CREATE_FOLDER
	};

	ProjectFile(const std::string& filename);

	std::string GetLayoutContent();

	std::string GetScriptContent();

	bool IsValid() const
	{
		return _is_valid;
	}

	// Return true on success.
	ProjectError CreateTempFolder(const std::string& folder_path);

	bool ExtractArchive(const std::string& path);

	bool SaveProject();

	bool SaveAsProject(const std::string& name);

	bool DeleteTempFolder();

	inline std::string GetTempPath() const
	{
		return _tmp_folder_path;
	}

	inline std::string GetProjectName() const
	{
		return _project_name;
	}

private:
	std::string _project_file_path;
	std::string _project_name;
	std::string _tmp_folder_path;

	at::FileArchive _archive;
	bool _is_valid;

	//	void CreateTempFiles(const std::string& folder_path);
};
}
