/// Copyright (c) 2024 Bartosz Lenart

#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include "assets_loader.h"
#include "logo.h"
#include "minus_button.h"
#include "plus_button.h"
#include "update_button.h"
#include "bee_button.h"
#include "bee_flying.h"
#include "honeycomb.h"
#include "map_button.h"
#include "learn_button.h"
#include "flower_a.h"
#include "flower_b.h"
#include "flower_c.h"
#include "flower_d.h"
#include "flower_e.h"
#include "flower_f.h"
#include "flower_g.h"
#include "flower_h.h"
#include "flower_i.h"
#include "flower_j.h"
#include "flower_k.h"
#include "flower_l.h"
#include "flower_m.h"
#include "flower_n.h"
#include "flower_o.h"
#include "flower_p.h"
#include "flower_q.h"
#include "flower_r.h"
#include "flower_s.h"
#include "flower_t.h"
#include "flower_u.h"
#include "flower_v.h"
#include "flower_w.h"
#include "flower_x.h"
#include "flower_y.h"
#include "flower_z.h"
#include "flower_0.h"
#include "flower_1.h"
#include "flower_2.h"
#include "flower_3.h"
#include "flower_4.h"
#include "flower_5.h"
#include "flower_6.h"
#include "flower_7.h"
#include "flower_8.h"
#include "flower_9.h"

#define printfError(str) printf("ERROR: %s \n", str)

