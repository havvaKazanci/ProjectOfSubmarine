/*
* #include <iostream>
#include <QApplication>
#include <QTcpSocket>
#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QObject>
#include <QMessageBox>
#include <QRegularExpression>
#include <QPoint>
#include <SDL2/SDL.h>
#include <QtNetwork/QTcpSocket>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <QString>
#include <QByteArray>
#include <QApplication>


using namespace std;

struct Point {
    int x, y;
};


bool IsValidFormat(const QString&);

int main(int argc, char* argv[])
{
    std::srand(std::time(0));
	QApplication app(argc, argv);
	
    SDL_Joystick* joystick = nullptr;

	QTcpSocket socket;
	socket.connectToHost("127.0.0.1", 1254);

	QWidget windowSystem;
	windowSystem.setWindowTitle("System Window");

    QString stringCurrentPosition = ("(0,0)");

    QLabel* labelTime = new QLabel("", &windowSystem);
    QLabel* labelTargetDestination = new QLabel("Target Destination Of Submarine: ");

    QLineEdit* lineEditTargetDestination = new QLineEdit();
    lineEditTargetDestination->setPlaceholderText("Please write the Target Location!");

    QTimer* timerSystem = new QTimer(&windowSystem);

    QObject::connect(timerSystem, &QTimer::timeout, [&]() {

        QString currentTime = QDateTime::currentDateTime().toString("Clock  hh:mm:ss");
        labelTime->setText(currentTime);

        });

    timerSystem->start(1000);

    QString stringInitialTime = QDateTime::currentDateTime().toString("Clock   hh:mm:ss");
    labelTime->setText(stringInitialTime);

    QPushButton* pushButtonStartStop = new QPushButton("START ENGINE");
    QPushButton* pushButtonManual = new QPushButton("CHANGE THE LOCATION MANUALLY");
    QPushButton* pushButtonJoystick = new QPushButton("CHANGE THE LOCATION WITH JOYSTICK");

    pushButtonJoystick->setEnabled(false);
    pushButtonManual->setEnabled(false);

    QFormLayout* layoutSystemWindow = new QFormLayout(&windowSystem);
    layoutSystemWindow->addRow(labelTime);
    layoutSystemWindow->addRow(pushButtonStartStop);
    layoutSystemWindow->addRow(labelTargetDestination, lineEditTargetDestination);
    layoutSystemWindow->addRow(pushButtonManual);
    layoutSystemWindow->addRow(pushButtonJoystick);


    QObject::connect(pushButtonStartStop, &QPushButton::clicked, [&socket, pushButtonStartStop, &pushButtonManual, &pushButtonJoystick, &lineEditTargetDestination]() {

        if (pushButtonStartStop->text() == "START ENGINE") {
            if (socket.state() == QAbstractSocket::ConnectedState)
            {
                pushButtonManual->setEnabled(true);
                pushButtonJoystick->setEnabled(true);

                pushButtonStartStop->setText("STOP ENGINE");
                QString stringStartMessage = "Engines started.";
                lineEditTargetDestination->setPlaceholderText("(0,0)");
                socket.write(stringStartMessage.toUtf8());
            }
            else
            {
                QMessageBox::warning(nullptr, "Error!", "Not connected to the server.");
            }

        }
        else {
            pushButtonStartStop->setText("START ENGINE");
            QString stringStopMessage = "Engines stopped.";
            socket.write(stringStopMessage.toUtf8());
            pushButtonManual->setEnabled(false);
            pushButtonJoystick->setEnabled(false);

        }

        });



    QObject::connect(pushButtonManual, &QPushButton::clicked, [&socket, &labelTargetDestination, &lineEditTargetDestination, &stringCurrentPosition]() {

        QString stringInputDestination = lineEditTargetDestination->text();

        if (!stringInputDestination.isEmpty() && socket.state() == QAbstractSocket::ConnectedState) {


            if (IsValidFormat(stringInputDestination))
            {
                if (stringInputDestination == stringCurrentPosition)
                {
                    QMessageBox::warning(nullptr, "Error!", "You are already at the destination you want to go to");
                }
                else
                {

                    stringCurrentPosition = stringInputDestination;
                    socket.write(stringCurrentPosition.toUtf8());
                }
            }
            else
            {
                QMessageBox::warning(nullptr, "Error!", "You have written an wrong format destination!");
            }
        }
        else {
            QMessageBox::warning(nullptr, "Error!", "The destination is not specified or is not connected to the server.");
        }
        });


    QObject::connect(pushButtonJoystick, &QPushButton::clicked, [&socket, &lineEditTargetDestination, pushButtonJoystick, &windowSystem, &joystick]() {



        if (pushButtonJoystick->text() == "CHANGE THE LOCATION WITH JOYSTICK" && socket.state() == QAbstractSocket::ConnectedState)
        {
            pushButtonJoystick->setText("START THE JOYSTICK");


            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
            {
                fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
                exit(1);
            }
            if (SDL_NumJoysticks() > 0)
            {
                SDL_JoystickEventState(SDL_ENABLE);
                joystick = SDL_JoystickOpen(0);
            }


        }
        else if (pushButtonJoystick->text() == "START THE JOYSTICK" && socket.state() == QAbstractSocket::ConnectedState)
        {
            QString stringStartedMessage = "P";
            socket.write(stringStartedMessage.toUtf8());
            pushButtonJoystick->setText("STOP THE JOYSTICK");
            bool quit = false;
            SDL_Event e;
            Point currentPoint = { 425, 425 };

            while (!quit) {
                // Event döngüsü
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                // Joystick verilerini oku
                int xValue = SDL_JoystickGetAxis(joystick, 0);
                int yValue = SDL_JoystickGetAxis(joystick, 1);

                // Joystick hareketini işleme
                if (xValue > 8000 || xValue < -8000) {
                    currentPoint.x += static_cast<int>(xValue / 32768.0 * 2);
                }
                if (yValue > 8000 || yValue < -8000) {
                    currentPoint.y += static_cast<int>(yValue / 32768.0 * 2);
                }

                // Sınırları kontrol et
                currentPoint.x = std::max(0, std::min(currentPoint.x, 850));
                currentPoint.y = std::max(0, std::min(currentPoint.y, 850));

                // Point verisini QString ve QByteArray'e dönüştür
                QString pointString = QString("(%1, %2)").arg(currentPoint.x).arg(currentPoint.y);
                QByteArray pointData = pointString.toUtf8();


                // Veriyi gönder
                socket.write(pointData);


                // Küçük bir gecikme ekle
                SDL_Delay(1000);
            }


        }
        else
        {
            if (socket.state() == QAbstractSocket::ConnectedState)
            {
                pushButtonJoystick->setText("CHANGE THE LOCATION WITH JOYSTICK");
                SDL_JoystickClose(joystick);
                SDL_Quit();
            }
        }
        });




	windowSystem.show();

	return app.exec();
}




bool IsValidFormat(const QString& input) {
    QRegularExpression regex("^\\(\\d+,\\d+\\)$");
    QRegularExpressionMatch match = regex.match(input);

    return match.hasMatch();
}
*/










