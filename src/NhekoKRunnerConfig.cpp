#include "NhekoKRunnerConfig.h"

#include <KAboutData>
#include <KPluginFactory>

#include <QGridLayout>
#include <QLabel>

K_PLUGIN_FACTORY(NhekoKRunnerConfigFactory, registerPlugin<NhekoKRunnerConfig>();)

NhekoKRunnerConfig::NhekoKRunnerConfig(QWidget *parent, const QVariantList &args)
    : KCModule{parent, args}
{
    auto about = new KAboutData{QStringLiteral("nheko-krunner"),
                                QStringLiteral("nheko"),
                                QStringLiteral("0.1.0"),
                                tr("List rooms and perform other actions with nheko."),
                                KAboutLicense::GPL_V3};
    about->addAuthor(QStringLiteral("Loren Burkholder"), tr("Author"), QStringLiteral("computersemiexpert@outlook.com"));
    setAboutData(about);

    setButtons(Apply);

    auto layout = new QGridLayout{this};

    layout->addWidget(new QLabel{QStringLiteral("Nothing to see yet.")});
}

#include "NhekoKRunnerConfig.moc"
