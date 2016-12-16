//
//  atArchive.hpp
//  atZip
//
//  Created by Alexandre Arsenault on 2016-04-20.
//  Copyright © 2016 OpenAX. All rights reserved.
//

#ifndef atArchive_hpp
#define atArchive_hpp

#include <string>
#include <zip.h>
#include <vector>
#include <OpenAX/Utils.h>

namespace at {
	class FileArchive {
	public:
		// Create archive file.
		FileArchive();
		
		~FileArchive();
		
		// Open archive.
		bool Open(const std::string& path);
		
		void Close();
		
//		bool Create(const std::string& path);
		
		bool AddFileContent(const std::string& name, void* data, unsigned int size);
		
		bool ReplaceFileContent(const std::string& name, void* data, unsigned int size);
		
		
		
		std::vector<char> GetFileContent(const std::string& file);
		
		std::vector<char> GetFileContent(unsigned int file_index, std::string& f_name);
		
		bool AddDirectory(const std::string& name);
		
		bool ExtractArchive(const std::string& path);
		
	private:
		zip* _archive;
	};
}

#endif /* atArchive_hpp */
