#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QFileSystemModel>

class FileSystemModel : public QFileSystemModel
{
	Q_OBJECT

public:
	FileSystemModel(QObject *parent = 0);
	~FileSystemModel();
protected:
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
private:
	
};

#endif // FILESYSTEMMODEL_H
