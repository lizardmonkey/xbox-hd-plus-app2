#include <stdio.h>
#include <string.h>
#include <lvgl.h>

#include "lvgl-extra/ButtonGroup.h"
#include "lvgl-extra/WarpObject.h"

#include "App.h"
#include "scenes/InterpolationSettings.h"

static const char *optionScaler[] = { "Truncate", "Bilinear", "" };
static const char *optionWeight[] = { "Truncate", "Round", "" };

static void ButtonEventHandler(lv_obj_t * obj, lv_event_t event) {
    InterpolationSettings* scene = static_cast<InterpolationSettings *>(obj->user_data);
    scene->OnObjectEvent(obj, event);
}

InterpolationSettings::InterpolationSettings() {
    // Set the size and position of our scene
    lv_obj_set_size(cont, 320, 420);
    lv_obj_set_pos(screen, 380, 20);
    lv_cont_set_layout(cont, LV_LAYOUT_COLUMN_LEFT);

    //
    uint8_t value = 1;
    buttonMatrix[0] = new ButtonGroup(cont, group, "X Scale", optionScaler, &value);
    setButtonMtxStyles(buttonMatrix[0]->buttons);

    //
    buttonMatrix[1] = new ButtonGroup(cont, group, "Y Scale", optionScaler, &value);
    setButtonMtxStyles(buttonMatrix[1]->buttons);

    //
    buttonMatrix[2] = new ButtonGroup(cont, group, "X Weight", optionWeight, &value);
    setButtonMtxStyles(buttonMatrix[2]->buttons);

    //
    buttonMatrix[3] = new ButtonGroup(cont, group, "Y Weight", optionWeight, &value);
    setButtonMtxStyles(buttonMatrix[3]->buttons);

    // Register a callbacks
    lv_group_add_obj_warp(group, ButtonEventHandler, static_cast<lv_obj_user_data_t>(this), buttonMatrix[0]->buttons);
    lv_group_add_obj_warp(group, ButtonEventHandler, static_cast<lv_obj_user_data_t>(this), buttonMatrix[1]->buttons);
    lv_group_add_obj_warp(group, ButtonEventHandler, static_cast<lv_obj_user_data_t>(this), buttonMatrix[2]->buttons);
    lv_group_add_obj_warp(group, ButtonEventHandler, static_cast<lv_obj_user_data_t>(this), buttonMatrix[3]->buttons);

    // Draw back button in default location
    DrawBackButton();

    // Reset warp so that the newly focused object is set as current
    WarpObjectReset(group);
}

InterpolationSettings::~InterpolationSettings(void)
{
    lv_obj_del(screen);
}

void InterpolationSettings::OnObjectEvent(lv_obj_t* obj, lv_event_t event)
{
    // Preform warp check on event
    if(WarpObjectOnEvent(obj, event, group)) { return; }

    if(event == LV_EVENT_KEY)
    {
        uint32_t event_key = *(uint32_t *)lv_event_get_data();

        if(event_key == LV_KEY_ESC)
            load_scene = SCENE::ADVANCED_SETTINGS;
    }
}