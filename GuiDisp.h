#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QPushButton>
#include <QDebug>
#include "GA.h"

class guiDisp : public QObject
{
    Q_OBJECT

    QGridLayout *layout;

    QLabel leftLabel, rightLabel;
    QPixmap leftpix, rightpix;
    QPushButton *start, *browse;

    int widthImg, heighImg;

    GA *algo1;


public:
    guiDisp(QGridLayout *l);
    guiDisp();
    ~guiDisp();
public slots:
    void startPressed();
    void browsePressed();
    void new_drawing_received(QPixmap *newPix);
};
