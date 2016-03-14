#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H
#include <QLabel>
#include <QMouseEvent>
#include <QEvent>

class ClickableLabel : public QLabel
{
    Q_OBJECT
    public:
        explicit ClickableLabel(QWidget* parent );
        ~ClickableLabel();
    signals:
        void clicked();
        void released();
    protected:
        void mousePressEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);
};

#endif // CLICKABLELABEL_H
