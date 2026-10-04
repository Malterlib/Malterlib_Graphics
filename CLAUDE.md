# CLAUDE.md - Graphics Module

## Module Overview

The Graphics module has images, the colors of their pixels, and color spaces:

- **Images** - `CImage` and `CImageMemory`, with pixel formats, conversions between them, and reading
  PNG and JPEG files
- **Pixels** - `CColorR8G8B8` and `CColorA8R8G8B8`, and blending
- **Color spaces** - primaries, curves and the conversions between color spaces, which images and
  renderers use

The namespace is `NMib::NGraphics`.

## Color Spaces

`Mib/Graphics/ColorSpace` has color spaces and the conversions between them. It does not depend on
anything that renders.

| Type | Is |
|---|---|
| `CColorPrimaries` | The chromaticities of red, green, blue and white. `fs_Rec709`, `fs_DisplayP3`, `fs_Rec2020`, `fs_AdobeRGB` and `fs_AcesAP1` |
| `EColorCurve` | How values are stored: linear, sRGB, gamma 2.2, gamma 2.4, a gamma of the color space, Rec. 709, PQ and HLG |
| `CColorSpace` | Primaries with a curve. `fs_sRGB`, `fs_DisplayP3`, `fs_Rec2020`, `fs_Rec2100PQ`, `fs_Rec2100HLG` and `fs_Linear` |
| `CColorMatrix` | 3 by 3, in `pfp64` |
| `CColor` | A color with its color space, in `pfp32` |

- `fg_GetColorMatrix(From, To)` goes through CIE XYZ, with the Bradford adaptation when the white
  points differ. It is for linear values.
- `fg_ColorCurveToLinear` and `fg_ColorCurveFromLinear` are for one value. The curves with a
  power keep the sign of values below 0, so colors outside the primaries go there and back. PQ
  and HLG have no values below 0.
- The linear values of PQ are parts of 10000 nits, and the ones of HLG are parts of the most
  that is encoded. `fg_ConvertColor` takes linear values to be the same light in every color
  space, so where white is with those curves is for the caller.
- `CColorPrimaries::f_Contains` takes a tolerance, as the red of Display P3 is 0.0012 outside
  Rec. 2020.
- `fg_GetColorSpaceFromCodingIndependent` is for the code points of H.273 that PNG and video
  have, `fg_GetColorSpaceFromProfile` for ICC profiles for RGB with a matrix and curves, and
  `fg_GetColorSpaceFromChromaticities` for the chromaticities and the gamma of PNG. They return
  the known primaries when what they read is close to them, so that comparing primaries tells
  if a conversion is needed.
- Everything is calculated in `pfp64`, for when something is set up. What is done for each pixel
  is a matrix that was calculated before, and a table for the curve.

```cpp
#include <Mib/Graphics/ColorSpace>

// The red of Display P3 as linear values with the primaries of Rec. 709, where it is outside
CColor Red(1.0f, 0.0f, 0.0f, 1.0f, CColorSpace::fs_DisplayP3());
CColor InRec709 = Red.f_ConvertTo(CColorSpace::fs_Linear(CColorPrimaries::fs_Rec709()));

// For many values: the curve, the matrix and the curve
CColorMatrix Matrix = fg_GetColorMatrix(CColorPrimaries::fs_Rec2020(), CColorPrimaries::fs_DisplayP3());

pfp64 Linear[3];
for (umint i = 0; i < 3; ++i)
	Linear[i] = fg_ColorCurveToLinear(EColorCurve::mc_Rec709, Values[i]);

Matrix.f_Transform(Linear, Linear);
```

## Testing

```bash
MalterlibBuildShowProgress=false ./mib build-target Tests Com_Test_Malterlib_Graphics
/opt/Deploy/Tests/RunAllTests --paths '["Malterlib/Graphics/*"]'
```
