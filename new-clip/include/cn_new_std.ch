#IfNDef cn_NEW_STD_CH

#define cn_NEW_STD_CH

#include "cn_command.ch"
#Include "cn_comment.ch"
#include "cn_set.ch"

#define  _DFSET(x, y)  Set( _SET_DATEFORMAT, iif(__SetCentury(), x, y) )

#translate millisec( <v1> )					=> sleep(<v1>/1000)

#include "cn_0_command.ch"
#include "cn_a_command.ch"
#include "cn_b_command.ch"
#include "cn_c_command.ch"
#include "cn_d_command.ch"
#include "cn_e_command.ch"
#include "cn_f_command.ch"
#include "cn_g_command.ch"
#include "cn_h_command.ch"
#include "cn_i_command.ch"
#include "cn_j_command.ch"
#include "cn_k_command.ch"
#include "cn_l_command.ch"
#include "cn_m_command.ch"
#include "cn_n_command.ch"
#include "cn_o_command.ch"
#include "cn_p_command.ch"
#include "cn_q_command.ch"
#include "cn_r_command.ch"
#include "cn_s_command.ch"
#include "cn_t_command.ch"
#include "cn_u_command.ch"
#include "cn_v_command.ch"
#include "cn_w_command.ch"
#include "cn_x_command.ch"
#include "cn_y_command.ch"
#include "cn_z_command.ch"
#endif