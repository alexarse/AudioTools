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

#include "project/atProjectFile.hpp"
#include <axlib/Util.hpp>
#include <iostream>

namespace at {
class ProjectManager {
public:
	ProjectManager();

	~ProjectManager();

	bool Open(const std::string& path);

	bool SaveAs(const std::string& path);

	bool Save();

	bool CreateNewProject(const std::string& path);

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
