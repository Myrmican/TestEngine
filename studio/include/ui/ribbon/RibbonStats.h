#include <string>
#include <QString>

class QMenu;
class QWidget;
class QLabel;

namespace Engine {
	namespace Ribbon {
		struct GetStatistic {
			QString statName;
			QString statValue;
		};

		GetStatistic getStatistic(std::string statName, QWidget* statsWidget);
		QLabel* statisticLabel(QWidget* parent, std::string statName);
		QMenu* createStatsToggleMenu(QWidget* parent, QWidget* statsLayout);
	}
}