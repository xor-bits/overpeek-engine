#include <engine/engine.h>

#include <string>
#include <thread>



int main(int argc, char* argv[]) {

	int port = 12221;

	// open the server
	auto server = new oe::networking::Server(); 
	server->open("localhost", port);
	server->setConnectCallback([&](size_t client_id) { spdlog::info("{} connected", client_id); });
	server->setDisconnectCallback([&](size_t client_id) { spdlog::info("{} disconnected", client_id); });
	server->setReciveCallback([&](size_t client_id, const unsigned char* data, size_t size) {
		// decompress recieved message
		size_t uncompressedSize;
		uint8_t* uncompressed = oe::utils::uncompress(uncompressedSize, data, size);
		spdlog::info("recieved {}", reinterpret_cast<const char*>(uncompressed));

		// compress message
		size_t compressedSize;
		uint8_t* compressed = oe::utils::compress(compressedSize, reinterpret_cast<const uint8_t*>("ping (server)"), 14);
		server->send(compressed, compressedSize, client_id);
	});
	
	// connect to it with client
	auto client = new oe::networking::Client();
	client->connect("localhost", port);
	client->setReciveCallback([&](const uint8_t* data, size_t size) { 
		// decompress recieved message
		size_t uncompressedSize;
		uint8_t* uncompressed = oe::utils::uncompress(uncompressedSize, data, size);
		spdlog::info("recieved {}", reinterpret_cast<const char*>(uncompressed));

		// compress message
		size_t compressedSize;
		uint8_t* compressed = oe::utils::compress(compressedSize, reinterpret_cast<const uint8_t*>("pong (client)"), 14);
		client->send(compressed, compressedSize);
	});

	// compress initial message
	size_t compressedSize;
	uint8_t* compressed = oe::utils::compress(compressedSize, reinterpret_cast<const uint8_t*>("invoke (client)"), 16);
	client->send(compressed, compressedSize);

	// wait for messages to get recieved
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	// close the client
	client->disconnect();
	client->close();

	// close the server
	server->close();

	return 0;
}