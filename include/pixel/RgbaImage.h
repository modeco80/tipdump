//
// TIPDump2
//
// (C) 2021-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef LIBPIXEL_RGBAIMAGE_H
#define LIBPIXEL_RGBAIMAGE_H

#include <pixel/RgbaColor.h>

#include <cstdint>
#include <memory>

namespace pixel {

	/**
	 * Simple image size structure.
	 * Pass by value.
	 */
	struct ImageSize {
		std::uint16_t width {};
		std::uint16_t height {};
	};

	/**
	 * A standard RGBA8888 image.
	 */
	struct RgbaImage {
		RgbaImage();

		// Technically it's a postcondition that the object not be used after destruction,
		// so unique_ptr default delete behaviour works just fine.
		//~RgbaImage();

		/**
		 * Constructor taking in an image size.
		 * Shorthand for creating an image and then doing Resize(size).
		 * \see RgbaImage::Resize
		 */
		explicit RgbaImage(ImageSize size);

		// Shouldn't copy.
		RgbaImage(const RgbaImage&) = delete;

		RgbaImage(RgbaImage&& movee) noexcept;

		/**
		 * Resize this image.
		 * \param[in] size Size to resize to.
		 */
		void Resize(ImageSize size);

		/**
		 * Clear this image, freeing its buffer.
		 */
		void Clear();

		/**
		 * Get access to this image's data buffer (const version).
		 * \return A const pointer to the image buffer.
		 */
		[[nodiscard]] const RgbaColor* GetBuffer() const;

		/**
		 * Get access to this image's data buffer for writing
		 * \return A pointer to the image buffer.
		 */
		[[nodiscard]] RgbaColor* GetBuffer();

		/**
		 * Get the stride of this image.
		 * \return The stride.
		 */
		[[nodiscard]] std::size_t GetStride() const;

		/**
		 * Get the size of this image.
		 * \return The size.
		 */
		[[nodiscard]] ImageSize GetSize() const;

		// TODO: Add these methods, to make sampling data a bit nicer.
		//
		// static RgbaImage From4Bpp(span<std::uint8_t> data, span<RgbaColor> palette, ImageSize size);
		// static RgbaImage From8Bpp(span<std::uint8_t> data, span<RgbaColor> palette, ImageSize size);
		// static RgbaImage From16Bpp(span<std::uint8_t> data, ImageSize size);
		// static RgbaImage From32Bpp(span<std::uint8_t> data, ImageSize size);

	   private:
		ImageSize size {};
		std::unique_ptr<RgbaColor[]> pixels;
	};

} // namespace pixel

#endif // TIPDUMP_RGBAIMAGE_H
