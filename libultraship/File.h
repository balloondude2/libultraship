#pragma once

#include <string>
#include <memory>
#include "GlobalCtx2.h"

namespace Ship {
	class Archive;

	class File
	{
	public:
		std::shared_ptr<Archive> parent;
		std::string path;
		std::shared_ptr<char[]> buffer;
		uint32_t dwBufferSize;
		bool bIsLoaded;
		std::condition_variable Notifier;
		std::mutex Mutex;
	};
}
