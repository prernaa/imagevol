#include <QApplication>
#include "guiDisp.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /// Create main window and set empty layout
    QWidget *mainWindow = new QWidget;
    QGridLayout *layout = new QGridLayout();
    mainWindow->setLayout(layout);

    /// Initialise guiDisp object
    guiDisp gui(layout);

    /*
    QImage imgtry;
    imgtry.load("myimage.jpg");
    QPixmap pm = QPixmap::fromImage(imgtry);
    QLabel lbl;
    lbl.setPixmap(pm);
    layout->addWidget(&lblL,0,0,1,1,Qt::AlignCenter);
    */


    /// show main window
    mainWindow->show();


    return a.exec();
}
