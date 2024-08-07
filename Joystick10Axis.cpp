#include <QMessageBox>
#include <QApplication>
#include <QWidget>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QGridLayout>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>
#include <QPainter>
#include <QPaintEvent>
#include <QTextEdit>
#include <QCloseEvent>
#include <QEvent>
#include <QDateTime>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <Qt>
#include <QRegularExpression>
#include <QCoreApplication>
#include <QDebug>
#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    // Qt uygulaması oluştur
    QApplication app(argc, argv);

    // SDL2'yi başlat
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Joystick'i aç
    SDL_Joystick* joystick = nullptr;
    if (SDL_NumJoysticks() > 0) {
        joystick = SDL_JoystickOpen(0);
        if (!joystick) {
            std::cerr << "Joystick could not be opened! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }
    }
    else {
        std::cerr << "No joysticks connected!" << std::endl;
        SDL_Quit();
        return 1;
    }

    // Qt penceresini oluştur
    QWidget window;
    window.setWindowTitle("Joystick Example");
    window.resize(400, 200);

    // QLabel'ı oluştur ve düzenle
    QLabel* label = new QLabel("Position: (0, 0)", &window);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);
    window.setLayout(layout);

    // Joystick verilerini güncellemek için bir zamanlayıcı oluştur
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        if (joystick) {
            SDL_JoystickUpdate();
            int axisX = SDL_JoystickGetAxis(joystick, 0);
            int axisY = SDL_JoystickGetAxis(joystick, 1);

            static int x = 0, y = 0;

            // Ölçekleme
            if (axisX > 8000) x += 10; // Sağ hareket
            if (axisX < -8000) x -= 10; // Sol hareket
            if (axisY > 8000) y -= 10; // Yukarı hareket
            if (axisY < -8000) y += 10; // Aşağı hareket

            // QLabel'ı güncelle
            label->setText(QString("Position: (%1, %2)").arg(x).arg(y));
        }
        });
    timer.start(100); // 100 ms'de bir güncelle

    // Pencereyi göster ve Qt olay döngüsünü başlat
    window.show();
    int result = app.exec();

    // Temizlik
    if (joystick) SDL_JoystickClose(joystick);
    SDL_Quit();

    return result;
}