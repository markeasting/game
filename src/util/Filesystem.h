#pragma once

#include "common.h"
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

class Filesystem {
public:

	Filesystem(Filesystem const&) = delete;
	~Filesystem() = default;

	static Filesystem& instance();

	std::string basePath;

	std::string getBasePath();

	std::string getFileContents(const std::string& relativePath);

private:

	Filesystem();

};
