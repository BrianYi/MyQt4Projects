#include "dirtreemodel.h"

DirTreeModel::DirTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

DirTreeModel::~DirTreeModel()
{

}

int DirTreeModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    if (!root) {
        return 0;
    }
    return root->children.count();
}

int DirTreeModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return 4;
}

QVariant DirTreeModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    if (!index.isValid() || !root || root->children.isEmpty()) {
        return QVariant();
    }

    Node *fileNode = root->children.at(index.row());
    if (index.column() == 0) {
        if (role == Qt::DecorationRole) {
            return fileNode->fileIcon;
        } else if (role == Qt::DisplayRole) {
            return fileNode->fileName;
        }
    } else if (index.column() == 1) {
        if (role == Qt::DisplayRole && !fileNode->isDir && !fileNode->isSystemLink) {
			qreal fileSize = fileNode->fileSize;
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
            return fileNode->fileType;
        } else if (role == Qt::TextAlignmentRole) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
    } else if (index.column() == 3) {
        if (role == Qt::DisplayRole) {
            return fileNode->modifyDate;
        } else if (role == Qt::TextAlignmentRole) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
    }
    return QVariant();
}

bool DirTreeModel::setData(const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole */)
{
    if (!index.isValid() || !root || root->children.isEmpty()) {
        return false;
    }

    Node *fileNode = root->children.at(index.row());
    if (index.column() == 1 && role == Qt::EditRole) {
        fileNode->fileSize = value.toLongLong();
        return true;
    }
    return false;
}

QVariant DirTreeModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        QString title = tr("��");
        if (section == 0) {
            title = tr("����");
        } else if (section == 1) {
            title = tr("��С");
        } else if (section == 2) {
            title = tr("����");
        } else if (section == 3) {
            title = tr("�޸�ʱ��");
        }
        return title;
    }
    return QVariant();
}

void DirTreeModel::setRootPath(const QString &path)
{
    QDir rootPath(path);
	if (!rootPath.exists()) {
		return ;
	}

    if (root) {
        delete root;
    }
    root = new Node;

    foreach(QFileInfo fileInfo, rootPath.entryInfoList(QDir::NoDot | QDir::AllEntries, 
		QDir::DirsFirst | QDir::IgnoreCase | QDir::Name)) {
            Node *p = new Node;
            p->fileName = fileInfo.fileName();
            p->fileIcon = provider.icon(fileInfo);
            p->fileSize = fileInfo.size();
            p->fileType = provider.type(fileInfo);
            p->modifyDate = fileInfo.lastModified().toString("yyyy/MM/dd hh:mm");
            p->isDir = fileInfo.isDir();
            p->isSystemLink = fileInfo.isSymLink();
            p->isFile = fileInfo.isFile();
            p->path = fileInfo.absolutePath();
        /*files.append(QPair<QFileInfo, qint64>(fileInfo, fileInfo.size()));*/
            root->children.append(p);
    }
	sort(0, Qt::AscendingOrder);
    reset();
}

void DirTreeModel::sort(int column, Qt::SortOrder order /* = Qt::AscendingOrder */)
{
    if (!root) {
        return ;
    }
	if (root->children.size() > 1) {
		QList<Node*>::Iterator beg = root->children.begin();
		if ((*beg)->fileName == tr("..")) {
			beg = beg+1;
		}
		qStableSort(beg, root->children.end(), [&](const Node *fileNode1, const Node *fileNode2)->bool{
			if ((fileNode1->isDir && fileNode2->isDir) ||
				(!fileNode1->isDir && !fileNode2->isDir)) {
				if (column == 0) {	// ����
					if (order == Qt::AscendingOrder) {
						return fileNode1->fileName.toLower() < fileNode2->fileName.toLower();
					}
					return fileNode1->fileName.toLower() > fileNode2->fileName.toLower();
				} else if (column == 1) {	// ��С
					if (order == Qt::AscendingOrder) {
						return fileNode1->fileSize < fileNode2->fileSize;
					}
					return fileNode1->fileSize > fileNode2->fileSize;
				} else if (column == 2) {	// ����
					if (order == Qt::AscendingOrder) {
						return fileNode1->fileType < fileNode2->fileType;
					}
					return fileNode1->fileType > fileNode2->fileType;
				} else if (column == 3) {	// �޸�ʱ��
					if (order == Qt::AscendingOrder) {
						return fileNode1->modifyDate < fileNode2->modifyDate;
					}
					return fileNode1->modifyDate > fileNode2->modifyDate;
				}
			}
			return false;
		});
		reset();
	}
}

void DirTreeModel::setRootIndex(const QModelIndex &index)
{
	if (!index.isValid() || !root || root->children.isEmpty()) {
		return ;
	}
    
	Node *fileNode = root->children.at(index.row());
	if (fileNode->isDir) {
		setRootPath(fileNode->path);
	}
}

QModelIndex DirTreeModel::index(int row, int column, const QModelIndex & parent /*= QModelIndex()*/) const
{
//     if (!parent.isValid()) {
//         return QModelIndex();
//     }
    Node *childNode = 0;
    if (!parent.isValid()) {
        childNode = root->children.at(row);
    } else {
        return QModelIndex();
    }
//     QModelIndex child = parent.child(row, column);
//     if (child.isValid()) {
//         return child;
//     }
    return createIndex(row, column, childNode);
}

QModelIndex DirTreeModel::parent(const QModelIndex & index) const
{
    return QModelIndex();
}