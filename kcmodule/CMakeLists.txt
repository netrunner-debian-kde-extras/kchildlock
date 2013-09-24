

set( kcm_kchildlock_SRCS 
     KchildlockKCM.cpp
     ConfConnect.cpp
     slots_general.cpp
     slots_usergroup.cpp
     UserGroupPageS.cpp
     GeneralPageS.cpp
     slots_usergroupapp.cpp
     FillGui.cpp
     ruler_scene.cpp
     usergroup_ui_functions.cpp
)

kde4_add_ui_files(kcm_kchildlock_SRCS 
    generalPage.ui
    usergroupPage.ui
    selectUserGroup.ui
    deleteUserGroup.ui
)

kde4_add_kcfg_files(kcm_kchildlock_SRCS 
    ../common/kchildlock_settings.kcfgc)

kde4_add_plugin(kcm_kchildlock ${kcm_kchildlock_SRCS})

target_link_libraries(kcm_kchildlock
     ${KDE4_KDECORE_LIBS}
     ${KDE4_KFILE_LIBS}
     ${KDE4_KDEUI_LIBRARY}
     ${KDE4_KIO_LIBS}
)

install( TARGETS kcm_kchildlock DESTINATION ${PLUGIN_INSTALL_DIR} )

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/kcm_kchildlock_kde.desktop ${CMAKE_CURRENT_BINARY_DIR}/kcm_kchildlock_kde.desktop)


########### install files ###############
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/kcm_kchildlock_kde.desktop DESTINATION ${SERVICES_INSTALL_DIR} 
    RENAME kcm_kchildlock.desktop ) 
