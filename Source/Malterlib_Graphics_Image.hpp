// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

namespace NMib::NGraphics
{
	// TCImageRect implementations

	template <typename t_CPoint>
	typename TCImageRect<t_CPoint>::CType TCImageRect<t_CPoint>::f_GetWidth()
	{
		return m_Corners[1][0] - m_Corners[0][0];
	}

	template <typename t_CPoint>
	typename TCImageRect<t_CPoint>::CType TCImageRect<t_CPoint>::f_GetHeight()
	{
		return m_Corners[1][1] - m_Corners[0][1];
	}

	template <typename t_CPoint>
	typename TCImageRect<t_CPoint>::CType TCImageRect<t_CPoint>::f_GetDepth()
	{
		return m_Corners[1][2] - m_Corners[0][2];
	}

	template <typename t_CPoint>
	typename TCImageRect<t_CPoint>::CType TCImageRect<t_CPoint>::f_GetNumArrays()
	{
		return m_Corners[1][3] - m_Corners[0][3];
	}

	// TCImageDimensions implementations

	template <typename t_CPoint>
	TCImageDimensions<t_CPoint>::TCImageDimensions(CType _Width, CType _Height, CType _Depth, CType _Arrays)
	{
		m_Dimensions[0] = _Width;
		m_Dimensions[1] = _Height;
		m_Dimensions[2] = _Depth;
		m_Dimensions[3] = _Arrays;
	}

	template <typename t_CPoint>
	typename TCImageDimensions<t_CPoint>::CType TCImageDimensions<t_CPoint>::f_GetWidth()
	{
		return m_Dimensions[0];
	}

	template <typename t_CPoint>
	typename TCImageDimensions<t_CPoint>::CType TCImageDimensions<t_CPoint>::f_GetHeight()
	{
		return m_Dimensions[1];
	}

	template <typename t_CPoint>
	typename TCImageDimensions<t_CPoint>::CType TCImageDimensions<t_CPoint>::f_GetDepth()
	{
		return m_Dimensions[2];
	}

	template <typename t_CPoint>
	typename TCImageDimensions<t_CPoint>::CType TCImageDimensions<t_CPoint>::f_GetNumArrays()
	{
		return m_Dimensions[3];
	}

	template <typename t_CPoint>
	bool TCImageDimensions<t_CPoint>::operator == (const TCImageDimensions &_Other) const noexcept
	{
		if (m_Dimensions[0] != _Other.m_Dimensions[0])
			return false;
		if (m_Dimensions[1] != _Other.m_Dimensions[1])
			return false;
		if (m_Dimensions[2] != _Other.m_Dimensions[2])
			return false;
		if (m_Dimensions[3] != _Other.m_Dimensions[3])
			return false;
		return true;
	}

	template <typename t_CPoint>
	COrdering_Strong TCImageDimensions<t_CPoint>::operator <=> (const TCImageDimensions &_Other) const noexcept
	{
		if (auto Result = m_Dimensions[0] <=> _Other.m_Dimensions[0]; Result != 0)
			return Result;
		if (auto Result = m_Dimensions[1] <=> _Other.m_Dimensions[1]; Result != 0)
			return Result;
		if (auto Result = m_Dimensions[2] <=> _Other.m_Dimensions[2]; Result != 0)
			return Result;
		if (auto Result = m_Dimensions[3] <=> _Other.m_Dimensions[3]; Result != 0)
			return Result;

		return COrdering_Strong::equal;
	}

	template <typename t_CPoint>
	typename TCImageDimensions<t_CPoint>::CType &TCImageDimensions<t_CPoint>::operator[] (int _Pos)
	{
		return m_Dimensions[_Pos];
	}

	// TCImageStride implementations

	template <typename t_CPoint>
	TCImageStride<t_CPoint>::TCImageStride()
	{
		m_Stride[0] = 0;
		m_Stride[1] = 0;
		m_Stride[2] = 0;
	}

	template <typename t_CPoint>
	TCImageStride<t_CPoint>::TCImageStride(CType _Width, CType _Height, CType _Depth)
	{
		m_Stride[0] = _Width;
		m_Stride[1] = _Height;
		m_Stride[2] = _Depth;
	}

	template <typename t_CPoint>
	typename TCImageStride<t_CPoint>::CType TCImageStride<t_CPoint>::f_GetWidth()
	{
		return m_Stride[0];
	}

	template <typename t_CPoint>
	typename TCImageStride<t_CPoint>::CType TCImageStride<t_CPoint>::f_GetHeight()
	{
		return m_Stride[1];
	}

	template <typename t_CPoint>
	typename TCImageStride<t_CPoint>::CType TCImageStride<t_CPoint>::f_GetDepth()
	{
		return m_Stride[2];
	}

	template <typename t_CPoint>
	typename TCImageStride<t_CPoint>::CType &TCImageStride<t_CPoint>::operator[] (int _Pos)
	{
		return m_Stride[_Pos];
	}

	// CImage template implementations

	template <uint64 _FromFormat, uint64 _ToFormat>
	void CImage::fs_ConvertImage(CImage &_From, CImage &_To)
	{
		CLockedData From;
		CLockedData To;
		if (!_From.f_LockRead(From))
			DMibError("Failed to lock source image");
		if (!_To.f_LockWrite(To))
		{
			_From.f_UnLock(From);
			DMibError("Failed to lock destinatio image");
		}
		CImageDimensions Dimensions;
		Dimensions.m_Dimensions[0] = fg_Min(_From.m_Dimensions.m_Dimensions[0], _To.m_Dimensions.m_Dimensions[0]);
		Dimensions.m_Dimensions[1] = fg_Min(_From.m_Dimensions.m_Dimensions[1], _To.m_Dimensions.m_Dimensions[1]);
		Dimensions.m_Dimensions[2] = fg_Min(_From.m_Dimensions.m_Dimensions[2], _To.m_Dimensions.m_Dimensions[2]);
		Dimensions.m_Dimensions[3] = fg_Min(_From.m_Dimensions.m_Dimensions[3], _To.m_Dimensions.m_Dimensions[3]);

		TCImageConversion<_FromFormat, _ToFormat>::fs_ConvertImage(From.m_pData, To.m_pData, Dimensions, From.m_Stride, To.m_Stride);

		_To.f_UnLock(To);
		_From.f_UnLock(From);
	}
}
