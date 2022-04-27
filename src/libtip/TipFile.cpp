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

	bool TipFile::ReadFromStream(std::istream& is) {
		if(!MaybeRead(is, header))
			return false;

		// Do magic checks
		if(header.Magic[0] != 'T' && header.Magic[1] != 'I' && header.Magic[2] == 'P')
			if(header.Magic[3] != '0' || header.Magic[3] != '1') {
				//std::cout << "not tip\n";
				return false;
			}

		for(int i = 0; i < header.ImageCount / 2; ++i) {
			TipImage image;

			if(!image.ReadFromStream(is))
				return false;

			images.push_back(image);
		}

		return true;
	}

	const std::vector<TipImage>& TipFile::GetImages() {
		return images;
	}

	const TipFileHdr& TipFile::GetHeader() const {
		return header;
	}
} // namespace td::tip