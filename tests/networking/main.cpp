#include <engine/include.hpp>

#include <string>
#include <thread>



/*

	Simple ping/pong with unnecessary compression

*/

int main(int argc, char* argv[]) {
	// engine
	oe::EngineInfo engine_info = {};
	engine_info.networking = true;
	oe::Engine::getSingleton().init(engine_info);

	int port = 12221;
	auto& compression = oe::utils::Compression::getSingleton();

	// open the server
	auto server = new oe::networking::Server(); 
	server->open("localhost", port);
	server->setConnectCallback([&](size_t client_id) { spdlog::info("{} connected", client_id); });
	server->setDisconnectCallback([&](size_t client_id) { spdlog::info("{} disconnected", client_id); });
	server->setReciveCallback([&](size_t client_id, const unsigned char* data, size_t size) {
		// decompress recieved message
		std::vector<uint8_t> uncompressed = compression.uncompress(data, size);
		spdlog::info("server recieved message: {}", compression.data_to_string(uncompressed));

		// compress message
		const std::string message = "ping (from server)";
		std::vector<uint8_t> compressed = compression.compress(compression.string_to_data(message));
		server->send(compressed.data(), compressed.size(), client_id);
	});
	
	// connect to it with client
	auto client = new oe::networking::Client();
	client->connect("localhost", port);
	client->setReciveCallback([&](const uint8_t* data, size_t size) {
		// decompress recieved message
		std::vector<uint8_t> uncompressed = compression.uncompress(data, size);
		spdlog::info("client recieved message: {}", compression.data_to_string(uncompressed));

		// compress message
		const std::string message = "pong (from client)";
		std::vector<uint8_t> compressed = compression.compress(compression.string_to_data(message));
		client->send(compressed.data(), compressed.size());
	});

	// compress initial message
	const std::string message = "invoke (from server)";
	std::vector<uint8_t> compressed = compression.compress(compression.string_to_data(message));
	server->send(compressed.data(), compressed.size());

	// wait for messages to get recieved
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	// close the client
	client->disconnect();
	client->close();

	// close the server
	server->close();

	return 0;
}