Texture2D assetLoad(enum AssetTexture2D a)
{
    Image img = {0};
    switch (a) {
    case Logo:
        img.format = LOGO_FORMAT;
        img.height = LOGO_HEIGHT;
        img.width = LOGO_WIDTH;
        img.data = LOGO_DATA;
        break;
    case MinusButton:
        img.format = MINUS_BUTTON_FORMAT;
        img.height = MINUS_BUTTON_HEIGHT;
        img.width = MINUS_BUTTON_WIDTH;
        img.data = MINUS_BUTTON_DATA;
        break;
    case PlusButton:
        img.format = PLUS_BUTTON_FORMAT;
        img.height = PLUS_BUTTON_HEIGHT;
        img.width = PLUS_BUTTON_WIDTH;
        img.data = PLUS_BUTTON_DATA;
        break;
    case UpdateButton:
        img.format = UPDATE_BUTTON_FORMAT;
        img.height = UPDATE_BUTTON_HEIGHT;
        img.width = UPDATE_BUTTON_WIDTH;
        img.data = UPDATE_BUTTON_DATA;
        break;
    case BeeButton:
        img.format = BEE_BUTTON_FORMAT;
        img.height = BEE_BUTTON_HEIGHT;
        img.width = BEE_BUTTON_WIDTH;
        img.data = BEE_BUTTON_DATA;
        break;
    case BeeFlying: 
        img.format = BEE_FLYING_FORMAT;
        img.height = BEE_FLYING_HEIGHT;
        img.width = BEE_FLYING_WIDTH;
        img.data = BEE_FLYING_DATA;
        break;
    case Honeycomb:
        img.format = HONEYCOMB_FORMAT;
        img.height = HONEYCOMB_HEIGHT;
        img.width = HONEYCOMB_WIDTH;
        img.data = HONEYCOMB_DATA;
        break;
    case MapButton:
        img.format = MAP_BUTTON_FORMAT;
        img.height = MAP_BUTTON_HEIGHT;
        img.width = MAP_BUTTON_WIDTH;
        img.data = MAP_BUTTON_DATA;
        break;
    case LearnButton:
        img.format = LEARN_BUTTON_FORMAT;
        img.height = LEARN_BUTTON_HEIGHT;
        img.width = LEARN_BUTTON_WIDTH;
        img.data = LEARN_BUTTON_DATA;
        break;
    case FlowerA:
        img.format = FLOWER_A_FORMAT;
        img.height = FLOWER_A_HEIGHT;
        img.width = FLOWER_A_WIDTH;
        img.data = FLOWER_A_DATA;
        break;    
    case FlowerB:
        img.format = FLOWER_B_FORMAT;
        img.height = FLOWER_B_HEIGHT;
        img.width = FLOWER_B_WIDTH;
        img.data = FLOWER_B_DATA;
        break;    
    case FlowerC:
        img.format = FLOWER_C_FORMAT;
        img.height = FLOWER_C_HEIGHT;
        img.width = FLOWER_C_WIDTH;
        img.data = FLOWER_C_DATA;
        break;    
    case FlowerD:
        img.format = FLOWER_D_FORMAT;
        img.height = FLOWER_D_HEIGHT;
        img.width = FLOWER_D_WIDTH;
        img.data = FLOWER_D_DATA;
        break;    
    case FlowerE:
        img.format = FLOWER_E_FORMAT;
        img.height = FLOWER_E_HEIGHT;
        img.width = FLOWER_E_WIDTH;
        img.data = FLOWER_E_DATA;
        break;    
    case FlowerF:
        img.format = FLOWER_F_FORMAT;
        img.height = FLOWER_F_HEIGHT;
        img.width = FLOWER_F_WIDTH;
        img.data = FLOWER_F_DATA;
        break;    
    case FlowerG:
        img.format = FLOWER_G_FORMAT;
        img.height = FLOWER_G_HEIGHT;
        img.width = FLOWER_G_WIDTH;
        img.data = FLOWER_G_DATA;
        break;    
    case FlowerH:
        img.format = FLOWER_H_FORMAT;
        img.height = FLOWER_H_HEIGHT;
        img.width = FLOWER_H_WIDTH;
        img.data = FLOWER_H_DATA;
        break;    
    case FlowerI:
        img.format = FLOWER_I_FORMAT;
        img.height = FLOWER_I_HEIGHT;
        img.width = FLOWER_I_WIDTH;
        img.data = FLOWER_I_DATA;
        break;    
    case FlowerJ:
        img.format = FLOWER_J_FORMAT;
        img.height = FLOWER_J_HEIGHT;
        img.width = FLOWER_J_WIDTH;
        img.data = FLOWER_J_DATA;
        break;    
    case FlowerK:
        img.format = FLOWER_K_FORMAT;
        img.height = FLOWER_K_HEIGHT;
        img.width = FLOWER_K_WIDTH;
        img.data = FLOWER_K_DATA;
        break;    
    case FlowerL:
        img.format = FLOWER_L_FORMAT;
        img.height = FLOWER_L_HEIGHT;
        img.width = FLOWER_L_WIDTH;
        img.data = FLOWER_L_DATA;
        break;    
    case FlowerM:
        img.format = FLOWER_M_FORMAT;
        img.height = FLOWER_M_HEIGHT;
        img.width = FLOWER_M_WIDTH;
        img.data = FLOWER_M_DATA;
        break;    
    case FlowerN:
        img.format = FLOWER_N_FORMAT;
        img.height = FLOWER_N_HEIGHT;
        img.width = FLOWER_N_WIDTH;
        img.data = FLOWER_N_DATA;
        break;    
    case FlowerO:
        img.format = FLOWER_O_FORMAT;
        img.height = FLOWER_O_HEIGHT;
        img.width = FLOWER_O_WIDTH;
        img.data = FLOWER_O_DATA;
        break;    
    case FlowerP:
        img.format = FLOWER_P_FORMAT;
        img.height = FLOWER_P_HEIGHT;
        img.width = FLOWER_P_WIDTH;
        img.data = FLOWER_P_DATA;
        break;    
    case FlowerQ:
        img.format = FLOWER_Q_FORMAT;
        img.height = FLOWER_Q_HEIGHT;
        img.width = FLOWER_Q_WIDTH;
        img.data = FLOWER_Q_DATA;
        break;    
    case FlowerR:
        img.format = FLOWER_R_FORMAT;
        img.height = FLOWER_R_HEIGHT;
        img.width = FLOWER_R_WIDTH;
        img.data = FLOWER_R_DATA;
        break;    
    case FlowerS:
        img.format = FLOWER_S_FORMAT;
        img.height = FLOWER_S_HEIGHT;
        img.width = FLOWER_S_WIDTH;
        img.data = FLOWER_S_DATA;
        break;    
    case FlowerT:
        img.format = FLOWER_T_FORMAT;
        img.height = FLOWER_T_HEIGHT;
        img.width = FLOWER_T_WIDTH;
        img.data = FLOWER_T_DATA;
        break;    
    case FlowerU:
        img.format = FLOWER_U_FORMAT;
        img.height = FLOWER_U_HEIGHT;
        img.width = FLOWER_U_WIDTH;
        img.data = FLOWER_U_DATA;
        break;    
    case FlowerV:
        img.format = FLOWER_V_FORMAT;
        img.height = FLOWER_V_HEIGHT;
        img.width = FLOWER_V_WIDTH;
        img.data = FLOWER_V_DATA;
        break;    
    case FlowerW:
        img.format = FLOWER_W_FORMAT;
        img.height = FLOWER_W_HEIGHT;
        img.width = FLOWER_W_WIDTH;
        img.data = FLOWER_W_DATA;
        break;    
    case FlowerX:
        img.format = FLOWER_X_FORMAT;
        img.height = FLOWER_X_HEIGHT;
        img.width = FLOWER_X_WIDTH;
        img.data = FLOWER_X_DATA;
        break;    
    case FlowerY:
        img.format = FLOWER_Y_FORMAT;
        img.height = FLOWER_Y_HEIGHT;
        img.width = FLOWER_Y_WIDTH;
        img.data = FLOWER_Y_DATA;
        break;    
    case FlowerZ:
        img.format = FLOWER_Z_FORMAT;
        img.height = FLOWER_Z_HEIGHT;
        img.width = FLOWER_Z_WIDTH;
        img.data = FLOWER_Z_DATA;
        break;    
    case Flower0:
        img.format = FLOWER_0_FORMAT;
        img.height = FLOWER_0_HEIGHT;
        img.width = FLOWER_0_WIDTH;
        img.data = FLOWER_0_DATA;
        break;    
    case Flower1:
        img.format = FLOWER_1_FORMAT;
        img.height = FLOWER_1_HEIGHT;
        img.width = FLOWER_1_WIDTH;
        img.data = FLOWER_1_DATA;
        break;    
    case Flower2:
        img.format = FLOWER_2_FORMAT;
        img.height = FLOWER_2_HEIGHT;
        img.width = FLOWER_2_WIDTH;
        img.data = FLOWER_2_DATA;
        break;    
    case Flower3:
        img.format = FLOWER_3_FORMAT;
        img.height = FLOWER_3_HEIGHT;
        img.width = FLOWER_3_WIDTH;
        img.data = FLOWER_3_DATA;
        break;    
    case Flower4:
        img.format = FLOWER_4_FORMAT;
        img.height = FLOWER_4_HEIGHT;
        img.width = FLOWER_4_WIDTH;
        img.data = FLOWER_4_DATA;
        break;    
    case Flower5:
        img.format = FLOWER_5_FORMAT;
        img.height = FLOWER_5_HEIGHT;
        img.width = FLOWER_5_WIDTH;
        img.data = FLOWER_5_DATA;
        break;    
    case Flower6:
        img.format = FLOWER_6_FORMAT;
        img.height = FLOWER_6_HEIGHT;
        img.width = FLOWER_6_WIDTH;
        img.data = FLOWER_6_DATA;
        break;    
    case Flower7:
        img.format = FLOWER_7_FORMAT;
        img.height = FLOWER_7_HEIGHT;
        img.width = FLOWER_7_WIDTH;
        img.data = FLOWER_7_DATA;
        break;    
    case Flower8:
        img.format = FLOWER_8_FORMAT;
        img.height = FLOWER_8_HEIGHT;
        img.width = FLOWER_8_WIDTH;
        img.data = FLOWER_8_DATA;
        break;    
    case Flower9:
        img.format = FLOWER_9_FORMAT;
        img.height = FLOWER_9_HEIGHT;
        img.width = FLOWER_9_WIDTH;
        img.data = FLOWER_9_DATA;
        break;    
    default:
        printfError("unknown resource");
        exit(1);
    }

    img.mipmaps = 1;
    return LoadTextureFromImage(img);
}

