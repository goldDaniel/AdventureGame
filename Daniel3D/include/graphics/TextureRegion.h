#pragma once 

#include "Texture.h"
#include <glm/glm.hpp>

namespace dg3d
{
	namespace graphics
	{
		class TextureRegion
		{
		private:
			Texture const* mTexture;

			float mRegionX;
			float mRegionY;
			float mRegionWidth;
			float mRegionHeight;

		public: 
			TextureRegion(Texture const* tex)
				: TextureRegion(tex, 
								0, 0, 
								static_cast<float>(tex->mWidth), 
								static_cast<float>(tex->mHeight))
			{}

			TextureRegion(Texture const* tex, float x, float y, float w, float h)
				: mTexture(tex)
				, mRegionX(0)
				, mRegionY(0)
				, mRegionWidth(w)
				, mRegionHeight(h)
			{}

			Texture const* GetTexture() const
			{
				return mTexture;
			}

			glm::vec4 GetRegion() const
			{
				return { mRegionX, mRegionY, mRegionWidth, mRegionHeight };
			}
		};
	}
}