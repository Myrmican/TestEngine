class QMainWindow;
class Project;
class Explorer;
class Properties;
class Output;
class Assets;

namespace EngineEditor {

    class DockManager {
    public:
        Explorer* explorerDock;
        Properties* propertiesDock;
        Output* outputDock;
        Assets* assetsDock;

        DockManager(QMainWindow* window, Project* project);
        ~DockManager() = default;
    };
}