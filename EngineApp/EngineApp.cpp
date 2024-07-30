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



using namespace Qt;

QList<float> AxisNumbersFromMessage(QString); //CREATES LIST XaxisNum , YaxisNum 
void addNumberLabels(QGraphicsScene*, bool, qreal, qreal, qreal); //ADDS NUMBERS IN X AND Y AXIS
QPointF SetQPointF(float, float); 


int main(int argc, char* argv[])
{
   
    QApplication app(argc, argv);

    QTcpServer server;
    server.listen(QHostAddress("10.10.14.132"), 1254);

    QWidget destWindow;
    destWindow.setWindowTitle("Engine Application");

    QLabel* explanation = new QLabel("Welcome to the SubMerge", &destWindow);
    QLabel* statusWri = new QLabel("Status of Engine: ", &destWindow);
    QLabel* status = new QLabel("Engines are not running. System login is successful.", &destWindow);
    QLabel* currentDest = new QLabel("Current Destination :", &destWindow);
    QLabel* currentDestWri = new QLabel("(0,0)", &destWindow);
    QLabel* writeLabel = new QLabel("", &destWindow);
    QLabel* timeLabel = new QLabel("Time: ", &destWindow);

    QTimer* timer = new QTimer(&destWindow);

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


    QVBoxLayout* destWindowLayout = new QVBoxLayout(&destWindow);
    destWindowLayout->addLayout(subLayoutDestWindow);

    QString comingData;

    QGraphicsScene graphScene;

    addNumberLabels(&graphScene, true, -850, 850, 50);
    addNumberLabels(&graphScene, false, -850, 850, 50);


    QGraphicsLineItem* xAxis = new QGraphicsLineItem(-850, 0, 850, 0);
    xAxis->setPen(QPen(Qt::blue, 2));
    graphScene.addItem(xAxis);

    QGraphicsLineItem* yAxis = new QGraphicsLineItem(0, -850, 0, 850);
    yAxis->setPen(QPen(Qt::blue, 2));
    graphScene.addItem(yAxis);

    
    QGraphicsView* viewGraph = new QGraphicsView(&graphScene);

    QList<float> axisNumbers;

    QWidget* graphisc = new QWidget();

    QList<QGraphicsEllipseItem*> pointItems;


    QObject::connect(&server, &QTcpServer::newConnection, [&server, &status, &currentDestWri, &directions,&comingData, &destWindow,&axisNumbers,&graphScene,&pointItems]() {
        QTcpSocket* socket = server.nextPendingConnection();
        QObject::connect(socket, &QTcpSocket::readyRead, [socket, &status, &currentDestWri, &directions,&comingData, &destWindow,&axisNumbers,&graphScene,&pointItems]() {
            QByteArray data = socket->readAll();
            QString message = QString(data).trimmed();
            comingData = message;
            if (message == "START ENGINE") {
                qDebug() << "ENGINE STARTED";
                status->setText("ENGINE STARTED");

            }
            else if (message == "STOP ENGINE")
            {
                qDebug() << "ENGINE STOPPED";
                status->setText("ENGINE STOPPED");

            }
            else
            {
                QString initialTime = QDateTime::currentDateTime().toString("hh:mm:ss");
                qDebug() << message;
                currentDestWri->setText(message);
                directions->append(message + "    Instructions were given at time: " + initialTime);
                axisNumbers= AxisNumbersFromMessage(comingData);
                QPointF point(axisNumbers[0], axisNumbers[1]);

                for (auto item : pointItems) {
                    item->setBrush(Qt::blue); 
                }

                QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem(point.x() - 2, point.y() - 2, 4, 4);
                pointItem->setBrush(Qt::red);
                graphScene.addItem(pointItem);

                QGraphicsTextItem* textItem = new QGraphicsTextItem(QString("(%1, %2)").arg(point.x()).arg(point.y()));
                textItem->setPos(point.x() + 10, point.y() + 10); 
                textItem->setDefaultTextColor(Qt::black); 
                graphScene.addItem(textItem);

                pointItems.append(pointItem);
            }
            });
        });

    graphic->addWidget(viewGraph);
    viewGraph->setSceneRect(-850, -850, 1700, 1700);

    destWindow.show();

    return app.exec();

}




QPointF SetQPointF(float numX, float numY)
{
    QPointF point(0, 0);
    point.setX(numX);
    point.setY(numY);

    return point;
}

void addNumberLabels(QGraphicsScene* scene, bool isXAxis, qreal start, qreal end, qreal step) {

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


QList<float> AxisNumbersFromMessage(QString message)
{

    QString numberMessage = message;
    numberMessage.remove('(').remove(')').trimmed();
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
    else
    {
        float xAxisNum = 0.0;
        float yAxisNum = 0.0;
    }

    return result;
   
}

