#include <QVBoxLayout>
#include <QFormLayout>
#include <QTimer>
#include <QDateTime>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsView>
#include <QPen>
#include <QObject>
#include <QApplication>
#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPainter>
#include <QPointF>
#include <QTimer>
#include <QDateTime>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QPointF>
#include <QWidget>

using namespace std;

QList<float> AxisNumbersFromMessage(QString);
void AddNumberLabels(QGraphicsScene*, bool, qreal, qreal, qreal);

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QTcpServer server;
    server.listen(QHostAddress("127.0.0.1"), 1254);

    QWidget windowDisplay;
    windowDisplay.setWindowTitle("Submarine Computer");

    QLabel* explanation = new QLabel("Welcome to the SubMerge", &windowDisplay);
    QLabel* statusWri = new QLabel("Status of Engine: ", &windowDisplay);
    QLabel* status = new QLabel("Engines are not running. System login is successful.", &windowDisplay);
    QLabel* currentDest = new QLabel("Current Destination :", &windowDisplay);
    QLabel* currentDestWri = new QLabel("(0,0)", &windowDisplay);
    QLabel* writeLabel = new QLabel("", &windowDisplay);
    QLabel* timeLabel = new QLabel("Time: ", &windowDisplay);

    QTimer* timer = new QTimer(&windowDisplay);
    QObject::connect(timer, &QTimer::timeout, [&]() {
        QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
        writeLabel->setText(currentTime);
        });
    timer->start(1000);

    QString initialTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    writeLabel->setText(initialTime);

    QTextEdit* directions = new QTextEdit();
    QVBoxLayout* graphic = new QVBoxLayout();

    QFormLayout* subLayoutDestWindow = new QFormLayout();
    subLayoutDestWindow->addRow(explanation);
    subLayoutDestWindow->addRow(timeLabel, writeLabel);
    subLayoutDestWindow->addRow(statusWri, status);
    subLayoutDestWindow->addRow(currentDest, currentDestWri);
    subLayoutDestWindow->addRow(directions, graphic);

    QVBoxLayout* destWindowLayout = new QVBoxLayout(&windowDisplay);
    destWindowLayout->addLayout(subLayoutDestWindow);

    QString comingData;

    QGraphicsScene graphScene;
    AddNumberLabels(&graphScene, true, -850, 850, 50);
    AddNumberLabels(&graphScene, false, -850, 850, 50);

    QGraphicsLineItem* xAxis = new QGraphicsLineItem(-850, 0, 850, 0);
    xAxis->setPen(QPen(Qt::blue, 6));
    graphScene.addItem(xAxis);

    QGraphicsLineItem* yAxis = new QGraphicsLineItem(0, -850, 0, 850);
    yAxis->setPen(QPen(Qt::blue, 6));
    graphScene.addItem(yAxis);

    QGraphicsView* viewGraph = new QGraphicsView(&graphScene);
    viewGraph->setSceneRect(-850, -850, 1700, 1700);

    QList<float> axisNumbers;
    QPointF previousPoint(0, 0);
    bool firstPoint = true;

    QObject::connect(&server, &QTcpServer::newConnection, [&server, &status, &currentDestWri, &directions, &comingData, &graphScene, &axisNumbers, &previousPoint, &firstPoint]() {
        QTcpSocket* socket = server.nextPendingConnection();
        QObject::connect(socket, &QTcpSocket::readyRead, [socket, &status, &currentDestWri, &directions, &comingData, &graphScene, &axisNumbers, &previousPoint, &firstPoint]() {
            QByteArray data = socket->readAll();
            QString message = QString(data).trimmed();
            comingData = message;
            if (message == "Engines started.") {
                qDebug() << "ENGINE STARTED";
                status->setText("ENGINE STARTED");
            }
            else if (message == "Engines stopped.") {
                qDebug() << "ENGINE STOPPED";
                status->setText("ENGINE STOPPED");
            }
            else if (message == "P") {
                qDebug() << "Joystick ready to read input. Push joystick when you ready!";
            }
            else {
                QString initialTime = QDateTime::currentDateTime().toString("hh:mm:ss");
                qDebug() << message;
                currentDestWri->setText(message);
                directions->append(message + "    Instructions were given at time: " + initialTime);
                axisNumbers = AxisNumbersFromMessage(comingData);
                QPointF currentPoint(axisNumbers[0], axisNumbers[1]);

                if (!firstPoint) {
                    QGraphicsLineItem* lineItem = new QGraphicsLineItem(QLineF(previousPoint, currentPoint));
                    lineItem->setPen(QPen(Qt::red, 4)); // Çizgi kalınlığını artır
                    graphScene.addItem(lineItem);
                }
                else {
                    firstPoint = false;
                }

                QGraphicsTextItem* textItem = new QGraphicsTextItem("");
                textItem->setPos(currentPoint.x() + 10, currentPoint.y() + 10);
                textItem->setDefaultTextColor(Qt::black);
                graphScene.addItem(textItem);

                previousPoint = currentPoint;
            }
            });
        });

    graphic->addWidget(viewGraph);
    windowDisplay.show();

    return app.exec();
}

