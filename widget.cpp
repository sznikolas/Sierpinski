#include "widget.h"
#include "./ui_widget.h"
#include "QEvent"
#include "QMouseEvent"
#include "QDebug"
#include "QString"
#include "QPainter"
#include "QFileDialog"
#include <fstream>
#include <QBrush>
//#include <cereal/archives/json.hpp>
//#include <cereal/types/vector.hpp>
//#include <cereal/types/string.hpp>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setMouseTracking(true);
    installEventFilter(this);

}

Widget::~Widget()
{
    delete ui;
}

//figyeli a kurzor pozíciót
bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {

        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        CoordinateX = mouseEvent->pos().x();
        CoordinateY = mouseEvent->pos().y();
//        qDebug() << (QString("Mouse move (%1,%2)")
//                                   .arg(mouseEvent->pos().x())
//                                   .arg(mouseEvent->pos().y()));
    }
    update();
    return false;
}

//itt történik az utak létrehozása az egér kattintással
void Widget::mousePressEvent(QMouseEvent *event) {
//    qDebug() << "Widget::mousePressEvent";
    Utvonal.push_back(event->pos());
    if (Utvonal.size()==3){

        Sierp.clear();
        Sierp.push_back({{Utvonal.at(0)},{Utvonal.at(1)},{Utvonal.at(2)}});
        Utvonal.clear();
    }

//    if (CoordinateX >=0 && CoordinateY >= 0) {
//        int CurrentX = event->pos().x();
//        int CurrentY = event->pos().y();}

    }


//a bal klikk elengedésekor frissíti az ablakot
void Widget::mouseReleaseEvent(QMouseEvent *event){
//    Utvonal.push_back(event->pos());
    update();
}


bool ellenorzihogyrajzoltamevalamit = false;
//meghívja a render funkciót
void Widget::paintEvent(QPaintEvent *event){
    Render(this);

//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing, true);
//    auto OrigPen = painter.pen();
//    if (DrawText) {
//        painter.setPen(Qt::blue);
//        painter.setFont(QFont("Arial",30));
//        painter.drawText(rect(), Qt::AlignCenter, "Qt");
//    }
//    painter.setPen(OrigPen);
//    if (Utvonal.size()>=2){
//        for (int q=0; q < Utvonal.size()-1; q++){
//            painter.drawLine(Utvonal.at(q), Utvonal.at(q + 1));
//        }
//    }
//    if (Utvonal.size() > 0){
//        painter.drawLine(Utvonal.back(),QPoint(CoordinateX, CoordinateY));
//        }
}



//void Widget::on_pushButton_clicked()
//{
//    DrawText = true;
//    update();

//}

//képmentés
void Widget::on_saveImageButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Fájl mentése"),
                                                    "C:/sierpinski.png",
                                                    tr("Images (*.png)"));
    if(!fileName.isEmpty()){

    QImage image (600,600, QImage::Format_RGB32);

    image.fill(QColor(207, 207, 207));
    Render(&image);
    image.save(fileName);
    }
}



void Widget::Render(QPaintDevice * PaintDevice) {
    QPainter painter(PaintDevice);
    //élek simítása
    painter.setRenderHint(QPainter::Antialiasing, true);
    //kirajzolja a vonalakat a haromszog rajzolaskor
        auto OrigPen = painter.pen();
//    if (DrawText) {
//        painter.setPen(Qt::blue);
//        painter.setFont(QFont("Arial",30));
//        painter.drawText(rect(), Qt::AlignCenter, "Qt");
//    }


//kirajzolja a háromszög oldalait
    painter.setPen(OrigPen);
    if (Utvonal.size()>=2){
        for (int q=0; q < Utvonal.size()-1; q++){
            painter.drawLine(Utvonal.at(q), Utvonal.at(q + 1));
        }
    }
    //húzza a csíkot
    if (Utvonal.size() > 0){
        ellenorzihogyrajzoltamevalamit=true;
        painter.drawLine(Utvonal.back(),QPoint(CoordinateX, CoordinateY));
    }
//a háromszög belseje
    QBrush brush;
    brush.setColor(QColor(222, 50, 38));
    brush.setStyle(Qt::BrushStyle::Dense1Pattern);
    painter.setBrush(brush);

    for (const auto & triangle: Sierp){
 //       drawPolygon(const QPointF *points, int pointCount, Qt::FillRule
 //                   fillRule = Qt::OddEvenFill)
       std::vector<QPointF> Points{triangle.A,triangle.B, triangle.C};
         painter.drawPolygon(Points.data(),3, Qt::FillRule::WindingFill);
//        painter.drawLine(triangle.A, triangle.B);
//        painter.drawLine(triangle.B, triangle.C);
//        painter.drawLine(triangle.C, triangle.A);
    }
}

//kilepes
void Widget::on_exitButton_clicked()
{
    QApplication::exit();
}

//iterálás végzése, oldalakat töri 2vel és nézi a iterálások számát<10
int n;
void Widget::on_iterateButton_clicked()
{
    if (ellenorzihogyrajzoltamevalamit == true){
     n = n+1;
    }
    std::vector<Triangle> ujSierpinskioHaromsz;
    for (const auto & triangle : Sierp){
        ujSierpinskioHaromsz.push_back({{triangle.A},{(triangle.A+triangle.B)/2},{(triangle.C+triangle.A)/2}});
        ujSierpinskioHaromsz.push_back({{(triangle.A+triangle.B)/2},{triangle.B},{(triangle.B+triangle.C)/2}});
        ujSierpinskioHaromsz.push_back({{(triangle.A+triangle.C)/2},{(triangle.B+triangle.C)/2},{triangle.C}});
    }
    Sierp = ujSierpinskioHaromsz;
    update();

    if (n == 9){
        ui->iterateButton->setEnabled(false);
        n=0;
        ellenorzihogyrajzoltamevalamit=false;
    }
}

//megprobaltam a jsont
void Widget::on_saveJsonButton_clicked()
{
//    auto fileName = QFileDialog::getSaveFileName(
//         this, tr("Save Image Json"), "C:/", tr("Image files (*.json)"));

//       if (!fileName.isEmpty()){
//           std::ofstream ofs(fileName.toStdString());
//           cereal::JSONOutputArchive archive(ofs);
//           archive(*this);
//       }

}

//megprobaltam a jsont
void Widget::on_loadJsonButton_clicked()
{
// auto fileName = QFileDialog::getOpenFileName(
//             this, tr("Save Image Json"), "C:/", tr("Image files (*.json)"));

//    if (!fileName.isEmpty()){
//        std::ifstream ifs(fileName.toStdString());
//        cereal::JSONInputArchive archive(ifs);
//        archive(*this);
//    }
}

//berajzol egy előre meghatarozott haromszoget probakent
void Widget::on_initButton_clicked()
{
    Sierp.clear(); //kitorlok barmit ami benne van
    Sierp.push_back({{10,10},{600,10},{300,320}}); //beletettem egy haromszoget
    update();
}

//törli az ablak rajzokat és az iterációs gombot újra engedélyezi
void Widget::on_resetButton_clicked()
{
     Sierp.clear(); //kitorlok barmit ami benne van
     ui->iterateButton->setEnabled(true);
     n=0;
}
