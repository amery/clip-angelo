#IfNDef DV_NEW_STD_CH

#define DV_NEW_STD_CH

#include "dv_command.ch"
#Include "dv_comment.ch"
#include "dv_set.ch"

#define  _DFSET(x, y)  Set( _SET_DATEFORMAT, iif(__SetCentury(), x, y) )

#translate millisec( <v1> )					=> sleep(<v1>/1000)

#include "dv_0_command.ch"
#include "dv_a_command.ch"
#include "dv_b_command.ch"
#include "dv_c_command.ch"
#include "dv_d_command.ch"
#include "dv_e_command.ch"
#include "dv_f_command.ch"
#include "dv_g_command.ch"
#include "dv_h_command.ch"
#include "dv_i_command.ch"
#include "dv_j_command.ch"
#include "dv_k_command.ch"
#include "dv_l_command.ch"
#include "dv_m_command.ch"
#include "dv_n_command.ch"
#include "dv_o_command.ch"
#include "dv_p_command.ch"
#include "dv_q_command.ch"
#include "dv_r_command.ch"
#include "dv_s_command.ch"
#include "dv_t_command.ch"
#include "dv_u_command.ch"
#include "dv_v_command.ch"
#include "dv_w_command.ch"
#include "dv_x_command.ch"
#include "dv_y_command.ch"
#include "dv_z_command.ch"
#endif