void AddNumberLabels(QGraphicsScene* scene, bool isXAxis, qreal start, qreal end, qreal step) {
    for (qreal i = start; i <= end; i += step) {
        QString label = QString::number(i, 'f', 1);
        QGraphicsTextItem* textItem = new QGraphicsTextItem(label);
        if (isXAxis) {
            textItem->setPos(i, 10);
            textItem->setTextWidth(80);
        }
        else {
            textItem->setPos(10, i);
            textItem->setTextWidth(80);
        }
        textItem->setDefaultTextColor(Qt::black);
        scene->addItem(textItem);
    }
}

QList<float> AxisNumbersFromMessage(QString message) {
    QString numberMessage = message;
    numberMessage.remove('P').remove('(').remove(')').trimmed();
    QStringList numbers = numberMessage.split(',');

    QList<float> result;

    if (numbers.size() == 2) {
        bool ok1, ok2;
        float xAxisNum = numbers[0].toFloat(&ok1);
        float yAxisNum = numbers[1].toFloat(&ok2);

        if (ok1 && ok2) {
            result.append(xAxisNum);
            result.append(yAxisNum);
        }
    }
    else {
        float xAxisNum = 0.0;
        float yAxisNum = 0.0;
    }

    return result;
}



















/*
#include <QApplication>
#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QTimer>
#include <QDateTime>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsView>
#include <QPen>

using namespace std;

QList<float> AxisNumbersFromMessage(QString);
void AddNumberLabels(QGraphicsScene*, bool, qreal, qreal, qreal);

int main(int argc, char* argv[])
{

    QApplication app(argc, argv);

    QTcpServer server;
    server.listen(QHostAddress("127.0.0.1"), 1254);

    QWidget windowDisplay;
    windowDisplay.setWindowTitle("Submarine Computer");

    QLabel* explanation = new QLabel("Welcome to the SubMerge", &windowDisplay);
    QLabel* statusWri = new QLabel("Status of Engine: ", &windowDisplay);
    QLabel* status = new QLabel("Engines are not running. System login is successful.", &windowDisplay);
    QLabel* currentDest = new QLabel("Current Destination :", &windowDisplay);
    QLabel* currentDestWri = new QLabel("(0,0)", &windowDisplay);
    QLabel* writeLabel = new QLabel("", &windowDisplay);
    QLabel* timeLabel = new QLabel("Time: ", &windowDisplay);


    QTimer* timer = new QTimer(&windowDisplay);

    QObject::connect(timer, &QTimer::timeout, [&]() {
        QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
        writeLabel->setText(currentTime);
        });

    timer->start(1000);

    QString initialTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    writeLabel->setText(initialTime);

    QTextEdit* directions = new QTextEdit();

    QVBoxLayout* graphic = new QVBoxLayout();

    QFormLayout* subLayoutDestWindow = new QFormLayout();
    subLayoutDestWindow->addRow(explanation);
    subLayoutDestWindow->addRow(timeLabel, writeLabel);
    subLayoutDestWindow->addRow(statusWri, status);
    subLayoutDestWindow->addRow(currentDest, currentDestWri);
    subLayoutDestWindow->addRow(directions, graphic);

    QVBoxLayout* destWindowLayout = new QVBoxLayout(&windowDisplay);
    destWindowLayout->addLayout(subLayoutDestWindow);

    QString comingData;

    QGraphicsScene graphScene;

    AddNumberLabels(&graphScene, true, -850, 850, 50);
    AddNumberLabels(&graphScene, false, -850, 850, 50);

    QGraphicsLineItem* xAxis = new QGraphicsLineItem(-850, 0, 850, 0);
    xAxis->setPen(QPen(Qt::blue, 6));
    graphScene.addItem(xAxis);

    QGraphicsLineItem* yAxis = new QGraphicsLineItem(0, -850, 0, 850);
    yAxis->setPen(QPen(Qt::blue, 6));
    graphScene.addItem(yAxis);

    QGraphicsView* viewGraph = new QGraphicsView(&graphScene);
    viewGraph->setSceneRect(-850, -850, 1700, 1700);






    QList<float> axisNumbers;
    QPointF previousPoint(0, 0);
    bool firstPoint = true;

    QObject::connect(&server, &QTcpServer::newConnection, [&server, &status, &currentDestWri, &directions, &comingData, &graphScene, &axisNumbers, &previousPoint, &firstPoint]() {
        QTcpSocket* socket = server.nextPendingConnection();
        QObject::connect(socket, &QTcpSocket::readyRead, [socket, &status, &currentDestWri, &directions, &comingData, &graphScene, &axisNumbers, &previousPoint, &firstPoint]() {
            QByteArray data = socket->readAll();
            QString message = QString(data).trimmed();
            comingData = message;
            if (message == "Engines started.") {
                qDebug() << "ENGINE STARTED";
                status->setText("ENGINE STARTED");
            }
            else if (message == "Engines stopped.") {
                qDebug() << "ENGINE STOPPED";
                status->setText("ENGINE STOPPED");
            }
            else if (message == "P")
            {
                qDebug() << "Joystick ready to read input. Push joystick when you ready!";


            }
            else {

                QString initialTime = QDateTime::currentDateTime().toString("hh:mm:ss");
                qDebug() << message;
                currentDestWri->setText(message);
                directions->append(message + "    Instructions were given at time: " + initialTime);
                axisNumbers = AxisNumbersFromMessage(comingData);
                QPointF currentPoint(axisNumbers[0], axisNumbers[1]);

                if (!firstPoint) {
                    QGraphicsLineItem* lineItem = new QGraphicsLineItem(QLineF(previousPoint, currentPoint));
                    lineItem->setPen(QPen(Qt::red, 4)); // Kalınlık 2'den 4'e artırıldı
                    graphScene.addItem(lineItem);
                }
                else {
                    firstPoint = false;
                }

                QGraphicsTextItem* textItem = new QGraphicsTextItem("");
                textItem->setPos(currentPoint.x() + 10, currentPoint.y() + 10);
                textItem->setDefaultTextColor(Qt::black);
                graphScene.addItem(textItem);

                previousPoint = currentPoint;
            }
            });
        });

    graphic->addWidget(viewGraph);

    windowDisplay.show();

    return app.exec();
}

void AddNumberLabels(QGraphicsScene* scene, bool isXAxis, qreal start, qreal end, qreal step) {
    for (qreal i = start; i <= end; i += step) {
        QString label = QString::number(i, 'f', 1);
        QGraphicsTextItem* textItem = new QGraphicsTextItem(label);
        if (isXAxis) {
            textItem->setPos(i, 10);
            textItem->setTextWidth(80);
        }
        else {
            textItem->setPos(10, i);
            textItem->setTextWidth(80);
        }
        textItem->setDefaultTextColor(Qt::black);
        scene->addItem(textItem);
    }
}

QList<float> AxisNumbersFromMessage(QString message) {
    QString numberMessage = message;
    numberMessage.remove('P').remove('(').remove(')').trimmed();
    QStringList numbers = numberMessage.split(',');

    QList<float> result;

    if (numbers.size() == 2) {
        bool ok1, ok2;
        float xAxisNum = numbers[0].toFloat(&ok1);
        float yAxisNum = numbers[1].toFloat(&ok2);

        if (ok1 && ok2) {
            result.append(xAxisNum);
            result.append(yAxisNum);
        }
    }
    else {
        float xAxisNum = 0.0;
        float yAxisNum = 0.0;
    }

    return result;
}

*/