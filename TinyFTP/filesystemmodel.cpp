#include "filesystemmodel.h"

FileSystemModel::FileSystemModel(QObject *parent)
	: QFileSystemModel(parent)
{

}

FileSystemModel::~FileSystemModel()
{

}

QVariant FileSystemModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole */) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
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
	return QFileSystemModel::headerData(section, orientation, role);
}

QModelIndex FileSystemModel::index(int row, int column, const QModelIndex & parent /*= QModelIndex()*/) const
{
	return QFileSystemModel::index(row, column, parent);
}
