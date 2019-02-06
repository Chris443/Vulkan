#include "../../vkf/common.h"
#include "../../vkf/Window.h"

//#include "Application.h"
#include "App_2.h"


void main() {
	std::shared_ptr<vkf::Window> window = std::make_shared<vkf::Window>(800, 1000, std::string("Vk_Testing"));

	//Application app(window);
	//app.run();

	App_2 app2(window);
	app2.run();
	return;
}