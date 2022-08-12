#include "NhekoKRunnerConfig.h"

#include <KAboutData>
#include <KPluginFactory>
#include <KConfigGroup>
#include <KSharedConfig>

#include <QGridLayout>
#include <QLabel> // deleteme

NhekoKRunnerConfig::NhekoKRunnerConfig(QWidget *parent, const QVariantList &args)
    : KCModule{parent, args}
{
    auto about = new KAboutData{QStringLiteral("nheko-krunner"),
                                QStringLiteral("nheko"),
                                QStringLiteral("0.1.0"),
                                tr("List rooms and perform other actions with nheko."),
                                KAboutLicense::LGPL_V2_1};
    about->addAuthor(QStringLiteral("Loren Burkholder"), tr("Author"), QStringLiteral("computersemiexpert@outlook.com"));
    setAboutData(about);
    setButtons(Apply);

    auto layout = new QGridLayout{this};
    m_showNotificationCounts = new QCheckBox{tr("Display unread notification counts next to rooms")};
    layout->addWidget(m_showNotificationCounts);

    connect(m_showNotificationCounts, &QCheckBox::stateChanged, this, &NhekoKRunnerConfig::markAsChanged);
}

void NhekoKRunnerConfig::load()
{
    KCModule::load();

    auto config = KSharedConfig::openConfig(QStringLiteral("krunnerrc"))
            ->group(QStringLiteral("Runners"))
                      .group(QStringLiteral("nheko-krunner"));

    m_showNotificationCounts->setChecked(config.readEntry(QStringLiteral("showNotificationCounts"), true));

    Q_EMIT changed(false);
}

void NhekoKRunnerConfig::save()
{
    KCModule::save();

    auto config = KSharedConfig::openConfig(QStringLiteral("krunnerrc"))
            ->group(QStringLiteral("Runners"))
                      .group(QStringLiteral("nheko-krunner"));

    config.writeEntry(QStringLiteral("showNotificationCounts"), m_showNotificationCounts->isChecked());
    config.sync();

    Q_EMIT changed(false);
}

void NhekoKRunnerConfig::defaults()
{
    KCModule::defaults();

    m_showNotificationCounts->setChecked(true);

    markAsChanged();
}

K_PLUGIN_CLASS(NhekoKRunnerConfig)

#include "NhekoKRunnerConfig.moc"
