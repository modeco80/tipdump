//
// TIPDump2
//
// (C) 2021-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef TIPDUMP_TIPIMAGE_H
#define TIPDUMP_TIPIMAGE_H

#include <pixel/RgbaImage.h>
#include <tip/TipStructures.h>

#include <cstdint>
#include <iosfwd>
#include <vector>

namespace td::tip {

	struct TipImage {
		/**
		 * Read an image from an IOStream.
		 *
		 * \param[in] is IOStream to read from.
		 * \return True on success; false otherwise.
		 */
		bool ReadFromStream(std::istream& is);

		/**
		 * Free this image's resources.
		 */
		void Clear();

		/**
		 * Get this image's size.
		 */
		[[nodiscard]] pixel::ImageSize Size() const;

		/**
		 * Convert this image to a RGBA8888 image.
		 *
		 * \return a RgbaImage containing a plain-format RGBA8888 bitmap.
		 */
		[[nodiscard]] pixel::RgbaImage ToRgba();

		/**
		 * Get the palette of this image, as an array of RGBA8888 colors.
		 *
		 * This is only computed once, so calling it more than once will
		 * return immediately without doing work.
		 *
		 * \return a 1D palette array.
		 */
		[[nodiscard]] const std::vector<pixel::RgbaColor>& Palette();

		/**
		 * RgbaImage index.
		 */
		[[nodiscard]] uint16_t Index() const;

		/**
		 * Get the raw image header.
		 */
		[[nodiscard]] const TipImageHdr& Header() const;

		/**
		 * Get whether this image is 8bpp or not.
		 */
		[[nodiscard]] bool Is8Bpp() const;

	   private:
		TipImageHdr imageHeader;
		TipImageHdr clutHeader;

		// make these unique_ptr<T[]>, probably,
		// since we don't need vector dynamic resize
		// *or* value initialization.

		std::vector<std::uint8_t> imageBytes;
		std::vector<std::uint8_t> clutBytes;

		std::vector<pixel::RgbaColor> palette; // TODO: use a 256/16*1 RgbaImage?
											   // what about a array<RgbaColor, 256>? It'd be bigger but no crazy hot path allocs
		bool paletteComputed { false };
	};

} // namespace td::tip

#endif // TIPDUMP_TIPIMAGE_H
