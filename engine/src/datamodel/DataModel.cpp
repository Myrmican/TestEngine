#include <datamodel/DataModel.h>
#include <Engine.h>
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
#include <services/logging/Logging.h>
#include <core/Reflection.h>

namespace Engine {
    DataModel::DataModel(EngineInstance* engine) : Instance("DataModel") {
        internalLocked = true;
        m_engine = engine;

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

        auto loggingServiceOwned = std::make_unique<Logging>();
        Logging* loggingService = loggingServiceOwned.get();
        addChild(std::move(loggingServiceOwned));

        engine->getProvider()->create(worldService);
        engine->getProvider()->create(playersService);
        engine->getProvider()->create(serverService);
        engine->getProvider()->create(clientService);
        engine->getProvider()->create(sharedService);
        engine->getProvider()->create(audioService);
        engine->getProvider()->create(selectService);
        engine->getProvider()->create(loggingService);

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
    }
}