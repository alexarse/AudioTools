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

#include "project/atProjectManager.hpp"

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
