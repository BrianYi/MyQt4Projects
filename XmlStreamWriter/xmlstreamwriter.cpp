#include <QtGui>
#include <QtXml>
#include <iostream>

struct taskInfoDesc
{
public:
	int		taskStatus;			//任务状态
	QString taskCalcIP;			//任务分发的计算节点
	QString taskResultPath;		//任务结果文件夹
	int		taskProcessValue;	//任务计算进度值
public:
	int		taskID;				//任务编号
	QString taskName;			//任务名称
	QString	exeName;
	int		timeOutLimit;
	QString	taskDesc;
public:
	bool operator==(const taskInfoDesc right)const{return taskID == right.taskID;};
	bool operator<(const taskInfoDesc right)const{return taskID < right.taskID;};
};

QVector<taskInfoDesc> m_taskInfo;


void populateTree(QTreeWidget *treeWidget)
{
    QStringList labels;
    labels << QObject::tr("Terms") << QObject::tr("Pages");

    treeWidget->setHeaderLabels(labels);
    treeWidget->header()->setResizeMode(QHeaderView::Stretch);
    treeWidget->setWindowTitle(QObject::tr("XML Stream Writer"));
    treeWidget->show();

    (void) new QTreeWidgetItem(treeWidget,
            QStringList() << "sidebearings" << "10, 34-35, 307-308");
    QTreeWidgetItem *subterm = new QTreeWidgetItem(treeWidget,
            QStringList() << "subtraction");
    (void) new QTreeWidgetItem(subterm,
            QStringList() << "of pictures" << "115, 244");
    (void) new QTreeWidgetItem(subterm,
            QStringList() << "of vectors" << "9");
}

void writeIndexEntry(QXmlStreamWriter *xmlWriter, QTreeWidgetItem *item)
{
    xmlWriter->writeStartElement("entry");
    xmlWriter->writeAttribute("term", item->text(0));
    QString pageString = item->text(1);
    if (!pageString.isEmpty()) {
        QStringList pages = pageString.split(", ");
        foreach (QString page, pages)
            xmlWriter->writeTextElement("page", page);
    }
    for (int i = 0; i < item->childCount(); ++i)
        writeIndexEntry(xmlWriter, item->child(i));
    xmlWriter->writeEndElement();
}

bool writeXml(const QString &fileName, QTreeWidget *treeWidget)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        std::cerr << "Error: Cannot write file "
                  << qPrintable(fileName) << ": "
                  << qPrintable(file.errorString()) << std::endl;
        return false;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("bookindex");
    for (int i = 0; i < treeWidget->topLevelItemCount(); ++i)
        writeIndexEntry(&xmlWriter, treeWidget->topLevelItem(i));
    xmlWriter.writeEndDocument();

    file.close();
    if (file.error()) {
        std::cerr << "Error: Cannot write file "
                  << qPrintable(fileName) << ": "
                  << qPrintable(file.errorString()) << std::endl;
        return false;
    }
    return true;
}

void writeTaskInfo(QXmlStreamWriter *_xmlStreamWriter, taskInfoDesc *task)
{
	_xmlStreamWriter->writeStartElement("task");

	_xmlStreamWriter->writeAttribute("id", QString::number(task->taskID));
	_xmlStreamWriter->writeAttribute("name", task->taskName);
	_xmlStreamWriter->writeAttribute("calcnode", task->taskCalcIP);
	_xmlStreamWriter->writeAttribute("progress", QString::number(task->taskProcessValue));
	_xmlStreamWriter->writeAttribute("status", QString::number(task->taskStatus));
	_xmlStreamWriter->writeAttribute("resultpath", QDir::toNativeSeparators(task->taskResultPath));

	_xmlStreamWriter->writeEndElement();
}

void saveCalcInfoToFile(QString fileName, qint64 currentProjId)
{
	QXmlStreamWriter *_xmlStreamWriter = new QXmlStreamWriter;
	QFile file(fileName);
	if (!file.exists()) 
	{
		file.open(QFile::WriteOnly|QFile::Truncate);
		_xmlStreamWriter->setDevice(&file);
		_xmlStreamWriter->writeStartDocument();
	} 
	else 
	{
		file.open(QFile::WriteOnly|QFile::Append);
		_xmlStreamWriter->setDevice(&file);
	}

	_xmlStreamWriter->setAutoFormatting(true);
	_xmlStreamWriter->writeStartElement("projects");
	_xmlStreamWriter->writeStartElement("project");
	_xmlStreamWriter->writeAttribute("id", QString::number(currentProjId));
	for (int i = 0; i < m_taskInfo.size(); i++) {
		writeTaskInfo(_xmlStreamWriter, &m_taskInfo[i]);
	}
	_xmlStreamWriter->writeEndElement();
	_xmlStreamWriter->writeEndElement();
	_xmlStreamWriter->writeEndDocument();

	/*file.close();*/
}

