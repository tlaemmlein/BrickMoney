#pragma once

#include <QObject>
#include <QQmlEngine>

class QDoubleValueArg : public QObject
{
    Q_OBJECT
public:
    QDoubleValueArg(double value) : Value(value)
    {
        QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    }
public:
    Q_PROPERTY(double value MEMBER Value)
public:
    double Value;
};

class QmlDoubleValuePreview : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
public:
    Q_PROPERTY(double value READ getValue WRITE setValue NOTIFY valueChanged)
public:
    Q_INVOKABLE void setValueDirect(double value)
    {
        if (m_value == value)
            return;

        m_value = value;
        emit valueChanged();
    }
public:
    inline double getValue() const { return m_value; }
    inline void setValue(double value)
    {
        if (m_value == value)
            return;

        QDoubleValueArg arg(value);
        emit valuePreview(&arg);
        if (m_value == arg.Value)
            return;

        m_value = arg.Value;
        emit valueChanged();
    }
signals:
    void valueChanged();
    void valuePreview(QDoubleValueArg *preview);
private:
    double m_value = 0;
};
