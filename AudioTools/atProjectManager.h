#ifndef __AT_PROJECT_MANAGER_H__
#define __AT_PROJECT_MANAGER_H__

#include "atProjectFile.h"
#include <OpenAX/Utils.h>
#include <iostream>

namespace at {
class ProjectManager {
public:
	ProjectManager();
	
	~ProjectManager();
	
	bool Open(const std::string& path);
	
	bool SaveAs(const std::string& path);
	
	bool Save();
	
	inline std::string GetTempPath() const
	{
		return _p_file->GetTempPath();
	}
	
	inline std::string GetLayoutPath() const
	{
		return _p_file->GetTempPath() + "/layout.xml";
	}
	
	inline std::string GetScriptPath() const
	{
		return _p_file->GetTempPath() + "/script.py";
	}
	
	inline bool IsProjectOpen() const
	{
		return ((_p_file != nullptr) && (_p_file->IsValid()));
	}
	
	inline std::string GetProjectName() const
	{
		return _p_file->GetProjectName();
	}
	
	void Close();
	
	void SaveAndClose();
	
private:
	at::ProjectFile* _p_file;
};
}

#endif // __AT_PROJECT_MANAGER_H__
