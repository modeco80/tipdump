//
// TIPDump2
//
// (C) 2021-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef TIPDUMP_RGBAIMAGE_H
#define TIPDUMP_RGBAIMAGE_H

#include <cstdint>
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
		std::uint32_t u32 {};

		// Color component accessor.
		struct {
			std::uint8_t r;
			std::uint8_t g;
			std::uint8_t b;
			std::uint8_t a;
		};
	};

	/**
	 * A standard RGBA8888 image.
	 */
	struct RgbaImage {
		// we should store ps1 vram x/y?

		RgbaImage();

		explicit RgbaImage(ImageSize size);

		void Resize(ImageSize size);

		void Clear();

		[[nodiscard]] const RgbaColor* GetBuffer() const;

		[[nodiscard]] RgbaColor* GetBuffer();

		[[nodiscard]] std::size_t GetStride() const;

		[[nodiscard]] ImageSize GetSize() const;

	   private:
		ImageSize size {};
		std::vector<RgbaColor> pixels;
	};

} // namespace td::tip

#endif // TIPDUMP_RGBAIMAGE_H
