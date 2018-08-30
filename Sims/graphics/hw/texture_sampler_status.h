/********************************************************************
	created:	2016/11/07
	created:	7:11:2016   16:39
	filename: 	D:\Code\Sims\Sims\graphics\hw\texture_sampler_status.h
	file path:	D:\Code\Sims\Sims\graphics\hw
	file base:	texture_sampler_status
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Texture Sampler Status
*********************************************************************/
#ifndef __TEXTURE_SAMPLER_STATUS_H__
#define __TEXTURE_SAMPLER_STATUS_H__

#include "graphics/graphics_fwd.h"

namespace sims
{
	class TextureSamplerStatus
	{
	public:
		TextureSamplerStatus()
			: filterMin_(TextureFilter::Linear)
			, filterMag_(TextureFilter::Linear)
			, filterMip_(TextureFilter::Linear)
			, wrapS_(TextureWrap::Clamp)
			, wrapT_(TextureWrap::Clamp)
			, borderColor_(0x00)
		{}

		TextureFilter::Type GetFilterMin() const { return filterMin_; }
		TextureFilter::Type GetFilterMag() const { return filterMag_; }
		TextureFilter::Type GetFilterMip() const { return filterMip_; }
		void SetFilterMin(TextureFilter::Type filter) { filterMin_ = filter; }
		void SetFilterMag(TextureFilter::Type filter) { filterMag_ = filter; }
		void SetFilterMip(TextureFilter::Type filter) { filterMip_ = filter; }

		TextureWrap::Type GetWrapS() const { return wrapS_; }
		TextureWrap::Type GetWrapT() const { return wrapT_; }
		Color GetBorderColor() const { return borderColor_; }
		void SetWrapS(TextureWrap::Type wrap) { wrapS_ = wrap; }
		void SetWrapT(TextureWrap::Type wrap) { wrapT_ = wrap; }
		void SetBorderColor(const Color& c) { borderColor_ = c; }
	private:
		Color borderColor_;
		TextureFilter::Type filterMin_;
		TextureFilter::Type filterMag_;
		TextureFilter::Type filterMip_;
		TextureWrap::Type wrapS_;
		TextureWrap::Type wrapT_;
	};
}

#endif
