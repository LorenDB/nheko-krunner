/*
    SPDX-FileCopyrightText: 2022 Loren Burkholder <computersemiexpert@outlook.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "nheko-krunner.h"

// KF
#include <KLocalizedString>

#include <QDBusInterface>
#include <QDBusReply>

#define OPEN_ROOM QStringLiteral("nheko-krunner.action.open_room")
#define JOIN_ROOM QStringLiteral("nheko-krunner.action.join_room")

NhekoKRunner::NhekoKRunner(QObject *parent, const KPluginMetaData &metadata, const QVariantList &args)
    : Plasma::AbstractRunner(parent, metadata, args)
{
    setObjectName(QStringLiteral("nheko"));
    setMinLetterCount(3);
    setPriority(Plasma::AbstractRunner::HighestPriority);

    RoomInfoItem::init();

    connect(this, &Plasma::AbstractRunner::prepare, this, [this] {
        if (!QDBusConnection::sessionBus().isConnected())
        {
            m_dbusConnected = false;
            return;
        }

        if (QDBusInterface interface{QStringLiteral(NHEKO_DBUS_SERVICE_NAME), QStringLiteral("/")}; interface.isValid())
        {
            if (QDBusReply<QVector<RoomInfoItem>> reply = interface.call(QStringLiteral("getRooms")); reply.isValid())
            {
                m_rooms = reply.value();
                m_dbusConnected = true;
            }
            else
                m_dbusConnected = false;
        }
        else
            m_dbusConnected = false;
    });
}

NhekoKRunner::~NhekoKRunner()
{
}

void NhekoKRunner::match(Plasma::RunnerContext &context)
{
    if (!m_dbusConnected)
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
            Plasma::QueryMatch match{this};
            match.setSubtext(room.roomName());
            match.setText(room.alias());
            match.setData(QStringList{} << OPEN_ROOM << room.roomId());
            match.setIcon(room.icon());
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
        QRegularExpression r{QStringLiteral("#.+?:.{3,}"), QRegularExpression::CaseInsensitiveOption};
        if (auto regexMatch = r.match(input); regexMatch.hasMatch())
        {
            Plasma::QueryMatch match{this};
            match.setSubtext(tr("Join %1").arg(input));
            match.setText(input);
            match.setData(QStringList{} << JOIN_ROOM << input);
            match.setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
            match.setType(Plasma::QueryMatch::ExactMatch);
            context.addMatch(match);
        }
    }
}

void NhekoKRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match)
{
    Q_UNUSED(context);

    if (QDBusInterface interface{QStringLiteral(NHEKO_DBUS_SERVICE_NAME), QStringLiteral("/")}; interface.isValid())
    {
        QStringList args = match.data().toStringList();

        if (args[0] == OPEN_ROOM)
            interface.call(QStringLiteral("activateRoom"), args[1]);
        else if (args[0] == JOIN_ROOM)
            ;
    }
}

K_EXPORT_PLASMA_RUNNER_WITH_JSON(NhekoKRunner, "plasma-runner-nheko-krunner.json")

// needed for the QObject subclass declared as part of K_EXPORT_PLASMA_RUNNER_WITH_JSON
#include "nheko-krunner.moc"
