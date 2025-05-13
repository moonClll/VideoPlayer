// You may need to build the project (run Qt uic code generator) to get "ui_VWidget.h" resolved
#include <glib.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <QMessageBox>

#include "VWidget.h"
#include "ui_VWidget.h"


VWidget::VWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::VWidget){
    ui->setupUi(this);
    mask_img.load(":/Mask.png");
    if(mask_img.isNull()){
        QMessageBox::critical(this,"ERROR","ERR");
    }

    palette_normal.setBrush(backgroundRole(),Qt::black);
}

VWidget::~VWidget() {
    delete ui;
}

void VWidget::overlay(void *obj){
    if(!obj)
        return;
    gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (obj), winId());
}

void VWidget::show_normal(){
    setPalette(palette_normal);
}

void VWidget::show_loss(){
    palette_loss.setBrush(backgroundRole(),QBrush(mask_img.scaled(size())));
    setPalette(palette_loss);
}

void VWidget::mouseDoubleClickEvent(QMouseEvent* event){
    emit doubleClicked();
    QWidget::mouseDoubleClickEvent(event);
}

