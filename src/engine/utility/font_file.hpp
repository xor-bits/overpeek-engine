#pragma once



namespace oe::utils
{
	class FileIO;
	using byte_string = std::vector<uint8_t>;

	

	class FontFile
	{
	private:
		size_t id;

	public:
		FontFile(const std::string& name);
		FontFile();

		const void load(const oe::utils::byte_string& bytes);

		[[nodiscard]] constexpr inline size_t getID() const noexcept { return id; }
		[[nodiscard]] const oe::utils::byte_string& fontFile() const;
		[[nodiscard]] const static oe::utils::byte_string& getFontFile(const size_t id);
		[[nodiscard]] const bool loaded();
		[[nodiscard]] constexpr inline bool operator==(const FontFile &other) const noexcept { return id == other.id; }
	};
}