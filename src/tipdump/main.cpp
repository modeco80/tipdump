//
// TIPDump2
//
// (C) 2021-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include <tip/TipFile.h>

#include <filesystem>
#include <fstream>
#include <iostream>

#include "stb_image_write.h"

namespace fs = std::filesystem;

// Cheap format. I may depend on fmt later, but for now, this certainly works.
template <class... Args>
std::string CheapFormat(std::string_view sv, Args&&... args) {
	static char tempFormatBuf[2048]; // only used for formatting, string ctor makes a copy of this
	int res = snprintf(&tempFormatBuf[0], std::size(tempFormatBuf), sv.data(), args...);

	if(res == -1)
		return "";

	return { &tempFormatBuf[0], static_cast<std::size_t>(res) };
}

int main(int argc, char** argv) {
	if(argc < 2) {
		if(argc == 1)
			std::cout << "Required path to TIP missing..\n";

		std::cout << "Usage: " << argv[0] << " [Path to .TIP file] [OPTIONAL: start address of TIP in hexadecimal]\n";
		std::cout << "Start address option is useful for extracting from Vol. 3 .DAT files, for instance..\n";
		return 0;
	}

	auto startOffset = 0u;

	// User provided a start offset
	if(argc == 3) {
		std::string hex = argv[2];

		// cut off trailing 0x for stoul(), if it's there
		if(hex.find_first_of("0x") != std::string::npos)
			hex.replace(hex.find_first_of("0x"), 2, "");

		try {
			startOffset = std::stoul(argv[2], nullptr, 16);
		} catch(std::exception& ex) {
			std::cout << "Invalid user start offset.\n";
			return 1;
		}
	}

	auto path = fs::path(argv[1]);

	if(!fs::exists(path)) {
		std::cout << "path \"" << path.string() << "\" doesn't exist.\n";
		return 1;
	}

	std::ifstream ifs(path.string(), std::ifstream::binary);

	if(!ifs) {
		std::cout << "couldn't open\n";
		return 0;
	}

	// Seek to the user-provided start offset, if required.
	if(startOffset != 0) {
		std::cout << CheapFormat("User-provided start offset: 0x%08x\n", startOffset);
		ifs.seekg(startOffset, std::istream::beg);

		if(ifs.eof()) {
			std::cout << "Error: User-provided start offset is past EOF.\n";
			return 1;
		}
	}

	td::tip::TipFile file;

	if(!file.ReadFromStream(ifs)) {
		std::cout << "Error: Couldn't read TIP file from stream.\n";
		return 1;
	}

	auto outputDir = path.remove_filename() / CheapFormat("%s", path.stem().c_str());

	std::cout << "Output Directory: " << outputDir << '\n';

	if(!fs::exists(outputDir))
		fs::create_directories(outputDir);

	for(auto& image : file.GetImages()) {
		auto rgba = image.ToRgba();

		auto outName = (outputDir / CheapFormat("%d.png", image.Index() / 2));

		std::cout << "Writing TIP image " << image.Index() / 2 << " to path " << std::quoted(outName.string()) << '\n';

		stbi_write_png(outName.c_str(), rgba.size.width, rgba.size.height, 4, rgba.pixels.data(), rgba.size.width * 4);
	}
}