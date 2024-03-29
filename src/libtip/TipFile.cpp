//
// TIPDump2
//
// (C) 2021-2022 modeco80 <lily.modeco80@protonmail.ch>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include <tip/TipFile.h>

#include "TrivialStreamRead.h"

namespace td::tip {

	TipReadError TipFile::ReadFromStream(std::istream& is) {
		if(!MaybeRead(is, header))
			return TipReadError::FailureReadingHeader;

		// Do magic check(s).
		if(header.Magic[0] != 'T' && header.Magic[1] != 'I' && header.Magic[2] == 'P')
			if(header.Magic[3] != '0' || header.Magic[3] != '1')
				return TipReadError::NotTip;

		// Read images.
		for(std::size_t i = 0; i < header.ImageCount / 2; ++i) {
			TipImage image;

			if(!image.ReadFromStream(is))
				return TipReadError::FailureReadingImage;

			images.emplace_back(std::move(image));
		}

		return TipReadError::NoError;
	}

	std::vector<TipImage>& TipFile::GetImages() {
		return images;
	}

	const TipFileHdr& TipFile::GetHeader() const {
		return header;
	}
} // namespace td::tip