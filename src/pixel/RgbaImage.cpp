//
// TIPDump2
//
// (C) 2021-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include <pixel/RgbaImage.h>

namespace pixel {

	RgbaImage::RgbaImage() = default;

	RgbaImage::RgbaImage(ImageSize size) {
		Resize(size);
	}

	// This is probably trivial.
	RgbaImage::RgbaImage(RgbaImage&& movee) noexcept {
		pixels = std::move(movee.pixels);
		size = std::move(movee.size);
	}

	void RgbaImage::Resize(ImageSize sz) {
		// Clear any remaining image.
		if(pixels)
			Clear();

		// if only i could use make_shared...
		pixels.reset(new RgbaColor[sz.width * sz.height]);
		size = sz;
	}

	void RgbaImage::Clear() {
		if(!pixels) {
			pixels.reset();
			size = { 0, 0 };
		}
	}

	const RgbaColor* RgbaImage::GetBuffer() const {
		return pixels.get();
	}

	RgbaColor* RgbaImage::GetBuffer() {
		return pixels.get();
	}

	std::size_t RgbaImage::GetStride() const {
		return 4 * size.width;
	}

	ImageSize RgbaImage::GetSize() const {
		return size;
	}

} // namespace td::tip