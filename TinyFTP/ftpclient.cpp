#include "ftpclient.h"
#include "remotedirwidget.h"

FTPClient::FTPClient(QObject *parent)
    : QFtp(parent)
{
    parentWidget = qobject_cast<RemoteDirWidget*>(parent);
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
}

FTPClient::~FTPClient()
{

}

int FTPClient::cd(const QString & dir)
{
    LOGSTREAM << DataPair(parentWidget, tr("CD \"%1\" is current directory").arg(dir));
    return QFtp::cd(dir);
}

int FTPClient::close()
{
    LOGSTREAM << DataPair(parentWidget, tr("CLOSE"));
    return QFtp::close();
}

int FTPClient::connectToHost(const QString & host, quint16 port /*= 21*/)
{
    LOGSTREAM << DataPair(parentWidget, tr("connect to host %1, port %2").arg(host).arg(port));
    return QFtp::connectToHost(host, port);
}

int FTPClient::get(const QString & file, QIODevice * dev /*= 0*/, TransferType type /*= Binary*/)
{
    LOGSTREAM << DataPair(parentWidget, tr("GET file %1").arg(file));
    return QFtp::get(file, dev, type);
}

int FTPClient::list(const QString & dir /*= QString()*/)
{
    LOGSTREAM << DataPair(parentWidget, tr("LIST %1").arg(dir));
    return QFtp::list(dir);
}

int FTPClient::login(const QString & user /*= QString()*/, const QString & password /*= QString()*/)
{
    LOGSTREAM << DataPair(parentWidget, tr("LOGIN username is %1, password is (hidden)").arg(user));
    return QFtp::login(user, password);
}

int FTPClient::mkdir(const QString & dir)
{
    LOGSTREAM << DataPair(parentWidget, tr("MKDIR %1").arg(dir));
    return QFtp::mkdir(dir);
}

int FTPClient::put(QIODevice * dev, const QString & file, TransferType type /*= Binary*/)
{
    LOGSTREAM << DataPair(parentWidget, tr("PUT file %1").arg(file));
    return QFtp::put(dev, file, type);
}

int FTPClient::remove(const QString & file)
{
    LOGSTREAM << DataPair(parentWidget, tr("REMOVE %1").arg(file));
    return QFtp::remove(file);
}

int FTPClient::rename(const QString & oldname, const QString & newname)
{
    LOGSTREAM << DataPair(parentWidget, tr("RENAME %1 to %2").arg(oldname).arg(newname));
    return QFtp::rename(oldname, newname);
}

int FTPClient::rmdir(const QString & dir)
{
    LOGSTREAM << DataPair(parentWidget, tr("RMDIR %1").arg(dir));
    return QFtp::rmdir(dir);
}

void FTPClient::stateChanged(int state)
{
    QString stateInfo = tr("");
    switch (state)
    {
    case QFtp::Unconnected:
    	{
            stateInfo = tr("There is no connection to the host.");
    		break;
    	}
    case QFtp::HostLookup:
        {
            stateInfo = tr("A host name lookup is in progress.");
            break;
        }
    case QFtp::Connecting:
        {
            stateInfo = tr("An attempt to connect to the host is in progress.");
            break;
        }
    case QFtp::Connected:
        {
            stateInfo = tr("Connection to the host has been achieved.");
            break;
        }
    case QFtp::LoggedIn:
        {
            stateInfo = tr("Connection and user login have been achieved.");
            break;
        }
    case QFtp::Closing:
        {
            stateInfo = tr("The connection is closing down, but it is not yet closed.");
            break;
        }
    default:
        {
            stateInfo = tr("UNKNOW state!!");
    	    break;
        }
    }
    LOGSTREAM << DataPair(parentWidget, stateInfo);
}
