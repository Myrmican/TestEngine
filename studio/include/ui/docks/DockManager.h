class QMainWindow;
class Project;
class Explorer;
class Properties;
class Output;
class Toolbox;

namespace DockManager {
    struct DockSet {
        Explorer* explorer;
        Properties* properties;
        Output* output;
        Toolbox* toolbox;
    };

    DockSet setup(QMainWindow* window, Project* project);
}