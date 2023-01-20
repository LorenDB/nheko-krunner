add_definitions(-DTRANSLATION_DOMAIN=\"plasma_runner_org.kde.nheko-krunner\")

add_library(nheko-krunner MODULE)

target_sources(nheko-krunner PRIVATE
    nheko-krunner.cpp
    nheko-krunner.h
    NhekoDBusApi.cpp
    NhekoDBusApi.h
)

kcoreaddons_desktop_to_json(nheko-krunner plasma-runner-nheko-krunner.desktop)
target_link_libraries(nheko-krunner
    KF5::Runner
    Qt5::DBus
)

install(TARGETS nheko-krunner DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf5/krunner)
install(FILES plasma-runner-nheko-krunner.desktop DESTINATION ${KDE_INSTALL_KSERVICESDIR})

# This prevents deprecated API from being used
include(ECMDeprecationSettings)
ecm_set_disabled_deprecation_versions(
    QT 5.15.2
    KF ${KF5_MIN_VERSION}
)

kcoreaddons_add_plugin(kcm_nheko-krunner SOURCES NhekoKRunnerConfig.cpp INSTALL_NAMESPACE kf5/krunner/kcms)
target_link_libraries(kcm_nheko-krunner
    KF5::KCMUtils
    KF5::ConfigCore
    KF5::ConfigWidgets
)
