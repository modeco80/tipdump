//
// TIPDump2
//
// (C) 2021-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include <pixel/ImageWriter.h>
#include <tip/TipFile.h>

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

// Cheap format hack.
// TODO: Replace with fmt.
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

		std::cout << "Usage: " << argv[0] << " [Path to .TIP file] [OPTIONAL start address of TIP in hexadecimal]\n";
		std::cout << "Start address option is useful for extracting from Vol. 3/4/5 .DAT files, for instance..\n";
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
	auto source = path; // copied as we mutate path a bit. this will always be the original user input

	if(!fs::exists(path)) {
		std::cout << "Error: Provided path " << std::quoted(source.string()) << " doesn't exist.\n";
		return 1;
	}

	std::ifstream ifs(path.string(), std::ifstream::binary);

	if(!ifs) {
		std::cout << "Error: Couldn't open source file for reading.\n";
		std::cout << "Please check permissions.\n";
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

	// refactor below code into a object probably.

	td::tip::TipFile file;
	pixel::ImageWriter writer {};

	if(auto res = file.ReadFromStream(ifs); res != td::tip::TipReadError::NoError) {
		std::cout << CheapFormat("Error: Could not read TIP file: %s\n", td::tip::TipReadErrorToString(res));
		return 1;
	}

	auto outputDir = path.remove_filename() / CheapFormat("%s", path.stem().c_str());

	std::cout << "Output Directory: " << outputDir << '\n';

	if(!fs::exists(outputDir))
		fs::create_directories(outputDir);

	// Create stat file

	auto statName = (outputDir / "stat.txt");
	std::ofstream statFile(statName.string());

	if(!statFile) {
		std::cout << "Error: Could not open stat file for writing\n";
		std::cout << "This probably means you don't have write permission in freshly created directories?\n";
		std::cout << "Don't know how you managed that, so bailing.\n";
		return 1;
	}

	statFile << CheapFormat("TD2 Stat File Dump for \"%s\"\r\n", source.c_str());

	for(auto& image : file.GetImages()) {
		auto rgba = image.ToRgba();
		writer.SetImage(rgba);

		auto outName = (outputDir / CheapFormat("%d.png", image.Index() / 2));

		// Write out some statistic information
		{
			auto& header = image.Header();
			auto rect = image.Size();
			const auto clutLen = image.Is8Bpp() ? 256 * sizeof(std::uint16_t) : 16 * sizeof(std::uint16_t);

			statFile << CheapFormat("Image #%d:\n", image.Index() / 2);
			statFile << CheapFormat("    Bitdepth: %s\n", image.Is8Bpp() ? "8bpp" : "4bpp");
			statFile << CheapFormat("    VRAM Rect: %dx%d (at %dx%d)\n", rect.width, rect.height, header.ImageRect.x, header.ImageRect.y);
			statFile << CheapFormat("    Data size: %u (0x%08x hex)\n", header.ImageDataSize(), header.ImageDataSize());
			statFile << CheapFormat("    CLUT data size: %u (0x%08x hex)\n\n", clutLen, clutLen);
		}

		std::cout << "Writing image " << image.Index() / 2 << " to path " << std::quoted(outName.string()) << '\n';

		writer.WritePng(outName);
	}

	return 0;
}