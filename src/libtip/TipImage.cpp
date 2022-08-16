//
// TIPDump2
//
// (C) 2021-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include <tip/TipImage.h>

#include "TrivialStreamRead.h"

namespace td::tip {

	bool TipImage::ReadFromStream(std::istream& is) {
		// RgbaImage header
		if(!MaybeRead(is, imageHeader))
			return false;

		// RgbaImage data
		imageBytes.reset(new std::uint8_t[imageHeader.ImageDataSize()]);
		if(!is.read(reinterpret_cast<char*>(imageBytes.get()), imageHeader.ImageDataSize()))
			return false;

		// Then the CLUT image
		// TODO: NMV1 LOB.TIP's first image does NOT have a CLUT???????

		// CLUT header
		if(!MaybeRead(is, clutHeader))
			return false;

		// CLUT data
		clutBytes.reset(new std::uint8_t[clutHeader.ImageDataSize()]);
		if(!is.read(reinterpret_cast<char*>(clutBytes.get()), clutHeader.ImageDataSize()))
			return false;

		// done!
		return true;
	}

	void TipImage::Clear() {
		if(imageBytes)
			imageBytes.reset();

		if(clutBytes)
			clutBytes.reset();
	}

	pixel::ImageSize TipImage::Size() const {
		return {
			// For some reason, depending on the BPP of the image, width needs to be multiplied by:
			// 8bpp : 2
			// 4bpp : 4
			//
			// I don't really understand why, especially since height is unaffected.
			static_cast<std::uint16_t>((imageHeader.ImageFlags & TipImageHdr::IMAGEFLAG_8BPP) ? imageHeader.ImageRect.w * 2 : imageHeader.ImageRect.w * 4),
			imageHeader.ImageRect.h
		};
	}

	pixel::RgbaImage TipImage::ToRgba() {
		 if(imageHeader.ImageFlags & TipImageHdr::IMAGEFLAG_8BPP)
		 		return pixel::RgbaImage::From8Bpp(imageBytes.get(), Palette().data(), Size());
		 else
		 		return pixel::RgbaImage::From4Bpp(imageBytes.get(), Palette().data(), Size());
	}

	const std::array<pixel::RgbaColor, 256>& TipImage::Palette() {
		// Lazily compute if we haven't computed the palette.
		if(!paletteComputed) {

			if(imageHeader.ImageFlags & TipImageHdr::IMAGEFLAG_8BPP)
				pixel::RgbaColor::PaletteFrom16Bpp(&palette[0], clutBytes.get(), 256);
			else
				pixel::RgbaColor::PaletteFrom16Bpp(&palette[0], clutBytes.get(), 16);

			// Sample the palette again for "working alpha".
			// Grody, but it works, so...

			const auto count = (imageHeader.ImageFlags & TipImageHdr::IMAGEFLAG_8BPP) ? 256 : 16;
			for(int i = 0; i < count; ++i) {
				const auto paletteIndex = i * sizeof(std::uint16_t);
				std::uint16_t total = (clutBytes[paletteIndex + 1] << 8) | clutBytes[paletteIndex];

				// PS1 GPU considers all black (0x0000) completely transparent.
				if(total == 0x0000)
					palette[i].a = 0;
			}

			paletteComputed = true;
		}

		// Otherwise we can return a const ref to the palette right away!
		return palette;
	}

	uint16_t TipImage::Index() const {
		return imageHeader.Index;
	}

	const TipImageHdr& TipImage::Header() const {
		return imageHeader;
	}

	bool TipImage::Is8Bpp() const {
		return imageHeader.ImageFlags & TipImageHdr::IMAGEFLAG_8BPP;
	}

} // namespace td::tip
