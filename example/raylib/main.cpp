#include <iostream>
#include <thread>
#include <chrono>

#include <raylib.h>
#include <minu.h>
#include <cstdio>

const int screenWidth = 128 * 2;
const int screenHeight = 64 * 2;

bool check_test1;
bool check_test2;
bool should_stop;

int itest = 16;
float ftest = 3.14;
int inc_i = 0;

void var_int_cb(void *para, uint16_t e)
{
    int *var = (int *)para;
    switch (e)
    {
        case MINU_EVENT_UP:
            *var += 10;
            break;
        case MINU_EVENT_DOWN:
            *var -= 10;
            break;
    }
}

void anim_timer(void)
{
    int interval_ms = 1;
    auto next = std::chrono::steady_clock::now();
    while (!should_stop)
    {
        // 1ms timer callback
        minu_tick_inc(interval_ms);
        next += std::chrono::milliseconds(interval_ms);
        std::this_thread::sleep_until(next);
    }
}

int main(int argc, char *argv[])
{
    int size[2] = {screenWidth, screenHeight};
    minu_port_new_disp_raylib(size);
    minu_handle_t m0 =
        minu_create(MINU_TYPE_SELECTOR_VERTICAL, "main", 0, 0, screenWidth, screenHeight);
    minu_addCheckBoxItem(m0, "check1 ", &check_test1);
    minu_addCheckBoxItem(m0, "check2", &check_test2);
    /* TODO: 使用特殊符号显示不同变量类型 */
    minu_addVariableItem(m0, "int", &itest, "%d", NULL);
    minu_addVariableItem(m0, "float", &ftest, "%.2f", NULL);
    minu_addVariableItem(m0, "int increase", &inc_i, "%d", &var_int_cb);
    char c = 'H';
    minu_addVariableItem(m0, "char show", &c, "%c", NULL);
    const char *ss = "Donocean";
    minu_addVariableItem(m0, "char show", (void *)ss, "%s", NULL);

    minu_handle_t m1 = minu_create(MINU_TYPE_SELECTOR_VERTICAL,
                                   "submenu",
                                   0,
                                   0,
                                   screenWidth,
                                   screenHeight);
    minu_addCheckBoxItem(m1, "submenu check0", &check_test1);
    minu_addCheckBoxItem(m1, "submenu check1", &check_test1);
    minu_addCheckBoxItem(m1, "submenu check2", &check_test1);
    minu_addCheckBoxItem(m1, "submenu check3", &check_test1);
    minu_addCheckBoxItem(m1, "submenu check4", &check_test1);
    minu_addSubmenuItem(m0, "submenu", m1);

    minu_viewer_handle_t viewer = minu_viewer_create(m0);
    std::thread anim_thread(&anim_timer);
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_K))
            minu_viewer_event_post_to(viewer, MINU_EVENT_UP);
        if (IsKeyPressed(KEY_J))
            minu_viewer_event_post_to(viewer, MINU_EVENT_DOWN);
        if (IsKeyPressed(KEY_L))
            minu_viewer_event_post_to(viewer, MINU_EVENT_ENTER);
        if (IsKeyPressed(KEY_H))
            minu_viewer_event_post_to(viewer, MINU_EVENT_QUIT);
        if (IsKeyPressed(KEY_D))
            minu_viewer_event_post_to(viewer, MINU_EVENT_DELETE);
        if (IsKeyPressed(KEY_Q))
            break;
        minu_viewer_update(viewer);
    }

    should_stop = true;
    if (anim_thread.joinable())
        anim_thread.join();

    return 0;
}
