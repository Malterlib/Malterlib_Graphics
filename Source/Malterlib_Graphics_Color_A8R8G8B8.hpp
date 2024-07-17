// Copyright © 2024 Unbroken AB
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib::NGraphics
{
	constexpr inline_always uint8 CColorA8R8G8B8::f_Alpha() const
	{
		return (m_Color >> 24) & uint32(0xff);
	}

	constexpr inline_always uint8 CColorA8R8G8B8::f_Red() const
	{
		return (m_Color >> 16) & uint32(0xff);
	}

	constexpr inline_always uint8 CColorA8R8G8B8::f_Green() const
	{
		return (m_Color >> 8) & uint32(0xff);
	}

	constexpr inline_always uint8 CColorA8R8G8B8::f_Blue() const
	{
		return m_Color & uint32(0xff);
	}

	constexpr inline_always CColorA8R8G8B8 CColorA8R8G8B8::fs_FromRGBA(uint8 _Red, uint8 _Green, uint8 _Blue, uint8 _Alpha)
	{
		return {.m_Color = uint32(_Alpha) << 24 | uint32(_Red) << 16 | uint32(_Green) << 8 | uint32(_Blue)};
	}

	template <typename tf_CStr>
	void CColorA8R8G8B8::f_Format(tf_CStr &o_Str) const
	{
		o_Str += typename tf_CStr::CFormat("0x{nfh,sj2,sf0}{nfh,sj2,sf0}{nfh,sj2,sf0}{nfh,sj2,sf0}") << f_Red() << f_Green() << f_Blue() << f_Alpha();
	}

	constexpr CColorA8R8G8B8 CColorA8R8G8B8::operator - (CColorA8R8G8B8 _Other) const
	{
		return CColorA8R8G8B8::fs_FromRGBA
			(
				fg_Max(int16(f_Red()) - int16(_Other.f_Red()), 0)
				, fg_Max(int16(f_Green()) - int16(_Other.f_Green()), 0)
				, fg_Max(int16(f_Blue()) - int16(_Other.f_Blue()), 0)
				, fg_Max(int16(f_Alpha()) - int16(_Other.f_Alpha()), 0)
			)
		;
	}

	constexpr CColorA8R8G8B8 CColorA8R8G8B8::operator + (CColorA8R8G8B8 _Other) const
	{
		return CColorA8R8G8B8::fs_FromRGBA
			(
				fg_Min(uint16(f_Red()) + uint16(_Other.f_Red()), 255)
				, fg_Min(uint16(f_Green()) + uint16(_Other.f_Green()), 255)
				, fg_Min(uint16(f_Blue()) + uint16(_Other.f_Blue()), 255)
				, fg_Min(uint16(f_Alpha()) + uint16(_Other.f_Alpha()), 255)
			)
		;
	}

	constexpr CColorA8R8G8B8 CColorA8R8G8B8::operator * (CColorA8R8G8B8 _Other) const
	{
		return CColorA8R8G8B8::fs_FromRGBA
			(
				(uint16(f_Red()) * uint16(_Other.f_Red())) >> 8
				, (uint16(f_Green()) * uint16(_Other.f_Green())) >> 8
				, (uint16(f_Blue()) * uint16(_Other.f_Blue())) >> 8
				, (uint16(f_Alpha()) * uint16(_Other.f_Alpha())) >> 8
			)
		;
	}

	constexpr CColorA8R8G8B8 CColorA8R8G8B8::f_Min(CColorA8R8G8B8 _Other) const
	{
		return CColorA8R8G8B8::fs_FromRGBA
			(
				fg_Min(f_Red(), _Other.f_Red())
				, fg_Min(f_Green(), _Other.f_Green())
				, fg_Min(f_Blue(), _Other.f_Blue())
				, fg_Min(f_Alpha(), _Other.f_Alpha())
			)
		;
	}

	constexpr CColorA8R8G8B8 CColorA8R8G8B8::f_Max(CColorA8R8G8B8 _Other) const
	{
		return CColorA8R8G8B8::fs_FromRGBA
			(
				fg_Max(f_Red(), _Other.f_Red())
				, fg_Max(f_Green(), _Other.f_Green())
				, fg_Max(f_Blue(), _Other.f_Blue())
				, fg_Max(f_Alpha(), _Other.f_Alpha())
			)
		;
	}

	constexpr CColorA8R8G8B8 CColorA8R8G8B8::f_Invert() const
	{
		return CColorA8R8G8B8::fs_FromRGBA
			(
				255u - f_Red()
				, 255u - f_Green()
				, 255u - f_Blue()
				, 255u - f_Alpha()
			)
		;
	}

	constexpr CColorA8R8G8B8 CColorA8R8G8B8::f_Blend(CColorA8R8G8B8 _DestinationColor, CColorBlend const &_Blend)
	{
		auto fGetColor = [&](EColorBlend _Blend) -> CColorA8R8G8B8
			{
				switch (_Blend)
				{
				case EColorBlend::mc_Zero: return CColorA8R8G8B8::fs_FromRGBA(0, 0, 0, 0);
				case EColorBlend::mc_One: return CColorA8R8G8B8::fs_FromRGBA(255, 255, 255, 255);
				case EColorBlend::mc_SrcColor: return *this;
				case EColorBlend::mc_SrcColorInv: return f_Invert();
				case EColorBlend::mc_SrcAlpha:
					{
						auto Alpha = f_Alpha();
						return CColorA8R8G8B8::fs_FromRGBA(Alpha, Alpha, Alpha, Alpha);
					}
				case EColorBlend::mc_SrcAlphaInv:
					{
						auto Alpha = 255 - f_Alpha();
						return CColorA8R8G8B8::fs_FromRGBA(Alpha, Alpha, Alpha, Alpha);
					}
				case EColorBlend::mc_DstColor: return _DestinationColor;
				case EColorBlend::mc_DstColorInv: return _DestinationColor.f_Invert();
				case EColorBlend::mc_DstAlpha:
					{
						auto Alpha = _DestinationColor.f_Alpha();
						return CColorA8R8G8B8::fs_FromRGBA(Alpha, Alpha, Alpha, Alpha);
					}
				case EColorBlend::mc_DstAlphaInv:
					{
						auto Alpha = 255 - _DestinationColor.f_Alpha();
						return CColorA8R8G8B8::fs_FromRGBA(Alpha, Alpha, Alpha, Alpha);
					}
				case EColorBlend::mc_SrcAlphaSat:
					{
						auto Value = fg_Min(fg_Min(f_Alpha(), 255 - _DestinationColor.f_Alpha()), 255);
						return CColorA8R8G8B8::fs_FromRGBA(Value, Value, Value, 255);
					}
				}

				return CColorA8R8G8B8::fs_FromRGBA(0, 0, 0, 0);
			}
		;

		auto fBlend = [&](EColorBlendOperation _Operation, CColorA8R8G8B8 _Color0, CColorA8R8G8B8 _Color1)
			{
				switch (_Operation)
				{
					case EColorBlendOperation::mc_Add: return _Color0 + _Color1;
					case EColorBlendOperation::mc_Subtract: return _Color1 - _Color0;
					case EColorBlendOperation::mc_ReverseSubtract: return _Color0 - _Color1;
					case EColorBlendOperation::mc_Min: return _Color0.f_Min(_Color1);
					case EColorBlendOperation::mc_Max: return _Color0.f_Max(_Color1);
				}
				return _Color0;
			}
		;

		CColorA8R8G8B8 Color = fBlend(_Blend.m_ColorBlendOperation, fGetColor(_Blend.m_SourceColorBlend), fGetColor(_Blend.m_DestinationColorBlend));
		CColorA8R8G8B8 Alpha = fBlend(_Blend.m_AlphaBlendOperation, fGetColor(_Blend.m_SourceAlphaBlend), fGetColor(_Blend.m_DestinationAlphaBlend));

		return CColorA8R8G8B8::fs_FromRGBA(Color.f_Red(), Color.f_Green(), Color.f_Blue(), Alpha.f_Alpha());
	}
}

#ifndef DMibPNoShortCuts
	using namespace NMib::NGraphics;
#endif
