//
//  atProjectManager.hpp
//  atZip
//
//  Created by Alexandre Arsenault on 2016-04-20.
//  Copyright © 2016 OpenAX. All rights reserved.
//

#ifndef atProjectManager_hpp
#define atProjectManager_hpp

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
	
	void Close();
	
	void SaveAndClose();
	
private:
	at::ProjectFile* _p_file;
};
}

#endif /* atProjectManager_hpp */
