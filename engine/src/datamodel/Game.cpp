#include <datamodel/Game.h>
#include <services/world/World.h>
#include <services/players/Players.h>
#include <services/server/Server.h>
#include <services/client/Client.h>
#include <services/shared/Shared.h>
#include <services/audio/Audio.h>
#include <services/interface/Interface.h>
#include <datamodel/instances/Folder.h>

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

        m_services.emplace("World", worldService);
        m_services.emplace("Players", playersService);
        m_services.emplace("Server", serverService);
        m_services.emplace("Client", clientService);
        m_services.emplace("Shared", sharedService);
        m_services.emplace("Audio", audioService);

        auto serverAssetsFolder = std::make_shared<Folder>();
        serverAssetsFolder->setParent(serverService.get(), true);

        auto serverSourceFolder = std::make_shared<Folder>();
        serverSourceFolder->setParent(serverService.get(), true);

        auto sharedAssetsFolder = std::make_shared<Folder>();
        sharedAssetsFolder->setParent(sharedService.get(), true);

        auto sharedSourceFolder = std::make_shared<Folder>();
        sharedSourceFolder->setParent(sharedService.get(), true);

        auto clientAssetsFolder = std::make_shared<Folder>();
        clientAssetsFolder->setParent(clientService.get(), true);

        auto clientSourceFolder = std::make_shared<Folder>();
        clientSourceFolder->setParent(clientService.get(), true);

        auto interfaceService = std::make_shared<Interface>();
        interfaceService->setParent(clientService.get(), true);
	}
}