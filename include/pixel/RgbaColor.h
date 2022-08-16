//
// TIPDump2
//
// (C) 2021-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef LIBPIXEL_RGBACOLOR_H
#define LIBPIXEL_RGBACOLOR_H

#include <cstdint>

namespace pixel {

	/**
	 * A RGBA8888 color.
	 */
	union RgbaColor {
	   private:
		/**
		 * Data representation.
		 */
		std::uint32_t u32 {};

	   public:
		// maybe provide ctors?

		struct {
			std::uint8_t r;
			std::uint8_t g;
			std::uint8_t b;
			std::uint8_t a;
		};

		[[nodiscard]] constexpr std::uint32_t GetRep() const {
			return u32;
		}

		// TODO: Maybe:
		//
		// RgbaColor ColorFrom16Bpp(uint16_t, endian);
		// unique_ptr<RgbaColor[]> PaletteFrom16Bpp(span<const uint16_t>, endian);
	};
} // namespace pixel

#endif // LIBPIXEL_RGBACOLOR_H
