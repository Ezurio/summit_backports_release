/** @file mlan_vndr.h
 *
 * @brief This file contains definitions,macros, enums for vendor commands.
 *
 * Copyright (C) 2025 Ezurio
 *
 * This software file (the "File") is distributed by Ezurio
 * under the terms of the GNU General Public License Version 2, June 1991
 * (the "License").  You may use, redistribute and/or modify this File in
 * accordance with the terms and conditions of the License, a copy of which
 * is available by writing to the Free Software Foundation, Inc.
 *
 * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE
 * ARE EXPRESSLY DISCLAIMED.  The License provides additional details about
 * this warranty disclaimer.
 */
 #ifndef _MLAN_VNDR_H_
 #define _MLAN_VNDR_H_
 
 #include "mlan_main.h"
 #include "mlan_fw.h"

mlan_status wlan_vndr_ioctl(pmlan_adapter pmadapter,pmlan_ioctl_req pioctl_req);
mlan_status wlan_cmd_vndr_mfg(pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd, t_u16 cmd_action, t_u32 cmd_oid, t_void *pdata_buf);
mlan_status wlan_ret_vndr_mfg_cmd(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf);
 #endif /* _MLAN_VNDR_H_ */