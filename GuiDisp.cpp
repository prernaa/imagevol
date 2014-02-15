#include "guiDisp.h"
#include <QFileDialog>

guiDisp::guiDisp(QGridLayout *l)
{
    layout = l; /// layout initialised

    /// Initialise height and width params
    widthImg = 100;
    heighImg = 100;

    /// Create initial empty left & right images

    QImage emptyImg(widthImg,heighImg, QImage::Format_RGB888);
    emptyImg.fill(QColor(255,255,255));
    //leftImg.load("myimage.jpg");
    //rightImg.load("myimage.jpg");
    //leftImg.save("try.png");




    /// set the empty images as labels
    leftpix = QPixmap::fromImage(emptyImg);
    rightpix = QPixmap::fromImage(emptyImg);
    leftLabel.setPixmap(leftpix);
    rightLabel.setPixmap(rightpix);

    /// Add 2 empty images
    layout->addWidget(&leftLabel,0,0,1,1, Qt::AlignCenter);
    layout->addWidget(&rightLabel,0,1,1,1, Qt::AlignCenter);

    /// Initialise and add buttons
    start = new QPushButton("START ALGO");
    start->setEnabled(false);
    browse = new QPushButton("BROWSE");
    connect(start, SIGNAL(clicked()), this, SLOT(startPressed()));
    connect(browse, SIGNAL(clicked()), this, SLOT(browsePressed()));

    algo1 = new GA();

    connect(algo1, SIGNAL(new_drawing_created(QPixmap *)), this, SLOT(new_drawing_received(QPixmap*)));

    layout->addWidget(start,1,0,1,1, Qt::AlignCenter);
    layout->addWidget(browse,1,1,1,1, Qt::AlignCenter);


}
guiDisp::~guiDisp(){

}
guiDisp::guiDisp(){

}
void guiDisp::startPressed(){
    //qDebug()<<"start algo";
    /// Connect signals and slots to receive new pixmap
    algo1->start(&rightpix, &leftpix);
}

void guiDisp::browsePressed(){
    //qDebug()<<"browse file";
    QString fileName = QFileDialog::getOpenFileName(0, tr("Open File"),"",tr("Files (*.*)"));
    //qDebug()<<fileName;
    leftpix.load(fileName);
    //leftpix.scaled(widthImg,heighImg);
    leftpix=leftpix.scaled(widthImg,heighImg,Qt::IgnoreAspectRatio);
    QImage emptyImg(leftpix.size(), QImage::Format_RGB888);
    emptyImg.fill(QColor(255,255,255));
    rightpix = QPixmap::fromImage(emptyImg);
    leftLabel.setPixmap(leftpix);
    rightLabel.setPixmap(rightpix);
    browse->setEnabled(false);
    start->setEnabled(true);
}
void guiDisp::new_drawing_received(QPixmap *newPix){
    rightLabel.setPixmap(*newPix);
}
