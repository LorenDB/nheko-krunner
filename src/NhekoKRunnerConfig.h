#ifndef NHEKOKRUNNERCONFIG_H
#define NHEKOKRUNNERCONFIG_H

#include <KCModule>

#include <QObject>
#include <QCheckBox>

class NhekoKRunnerConfig : public KCModule
{
    Q_OBJECT

public:
    NhekoKRunnerConfig(QWidget *parent = nullptr, const QVariantList &args = {});

    virtual void load() override;
    virtual void save() override;
    virtual void defaults() override;

private:
    QCheckBox *m_showNotificationCounts;
};

#endif // NHEKOKRUNNERCONFIG_H
