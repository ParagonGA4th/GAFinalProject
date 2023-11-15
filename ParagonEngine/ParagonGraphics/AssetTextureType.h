#pragma once

/// <summary>
/// Texture Load에 있어서 Define 하나에 TextureType까지 정의하면
/// 필요 이상의 데이터 로드 필요, 분리.
/// </summary>
namespace Pg::Graphics
{
	enum eAssetTextureType
	{
		/** Dummy value.
		 *
		 *  No texture, but the value to be used as 'texture semantic'
		 *  ("#aiMaterialProperty::mSemantic) for all material properties
		 *  *not* related to textures.
		 */
		MGRT_TextureType_NONE = 0,

		/** LEGACY API MATERIALS
		 * Legacy refers to materials which
		 * Were originally implemented in the specifications around 2000.
		 * These must never be removed, as most engines support them.
		 */

		 /** The texture is combined with the result of the diffuse
		  *  lighting equation.
		  *  OR
		  *  PBR Specular/Glossiness
		  */
		  MGRT_TextureType_DIFFUSE = 1,

		  /** The texture is combined with the result of the specular
		   *  lighting equation.
		   *  OR
		   *  PBR Specular/Glossiness
		   */
		   MGRT_TextureType_SPECULAR = 2,

		   /** The texture is combined with the result of the ambient
			*  lighting equation.
			*/
			MGRT_TextureType_AMBIENT = 3,

			/** The texture is added to the result of the lighting
			 *  calculation. It isn't influenced by incoming light.
			 */
			 MGRT_TextureType_EMISSIVE = 4,

			 /** The texture is a height map.
			  *
			  *  By convention, higher gray-scale values stand for
			  *  higher elevations from the base height.
			  */
			  MGRT_TextureType_HEIGHT = 5,

			  /** The texture is a (tangent space) normal-map.
			   *
			   *  Again, there are several conventions for tangent-space
			   *  normal maps. Assimp does (intentionally) not
			   *  distinguish here.
			   */
			   MGRT_TextureType_NORMALS = 6,

			   /** The texture defines the glossiness of the material.
				*
				*  The glossiness is in fact the exponent of the specular
				*  (phong) lighting equation. Usually there is a conversion
				*  function defined to map the linear color values in the
				*  texture to a suitable exponent. Have fun.
			   */
			   MGRT_TextureType_SHININESS = 7,

			   /** The texture defines per-pixel opacity.
				*
				*  Usually 'white' means opaque and 'black' means
				*  'transparency'. Or quite the opposite. Have fun.
			   */
			   MGRT_TextureType_OPACITY = 8,

			   /** Displacement texture
				*
				*  The exact purpose and format is application-dependent.
				*  Higher color values stand for higher vertex displacements.
			   */
			   MGRT_TextureType_DISPLACEMENT = 9,

			   /** Lightmap texture (aka Ambient Occlusion)
				*
				*  Both 'Lightmaps' and dedicated 'ambient occlusion maps' are
				*  covered by this material property. The texture contains a
				*  scaling value for the final color value of a pixel. Its
				*  intensity is not affected by incoming light.
			   */
			   MGRT_TextureType_LIGHTMAP = 10,

			   /** Reflection texture
				*
				* Contains the color of a perfect mirror reflection.
				* Rarely used, almost never for real-time applications.
			   */
			   MGRT_TextureType_REFLECTION = 11,

			   /** PBR Materials
				* PBR definitions from maya and other modelling packages now use this standard.
				* This was originally introduced around 2012.
				* Support for this is in game engines like Godot, Unreal or Unity3D.
				* Modelling packages which use this are very common now.
				*/

				MGRT_TextureType_BASE_COLOR = 12,
				MGRT_TextureType_NORMAL_CAMERA = 13,
				MGRT_TextureType_EMISSION_COLOR = 14,
				MGRT_TextureType_METALNESS = 15,
				MGRT_TextureType_DIFFUSE_ROUGHNESS = 16,
				MGRT_TextureType_AMBIENT_OCCLUSION = 17,

				/** PBR Material Modifiers
				* Some modern renderers have further PBR modifiers that may be overlaid
				* on top of the 'base' PBR materials for additional realism.
				* These use multiple texture maps, so only the base type is directly defined
				*/

				/** Sheen
				* Generally used to simulate textiles that are covered in a layer of microfibers
				* eg velvet
				* https://github.com/KhronosGroup/glTF/tree/master/extensions/2.0/Khronos/KHR_materials_sheen
				*/
				MGRT_TextureType_SHEEN = 19,

				/** Clearcoat
				* Simulates a layer of 'polish' or 'lacquer' layered on top of a PBR substrate
				* https://autodesk.github.io/standard-surface/#closures/coating
				* https://github.com/KhronosGroup/glTF/tree/master/extensions/2.0/Khronos/KHR_materials_clearcoat
				*/
				MGRT_TextureType_CLEARCOAT = 20,

				/** Transmission
				* Simulates transmission through the surface
				* May include further information such as wall thickness
				*/
				MGRT_TextureType_TRANSMISSION = 21,

				/** Unknown texture
				 *
				 *  A texture reference that does not match any of the definitions
				 *  above is considered to be 'unknown'. It is still imported,
				 *  but is excluded from any further post-processing.
				*/
				MGRT_TextureType_UNKNOWN = 18,

#ifndef SWIG
				MGRT_TextureType_Force32Bit = INT_MAX
#endif
	};
}