//
// TIPDump2
//
// (C) 2021-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef LIBPIXEL_IMAGEWRITER_H
#define LIBPIXEL_IMAGEWRITER_H

#include <filesystem>

namespace pixel {

	// Forward declaration
	struct RgbaImage;

	/**
	 * Image writer. Abstracts low-level routines
	 * to write RgbaImage instances to given image formats.
	 */
	struct ImageWriter {

		/**
		 * Set the image this writer will work with.
		 * \param[in] image The image this writer will work with.
		 */
		void SetImage(RgbaImage& image);

		/**
		 * Write the selected image as a PNG file.
		 * \param[in] outputPath The output path. Directory tree will be created if required.
		 */
		void WritePng(const std::filesystem::path& outputPath);

		// Other output formats? Would be nice :)

	   private:
		RgbaImage* imagePtr;
	};

}

#endif // LIBPIXEL_IMAGEWRITER_H