void skipUnknownElement(QXmlStreamReader *reader);
void readProjectElement(QXmlStreamReader *reader, QVector<taskInfoDesc> &taskInfo);
void readTaskElement(QXmlStreamReader *reader, taskInfoDesc& task);
void readProjectsElement(QXmlStreamReader *reader, QMap<qint64, QVector<taskInfoDesc>> &taskInfo);

void readXML(QMap<qint64, QVector<taskInfoDesc>> &taskInfo, QString fileName)
{
	QXmlStreamReader *reader = new QXmlStreamReader;
	QFile file(fileName);
	if (!file.exists() || !file.open(QFile::ReadOnly)) 
		return ;

	reader->setDevice(&file);

	reader->readNext();
	while (!reader->atEnd()) 
	{
		if (reader->isStartElement()) 
		{
			if (reader->name() == "projects") 
			{
				readProjectsElement(reader, taskInfo);
			}
			else
			{
				reader->raiseError(QObject::tr("Not a project element!"));
			}
		}
		else 
		{
			reader->readNext();
		}
	}

	if (reader->hasError()) 
	{
		QString a = reader->errorString();
		a =a;
	}
}

void readProjectsElement(QXmlStreamReader *reader, QMap<qint64, QVector<taskInfoDesc>> &taskInfo)
{
	reader->readNext();
	while (!reader->atEnd()) 
	{
		if (reader->isEndElement()) {
			reader->readNext();
			break;
		}

		if (reader->isStartElement()) 
		{
			if (reader->name() == "project") 
			{
				int prjId = reader->attributes().value("id").toString().toInt();
				taskInfo[prjId] = QVector<taskInfoDesc>();
				readProjectElement(reader, taskInfo[prjId]);
			}
			else
			{
				skipUnknownElement(reader);
			}
		}
		else 
		{
			reader->readNext();
		}
	}
}

void readProjectElement(QXmlStreamReader *reader, QVector<taskInfoDesc> &taskInfo)
{
	reader->readNext();
	while (!reader->atEnd())
	{
		if (reader->isEndElement()) 
		{
			reader->readNext();
			break;
		}

		if (reader->isStartElement()) 
		{
			if (reader->name() == "task") 
			{
				QXmlStreamAttributes attr = reader->attributes();
				taskInfoDesc task;
				task.taskID				= attr.value("id").toString().toInt();
				task.taskName			= attr.value("name").toString();
				task.taskCalcIP			= attr.value("calcnode").toString();
				task.taskProcessValue	= attr.value("progress").toString().toInt();
				task.taskStatus			= attr.value("status").toString().toInt();
				task.taskResultPath		= attr.value("resultpath").toString();
				taskInfo.push_back(task);
				readTaskElement(reader, task);
			} 
			else 
			{
				skipUnknownElement(reader);
			}
		} else {
			reader->readNext();
		}
	}
}

void readTaskElement(QXmlStreamReader *reader, taskInfoDesc& task)
{
	reader->readNext();
	while (!reader->atEnd()) {
		if (reader->isEndElement()) {
			reader->readNext();
			break;
		}

		if (reader->isStartElement()) {
			skipUnknownElement(reader);
		} else {
			reader->readNext();
		}
	}
}

void skipUnknownElement(QXmlStreamReader *reader)
{
	reader->readNext();
	while (!reader->atEnd()) 
	{
		if (reader->isEndElement()) 
		{
			reader->readNext();
			break;
		}

		if (reader->isStartElement()) 
		{
			skipUnknownElement(reader);
		}
		else 
		{
			reader->readNext();
		}
	}

}


int main(int argc, char *argv[])
{
	taskInfoDesc taskInfo[3] = {
		0,	"127.0.0.1",	"c:/asdas",		20,	0,	"task1",	"",	0,	"",
		1,	"127.0.0.2",	"d:/asd/asd",	30,	1,	"task2",	"",	1,	"",
		2,	"127.0.0.3",	"e:/asd/ass",	40,	2,	"task3",	"",	2,	""
	};
	m_taskInfo.push_back(taskInfo[0]);
	m_taskInfo.push_back(taskInfo[1]);
	m_taskInfo.push_back(taskInfo[2]);
//     QApplication app(argc, argv);
//     QTreeWidget treeWidget;
//     populateTree(&treeWidget);
// 	for (int i = 0; i < 1; i++) 
// 	{
// 		saveCalcInfoToFile("taskInfo.xml", i);
// 	}

	QMap<qint64, QVector<taskInfoDesc>> taskInfoMap;
	readXML(taskInfoMap, "taskInfo.xml");

    //return app.exec();
	return 0;
}
