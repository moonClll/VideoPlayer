#ifndef SKTOP_LITE_LIVEPANEL_H
#define SKTOP_LITE_LIVEPANEL_H

#include <vector>
#include <QWidget>
#include <QGridLayout>
#include <QMdiSubWindow>
#include "livewindow/LiveWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LivePanel; }
QT_END_NAMESPACE

class LivePanel : public QWidget {
Q_OBJECT

public:
    explicit LivePanel(QWidget *parent = nullptr);

    ~LivePanel() override;

    void import_uris(std::vector<QString> uris);
    std::vector<QString> export_uris();

public slots:
    void on_btmAdd_clicked();
    void on_btmDel_clicked();

private:
    Ui::LivePanel *ui;
    QGridLayout   *layout;
    QPoint        next_pos;
    QPoint        current_pos;

    bool          pending_vmdiarea_update;

    std::vector<std::shared_ptr<LiveWindow>> livewindow_list;
    void update_vmdiarea();

    void set_preview_widget_show(bool show);
};


#endif //SKTOP2_LIVEPANEL_H
