// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

namespace NMib::NGraphics
{
	// TCImageRect implementations

	template <typename t_CType>
	typename TCImageRect<t_CType>::CType TCImageRect<t_CType>::f_GetWidth()
	{
		return m_Corners[1][0] - m_Corners[0][0];
	}

	template <typename t_CType>
	typename TCImageRect<t_CType>::CType TCImageRect<t_CType>::f_GetHeight()
	{
		return m_Corners[1][1] - m_Corners[0][1];
	}

	template <typename t_CType>
	typename TCImageRect<t_CType>::CType TCImageRect<t_CType>::f_GetDepth()
	{
		return m_Corners[1][2] - m_Corners[0][2];
	}

	template <typename t_CType>
	typename TCImageRect<t_CType>::CType TCImageRect<t_CType>::f_GetNumArrays()
	{
		return m_Corners[1][3] - m_Corners[0][3];
	}

	// TCImageDimensions implementations

	template <typename t_CType>
	TCImageDimensions<t_CType>::TCImageDimensions(CType _Width, CType _Height, CType _Depth, CType _Arrays)
	{
		m_Dimensions[0] = _Width;
		m_Dimensions[1] = _Height;
		m_Dimensions[2] = _Depth;
		m_Dimensions[3] = _Arrays;
	}

	template <typename t_CType>
	typename TCImageDimensions<t_CType>::CType TCImageDimensions<t_CType>::f_GetWidth()
	{
		return m_Dimensions[0];
	}

	template <typename t_CType>
	typename TCImageDimensions<t_CType>::CType TCImageDimensions<t_CType>::f_GetHeight()
	{
		return m_Dimensions[1];
	}

	template <typename t_CType>
	typename TCImageDimensions<t_CType>::CType TCImageDimensions<t_CType>::f_GetDepth()
	{
		return m_Dimensions[2];
	}

	template <typename t_CType>
	typename TCImageDimensions<t_CType>::CType TCImageDimensions<t_CType>::f_GetNumArrays()
	{
		return m_Dimensions[3];
	}

	template <typename t_CType>
	bool TCImageDimensions<t_CType>::operator == (const TCImageDimensions &_Other) const noexcept
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

	template <typename t_CType>
	COrdering_Strong TCImageDimensions<t_CType>::operator <=> (const TCImageDimensions &_Other) const noexcept
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

	template <typename t_CType>
	typename TCImageDimensions<t_CType>::CType &TCImageDimensions<t_CType>::operator[] (int _Pos)
	{
		return m_Dimensions[_Pos];
	}

	// TCImageStride implementations

	template <typename t_CType>
	TCImageStride<t_CType>::TCImageStride()
	{
		m_Stride[0] = 0;
		m_Stride[1] = 0;
		m_Stride[2] = 0;
	}

	template <typename t_CType>
	TCImageStride<t_CType>::TCImageStride(CType _Width, CType _Height, CType _Depth)
	{
		m_Stride[0] = _Width;
		m_Stride[1] = _Height;
		m_Stride[2] = _Depth;
	}

	template <typename t_CType>
	typename TCImageStride<t_CType>::CType TCImageStride<t_CType>::f_GetWidth()
	{
		return m_Stride[0];
	}

	template <typename t_CType>
	typename TCImageStride<t_CType>::CType TCImageStride<t_CType>::f_GetHeight()
	{
		return m_Stride[1];
	}

	template <typename t_CType>
	typename TCImageStride<t_CType>::CType TCImageStride<t_CType>::f_GetDepth()
	{
		return m_Stride[2];
	}

	template <typename t_CType>
	typename TCImageStride<t_CType>::CType &TCImageStride<t_CType>::operator[] (int _Pos)
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
