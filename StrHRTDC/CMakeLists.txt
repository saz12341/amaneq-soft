set(FW_NAME StrHRTDC)
set(LIB_NAME FwFuncs)

foreach(val IN ITEMS
    get_version_hrtdc;
    initialize;
    inject_error_mzn_sem;
    read_mzn_register;
    read_mzn_sem;
    read_mzn_xadc;
    read_mzn_scr;
    reconfig_mzn;
    reset_mzn_sem;
    set_tdcmask;
    show_mzn_laccp;
    show_mzn_mikumari;
    write_mzn_register;
    )

  set(MY_OUTPUT_NAME ${val})  
  set(MY_TARGET ${FW_NAME}_${val})
  add_executable(${MY_TARGET}
    ${MY_OUTPUT_NAME}_main.cc
    )

  if(${MYDEBUG})
    message(STATUS "MY_TARGET @ Common: " ${MY_TARGET})
  endif()

  target_compile_options(${MY_TARGET} PUBLIC
    ${my_cxx_options}
    )
  target_compile_features(${MY_TARGET} PUBLIC
    ${my_cxx_features}
    )
  target_include_directories(${MY_TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_NAME}
    )
  target_link_libraries(${MY_TARGET} PUBLIC
    HulCore;
    ${FW_NAME}${LIB_NAME};
    )

  set_target_properties(${MY_TARGET} PROPERTIES
    OUTPUT_NAME ${MY_OUTPUT_NAME}
    )

  install(TARGETS ${MY_TARGET}
    RUNTIME       DESTINATION ${CMAKE_INSTALL_PREFIX}/${FW_NAME}/bin
    )
endforeach()

add_subdirectory(FwFuncs)
