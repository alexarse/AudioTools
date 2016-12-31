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

#include "project/atArchive.hpp"
#include <axlib/Util.hpp>
#include <fstream>
#include <iostream>

namespace at {
FileArchive::FileArchive()
	: _archive(nullptr)
{
}

FileArchive::~FileArchive()
{
	if (_archive != nullptr) {
		zip_close(_archive);
		_archive = nullptr;
	}
}

// Open archive.
bool FileArchive::Open(const std::string& path)
{
	if (_archive != nullptr) {
		zip_close(_archive);
		_archive = nullptr;
	}

	int err = 0;
	_archive = zip_open(path.c_str(), ZIP_CREATE, &err);

	if (err != ZIP_ER_OK) {
		std::cout << err << std::endl;
		std::cout << "error open archive: " << zip_strerror(_archive) << std::endl;
		return false;
	}

	return true;
}

void FileArchive::Close()
{
	if (_archive != nullptr) {
		zip_close(_archive);
		_archive = nullptr;
	}
}

bool FileArchive::AddFileContent(const std::string& name, void* data, unsigned int size)
{
	zip_source* s = zip_source_buffer(_archive, data, size, 0);

	if (s == nullptr) {
		zip_source_free(s);
		std::cout << "error adding file: " << zip_strerror(_archive) << std::endl;
		return false;
	}

	if (zip_file_add(_archive, name.c_str(), s, ZIP_FL_OVERWRITE) < 0) {
		zip_source_free(s);
		std::cout << "error adding file: " << zip_strerror(_archive) << std::endl;
		return false;
	}

	return true;
}

bool FileArchive::ReplaceFileContent(const std::string& name, void* data, unsigned int size)
{
	zip_source* s = zip_source_buffer(_archive, data, size, 0);

	if (s == nullptr) {
		zip_source_free(s);
		std::cout << "error adding file: " << zip_strerror(_archive) << std::endl;
		return false;
	}

	zip_int64_t f_id = zip_name_locate(_archive, name.c_str(), 0);

	if (zip_file_replace(_archive, f_id, s, ZIP_FL_ENC_UTF_8) < 0) {
		zip_source_free(s);
		std::cout << "error adding file: " << zip_strerror(_archive) << std::endl;
		return false;
	}

	return true;
}

std::vector<char> FileArchive::GetFileContent(const std::string& filename)
{
	struct zip_stat stat;
	if (zip_stat(_archive, filename.c_str(), 0, &stat) < 0) {
		//		std::cerr << "File not found." << std::endl;
		std::cout << "error file: " << zip_strerror(_archive) << std::endl;

		return std::vector<char>();
	}

	zip_file* file = zip_fopen(_archive, filename.c_str(), 0);

	if (file == nullptr) {
		return std::vector<char>();
	}

	std::vector<char> buffer(stat.size);
	zip_fread(file, buffer.data(), stat.size);
	//	zip_fclose(file);
	return buffer;
}

std::vector<char> FileArchive::GetFileContent(unsigned int file_index, std::string& f_name)
{
	struct zip_stat stat;
	if (zip_stat_index(_archive, file_index, 0, &stat) < 0) {
		//		std::cerr << "File not found." << std::endl;
		// std::cout << "error file: " << zip_strerror(_archive) << std::endl;

		return std::vector<char>();
	}

	f_name = stat.name;

	zip_file* file = zip_fopen_index(_archive, file_index, 0);

	if (file == nullptr) {
		return std::vector<char>();
	}

	std::vector<char> buffer(stat.size);
	zip_fread(file, buffer.data(), stat.size);
	zip_fclose(file);
	return buffer;
}

bool FileArchive::AddDirectory(const std::string& name)
{
	if (zip_dir_add(_archive, name.c_str(), ZIP_FL_ENC_UTF_8) < 0) {
		return false;
	}

	return true;
}

bool FileArchive::ExtractArchive(const std::string& path)
{
	zip_int64_t n_file = zip_get_num_entries(_archive, 0);

	if (n_file < 1) {
		return false;
	}

	for (int i = 0; i < n_file; i++) {
		std::string f_name;
		std::vector<char> f_content = GetFileContent(i, f_name);

		if (f_content.empty()) {
			continue;
		}

		std::ofstream f_stream(path + f_name, std::ios::out | std::ios::binary);
		f_stream.write(f_content.data(), f_content.size());
		//		std::string data_str(f_content.data());
		//		f_stream << data_str;
		f_stream.close();
	}

	return true;
}
}
