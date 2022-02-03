/*
    SPDX-FileCopyrightText: 2022 Loren Burkholder <computersemiexpert@outlook.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef NHEKOKRUNNER_H
#define NHEKOKRUNNER_H

#include <KRunner/AbstractRunner>

#include "NhekoDBusInterface.h"

enum ActionType
{
    OpenRoom,
    JoinRoom,
};

struct NhekoAction
{
    QString roomid;
    ActionType actionType;
};
Q_DECLARE_METATYPE(NhekoAction)

class NhekoKRunner : public Plasma::AbstractRunner
{
    Q_OBJECT

public:
    NhekoKRunner(QObject *parent, const KPluginMetaData &metadata, const QVariantList &args);
    ~NhekoKRunner() override;

public: // Plasma::AbstractRunner API
    void match(Plasma::RunnerContext &context) override;
    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) override;

private:
    QVector<nheko::dbus::RoomInfoItem> m_rooms;
    bool m_dbusConnected{true};
};

#endif
