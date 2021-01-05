#include <engine/include.hpp>

#include <string>
#include <thread>

#include <enet/enet.h>



/*

	Simple server/client ping spam

*/

constexpr std::string_view server_message = "ping (from server)";
constexpr std::string_view client_message = "ping (from client)";
constexpr std::string_view init_message = "invoke (from server)";

int main()
{

	// engine
	oe::Engine::getSingleton().init({});
	constexpr int32_t port = 12221;
	oe::networking::Server server;
	oe::utils::connect_guard cg_server_connect, cg_server_disconnect, cg_server_receive;
	oe::networking::Client client;
	oe::utils::connect_guard cg_client_receive;
	oe::networking::result result;

	// open the server
	cg_server_connect.connect<oe::networking::ServerConnectEvent>(server.m_dispatcher, [](const oe::networking::ServerConnectEvent &e) {
		spdlog::info("{} connected", e.client_id);
	});
	cg_server_disconnect.connect<oe::networking::ServerDisconnectEvent>(server.m_dispatcher, [](const oe::networking::ServerDisconnectEvent &e) {
		spdlog::info("{} disconnected", e.client_id);
	});
	cg_server_receive.connect<oe::networking::ServerReceiveEvent>(server.m_dispatcher, [&server](const oe::networking::ServerReceiveEvent &e) {
		spdlog::info("server recieved message: {}", std::string(reinterpret_cast<const char *>(e.data.data()), e.data.size()));
		auto result = server.send(server_message.begin(), server_message.end());
		if (result.failed())
			spdlog::critical("Server send failed: {}", result.message());
	});
	result = server.open(port);
	if (result.failed())
		spdlog::critical("Server open failed: {}", result.message());

	// connect to it with client
	cg_client_receive.connect<oe::networking::ClientReceiveEvent>(client.m_dispatcher, [&client](const oe::networking::ClientReceiveEvent &e) {
		spdlog::info("client recieved message: {}", std::string(reinterpret_cast<const char *>(e.data.data()), e.data.size()));
		auto result = client.send(client_message.begin(), client_message.end());
		if (result.failed())
			spdlog::critical("Client send failed: {}", result.message());
	});
	result = client.connect("localhost", port);
	if (result.failed())
		spdlog::critical("Client connect failed: {}", result.message());

	// wait for client to be connected
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	// initial message
	result = server.send(init_message.begin(), init_message.end());
	if (result.failed())
		spdlog::critical("Server send failed: {}", result.message());

	// wait for messages to get recieved
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	client.close();
	server.close();
	return 0;
}