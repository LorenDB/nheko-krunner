/*
    SPDX-FileCopyrightText: 2022 Loren Burkholder <computersemiexpert@outlook.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "nheko-krunner.h"

// KF
#include <KLocalizedString>
#include <KConfigGroup>

#include <QDebug>
#include <QDBusInterface>
#include <QDBusReply>

auto logger()
{
    return qInfo().noquote() << "[nheko-krunner]";
}

NhekoKRunner::NhekoKRunner(QObject *parent, const KPluginMetaData &metadata, const QVariantList &args)
    : Plasma::AbstractRunner(parent, metadata, args)
{
    setObjectName(QStringLiteral("nheko"));
    setMinLetterCount(3);
    setPriority(Plasma::AbstractRunner::HighestPriority);

    nheko::dbus::init();

    connect(this, &Plasma::AbstractRunner::prepare, this, [this] {
        if (QDBusConnection::sessionBus().isConnected())
        {
            if (nheko::dbus::apiVersionIsCompatible(QVersionNumber::fromString(nheko::dbus::apiVersion())))
            {
                m_dbusConnected = true;
                m_rooms = nheko::dbus::rooms();

                if (m_rooms.isEmpty())
                    logger() << tr("Connected to nheko, but no rooms were provided.");
                else
                    logger() << tr("Retrieved %n room(s) from nheko.", nullptr, m_rooms.length());
                return;
            }
            else
                logger() << tr("Incompatible nheko API!");
        }

        m_dbusConnected = false;
        logger() << tr("Couldn't connect to nheko!");
    });
}

NhekoKRunner::~NhekoKRunner()
{
}

void NhekoKRunner::match(Plasma::RunnerContext &context)
{
    if (!m_dbusConnected || m_rooms.isEmpty())
        return;

    bool roomFound{false};
    const auto &input = context.query();
    for (const auto &room : std::as_const(m_rooms))
    {
        bool roomMatches{false};
        QStringView matchingContent;
        if (room.alias().contains(input, Qt::CaseInsensitive))
        {
            roomMatches = true;
            matchingContent = room.alias();
        }
        else if (room.roomId().contains(input, Qt::CaseInsensitive))
        {
            roomMatches = true;
            matchingContent = room.roomId();
        }
        else if (room.roomName().contains(input, Qt::CaseInsensitive))
        {
            roomMatches = true;
            matchingContent = room.roomName();
        }

        if (roomMatches)
        {
            auto text{room.roomName()};
            if (room.unreadNotifications() > 0)
                text.append(QStringLiteral(" (%1)").arg(room.unreadNotifications()));

            Plasma::QueryMatch match{this};
            match.setText(text);
            match.setSubtext(room.alias());
            match.setData(QVariant::fromValue(NhekoAction{.id{room.roomId()}, .actionType{ActionType::OpenRoom}}));
            match.setIcon(QIcon{QPixmap::fromImage(room.image())});
            match.setType(matchingContent.compare(input, Qt::CaseInsensitive) == 0 ? Plasma::QueryMatch::ExactMatch : Plasma::QueryMatch::PossibleMatch);
            context.addMatch(match);

            if (room.roomId() == input)
                roomFound = true;
        }

        if (!context.isValid())
            return;
    }

    if (!roomFound)
    {
        QRegularExpression roomRegex{QStringLiteral("#.+?:.{3,}"), QRegularExpression::CaseInsensitiveOption};
        QRegularExpression userRegex{QStringLiteral("@.+?:.{3,}"), QRegularExpression::CaseInsensitiveOption};
        
        if (auto regexMatch = roomRegex.match(input); regexMatch.hasMatch())
        {
            Plasma::QueryMatch match{this};
            match.setSubtext(tr("Join %1").arg(input));
            match.setText(input);
            match.setData(QVariant::fromValue(NhekoAction{.id{input}, .actionType{ActionType::JoinRoom}}));
            match.setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
            match.setType(Plasma::QueryMatch::ExactMatch);
            context.addMatch(match);

            logger() << tr("Input is a room alias.");
        }
        else if (auto regexMatch = userRegex.match(input); regexMatch.hasMatch())
        {
            Plasma::QueryMatch match{this};
            match.setSubtext(tr("Direct message %1").arg(input));
            match.setText(input);
            match.setData(QVariant::fromValue(NhekoAction{.id{input}, .actionType{ActionType::DirectMessage}}));
            match.setIcon(QIcon::fromTheme(QStringLiteral("user")));
            match.setType(Plasma::QueryMatch::ExactMatch);
            context.addMatch(match);

            logger() << tr("Input is a user ID.");
        }
    }
    else
        logger() << tr("Found %n matching room(s).", nullptr, context.matches().length());
}

void NhekoKRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match)
{
    Q_UNUSED(context);

    if (QDBusInterface interface{QStringLiteral(NHEKO_DBUS_SERVICE_NAME), QStringLiteral("/")}; interface.isValid())
    {
        auto data = match.data().value<NhekoAction>();

        switch (data.actionType)
        {
        case ActionType::OpenRoom:
            nheko::dbus::activateRoom(data.id);
            break;
        case ActionType::JoinRoom:
            nheko::dbus::joinRoom(data.id);
            break;
        case ActionType::DirectMessage:
            nheko::dbus::directChat(data.id);
            break;
        default:
            break;
        }
    }
}

void NhekoKRunner::reloadConfiguration()
{
    KConfigGroup conf = config();

    m_showNotificationCounts = conf.readEntry(QStringLiteral("showNotificationCounts"), true);
}

K_EXPORT_PLASMA_RUNNER_WITH_JSON(NhekoKRunner, "plasma-runner-nheko-krunner.json")

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER_WITH_JSON
#include "nheko-krunner.moc"
