#pragma once 


namespace dg3d
{
	namespace graphics
	{
		enum class PlayMode
		{
			Normal,
			Loop,
			Reverse,
			ReverseLoop
		};

		class TextureRegion;

		class Animation
		{
		private:
			std::vector<TextureRegion> mFrames;

			float mAnimationTime;
			float mCurrentTime;

			PlayMode mPlayMode;

		public:
			Animation(const std::vector<TextureRegion>& frames, float animationTime, PlayMode mode = PlayMode::Normal)
				: mFrames(frames)
				, mAnimationTime(animationTime)
				, mCurrentTime(0)
				, mPlayMode(mode)
			{
			}

			void Update(float dt)
			{
				mCurrentTime += dt;
				if (mCurrentTime > mAnimationTime)
				{
					if (mPlayMode == PlayMode::Normal ||
						mPlayMode == PlayMode::Reverse)
					{	
						mCurrentTime = mAnimationTime;	
					}
					else
					{
						while (mCurrentTime > mAnimationTime)
						{
							mCurrentTime -= mAnimationTime;
						}
					}
				}
			}

			const TextureRegion& GetCurrentFrame() const
			{
				float percent = mCurrentTime / mAnimationTime;
				
				if (mPlayMode == PlayMode::ReverseLoop ||
					mPlayMode == PlayMode::Loop)
				{
					while (percent >= 1.0f)
					{
						percent -= 1.0f;
					}
				}
				
				if (mPlayMode == PlayMode::Normal ||
					mPlayMode == PlayMode::Reverse)
				{
					if (percent > 1.0f)
					{
						percent = 1.0f;
					}
				}

				if (mPlayMode == PlayMode::Reverse ||
					mPlayMode == PlayMode::ReverseLoop)
				{
					percent = 1.0f - percent;
				}

				const size_t index = static_cast<size_t>(glm::round(percent * (mFrames.size() - 1)));
				return mFrames[index];
			}
		};
	}
}