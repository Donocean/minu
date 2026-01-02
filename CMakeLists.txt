set(src src/minu.c
        src/minu_item.c
        src/minu_disp.c
        src/minu_viewer.c
        src/minu_base.c
        src/minu_anim.c
        src/minu_anim_easing.c
        utils/minu_mem.c
        utils/minu_vector.c
        src/items/minu_item_checkbox.c
        src/items/minu_item_submenu.c
        src/items/minu_item_variable.c
        src/items/minu_item_window.c
    )

set(inc inc
        port
        utils
        src/items
    )

if(DEFINED ENV{IDF_PATH})
    list(APPEND src port/minu_port_u8g2.c)

    idf_component_register(SRCS ${src}
                           INCLUDE_DIRS ${inc}
                           PRIV_REQUIRES u8g2
                           )
else()
    list(APPEND src port/minu_port_raylib.c)

    # 编译cpp文件到顶层cmake 执行文件中
    add_library(minu STATIC ${src})

    target_include_directories(minu
        PUBLIC
        ${inc}
    )
    target_compile_definitions(minu PRIVATE NO_ESP_IDF)

    # 如果需要 raylib（可选）
    target_link_libraries(minu PUBLIC raylib)

endif()
