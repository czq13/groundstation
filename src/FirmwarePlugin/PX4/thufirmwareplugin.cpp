#include "thufirmwareplugin.h"
#include "FirmwarePlugin/PX4/px4_custom_mode.h"
#include "AutoPilotPlugins/Generic/GenericAutoPilotPlugin.h"

THUFirmwarePlugin::THUFirmwarePlugin() :
    _hoverFixedPoint("Hover:fixed point"),
    _hoverHldAltitude("Hover:hold altitude"),
    _hoverStabilize("Hover:stabilize"),
    _hoverMission("Hover:mission"),
    _hoverProgram("Hover:program"),
    _hoverTransition("Hover:trans to level mode"),
    _levelHldAltitude("Level:hold altitude"),
    _levelStabilize("Level:stabilize"),
    _levelMission("Level:mission"),
    _levelProgram("Level:program"),
    _levelTransition("Level:trans to hover mode"),
    _transForwardS1("Hover:transition s1"),
    _transForwardS2("Hover:transition s2"),
    _transBackwardS1("Level:transition s1"),
    _transBackwardS2("Level:transition s2")
{
    _flightModeInfoList.clear();
    struct Modes2Name {
        uint8_t     main_mode;
        uint8_t     sub_mode;
        bool        canBeSet;   ///< true: Vehicle can be set to this flight mode
        bool        fixedWing;  /// fixed wing compatible
        bool        multiRotor;  /// multi rotor compatible
    };

    static const struct Modes2Name rgModes2Name[] = {
        //main_mode                         sub_mode                                canBeSet  FW      MC
        { HOVER,                            HOVER_FIXED_POINT,                      true,   false,   true },
        { HOVER,                            HOVER_HLD_ALTITUDE,                     true,   false,   true },
        { HOVER,                            HOVER_MISSION,                          true,   false,   true },
        { HOVER,                            HOVER_PROGRAM,                          true,   false,   true },
        { HOVER,                            HOVER_STABILIZE,                        true,   false,   true },
        { HOVER,                            HOVER_TRANSITION,                       true,   false,   true },
        { LEVEL,                            LEVEL_HLD_ALTITUDE,                     true,   true,    false },
        { LEVEL,                            LEVEL_MISSION,                          true,   true,    false },
        { LEVEL,                            LEVEL_STABILIZE,                        true,   true,    false },
        { LEVEL,                            LEVEL_PROGRAM,                          true,   true,    false },
        { LEVEL,                            LEVEL_TRANSITION,                       true,   true,    false },
        { TRANSITION,                       TRANS_FORWARD_S1,                       true,   false,   false },
        { TRANSITION,                       TRANS_FORWARD_S2,                       true,   false,   false },
        { TRANSITION,                       TRANS_BACKWARD_S1,                      true,   false,   false  },
        { TRANSITION,                       TRANS_BACKWARD_S2,                      true,   false,   false },
    };

    // Must be in same order as above structure
    const QString* rgModeNames[] = {
        &_hoverFixedPoint,
        &_hoverHldAltitude,
        &_hoverMission,
        &_hoverProgram,
        &_hoverStabilize,
        &_hoverTransition,
        &_levelHldAltitude,
        &_levelMission,
        &_levelStabilize,
        &_levelProgram,
        &_levelTransition,
        &_transForwardS1,
        &_transForwardS2,
        &_transBackwardS1,
        &_transBackwardS2,
    };

    // Convert static information to dynamic list. This allows for plugin override class to manipulate list.
    for (size_t i=0; i<sizeof(rgModes2Name)/sizeof(rgModes2Name[0]); i++) {
        const struct Modes2Name* pModes2Name = &rgModes2Name[i];

        FlightModeInfo_t info;

        info.main_mode =    pModes2Name->main_mode;
        info.sub_mode =     pModes2Name->sub_mode;
        info.name =         rgModeNames[i];
        info.canBeSet =     pModes2Name->canBeSet;
        info.fixedWing =    pModes2Name->fixedWing;
        info.multiRotor =   pModes2Name->multiRotor;

        _flightModeInfoList.append(info);
    }
}

AutoPilotPlugin*    THUFirmwarePlugin::autopilotPlugin(Vehicle* vehicle) {
    return new GenericAutoPilotPlugin(vehicle,vehicle);
}
bool THUFirmwarePlugin::adjustIncomingMavlinkMessage(Vehicle* vehicle, mavlink_message_t* message) {
    return true;
}

QStringList THUFirmwarePlugin::flightModes(Vehicle* vehicle)
{
    QStringList flightModes;
    uint32_t cusmode = vehicle->customMode();
    uint8_t mainmode = (cusmode >> 16) &0x0FF;
    foreach (const FlightModeInfo_t& info, _flightModeInfoList) {
        if (info.canBeSet) {
            if (mainmode == info.main_mode) {
                flightModes += *info.name;
            }
        }
    }

    return flightModes;
}
