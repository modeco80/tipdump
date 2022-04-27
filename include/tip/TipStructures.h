//
// TIPDump2
//
// (C) 2021-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef TIPDUMP_TIP_STRUCTURES_H
#define TIPDUMP_TIP_STRUCTURES_H

#include <cstdint>

// All structures here are little endian...

namespace td::tip {

	/**
	 * TIP file header.
	 */
	struct TipFileHdr {
		char Magic[4];			  // 'TIP0' - rarely 'TIP1'
		std::uint32_t ImageCount; // All images including CLUT.
	};

	// PS1 `RECT` structure. Seems to be bit accurate to
	// PS1 Runtime Library.
	struct Ps1Rect {
		std::uint16_t x, y;
		std::uint16_t w, h;
	};

	/**
	 * TIP image header.
	 */
	struct TipImageHdr {
		/**
		 * Flag for indicating this image is actually 8 bits per pixel.
		 * The default seems to be 4bpp.
		 */
		constexpr static auto IMAGEFLAG_8BPP = 0x1;

		/**
		 * Flag for indicating this image is a CLUT.
		 * This (usually) means the CLUT is for the image at (CLUT_index - 1).
		 */
		constexpr static auto IMAGEFLAG_CLUT = 0x4;

		std::uint32_t Length;	 ///< Length of this image, including the size of image data.
		std::uint16_t Index;	 ///< The index of this image. Seems to be used for self-referential stuff.
		std::uint8_t ImageFlags; ///< Image flags. See TipImageHdr::IMAGEFLAG_* for all applicable flags

		std::uint8_t IsFastPath; ///< Code takes a diff path if this == 1? Unknown what this really does

		/**
		 * Where the image should be placed in VRAM. Includes width and height, which
		 * seem to be kinda screwy.
		 */
		Ps1Rect ImageRect;

		/**
		 * Handy helper function to calculate image data size.
		 */
		[[nodiscard]] constexpr std::size_t ImageDataSize() const {
			return Length - sizeof(*this);
		}

		// image data (length of data being (this->Length - sizeof(*this)))
	};

	// Some checks for proper structure sizes.
	static_assert(sizeof(TipFileHdr) == 0x8, "TipFileHdr size is wrong, should be 8 (0x8 hex).");
	static_assert(sizeof(Ps1Rect) == 0x8, "Ps1Rect size is wrong, should be 8 (0x8 hex).");
	static_assert(sizeof(TipImageHdr) == 0x10, "TipImageHdr size is wrong, should be 16 bytes (0x10 hex).");

} // namespace td::tip
#endif // TIPDUMP_TIP_STRUCTURES_H