#include <gst/gst.h>
#include <gst/gstregistry.h>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include "MainPanel.h"

int main(int argc, char *argv[]) {
    //Gstreamer Initial
    gst_init(nullptr,nullptr);

    //Resource Initial
    QDir dir(QDir::currentPath());
    for(int i=0;i<4;i++){
        if(!dir.exists(QString("video%1").arg(i)))
            dir.mkdir(QString("video%1").arg(i));
    }

    //Application Initial and Run
    QApplication a(argc, argv);
    MainPanel mainPanel;
    mainPanel.show();

    //Application Destroy
    int app_res=QApplication::exec();

    return app_res;
}