#include <iostream>
#include <QApplication>
#include <QTcpSocket>
#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QObject>
#include <QMessageBox>
#include <QRegularExpression>
#include <QPoint>
#include <SDL2/SDL.h>
#include <QtNetwork/QTcpSocket>
#include <cstdlib>
#include <ctime>
#include <QString>
#include <QByteArray>


using namespace std;

struct Point {
    int x, y;
};

bool IsValidFormat(const QString&);

int main(int argc, char* argv[])
{
    std::srand(std::time(0));
    QApplication app(argc, argv);

    SDL_Joystick* joystick = nullptr;

    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 1254);

    QWidget windowSystem;
    windowSystem.setWindowTitle("System Window");

    QString stringCurrentPosition = "(0,0)";

    QLabel* labelTime = new QLabel("", &windowSystem);
    QLabel* labelTargetDestination = new QLabel("Target Destination Of Submarine: ");

    QLineEdit* lineEditTargetDestination = new QLineEdit();
    lineEditTargetDestination->setPlaceholderText("Please write the Target Location!");

    QTimer* timerSystem = new QTimer(&windowSystem);

    QObject::connect(timerSystem, &QTimer::timeout, [&]() {
        QString currentTime = QDateTime::currentDateTime().toString("Clock  hh:mm:ss");
        labelTime->setText(currentTime);
        });

    timerSystem->start(1000);

    QString stringInitialTime = QDateTime::currentDateTime().toString("Clock   hh:mm:ss");
    labelTime->setText(stringInitialTime);

    QPushButton* pushButtonStartStop = new QPushButton("START ENGINE");
    QPushButton* pushButtonManual = new QPushButton("CHANGE THE LOCATION MANUALLY");
    QPushButton* pushButtonJoystick = new QPushButton("CHANGE THE LOCATION WITH JOYSTICK");

    pushButtonJoystick->setEnabled(false);
    pushButtonManual->setEnabled(false);

    // Yeni etiket ekleyin
    QLabel* labelJoystickPosition = new QLabel("(0, 0)", &windowSystem);

    QFormLayout* layoutSystemWindow = new QFormLayout(&windowSystem);
    layoutSystemWindow->addRow(labelTime);
    layoutSystemWindow->addRow(pushButtonStartStop);
    layoutSystemWindow->addRow(labelTargetDestination, lineEditTargetDestination);
    layoutSystemWindow->addRow(pushButtonManual);
    layoutSystemWindow->addRow(pushButtonJoystick);
    layoutSystemWindow->addRow(labelJoystickPosition);  // Yeni etiket ekleyin

    QObject::connect(pushButtonStartStop, &QPushButton::clicked, [&socket, pushButtonStartStop, &pushButtonManual, &pushButtonJoystick, &lineEditTargetDestination]() {
        if (pushButtonStartStop->text() == "START ENGINE") {
            if (socket.state() == QAbstractSocket::ConnectedState) {
                pushButtonManual->setEnabled(true);
                pushButtonJoystick->setEnabled(true);
                pushButtonStartStop->setText("STOP ENGINE");
                QString stringStartMessage = "Engines started.";
                lineEditTargetDestination->setPlaceholderText("(0,0)");
                socket.write(stringStartMessage.toUtf8());
            }
            else {
                QMessageBox::warning(nullptr, "Error!", "Not connected to the server.");
            }
        }
        else {
            pushButtonStartStop->setText("START ENGINE");
            QString stringStopMessage = "Engines stopped.";
            socket.write(stringStopMessage.toUtf8());
            pushButtonManual->setEnabled(false);
            pushButtonJoystick->setEnabled(false);
        }
        });

    QObject::connect(pushButtonManual, &QPushButton::clicked, [&socket, &labelTargetDestination, &lineEditTargetDestination, &stringCurrentPosition]() {
        QString stringInputDestination = lineEditTargetDestination->text();
        if (!stringInputDestination.isEmpty() && socket.state() == QAbstractSocket::ConnectedState) {
            if (IsValidFormat(stringInputDestination)) {
                if (stringInputDestination == stringCurrentPosition) {
                    QMessageBox::warning(nullptr, "Error!", "You are already at the destination you want to go to");
                }
                else {
                    stringCurrentPosition = stringInputDestination;
                    socket.write(stringCurrentPosition.toUtf8());
                }
            }
            else {
                QMessageBox::warning(nullptr, "Error!", "You have written a wrong format destination!");
            }
        }
        else {
            QMessageBox::warning(nullptr, "Error!", "The destination is not specified or is not connected to the server.");
        }
        });

    QObject::connect(pushButtonJoystick, &QPushButton::clicked, [&socket, &lineEditTargetDestination, pushButtonJoystick, &windowSystem, &joystick, &labelJoystickPosition]() {
        if (pushButtonJoystick->text() == "CHANGE THE LOCATION WITH JOYSTICK" && socket.state() == QAbstractSocket::ConnectedState) {
            pushButtonJoystick->setText("START THE JOYSTICK");
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
                fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
                exit(1);
            }
            if (SDL_NumJoysticks() > 0) {
                SDL_JoystickEventState(SDL_ENABLE);
                joystick = SDL_JoystickOpen(0);
            }
        }
        else if (pushButtonJoystick->text() == "START THE JOYSTICK" && socket.state() == QAbstractSocket::ConnectedState) {
            QString stringStartedMessage = "P";
            socket.write(stringStartedMessage.toUtf8());
            pushButtonJoystick->setText("STOP THE JOYSTICK");
            bool quit = false;
            SDL_Event e;
            Point currentPoint = { 425, 425 };
            while (!quit) {
                // Event döngüsü
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                // Joystick verilerini oku
                int xValue = SDL_JoystickGetAxis(joystick, 0);
                int yValue = SDL_JoystickGetAxis(joystick, 1);

                // Joystick hareketini işleme
                if (xValue > 8000 || xValue < -8000) {
                    currentPoint.x += static_cast<int>(xValue / 32768.0 * 5);
                }
                if (yValue > 8000 || yValue < -8000) {
                    currentPoint.y += static_cast<int>(yValue / 32768.0 * 5);
                }

                // Sınırları kontrol et
                currentPoint.x = std::max(0, std::min(currentPoint.x, 850));
                currentPoint.y = std::max(0, std::min(currentPoint.y, 850));

                // Point verisini QString ve QByteArray'e dönüştür
                QString pointString = QString("(%1, %2)").arg(currentPoint.x).arg(currentPoint.y);
                QByteArray pointData = pointString.toUtf8();

                // Etiket güncelle
                labelJoystickPosition->setText(pointString);

                // Veriyi gönder
                socket.write(pointData);

                // Küçük bir gecikme ekle
                SDL_Delay(1000); // Gecikmeyi azaltarak daha hızlı güncelleme
            }
        }
        else {
            if (socket.state() == QAbstractSocket::ConnectedState) {
                pushButtonJoystick->setText("CHANGE THE LOCATION WITH JOYSTICK");
                SDL_JoystickClose(joystick);
                SDL_Quit();
            }
        }
        });

    windowSystem.show();
    return app.exec();
}

