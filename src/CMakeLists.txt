add_definitions(-DTRANSLATION_DOMAIN=\"plasma_runner_org.kde.nheko-krunner\")

add_library(nheko-krunner MODULE)
add_library(nheko-krunner-kcm MODULE)

target_sources(nheko-krunner PRIVATE
    nheko-krunner.cpp
    nheko-krunner.h
    NhekoDBusApi.cpp
    NhekoDBusApi.h
)

target_sources(nheko-krunner-kcm PRIVATE
    NhekoKRunnerConfig.cpp
    NhekoKRunnerConfig.h
)

kcoreaddons_desktop_to_json(nheko-krunner plasma-runner-nheko-krunner.desktop)
target_link_libraries(nheko-krunner
    KF5::Runner
    Qt5::DBus
)
target_link_libraries(nheko-krunner-kcm
    KF5::KCMUtils
)

install(TARGETS nheko-krunner DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf5/krunner)

install(FILES plasma-runner-nheko-krunner.desktop DESTINATION ${KDE_INSTALL_KSERVICESDIR})
