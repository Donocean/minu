set(src src/minu.c
        src/minu_disp.c
        src/minu_anim.c
        src/minu_viewer.c
        src/minu_anim_easing.c
        utils/minu_mem.c
        utils/minu_vector.c
        port/minu_port_u8g2.c
    )

set(inc inc
        port
        utils
    )

idf_component_register(SRCS ${src}
                       INCLUDE_DIRS ${inc}
                       PRIV_REQUIRES u8g2
                       )

