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
		const auto size = Size();
		pixel::RgbaImage img(size);
		const auto* pal = Palette().data();
		auto* buffer = img.GetBuffer();

		// when API implemented in libpixel, replace this with
		// if(imageHeader.ImageFlags & TipImageHdr::IMAGEFLAG_8BPP)
		// 		return pixel::RgbaImage::From8Bpp(imageBytes.data(), Palette().Data(), Size);
		// else
		// 		return pixel::RgbaImage::From4Bpp(imageBytes.data(), Palette().Data(), Size);

		if(imageHeader.ImageFlags & TipImageHdr::IMAGEFLAG_8BPP) {
			for(std::size_t i = 0; i < size.width * size.height; ++i)
				*(buffer++) = pal[imageBytes[i]];
		} else {
			// Sample as 4bpp.
			for(std::size_t i = 0; i < (size.width * size.height / 2); ++i)
				for(std::size_t b = 0; b < 2; b++)
					*(buffer++) = pal[static_cast<std::uint16_t>(((imageBytes[i] & (0x0F << (b * 4))) >> (b * 4)))];
		}

		// It might be tempting to place a std::move() here, but that inhibits RVO, which will itself
		// avoid a call to the copy constructor.
		// There's no real failure case, so we're cool.
		return img;
	}

	const std::array<pixel::RgbaColor, 256>& TipImage::Palette() {
		// Lazily compute if we haven't computed the palette.
		if(!paletteComputed) {
			const auto count = (imageHeader.ImageFlags & TipImageHdr::IMAGEFLAG_8BPP) ? 256 : 16;

			for(int i = 0; i < count; ++i) {
				const auto paletteIndex = i * sizeof(std::uint16_t);
				std::uint16_t total = (clutBytes[paletteIndex + 1] << 8) | clutBytes[paletteIndex];

				// PS1 GPU considers all black (0x0000) completely transparent.
				if(total == 0x0000)
					palette[i].a = 0;
				else
					// This is where things potentially get a bit more complicated.
					//
					// In theory, we would need to test for the semi-transparent bit
					// (see https://psx-spx.consoledev.net/graphicsprocessingunitgpu/#texture-bitmaps and
					// https://psx-spx.consoledev.net/graphicsprocessingunitgpu/#texture-palettes-clut-color-lookup-table)
					//
					// but that doesn't seem like it matters too much (and any attempt to
					// test for it breaks all other images, so for now we don't).
					palette[i].a = 255;

				palette[i].b = (total & 0x7C00) >> 7;
				palette[i].g = (total & 0x03E0) >> 2;
				palette[i].r = (total & 0x001F) << 3;

				// std::cout << "color " << i << ": " << (int)color.r << ',' << (int)color.g << ',' << (int)color.b << '\n';
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
