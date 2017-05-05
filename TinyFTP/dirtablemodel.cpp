#include "dirtablemodel.h"

DirTableModel::DirTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

DirTableModel::~DirTableModel()
{

}

int DirTableModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return files.count();
}

int DirTableModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return 4;
}

QVariant DirTableModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    if (!index.isValid() || files.isEmpty()) {
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
			qreal fileSize = file.size();
			int level = Byte;
			QString sizeInfo = "";
			while (qFloor(fileSize / 1024)) {
				fileSize /= 1024.0;
				level++;
				if (level >= GigaByte)
					break;
			}
			
			fileSize = QString::number(fileSize, 'f', 2).toDouble();
			if (level == Byte) {
				sizeInfo = tr("%1 B").arg(fileSize);
			} else if (level == KiloByte) {
				sizeInfo = tr("%1 KB").arg(fileSize);
			} else if (level == MegaByte) {
				sizeInfo = tr("%1 MB").arg(fileSize);
			} else if (level >= GigaByte) {
				sizeInfo = tr("%1 GB").arg(fileSize);
			}
            return sizeInfo;
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

QVariant DirTableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
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

void DirTableModel::setRootPath(const QString &path)
{
    QDir rootPath(path);
	if (!rootPath.exists()) {
		return ;
	}

    files.clear();
    foreach(QFileInfo file, rootPath.entryInfoList(QDir::NoDot | QDir::AllEntries, 
		QDir::DirsFirst | QDir::IgnoreCase | QDir::Name)) {
        files.append(file);
    }
	sort(0, Qt::AscendingOrder);
    reset();
}

void DirTableModel::sort(int column, Qt::SortOrder order /* = Qt::AscendingOrder */)
{
	if (files.size() > 1) {
		QList<QFileInfo>::Iterator beg = files.begin();
		if (beg->fileName() == tr("..")) {
			beg = beg+1;
		}
		qStableSort(beg, files.end(), [&](const QFileInfo &file1, const QFileInfo &file2)->bool{
			if ((file1.isDir() && file2.isDir()) ||
				(!file1.isDir() && !file2.isDir())) {
				if (column == 0) {	// 名称
					if (order == Qt::AscendingOrder) {
						return file1.fileName().toLower() < file2.fileName().toLower();
					}
					return file1.fileName().toLower() > file2.fileName().toLower();
				} else if (column == 1) {	// 大小
					if (order == Qt::AscendingOrder) {
						return file1.size() < file2.size();
					}
					return file1.size() > file2.size();
				} else if (column == 2) {	// 类型
					if (order == Qt::AscendingOrder) {
						return provider.type(file1) < provider.type(file2);
					}
					return provider.type(file1) > provider.type(file2);
				} else if (column == 3) {	// 修改时间
					if (order == Qt::AscendingOrder) {
						return file1.lastModified() < file2.lastModified();
					}
					return file1.lastModified() > file2.lastModified();
				}
			}
			return false;
		});
		reset();
	}
}

void DirTableModel::setRootIndex(const QModelIndex &index)
{
	if (!index.isValid() || files.isEmpty()) {
		return ;
	}

	QFileInfo file = files.at(index.row());
	if (file.isDir()) {
		setRootPath(file.absoluteFilePath());
	}
}
