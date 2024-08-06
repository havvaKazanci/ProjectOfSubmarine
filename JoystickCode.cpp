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
#include <QPushButton>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QPointF>
#include <QWidget>
#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Joystick* joystick = SDL_JoystickOpen(0);

    if (!joystick) {
        std::cerr << "Couldn't open joystick: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    QWidget systemWindow;
    systemWindow.setWindowTitle("SubMerge Navigator System Page");

    QPushButton* newPageButton = new QPushButton("OPEN PROCESS PAGE");
    QPushButton* sdlClose = new QPushButton("Close SDL");

    QFormLayout* systemWindowLayout = new QFormLayout(&systemWindow);
    systemWindowLayout->addRow(newPageButton);

    QWidget processWindow;
    processWindow.setWindowTitle("SubMerge Navigator Process Page");

    QLabel* destination = new QLabel("");

    QFormLayout* processWindowLayout = new QFormLayout(&processWindow);
    processWindowLayout->addRow(destination);
    processWindowLayout->addRow(sdlClose);

    QObject::connect(newPageButton, &QPushButton::clicked, [&]() {
        processWindow.show();
        systemWindow.hide();
        });

    QObject::connect(sdlClose, &QPushButton::clicked, [&]() {
        SDL_JoystickClose(joystick);
        SDL_Quit();
        });

    QTimer* timer = new QTimer(&processWindow);

    QObject::connect(timer, &QTimer::timeout, [&]() {
        SDL_JoystickUpdate();
        int16_t x = SDL_JoystickGetAxis(joystick, 0); // X ekseni
        int16_t y = SDL_JoystickGetAxis(joystick, 1); // Y ekseni

        // Joystick'in değer aralığını -850 ile 850 arasına dönüştürme
        int16_t xScaled = ((x + 32768) * 1700 / 65535) - 850;
        int16_t yScaled = ((y + 32768) * 1700 / 65535) - 850;

        QString joystickText = QString("Joystick Position: (%1, %2)").arg(xScaled).arg(yScaled);
        destination->setText(joystickText);
        });

    timer->start(1000);

    systemWindow.show();

    return application.exec();
}