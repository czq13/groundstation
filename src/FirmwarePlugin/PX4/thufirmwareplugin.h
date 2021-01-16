#ifndef THUFIREWAREPLUGIN_H
#define THUFIREWAREPLUGIN_H

#include "PX4FirmwarePlugin.h"

class THUFirmwarePlugin : public PX4FirmwarePlugin
{
public:
    THUFirmwarePlugin();
    QString             _internalParameterMetaDataFile  (Vehicle* vehicle) override {Q_UNUSED(vehicle);return QString(":/FirmwarePlugin/PX4/param-thu.xml");}
    AutoPilotPlugin*    autopilotPlugin                 (Vehicle* vehicle) override;
    bool                adjustIncomingMavlinkMessage    (Vehicle* vehicle, mavlink_message_t* message) override;
    QStringList         flightModes(Vehicle* vehicle) override;

    QString _hoverFixedPoint;
    QString _hoverHldAltitude;
    QString _hoverStabilize;
    QString _hoverMission;
    QString _hoverProgram;
    QString _hoverTransition;
    QString _levelHldAltitude;
    QString _levelStabilize;
    QString _levelMission;
    QString _levelProgram;
    QString _levelTransition;
    QString _transForwardS1;
    QString _transForwardS2;
    QString _transBackwardS1;
    QString _transBackwardS2;

};

#endif // THUFIREWAREPLUGIN_H
