
#pragma once

#include <QWidget>
#include <QTimer>
#include <vector>

// forward declaration
class ScopeBuffer;

QT_BEGIN_NAMESPACE
namespace Ui { class Scope; }
QT_END_NAMESPACE

class Scope : public QWidget {
    Q_OBJECT
public:
    explicit Scope(QWidget* parent = nullptr);
    ~Scope();

    void setScopeBuffer(ScopeBuffer* buffer);

protected:
    // autocalled on QT's refresh loop
    // scope drawing happens here
    void paintEvent(QPaintEvent*) override;

private:

    Ui::Scope* ui_;

    ScopeBuffer* buffer_ = nullptr;
    std::vector<float> samples_;
    QTimer timer_;
    
};
