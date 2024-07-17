// Copyright © 2024 Favro Holding AB
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib::NGraphics
{
	enum class EColorBlend : uint8
	{
		mc_Zero				// (0, 0, 0, 0)
		, mc_One			// (1, 1, 1, 1)
		, mc_SrcColor		// (Rs, Gs, Bs, As)
		, mc_SrcColorInv	// (1 - Rs, 1 - Gs, 1 - Bs, 1 - As)
		, mc_SrcAlpha		// (As, As, As, As)
		, mc_SrcAlphaInv	// (1 - As, 1 - As, 1 - As, 1 - As)
		, mc_DstColor		// (Rd, Gd, Bd, Ad)
		, mc_DstColorInv	// (1 - Rd, 1 - Gd, 1 - Bd, 1 - Ad)
		, mc_DstAlpha		// (Ad, Ad, Ad, Ad)
		, mc_DstAlphaInv	// (1 - Ad, 1 - Ad, 1 - Ad, 1 - Ad)
		, mc_SrcAlphaSat	// (f, f, f, 1), f = min(min(As, 1 - Ad), 1)
	};

	enum class EColorBlendOperation : uint8
	{
		mc_Add					// S0 + S1
		, mc_Subtract			// S1 - S0
		, mc_ReverseSubtract	// S0 - S1
		, mc_Min				// min(S0, S1)
		, mc_Max				// max(S0, S1)
	};

	struct CColorBlend
	{
		auto operator <=> (CColorBlend const &_Right) const = default;

		EColorBlend m_SourceColorBlend = EColorBlend::mc_One;
		EColorBlend m_DestinationColorBlend = EColorBlend::mc_Zero;
		EColorBlendOperation m_ColorBlendOperation = EColorBlendOperation::mc_Add;

		EColorBlend m_SourceAlphaBlend = EColorBlend::mc_One;
		EColorBlend m_DestinationAlphaBlend = EColorBlend::mc_Zero;
		EColorBlendOperation m_AlphaBlendOperation = EColorBlendOperation::mc_Add;
	};
}

#ifndef DMibPNoShortCuts
	using namespace NMib::NGraphics;
#endif