bool IsValidFormat(const QString& input) {
    QRegularExpression regex("^\\(\\d+,\\d+\\)$");
    QRegularExpressionMatch match = regex.match(input);
    return match.hasMatch();
}






/*

struct Point {
    int x, y;
};

// Rastgele sapmalar ekleyen fonksiyon
Point jitter(Point p, int intensity) {
    p.x += (rand() % (intensity * 2)) - intensity;
    p.y += (rand() % (intensity * 2)) - intensity;
    return p;
}

// Organik çizgiler çizme fonksiyonu
QString drawOrganicLine(Point p1, Point p2, int intensity) {
    const int steps = 10; // Çizgi boyunca kaç adımda sapma olacağı
    Point lastPoint = p1;
    QString result;
    for (int i = 1; i <= steps; ++i) {
        float t = (float)i / steps;
        Point point;
        point.x = p1.x + (p2.x - p1.x) * t;
        point.y = p1.y + (p2.y - p1.y) * t;
        point = jitter(point, intensity);

        result += QString("(%1, %2) -> ").arg(point.x).arg(point.y);
        lastPoint = point;
    }
    return result;
}

// Kodun diğer kısımları...

QObject::connect(pushButtonJoystick, &QPushButton::clicked, [&socket, &lineEditTargetDestination, pushButtonJoystick, &windowSystem, &joystick]() {

    if (pushButtonJoystick->text() == "CHANGE THE LOCATION WITH JOYSTICK" && socket.state() == QAbstractSocket::ConnectedState) {
        pushButtonJoystick->setText("START THE JOYSTICK");

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
            fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
            exit(1);
        }
        if (SDL_NumJoysticks() > 0) {
            SDL_JoystickEventState(SDL_ENABLE);
            joystick = SDL_JoystickOpen(0);
        }

    }
    else if (pushButtonJoystick->text() == "START THE JOYSTICK" && socket.state() == QAbstractSocket::ConnectedState) {
        QString stringStartedMessage = "P";
        socket.write(stringStartedMessage.toUtf8());
        pushButtonJoystick->setText("STOP THE JOYSTICK");
        bool quit = false;
        SDL_Event e;
        Point currentPoint = { 425, 425 };

        while (!quit) {
            // Event döngüsü
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            // Joystick verilerini oku
            int xValue = SDL_JoystickGetAxis(joystick, 0);
            int yValue = SDL_JoystickGetAxis(joystick, 1);

            // Önceki konumu kaydet
            Point prevPoint = currentPoint;

            // Joystick hareketini işleme
            if (xValue > 8000 || xValue < -8000) {
                currentPoint.x += static_cast<int>(xValue / 32768.0 * 15);
            }
            if (yValue > 8000 || yValue < -8000) {
                currentPoint.y += static_cast<int>(yValue / 32768.0 * 15);
            }

            // Sınırları kontrol et
            currentPoint.x = std::max(0, std::min(currentPoint.x, 850));
            currentPoint.y = std::max(0, std::min(currentPoint.y, 850));

            // Point verisini organik çizgi olarak QString'e dönüştür
            QString organicPath = drawOrganicLine(prevPoint, currentPoint, 5);

            // Veriyi gönder
            socket.write(organicPath.toUtf8());

            // Küçük bir gecikme ekle
            SDL_Delay(1000);
        }

    }
    else {
        if (socket.state() == QAbstractSocket::ConnectedState) {
            pushButtonJoystick->setText("CHANGE THE LOCATION WITH JOYSTICK");
            SDL_JoystickClose(joystick);
            SDL_Quit();
        }
    }
    });
*/



























