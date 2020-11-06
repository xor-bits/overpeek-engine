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
			return oe::utils::FontFile(AssetLoader::get().path() / "font/Roboto/Roboto-Regular.ttf");
		}
		
		static oe::utils::FontFile roboto_italic()
		{
			return oe::utils::FontFile(AssetLoader::get().path() / "font/Roboto/Roboto-Italic.ttf");
		}
	};
}