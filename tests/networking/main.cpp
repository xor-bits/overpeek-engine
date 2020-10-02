#include <engine/include.hpp>

#include <string>
#include <thread>



/*

	Simple ping/pong

*/

int main(int argc, char* argv[]) {
	// engine
	oe::EngineInfo engine_info = {};
	engine_info.networking = true;
	oe::Engine::getSingleton().init(engine_info);

	int port = 12221;

	// open the server
	auto server = new oe::networking::Server(); 
	server->open("localhost", port);
	server->setConnectCallback([&](size_t client_id) { spdlog::info("{} connected", client_id); });
	server->setDisconnectCallback([&](size_t client_id) { spdlog::info("{} disconnected", client_id); });
	server->setReciveCallback([&](size_t client_id, const unsigned char* data, size_t size) {
		spdlog::info("server recieved message: {}", std::string(reinterpret_cast<const char*>(data), size));

		constexpr std::string_view message = "ping (from server)";
		server->send(reinterpret_cast<const unsigned char*>(message.data()), message.size(), client_id);
	});
	
	// connect to it with client
	auto client = new oe::networking::Client();
	client->connect("localhost", port);
	client->setReciveCallback([&](const uint8_t* data, size_t size) {
		spdlog::info("client recieved message: {}", std::string(reinterpret_cast<const char*>(data), size));

		constexpr std::string_view message = "pong (from client)";
		client->send(reinterpret_cast<const unsigned char*>(message.data()), message.size());
	});

	// wait for client to be connected
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	// initial message
	const std::string message = "invoke (from server)";
	server->send(reinterpret_cast<const unsigned char*>(message.data()), message.size());

	// wait for messages to get recieved
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	// close the client
	client->disconnect();
	client->close();

	// close the server
	server->close();

	return 0;
}