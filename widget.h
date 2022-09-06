#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//#include <cereal/archives/json.hpp>
//#include <cereal/types/vector.hpp>
#include <vector>
#include <triangle.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

//template <class Archive>
//void serialize (Archive &archive, QPoint &point){
//    archive(point.rx());
//    archive(point.ry());
//}


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


    bool eventFilter(QObject *obj, QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);

private slots:

    void on_saveImageButton_clicked();

    void on_iterateButton_clicked();

    void on_exitButton_clicked();

    void on_saveJsonButton_clicked();

    void on_loadJsonButton_clicked();

    void on_initButton_clicked();

    void on_resetButton_clicked();

private:
    int CoordinateX = -1;
    int CoordinateY = -1;
//    bool DrawText = false;
    std::vector<QPoint> Utvonal;
//    int counter = 10000100;

//public:
//    template <class Archiver>
//    void serialize(Archiver &archive){
//        archive(CEREAL_NVP(CoordX));
//        archive(CEREAL_NVP(CoordY));
//        archive(CEREAL_NVP(DrawText));
//        archive(CEREAL_NVP(counter));
//    }


    void Render(QPaintDevice * PaintDevice);

    std::vector<Triangle> Sierp;


private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
