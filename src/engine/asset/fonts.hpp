#pragma once

#include "engine/utility/fileio.hpp"
#include "asset_loader.hpp"



namespace oe::asset
{
	class Fonts
	{
	public:
		static oe::utils::FontFile roboto_regular()
		{
			auto ff = oe::utils::FontFile("Roboto-Regular.ttf");
			if(!ff.loaded())
				ff.load(AssetLoader::resource_bytes("font/Roboto/Roboto-Regular.ttf"));
			return ff;
		}
		
		static oe::utils::FontFile roboto_italic()
		{
			auto ff = oe::utils::FontFile("Roboto-Italic.ttf");
			if(!ff.loaded())
				ff.load(AssetLoader::resource_bytes("font/Roboto/Roboto-Italic.ttf"));
			return ff;
		}
	};
}