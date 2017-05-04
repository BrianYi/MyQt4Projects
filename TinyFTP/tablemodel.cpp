#include "tablemodel.h"

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

TableModel::~TableModel()
{

}

int TableModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return files.count();
}

int TableModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return 4;
}

QVariant TableModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    QFileInfo file = files.at(index.row());

    if (index.column() == 0) {
        if (role == Qt::DecorationRole) {
            return provider.icon(file);
        } else if (role == Qt::DisplayRole) {
            return file.fileName();
        }
    } else if (index.column() == 1) {
        if (role == Qt::DisplayRole && !file.isDir() && !file.isSymLink()) {
            return tr("%1 KB").arg(qCeil(file.size() / 1024.0));
        } else if (role == Qt::TextAlignmentRole) {
            return int(Qt::AlignRight | Qt::AlignVCenter);
        }
    } else if (index.column() == 2) {
        if (role == Qt::DisplayRole) {
            return provider.type(file);
        } else if (role == Qt::TextAlignmentRole) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
    } else if (index.column() == 3) {
        if (role == Qt::DisplayRole) {
            return file.lastModified().toString("yyyy/MM/dd hh:mm");
        } else if (role == Qt::TextAlignmentRole) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
    }
    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        QString title = tr("无");
        if (section == 0) {
            title = tr("名称");
        } else if (section == 1) {
            title = tr("大小");
        } else if (section == 2) {
            title = tr("类型");
        } else if (section == 3) {
            title = tr("修改时间");
        }
        return title;
    }
    return QVariant();
}

void TableModel::setRootPath(const QString &path)
{
    QDir rootPath(path);
    files.clear();
    foreach(QFileInfo file, rootPath.entryInfoList()) {
        files.append(file);
    }
    reset();
}
