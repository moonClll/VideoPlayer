#ifndef SKTOP_LITE_VMDIAREA_H
#define SKTOP_LITE_VMDIAREA_H

#include <QMdiArea>

QT_BEGIN_NAMESPACE
namespace Ui { class VMdiArea; }
QT_END_NAMESPACE

class VMdiArea : public QMdiArea {
Q_OBJECT

public:
    explicit VMdiArea(QWidget *parent = nullptr);

    ~VMdiArea() override;

private:
    Ui::VMdiArea *ui;
};


#endif //SKTOP2_VMDIAREA_H
