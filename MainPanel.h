#ifndef SKTOP_LITE_MAINPANEL_H
#define SKTOP_LITE_MAINPANEL_H

#include <QMainWindow>
#include <QFileDialog>
#include <fstream>
#include <yaml-cpp/yaml.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainPanel; }
QT_END_NAMESPACE

class MainPanel : public QMainWindow {
Q_OBJECT

public:
    explicit MainPanel(QWidget *parent = nullptr);

    ~MainPanel() override;

public slots:
    void on_actionImport_triggered();
    void on_actionExport_triggered();

private:
    Ui::MainPanel *ui;
};


#endif //SKTOP2_MAINPANEL_H
