//
// TIPDump2
//
// (C) 2021-2022 modeco80 <lily.modeco80@protonmail.ch>
//

#ifndef TIPDUMP_TIPFILE_H
#define TIPDUMP_TIPFILE_H

#include <tip/TipImage.h>
#include <tip/TipStructures.h>

#include <iosfwd>
#include <vector>

namespace td::tip {

	enum class TipReadError : std::uint8_t {
		NoError = 0,
		NotTip,				  ///< NOT a TIP0/TIP1 file
		FailureReadingHeader, ///< Failure reading TIP header.
		FailureReadingImage	  ///< Failure reading an image
	};

	constexpr const char* TipReadErrorToString(TipReadError err) {
		constexpr const char* table[] = {
			"No error",
			"Not a TIP0/TIP1 file. Please choose a different file",
			"Failure reading TIP header",
			"Failure reading a TIP image"
		};
		return table[static_cast<std::size_t>(err)];
	}

	/**
	 * High level version of a tip file.
	 */
	struct TipFile {
		/**
		 * Read from a binary C++ iostream
		 */
		TipReadError ReadFromStream(std::istream& is);

		/**
		 * Peek at all images.
		 */
		const std::vector<TipImage>& GetImages();

		const TipFileHdr& GetHeader() const;

	   private:
		TipFileHdr header;
		std::vector<TipImage> images;
	};

} // namespace td::tip

#endif // TIPDUMP_TIPFILE_H
