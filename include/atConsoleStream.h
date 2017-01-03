//
//  atConsoleStream.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-15.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#pragma once

#include <axlib/axlib.hpp>

namespace at {
class ConsoleStream : public ax::event::Object {
public:
	enum Events : ax::event::Id { WRITE_NEW_LINE, WRITE_ERROR };

	static inline ConsoleStream* GetInstance()
	{
		if (_instance == nullptr) {
			_instance.reset(new ConsoleStream());
		}

		return _instance.get();
	}

	//	void Write();
	//
	//	template <typename T, typename... P> void Write(T t, P... p)
	//	{
	//		_stream << t << ' ';
	//		{
	//			Write(p...);
	//		}
	//	}

	//	void Error();
	//
	//	template <typename T, typename... P> void Error(T t, P... p)
	//	{
	//		_stream << t << ' ';
	//		{
	//			Error(p...);
	//		}
	//	}

	void Write(const std::string& msg);
	void Error(const std::string& err_msg);

	//	std::stringstream& GetStream()
	//	{
	//		return _stream;
	//	}
	//
	//	std::string GetString()
	//	{
	//		return _stream.str();
	//	}
	//
	//	ax::StringVector GetStreamLines()
	//	{
	//		return ax::util::String::Split(_stream.str(), "\n");
	//	}

	//	ax::StringVector GetStreamNLastLines(int n_lines)
	//	{
	//		ax::StringVector lines(ax::util::String::Split(_stream.str(), "\n"));
	//
	//		if(lines.size() > n_lines) {
	//
	//			int erase_size = (int(lines.size()) - n_lines) - 1;
	//
	//			if(erase_size < 0) {
	//				erase_size = 0;
	//			}
	//
	//			lines.erase(lines.begin(), lines.begin() + erase_size);
	//		}
	//
	//		return lines;
	//	}

private:
	static std::unique_ptr<ConsoleStream> _instance;
	//	std::stringstream _stream;

	ConsoleStream();
};
}
