// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include <Mib/Function/Function>

#include "Malterlib_Graphics_PixelFont.h"

namespace NMib::NGraphics
{
	// Reads a file of Fonts/Unbroken by its name, as Unbroken-Regular.ttf
	using FReadUnbrokenFontFile = NFunction::TCFunction<NContainer::CByteVector (NStr::CStr const &_FileName)>;

	CPixelFontSet fg_CreateUnbrokenFontSet(FReadUnbrokenFontFile const &_fReadFile);
}
