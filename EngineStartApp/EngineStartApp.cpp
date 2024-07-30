
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




using namespace std; 


bool IsValidFormat(const QString&);


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);  //INITILAZING A Q APPLICATION FIRST

    QTcpSocket socket;
    socket.connectToHost("10.10.14.132", 1254);     //CONNECTION PART

    //INITILAZING LOGIN PAGE (FIRST PAGE)

    QWidget loginWindow;
    loginWindow.setWindowTitle("Welcome to SubMerge Navigator System");

    QLabel* marketName = new QLabel("SubMerge Navigator Login System", &loginWindow);
    QLabel* userId = new QLabel("User ID: ", &loginWindow);
    QLabel* password = new QLabel("Password: ", &loginWindow);
    QLabel* explanation = new QLabel("Login credentials are essential to initiate and maintain operational integrity within the submarine control system.", &loginWindow);

    QLineEdit* userIdWri = new QLineEdit();
    userIdWri->setPlaceholderText("example@sub.tr");
    QLineEdit* passwordWri = new QLineEdit();
    passwordWri->setPlaceholderText("Please login your password.");
    passwordWri->setEchoMode(QLineEdit::Password);

    QPushButton* loginButton = new QPushButton("LOG IN");

    QFormLayout* formLayoutLogWindow = new QFormLayout(&loginWindow);
    formLayoutLogWindow->addRow(marketName);
    formLayoutLogWindow->addRow(userId, userIdWri);
    formLayoutLogWindow->addRow(password, passwordWri);
    formLayoutLogWindow->addRow(loginButton);
    formLayoutLogWindow->addRow(explanation);


    //END OF THE FIRST (LOGIN) PAGE INITILAZING

    //SYSTEM WINDOW INITILAZING

    QWidget systemWindow;
    systemWindow.setWindowTitle("SubMerge Navigator System Properties");

    QLabel* statusOfSubmarLabel = new QLabel("Status of Submarine: ", &systemWindow);
    QLabel* currLocaOfSubmarLabel = new QLabel("Current Location of Submarine: ", &systemWindow);
    QLabel* currLocaOfSubmarWri = new QLabel("(0,0)");
    QLabel* engineMessage = new QLabel("INITIAL STATE ENGINE NOT WORKING!", &systemWindow);
    QLabel* writeLabel = new QLabel("", &systemWindow);
    QLabel* timeLabel = new QLabel("Time: ", &systemWindow);
    //SYSTEM TIME PROCESSES

    QTimer* timer = new QTimer(&systemWindow);

    

    QObject::connect(timer, &QTimer::timeout, [&]() {

        QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
        writeLabel->setText(currentTime);

        });


    timer->start(1000);

    QString initialTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    writeLabel->setText(initialTime);

    // SYSTEM TIME PROCESSES ENDING 

    QPushButton* startButton = new QPushButton("START ENGINE");
    QPushButton* stopButton = new QPushButton("STOP ENGINE");
    QPushButton* changLocaButton = new QPushButton("CHANGE THE LOCATION OF SUBMARINE");
    changLocaButton->setEnabled(false);
    stopButton->setEnabled(false);

    QFormLayout* layoutSystemWindow = new QFormLayout();
    layoutSystemWindow->addRow(timeLabel, writeLabel);
    layoutSystemWindow->addRow(statusOfSubmarLabel, engineMessage);
    layoutSystemWindow->addRow(currLocaOfSubmarLabel, currLocaOfSubmarWri);
    layoutSystemWindow->addRow(startButton);
    layoutSystemWindow->addRow(changLocaButton);
    layoutSystemWindow->addRow(stopButton);


    QVBoxLayout* systemLayout = new QVBoxLayout(&systemWindow);
    systemLayout->addLayout(layoutSystemWindow);

    //SYSTEM WINDOW INITILAZING END

    //DEST WINDOW (THIRD PAGE)

    QWidget destWindow;
    destWindow.setWindowTitle("Process Page Of Submarine");

    QLabel* currentLoca = new QLabel("Current Location Of Submarine: ");
    QLabel* currentLocaWri = new QLabel("(0,0)");
    QLabel* targetDest = new QLabel("Target Destination Of Submarine: ");


    QLineEdit* targetDestWri = new QLineEdit();
    targetDestWri->setPlaceholderText("Please write the Target Location!");

    QPushButton* goToDest = new QPushButton("Complete the Process!");
    QPushButton* backButton = new QPushButton("Back");

    QFormLayout* subLayoutOfDest = new QFormLayout();              
    subLayoutOfDest->addRow(currentLoca, currentLocaWri);
    subLayoutOfDest->addRow(targetDest, targetDestWri);
    subLayoutOfDest->addRow(backButton, goToDest);

    QVBoxLayout* processLayout = new QVBoxLayout(&destWindow);     
    processLayout->addLayout(subLayoutOfDest);


    //DEST WINDOW INITILAZING END



    QObject::connect(loginButton, &QPushButton::clicked, [&]() {
        QString userId = userIdWri->text();
        QString password = passwordWri->text();


        if (userId == "admin@sub.tr" && password == "password") {
            loginWindow.hide();
            systemWindow.show();
        }
        else {
            QMessageBox::warning(&loginWindow, "Login Failed", "Invalid username or password");
        }
        });




    QObject::connect(startButton, &QPushButton::clicked, [&socket, &engineMessage, startButton, &stopButton, &changLocaButton]() {

        if (socket.state() == QAbstractSocket::ConnectedState) {
            QString openStatu = "ENGINES ARE RUNNING!";
            engineMessage->setText(openStatu);
            QString startMessage = "START ENGINE";
            socket.write(startMessage.toUtf8());
            startButton->setEnabled(false);
            stopButton->setEnabled(true);
            changLocaButton->setEnabled(true);
  

        }
        else {
            QMessageBox::warning(nullptr, "Error!", "Not connected to server.");
        }
        });





    QObject::connect(stopButton, &QPushButton::clicked, [&socket, &engineMessage, stopButton,&startButton, &changLocaButton]() {

        if (socket.state() == QAbstractSocket::ConnectedState) {
            QString closeStatu = "ENGINES ARE OFF!";
            engineMessage->setText(closeStatu);
            QString stopMessage = "STOP ENGINE";
            socket.write(stopMessage.toUtf8());
            stopButton->setEnabled(false);
            startButton->setEnabled(true);
            changLocaButton->setEnabled(false);
            
        }
        else {
            QMessageBox::warning(nullptr, "Error!", "Not connected to server.");
        }
        });




    QObject::connect(changLocaButton, &QPushButton::clicked, [&]() {

        systemWindow.hide();
        destWindow.show();
        });




    QObject::connect(goToDest, &QPushButton::clicked, [&socket, &targetDestWri, &currentLocaWri, &currLocaOfSubmarWri]() {
        QString currentDestination = currentLocaWri->text();
        QString destination = targetDestWri->text();
        if (!destination.isEmpty() && socket.state() == QAbstractSocket::ConnectedState ) {
                
                if (IsValidFormat(destination))
                {
                    if (destination == currentDestination)
                    {
                        QMessageBox::warning(nullptr, "Error!", "You are already at the destination you want to go to");
                    }
                    else
                    {
                        currLocaOfSubmarWri->setText(destination);
                        currentLocaWri->setText(destination);
                        QString message = destination;
                        socket.write(message.toUtf8());
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





    QObject::connect(backButton, &QPushButton::clicked, [&]() {
        systemWindow.show();
        destWindow.hide();
    });




    loginWindow.show();

    return app.exec();
}


bool IsValidFormat(const QString& input)    //FUNC FOR VALID INPUT CHECK IS LIKE ?(NUM,NUM)?
{
    QRegularExpression regex("^\\(\\d+,\\d+\\)$");

    
    QRegularExpressionMatch match = regex.match(input);

    return match.hasMatch();
}

