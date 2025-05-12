#ifndef SKTOP_LITE_VWIDGET_H
#define SKTOP_LITE_VWIDGET_H

#include <QImage>
#include <QPalette>
#include <QWidget>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class VWidget; }
QT_END_NAMESPACE

class VWidget : public QWidget {
Q_OBJECT

public:
    explicit VWidget(QWidget *parent = nullptr);

    ~VWidget() override;

    void overlay(void *obj);
    void show_normal();
    void show_loss();

signals:
    void doubleClicked();

private:
    Ui::VWidget *ui;

    QPixmap             mask_img;
    QPalette            palette_loss, palette_normal;

    void mouseDoubleClickEvent(QMouseEvent* event) override;
};


#endif //SKTOP2_VWIDGET_H