/* ASIL ÇALIŞAN KOD */
/*QObject::connect(pushButtonJoystick, &QPushButton::clicked, [&socket, &lineEditTargetDestination, pushButtonJoystick, &windowSystem,&joystick]() {



        if (pushButtonJoystick->text() == "CHANGE THE LOCATION WITH JOYSTICK" && socket.state() == QAbstractSocket::ConnectedState)
        {
            pushButtonJoystick->setText("START THE JOYSTICK");
            

            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
            {
                fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
                exit(1);
            }
            if (SDL_NumJoysticks() > 0)
            {
                SDL_JoystickEventState(SDL_ENABLE);
                joystick = SDL_JoystickOpen(0);
            }
            

        }
        else if (pushButtonJoystick->text() == "START THE JOYSTICK" && socket.state() == QAbstractSocket::ConnectedState)
        {
            QString stringStartedMessage = "P";
            socket.write(stringStartedMessage.toUtf8());
            pushButtonJoystick->setText("STOP THE JOYSTICK");
            bool quit = false;
            SDL_Event e;
            Point currentPoint = { 425, 425 };

            while (!quit) {
                // Event döngüsü
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                // Joystick verilerini oku
                int xValue = SDL_JoystickGetAxis(joystick, 0);
                int yValue = SDL_JoystickGetAxis(joystick, 1);

                // Joystick hareketini işleme
                if (xValue > 8000 || xValue < -8000) {
                    currentPoint.x += static_cast<int>(xValue / 32768.0 *15);
                }
                if (yValue > 8000 || yValue < -8000) {
                    currentPoint.y += static_cast<int>(yValue / 32768.0 *15);
                }

                // Sınırları kontrol et
                currentPoint.x = std::max(0, std::min(currentPoint.x, 850));
                currentPoint.y = std::max(0, std::min(currentPoint.y, 850));

                // Point verisini QString ve QByteArray'e dönüştür
                QString pointString = QString("(%1, %2)").arg(currentPoint.x).arg(currentPoint.y);
                QByteArray pointData = pointString.toUtf8();


                // Veriyi gönder
                socket.write(pointData);
                

                // Küçük bir gecikme ekle
                SDL_Delay(1000);
            }
            

        }
        else
        {
            if (socket.state() == QAbstractSocket::ConnectedState)
            {
                pushButtonJoystick->setText("CHANGE THE LOCATION WITH JOYSTICK");
                SDL_JoystickClose(joystick);
                SDL_Quit();
            }
        }
        });*/













