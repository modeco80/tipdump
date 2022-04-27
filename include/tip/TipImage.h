//
// TIPDump2
//
// (C) 2021-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef TIPDUMP_TIPIMAGE_H
#define TIPDUMP_TIPIMAGE_H

#include <tip/TipStructures.h>

#include <cstdint>
#include <iosfwd>
#include <vector>

namespace td::tip {

	/**
	 * Simple image size structure.
	 */
	struct ImageSize {
		std::uint16_t width {};
		std::uint16_t height {};
	};

	/**
	 * A RGBA color.
	 */
	union RgbaColor {
		std::uint32_t u32 { 0x000000ff }; // R=0, G=0, B=0, A=255
		struct {
			std::uint8_t r;
			std::uint8_t g;
			std::uint8_t b;
			std::uint8_t a;
		};
	};

	/**
	 * A generic RGBA image.
	 */
	struct RgbaImage {

		// we should store ps1 vram x/y?
		ImageSize size;
		std::vector<RgbaColor> pixels;
	};

	struct TipImage {
		/**
		 * Read an image from an IOStream.
		 *
		 * \param[in] is IOStream to read from.
		 * \return True on success; false otherwise.
		 */
		bool ReadFromStream(std::istream& is);

		/**
		 * Get this image's size.
		 */
		ImageSize Size() const;

		/**
		 * Convert this image to a RGBA8888 image.
		 *
		 * \return a RgbaImage containing a plain-format RGBA8888 bitmap.
		 */
		RgbaImage ToRgba();

		/**
		 * Get the palette of this image, as an array of RGBA8888 colors.
		 *
		 * This is only computed once, so calling it more than once will
		 * return immediately without doing work.
		 *
		 * \return a 1D palette array.
		 */
		const std::vector<RgbaColor>& Palette();

		/**
		 * RgbaImage index.
		 */
		uint16_t Index() const;

		/**
		 * Get the raw image header.
		 */
		const TipImageHdr& Header() const;

	   private:
		TipImageHdr imageHeader;
		TipImageHdr clutHeader;

		std::vector<std::uint8_t> imageBytes;
		std::vector<std::uint8_t> clutBytes;

		std::vector<RgbaColor> palette;
		bool paletteComputed{false};
	};

} // namespace td::tip

#endif // TIPDUMP_TIPIMAGE_H
