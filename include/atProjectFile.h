#ifndef atProjectFile_hpp
#define atProjectFile_hpp

#include "atArchive.h"
#include <string>
#include <OpenAX/Utils.h>

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

#endif /* atProjectFile_hpp */
