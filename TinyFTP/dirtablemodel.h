#ifndef DIRTABLEMODEL_H
#define DIRTABLEMODEL_H

#include <QAbstractTableModel>
#include <QtGui>

class DirTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
	enum {
		Byte,
		KiloByte,
		MegaByte,
		GigaByte
	};
    DirTableModel(QObject *parent = 0);
    ~DirTableModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
        int role = Qt::DisplayRole) const;
    void setRootPath(const QString &path);
	void sort(int column, Qt::SortOrder order /* = Qt::AscendingOrder */);
	public slots:
		void setRootIndex(const QModelIndex &index);
private:
    QList<QFileInfo> files;
    QFileIconProvider provider;
};

#endif // DIRTABLEMODEL_H
