#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QEventLoop>

namespace System {

	bool IsVirtualEnvironment() {
		int CPUInfo[4] = { 0 };
		__cpuid(CPUInfo, 1);
		return false; //(CPUInfo[2] & (1 << 31)) != 0;
	}

	bool IsConnectedToNetwork() {
		QNetworkAccessManager manager;
		QNetworkRequest request(QUrl("http://cp.cloudflare.com/generate_204"));

		QNetworkReply* reply = manager.get(request);

		QEventLoop loop;
		QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
		loop.exec();

		bool connected = (reply->error() == QNetworkReply::NoError);
		reply->deleteLater();
		return connected;
	}
}