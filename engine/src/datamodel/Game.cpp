#include <datamodel/Game.h>
#include <services/world/World.h>
#include <services/players/Players.h>
#include <services/server/Server.h>
#include <services/client/Client.h>
#include <services/shared/Shared.h>
#include <services/audio/Audio.h>
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

        worldService->setParent(this);
        playersService->setParent(this);
        serverService->setParent(this);
        clientService->setParent(this);
        sharedService->setParent(this);
        audioService->setParent(this);

        m_services.emplace("World", worldService);
        m_services.emplace("Players", playersService);
        m_services.emplace("Server", serverService);
        m_services.emplace("Client", clientService);
        m_services.emplace("Shared", sharedService);
        m_services.emplace("Audio", audioService);

        auto serverAssetsFolder = std::make_shared<Folder>();
        serverAssetsFolder->setParent(serverService.get());

        auto serverSourceFolder = std::make_shared<Folder>();
        serverSourceFolder->setParent(serverService.get());

        auto sharedAssetsFolder = std::make_shared<Folder>();
        sharedAssetsFolder->setParent(sharedService.get());

        auto sharedSourceFolder = std::make_shared<Folder>();
        sharedSourceFolder->setParent(sharedService.get());

        auto clientAssetsFolder = std::make_shared<Folder>();
        clientAssetsFolder->setParent(clientService.get());

        auto clientSourceFolder = std::make_shared<Folder>();
        clientSourceFolder->setParent(clientService.get());
	}
}