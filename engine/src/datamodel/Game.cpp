#include <datamodel/Game.h>
#include <services/world/World.h>
#include <services/players/Players.h>
#include <services/server/Server.h>
#include <services/client/Client.h>
#include <services/shared/Shared.h>
#include <services/audio/Audio.h>
#include <services/interface/Interface.h>
#include <datamodel/instances/Folder.h>
#include <datamodel/instances/Part.h>
#include <datamodel/instances/PlayerTemplate.h>
#include <datamodel/instances/Camera.h>

namespace Engine {
	Game::Game() : Instance("DataModel") {
        internalLocked = true;

        auto worldService = std::make_shared<World>();
        auto playersService = std::make_shared<Players>();
        auto serverService = std::make_shared<Server>();
        auto clientService = std::make_shared<Client>();
        auto sharedService = std::make_shared<Shared>();
        auto audioService = std::make_shared<Audio>();

        worldService->setParent(this, true);
        playersService->setParent(this, true);
        serverService->setParent(this, true);
        clientService->setParent(this, true);
        sharedService->setParent(this, true);
        audioService->setParent(this, true);

        m_services.emplace_back("World", worldService);
        m_services.emplace_back("Players", playersService);
        m_services.emplace_back("Server", serverService);
        m_services.emplace_back("Client", clientService);
        m_services.emplace_back("Shared", sharedService);
        m_services.emplace_back("Audio", audioService);

        auto serverAssetsFolder = std::make_shared<Folder>();
		serverAssetsFolder->setName("Assets");
        serverAssetsFolder->setParent(serverService.get(), true);

        auto serverSourceFolder = std::make_shared<Folder>();
        serverSourceFolder->setName("Source");
        serverSourceFolder->setParent(serverService.get(), true);

        auto sharedAssetsFolder = std::make_shared<Folder>();
        sharedAssetsFolder->setName("Assets");
        sharedAssetsFolder->setParent(sharedService.get(), true);

        auto sharedSourceFolder = std::make_shared<Folder>();
        sharedSourceFolder->setName("Source");
        sharedSourceFolder->setParent(sharedService.get(), true);

        auto clientAssetsFolder = std::make_shared<Folder>();
        clientAssetsFolder->setName("Assets");
        clientAssetsFolder->setParent(clientService.get(), true);

        auto clientSourceFolder = std::make_shared<Folder>();
        clientSourceFolder->setName("Source");
        clientSourceFolder->setParent(clientService.get(), true);

        auto playerTemplate = std::make_shared<PlayerTemplate>();
        playerTemplate->setParent(playersService.get(), true);

        auto defaultInterface = std::make_shared<Interface>();
        defaultInterface->setParent(playerTemplate.get(), true);

        auto baseplatePart = std::make_shared<Part>();
		baseplatePart->setName("Baseplate");
        baseplatePart->setParent(worldService.get(), true);

        auto worldCamera = std::make_shared<Camera>();
        worldCamera->setParent(worldService.get(), true);
	}
}