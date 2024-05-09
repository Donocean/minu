#ifndef __MINU_PORT_H_
#define __MINU_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Note: call these function after the specific UI library has been initialized! */
void minu_port_new_disp_u8g2(u8g2_t *obj, minu_monitor_t *moni);
void minu_port_new_disp_tft_espi(u8g2_t *obj, minu_monitor_t *moni);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_PORT_H_ */
