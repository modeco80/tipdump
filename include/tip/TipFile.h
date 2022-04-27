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

	/**
	 * High level version of a tip file.
	 */
	struct TipFile {
		/**
		 * Read from a binary C++ iostream
		 */
		bool ReadFromStream(std::istream& is);

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
