#include <engine/utility/ts_queue.hpp>
#include <spdlog/spdlog.h>



int main()
{
	oe::utils::ts_queue<std::string> events;
	
	std::atomic<bool> run{ true };
	std::thread event_creator{[&](){
		while(run)
		{
			events.push("test 1");
			events.push("test 2");

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}};

	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::thread event_listener{[&](){
		while(run)
		{
			std::string copied_string{};
			bool had_value{};
			{
				auto front = events.wait_for_front(std::chrono::milliseconds(100));
				had_value = front.has_value();
				if(had_value)
					copied_string = front.value().first;
			}

			if(had_value)
			{
				events.pop();
				spdlog::info(copied_string);
			}
		}
	}};

	std::this_thread::sleep_for(std::chrono::seconds(1));
	run = false;
	if(event_creator.joinable()) event_creator.join();
	if(event_listener.joinable()) event_listener.join();

	return 0;
}