#include <QtCore>
#include <QtNetwork>
#include <iostream>

#include "spider.h"

Spider::Spider(QObject *parent)
    : QObject(parent)
{
    connect(&ftp, SIGNAL(done(bool)), this, SLOT(ftpDone(bool)));
    connect(&ftp, SIGNAL(listInfo(const QUrlInfo &)),
            this, SLOT(ftpListInfo(const QUrlInfo &)));
	connect(&ftp, SIGNAL(commandStarted(int)), this, SLOT(ftpCommandStarted(int)));
	connect(&ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(ftpCommandFinished(int, bool)));
	connectID = loginID = getID = cdID = listID = -1;
}

bool Spider::getDirectory(const QUrl &url)
{
    if (!url.isValid()) {
        std::cerr << "Error: Invalid URL" << std::endl;
        return false;
    }

    if (url.scheme() != "ftp") {
        std::cerr << "Error: URL must start with 'ftp:'" << std::endl;
        return false;
    }

    connectID = ftp.connectToHost(url.host(), url.port(21));
    loginID = ftp.login("brianyi", "123456");

    QString path = url.path();
    if (path.isEmpty())
        path = "/";

    pendingDirs.append(path);
    processNextDirectory();

    return true;
}

void Spider::ftpDone(bool error)
{
    if (error) {
        std::cerr << "Error: " << qPrintable(ftp.errorString())
                  << std::endl;
    } else {
        std::cout << "Downloaded " << qPrintable(currentDir) << " to "
                  << qPrintable(QDir::toNativeSeparators(
                                QDir(currentLocalDir).canonicalPath()))
				  << std::endl;
    }

    qDeleteAll(openedFiles);
    openedFiles.clear();

    processNextDirectory();
}

void Spider::ftpListInfo(const QUrlInfo &urlInfo)
{
    if (urlInfo.isFile()) {
        if (urlInfo.isReadable()) {
            QFile *file = new QFile(currentLocalDir + "/"
                                    + urlInfo.name());

            if (!file->open(QIODevice::WriteOnly)) {
                std::cerr << "Warning: Cannot write file "
                          << qPrintable(QDir::toNativeSeparators(
                                        file->fileName()))
                          << ": " << qPrintable(file->errorString())
                          << std::endl;
                return;
            }

            getID = ftp.get(urlInfo.name(), file);
			std::cout << "get:" << getID << std::endl;
            openedFiles.append(file);
        }
    } else if (urlInfo.isDir() && !urlInfo.isSymLink()) {
        pendingDirs.append(currentDir + "/" + urlInfo.name());
    }
}


void Spider::ftpCommandStarted(int id)
{
	if (id == connectID) {
		std::cout << qPrintable(tr("[start]: connect(%1)").arg(id)) << std::endl;
	} else if (id == cdID) {
		std::cout << qPrintable(tr("[start]: cd(%1)").arg(id)) << std::endl;
	} else if (id == getID) {
		std::cout << qPrintable(tr("[start]: get(%1)").arg(id)) << std::endl;
	} else if (id == listID) {
		std::cout << qPrintable(tr("[start]: list(%1)").arg(id)) << std::endl;
	} else if (id == loginID) {
		std::cout << qPrintable(tr("[start]: login(%1)").arg(id)) << std::endl;
	} else {
		std::cout << qPrintable(tr("[start]: UNKNOWN(%1)").arg(id)) << std::endl;
	}
}


void Spider::ftpCommandFinished(int id, bool error)
{
	if (!error){
		if (id == connectID) {
			std::cout << qPrintable(tr("[finished]: connect(%1)").arg(id)) << std::endl;
		} else if (id == cdID) {
			std::cout << qPrintable(tr("[finished]: cd(%1)").arg(id)) << std::endl;
		} else if (id == getID) {
			std::cout << qPrintable(tr("[finished]: get(%1)").arg(id)) << std::endl;
		} else if (id == listID) {
			std::cout << qPrintable(tr("[finished]: list(%1)").arg(id)) << std::endl;
		} else if (id == loginID) {
			std::cout << qPrintable(tr("[finished]: login(%1)").arg(id)) << std::endl;
		} else {
			std::cout << qPrintable(tr("[finished]: UNKNOWN(%1)").arg(id)) << std::endl;
		}
	} else {
		std::cout << "[finished]: error!!!" << std::endl;
	}
}

void Spider::processNextDirectory()
{
    if (!pendingDirs.isEmpty()) {
        currentDir = pendingDirs.takeFirst();
        currentLocalDir = "downloads/" + currentDir;
        QDir(".").mkpath(currentLocalDir);

        cdID = ftp.cd(currentDir);
        listID = ftp.list();
    } else {
        emit done();
    }
}
