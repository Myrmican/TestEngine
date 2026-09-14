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
#include <services/selection/Selection.h>
#include <core/Reflection.h>

namespace Engine {
    Game::Game() : Instance("DataModel") {
        internalLocked = true;

        auto worldServiceOwned = std::make_unique<World>();
        World* worldService = worldServiceOwned.get();
        addChild(std::move(worldServiceOwned));

        auto playersServiceOwned = std::make_unique<Players>();
        Players* playersService = playersServiceOwned.get();
        addChild(std::move(playersServiceOwned));

        auto serverServiceOwned = std::make_unique<Server>();
        Server* serverService = serverServiceOwned.get();
        addChild(std::move(serverServiceOwned));

        auto clientServiceOwned = std::make_unique<Client>();
        Client* clientService = clientServiceOwned.get();
        addChild(std::move(clientServiceOwned));

        auto sharedServiceOwned = std::make_unique<Shared>();
        Shared* sharedService = sharedServiceOwned.get();
        addChild(std::move(sharedServiceOwned));

        auto audioServiceOwned = std::make_unique<Audio>();
        Audio* audioService = audioServiceOwned.get();
        addChild(std::move(audioServiceOwned));

        auto selectServiceOwned = std::make_unique<Selection>();
        Selection* selectService = selectServiceOwned.get();
        addChild(std::move(selectServiceOwned));

        m_services.emplace_back("World", worldService);
        m_services.emplace_back("Players", playersService);
        m_services.emplace_back("Server", serverService);
        m_services.emplace_back("Client", clientService);
        m_services.emplace_back("Shared", sharedService);
        m_services.emplace_back("Audio", audioService);
        m_services.emplace_back("Selection", selectService);

        auto serverAssetsFolder = std::make_unique<Folder>();
        serverAssetsFolder->setName("Assets");
        serverService->addChild(std::move(serverAssetsFolder));

        auto serverSourceFolder = std::make_unique<Folder>();
        serverSourceFolder->setName("Source");
        serverService->addChild(std::move(serverSourceFolder));

        auto sharedAssetsFolder = std::make_unique<Folder>();
        sharedAssetsFolder->setName("Assets");
        sharedService->addChild(std::move(sharedAssetsFolder));

        auto sharedSourceFolder = std::make_unique<Folder>();
        sharedSourceFolder->setName("Source");
        sharedService->addChild(std::move(sharedSourceFolder));

        auto clientAssetsFolder = std::make_unique<Folder>();
        clientAssetsFolder->setName("Assets");
        clientService->addChild(std::move(clientAssetsFolder));

        auto clientSourceFolder = std::make_unique<Folder>();
        clientSourceFolder->setName("Source");
        clientService->addChild(std::move(clientSourceFolder));

        auto playerTemplateOwned = std::make_unique<PlayerTemplate>();
        PlayerTemplate* playerTemplate = playerTemplateOwned.get();
        playersService->addChild(std::move(playerTemplateOwned));

        auto defaultInterface = std::make_unique<Interface>();
        playerTemplate->addChild(std::move(defaultInterface));

        auto baseplatePart = std::make_unique<Part>();
        baseplatePart->setName("Baseplate");
        worldService->addChild(std::move(baseplatePart));

        auto worldCamera = std::make_unique<Camera>();
        worldService->addChild(std::move(worldCamera));
    }
}