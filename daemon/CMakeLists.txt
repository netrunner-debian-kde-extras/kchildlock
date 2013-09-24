
set( kded_kchildlockdaemon_SRCS
    KchildlockDaemon.cpp
    user_session.cpp
    get_user_limits.cpp
    check_user_limits.cpp
    warn_logout.cpp
    warn_finishapp.cpp
    consumption_file.cpp
    systray_status.cpp
)

set(ksmserver_xml  org.kde.KSMServerInterface.xml)


kde4_add_ui_files(kded_kchildlockdaemon_SRCS 
    userInfoPage.ui
)

qt4_add_dbus_interface(kded_kchildlockdaemon_SRCS ${ksmserver_xml} ksmserver_interface )

kde4_add_kcfg_files(kded_kchildlockdaemon_SRCS
    ../common/kchildlock_settings.kcfgc)

kde4_add_plugin( kded_kchildlockdaemon 
    ${kded_kchildlockdaemon_SRCS} 
)

target_link_libraries(kded_kchildlockdaemon 
    ${KDE4_KDECORE_LIBS} 
    ${KDE4_KDEUI_LIBS}
)

install( TARGETS kded_kchildlockdaemon DESTINATION ${PLUGIN_INSTALL_DIR} )

install( FILES kchildlockdaemon.desktop DESTINATION ${SERVICES_INSTALL_DIR}/kded )

file (MAKE_DIRECTORY ../var/opt/kchildlock)
#install (DIRECTORY ../var/opt/kchildlock
#        DESTINATION ${CMAKE_INSTALL_PREFIX}/../var/opt/kchildlock PATTERN "dummyfile" 
#        PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_WRITE GROUP_READ GROUP_EXECUTE 
#        WORLD_WRITE WORLD_READ WORLD_EXECUTE)
install( FILES dummy.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/../var/opt/kchildlock )
