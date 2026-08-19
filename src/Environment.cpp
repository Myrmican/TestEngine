#include <intrin.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

namespace System {

	bool IsVirtualEnvironment() {
		int CPUInfo[4] = { 0 };
		__cpuid(CPUInfo, 1);
		return (CPUInfo[2] & (1 << 31)) != 0;
	}

	bool IsConnectedToNetwork() {
		QNetworkRequest request(QUrl("http://cp.cloudflare.com/generate_204"));

		QNetworkReply* reply = QNetworkAccessManager().get(request);

		bool online = (reply->error() == QNetworkReply::NoError);
		reply->deleteLater();
		return online;
	}
}