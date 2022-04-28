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
		imageBytes.resize(imageHeader.ImageDataSize());
		if(!is.read(reinterpret_cast<char*>(&imageBytes[0]), imageHeader.ImageDataSize()))
			return false;

		// Then the CLUT image
		// TODO: NMV1 LOB.TIP's first image does NOT have a CLUT???????

		// CLUT header
		if(!MaybeRead(is, clutHeader))
			return false;

		// CLUT data
		clutBytes.resize(clutHeader.ImageDataSize());
		if(!is.read(reinterpret_cast<char*>(&clutBytes[0]), clutHeader.ImageDataSize()))
			return false;

		// done!
		return true;
	}

	ImageSize TipImage::Size() const {
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

	RgbaImage TipImage::ToRgba() {
		RgbaImage img;
		auto size = Size();
		auto& pal = Palette();

		// Setup the image properly
		img.size = size;
		img.pixels.resize(size.width * size.height);

		auto* imagePtr = img.pixels.data();

		if(imageHeader.ImageFlags & TipImageHdr::IMAGEFLAG_8BPP) {
			for(uint32_t i = 0; i < size.width * size.height; ++i)
				*(imagePtr++) = pal[imageBytes[i]];
		} else {
			// Sample as 4bpp.
			for(uint32_t i = 0; i < (size.width * size.height / 2); ++i) {
				auto bt = imageBytes[i];

				for(int b = 0; b < 2; b++) {
					auto t = (uint16_t)((bt & (0x0F << (b * 4))) >> (b * 4));
					*(imagePtr++) = pal[t];
				}
			}
		}

		return img;
	}

	const std::vector<RgbaColor>& TipImage::Palette() {
		// Lazily compute if we haven't computed the palette.

		if(!paletteComputed) {
			palette.resize(imageHeader.ImageFlags & TipImageHdr::IMAGEFLAG_8BPP ? 255 : 16);

			std::size_t i = 0;

			// std::cout << "clut's at " << clutHeader.ImageRect.x << 'x' << clutHeader.ImageRect.y << '\n';

			for(auto& color : palette) {
				std::uint16_t total = (clutBytes[i + 1] << 8) | clutBytes[i];

				// PS1 GPU considers all black (0x0000) completely transparent.
				if(total == 0x0000)
					color.a = 0;
				else
					// This is where things potentionally get a bit more complicated.
					//
					// In theory we would need to test for the semi-transparent bit
					// (see https://psx-spx.consoledev.net/graphicsprocessingunitgpu/#texture-bitmaps and
					// https://psx-spx.consoledev.net/graphicsprocessingunitgpu/#texture-palettes-clut-color-lookup-table)
					//
					// but that doesn't seem like it matters too much (and any attempt to
					// test for it breaks all other images, so for now we don't).
					color.a = 255;

				color.b = (total & 0x7C00) >> 7;
				color.g = (total & 0x03E0) >> 2;
				color.r = (total & 0x001F) << 3;

				// std::cout << "color " << i / 2 << ": " << (int)color.r << ',' << (int)color.g << ',' << (int)color.b << '\n';

				i += sizeof(std::uint16_t); // The color is 16 bits, 2 bytes per color.
			}
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
