#include <datamodel/DataModel.h>
#include <Engine.h>
#include <core/Reflection.h>
#include <serialization/PlaceDeserializer.h>

namespace Engine {
    DataModel::DataModel(EngineInstance* engine) : Instance("DataModel") {
        internalLocked = true;
        m_engine = engine;


    }
}