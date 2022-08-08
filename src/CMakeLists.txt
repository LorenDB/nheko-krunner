add_definitions(-DTRANSLATION_DOMAIN=\"plasma_runner_org.kde.nheko-krunner\")

add_library(nheko-krunner MODULE)
add_library(kcm_nheko-krunner MODULE)

target_sources(nheko-krunner PRIVATE
    nheko-krunner.cpp
    nheko-krunner.h
    NhekoDBusApi.cpp
    NhekoDBusApi.h
)

target_sources(kcm_nheko-krunner PRIVATE
    NhekoKRunnerConfig.cpp
    NhekoKRunnerConfig.h
)

kcoreaddons_desktop_to_json(nheko-krunner plasma-runner-nheko-krunner.desktop)
target_link_libraries(nheko-krunner
    KF5::Runner
    Qt5::DBus
)
target_link_libraries(kcm_nheko-krunner
    KF5::KCMUtils
)

install(TARGETS nheko-krunner DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf5/krunner)
install(TARGETS kcm_nheko-krunner DESTINATION ${KDE_INSTALL_PLUGINDIR})

install(FILES plasma-runner-nheko-krunner.desktop DESTINATION ${KDE_INSTALL_KSERVICESDIR})
install(FILES kcm_nheko-krunner.desktop DESTINATION ${KDE_INSTALL_KSERVICESDIR})
