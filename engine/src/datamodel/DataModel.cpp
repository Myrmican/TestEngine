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
#include <datamodel/instances/SpawnPoint.h>
#include <datamodel/instances/Camera.h>
#include <services/selection/Selection.h>
#include <services/logging/Logging.h>
#include <datamodel/instances/File.h>
#include <datamodel/instances/AssetsFolder.h>
#include <datamodel/instances/SourceFolder.h>
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

        auto clientServiceOwned = std::make_unique<Client>();
        Client* clientService = clientServiceOwned.get();
        addChild(std::move(clientServiceOwned));

        auto sharedServiceOwned = std::make_unique<Shared>();
        Shared* sharedService = sharedServiceOwned.get();
        addChild(std::move(sharedServiceOwned));

        auto serverServiceOwned = std::make_unique<Server>();
        Server* serverService = serverServiceOwned.get();
        addChild(std::move(serverServiceOwned));

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
        serverAssetsFolder->internalLocked = true;
        serverService->addChild(std::move(serverAssetsFolder));

        auto serverSourceFolderOwned = std::make_unique<SourceFolder>();
        SourceFolder* serverSourceFolder = serverSourceFolderOwned.get();
        serverService->addChild(std::move(serverSourceFolderOwned));

        auto sharedAssetsFolder = std::make_unique<Folder>();
        sharedAssetsFolder->setName("Assets");
        sharedAssetsFolder->internalLocked = true;
        sharedService->addChild(std::move(sharedAssetsFolder));

        auto sharedSourceFolder = std::make_unique<SourceFolder>();
        sharedService->addChild(std::move(sharedSourceFolder));

        auto clientAssetsFolder = std::make_unique<Folder>();
        clientAssetsFolder->setName("Assets");
        clientAssetsFolder->internalLocked = true;
        clientService->addChild(std::move(clientAssetsFolder));

        auto clientSourceFolderOwned = std::make_unique<SourceFolder>();
        SourceFolder* clientSourceFolder = clientSourceFolderOwned.get();
        clientService->addChild(std::move(clientSourceFolderOwned));

        auto playerTemplateOwned = std::make_unique<PlayerTemplate>();
        PlayerTemplate* playerTemplate = playerTemplateOwned.get();
        playersService->addChild(std::move(playerTemplateOwned));

        auto defaultInterface = std::make_unique<Interface>();
        playerTemplate->addChild(std::move(defaultInterface));

        auto baseplatePart = std::make_unique<Part>();
        baseplatePart->setName("Baseplate");
        worldService->addChild(std::move(baseplatePart));

        auto spawnPoint = std::make_unique<SpawnPoint>();
        worldService->addChild(std::move(spawnPoint));

        //Main entry files

        auto serverMainFile = std::make_unique<File>();
        serverMainFile->setName("Server.kts");
        serverSourceFolder->addChild(std::move(serverMainFile));

        auto clientMainFile = std::make_unique<File>();
        clientMainFile->setName("Client.kts");
        clientSourceFolder->addChild(std::move(clientMainFile));
    }
}