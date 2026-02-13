/** @file moal_vndr.c
 *
 * @brief This file contains the functions for vendor commands
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
#include "moal_vndr.h"
#include "moal_cfg80211.h"
#include "mlan_ioctl.h"

enum SONA_MFG_ATTRS {
	SONA_MFG_ATTR_CMD_RSP = 1,
	SONA_MFG_ATTR_DATA,
	SONA_MFG_ATTR_MAX
};

enum SONA_REG_ATTRS {
	SONA_REG_ATTR_RSP_CMD = 1,
	SONA_REG_ATTR_RSP_DATA
};

typedef MLAN_PACK_START struct _sona_user_data {
	t_u8   ver;
	t_u8   mac;
	t_u8   type;
	t_u16  flags;
	t_u8   reserved[3];
} MLAN_PACK_END sona_user_data;

typedef MLAN_PACK_START struct _sona_rx_data {
	t_u32   total;
	t_u32   bcast;
	t_u32   err;
} MLAN_PACK_END sona_rx_data;

/**
 * @brief vendor function to retrieve user data from radio's OTP
 *
 * @param priv     a pointer to moal_private struct
 * @param data     a pointer to data
 * @param len      data length
 *
 * @return      length >= 0: otherwise failure
 */
static int woal_vndr_get_user_data(moal_private *priv, void* respbuf, t_u32 respbuflen)
{
	mlan_ioctl_req *req = NULL;
	mlan_ds_misc_cfg *misc = NULL;
	mlan_ds_misc_otp_user_data *otp = NULL;
	mlan_status status = MLAN_STATUS_SUCCESS;
	int ret = -ENOSYS;

	ENTER();

	req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
	if (req == NULL) {
		ret = -ENOMEM;
		goto done;
	}
	req->action = MLAN_ACT_GET;
	req->req_id = MLAN_IOCTL_MISC_CFG;

	misc = (mlan_ds_misc_cfg *)req->pbuf;
	misc->sub_command = MLAN_OID_MISC_OTP_USER_DATA;
	misc->param.otp_user_data.user_data_length = respbuflen;

	status = woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT);
	if (status != MLAN_STATUS_SUCCESS) {
		ret = -EFAULT;
		goto done;
	}
	otp = (mlan_ds_misc_otp_user_data *)req->pbuf;

	if (req->action == MLAN_ACT_GET) {
		ret = MIN(otp->user_data_length, respbuflen);
		moal_memcpy_ext(priv->phandle, respbuf, otp->user_data, ret, respbuflen);
	}

done:
	if (status != MLAN_STATUS_PENDING)
		kfree(req);
	LEAVE();
	return ret;
}

/**
 * @brief vendor function to which returns the radio format
 *
 * @param moal_handle   a handle to the moal_handle structure
 *
 * @return   0 success, otherwise failure
 */
int woal_vndr_get_radio_form(moal_handle *handle, attr_rid_form *form)
{
	sona_user_data udata ;
	moal_private *priv;
	int ret = MLAN_STATUS_SUCCESS;
	int len = 0;

	ENTER();

	if (!IS_SD9177(handle->card_type)) {
		ret = -ENODEV;
		goto done;
	}

	priv = woal_get_priv(handle, MLAN_BSS_ROLE_ANY);
	if (!priv) {
		ret = -EFAULT;
		goto done;
	}

	len = woal_vndr_get_user_data(priv, &udata, sizeof(udata));
	if (len >= sizeof(sona_user_data) && (udata.ver >=2)) {
		switch (udata.type) {
			case 1:
			case 2:
				*form = RID_ATTR_FRM_SIP;
			break;
			case 3:
			case 4:
				*form = RID_ATTR_FRM_1216;
			break;
			case 5:
				*form = RID_ATTR_FRM_1218;
			break;
			default:
				ret = -EINVAL;
				goto done;
			break;
		}
	}
	else {
		ret =  -EINVAL;
		goto done;
	}

done:
	LEAVE();
	return ret;
}

/**
 * @brief vendor command to identify radio type
 *
 * @param wiphy    A pointer to wiphy struct
 * @param wdev     A pointer to wireless_dev struct
 * @param data     a pointer to data
 * @param len      data length
 *
 * @return      0: success, otherwise failure
 */
int woal_vndr_radio_id(struct wiphy *wiphy, struct wireless_dev *wdev,
			       const void *data, int data_len)
{
	struct net_device *dev = wdev->netdev;
	moal_private *priv = (moal_private *)woal_get_netdev_priv(dev);
	struct sk_buff *skb  = NULL;
	attr_rid_form form;
	int ret = -ENOSYS;

	ENTER();

	if (!IS_SD9177(priv->phandle->card_type)) {
		ret = -ENODEV;
		goto done;
	}

	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, (sizeof(struct nlattr) + sizeof(u32)) * 4 );
	if (unlikely(!skb)) {
		PRINTM(MERROR, "skb alloc failed\n");
		ret = -ENOMEM;
		goto done;
	}

	nla_put_u32(skb, ATTR_RID_FAMILY, RID_ATTR_FAMILY_NXP);
	nla_put_u32(skb, ATTR_RID_TYPE, RID_ATTR_TYPE_611 );
	nla_put_u32(skb, ATTR_RID_WIFI_BUS, RID_ATTR_BUS_SDIO);

	if (MLAN_STATUS_SUCCESS == woal_vndr_get_radio_form(priv->phandle, &form)) {
		nla_put_u32(skb, ATTR_RID_FORM, form);
	}

	ret = cfg80211_vendor_cmd_reply(skb);
	if (unlikely(ret))
		PRINTM(MERROR, "Vendor Command reply failed ret:%d\n", ret);

done:
	LEAVE();
	return ret;
}

/**
 * @brief vendor command to start mfg processes
 *
 * @param wiphy    A pointer to wiphy struct
 * @param wdev     A pointer to wireless_dev struct
 * @param data     a pointer to data
 * @param len      data length
 *
 * @return      0: success, otherwise failure
 */
int woal_vndr_mfg_start(struct wiphy *wiphy, struct wireless_dev *wdev,
			       const void *data, int data_len)
{
	struct net_device *dev = wdev->netdev;
	moal_private *priv = (moal_private *)woal_get_netdev_priv(dev);
	mlan_ioctl_req *req = NULL;
	mlan_vndr_cmd *vndr_cmd = NULL;
	mlan_status status = MLAN_STATUS_SUCCESS;
	struct sk_buff  *skb  = NULL;
	int ret = -ENOSYS;

	ENTER();

	if (!IS_SD9177(priv->phandle->card_type)) {
		ret = -ENODEV;
		goto done;
	}

	req = woal_alloc_mlan_ioctl_req(sizeof(mlan_vndr_cmd));
	if (req == NULL) {
		ret = -ENOMEM;
		goto done;
	}

	vndr_cmd = (mlan_vndr_cmd *)req->pbuf;
	req->req_id = MLAN_IOCTL_VNDR;
	req->action = MLAN_ACT_SET;
	vndr_cmd->sub_command = MLAN_OID_VNDR_MFG_START;

	status = woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT);
	if (status != MLAN_STATUS_SUCCESS) {
		ret = -EFAULT;
		goto done;
	}

	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, sizeof(struct nlattr)*2 + sizeof(u32) + sizeof(mlan_vndr_mfg_start));

	if (unlikely(!skb)) {
		PRINTM(MERROR, "skb alloc failed\n");
		ret = -ENOMEM;
		goto done;
	}

	nla_put_u32(skb, SONA_MFG_ATTR_CMD_RSP, req->status_code);
	nla_put(skb, SONA_MFG_ATTR_DATA, sizeof(vndr_cmd->param.mfg_start.data), vndr_cmd->param.mfg_start.data);

	ret = cfg80211_vendor_cmd_reply(skb);
	if (unlikely(ret))
		PRINTM(MERROR, "Vendor Command reply failed ret:%d\n", ret);

done:
	if (status != MLAN_STATUS_PENDING)
		kfree(req);

	LEAVE();
	return ret;
}

/**
 * @brief vendor command to write mfg data
 *
 * @param wiphy    A pointer to wiphy struct
 * @param wdev     A pointer to wireless_dev struct
 * @param data     a pointer to data
 * @param len      data length
 *
 * @return      0: success, otherwise failure
 */
int woal_vndr_mfg_write(struct wiphy *wiphy, struct wireless_dev *wdev,
			       const void *data, int data_len)
{
	struct net_device *dev = wdev->netdev;
	moal_private *priv = (moal_private *)woal_get_netdev_priv(dev);
	mlan_ioctl_req *req = NULL;
	mlan_vndr_cmd *vndr_cmd = NULL;
	mlan_status status = MLAN_STATUS_SUCCESS;
	struct sk_buff  *skb  = NULL;
	int ret = -ENOSYS;

	ENTER();

	if (!IS_SD9177(priv->phandle->card_type)) {
		ret = -ENODEV;
		goto done;
	}

	req = woal_alloc_mlan_ioctl_req(sizeof(mlan_vndr_cmd) + data_len);
	if (req == NULL) {
		ret = -ENOMEM;
		goto done;
	}

	vndr_cmd = (mlan_vndr_cmd *)req->pbuf;
	req->req_id = MLAN_IOCTL_VNDR;
	req->action = MLAN_ACT_SET;
	vndr_cmd->sub_command = MLAN_OID_VNDR_MFG_WRITE;

	if (data && data_len) {
		vndr_cmd->param.mfg_write.data_len = data_len;
		moal_memcpy_ext(priv->phandle, vndr_cmd->param.mfg_write.data, data, data_len, data_len);
	}

	status = woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT);
	if (status != MLAN_STATUS_SUCCESS) {
		ret = -EFAULT;
		goto done;
	}

	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, sizeof(struct nlattr) + sizeof(u32));

	if (unlikely(!skb)) {
		PRINTM(MERROR, "skb alloc failed\n");
		ret = -ENOMEM;
		goto done;
	}

	nla_put_u32(skb, SONA_MFG_ATTR_CMD_RSP, req->status_code);

	ret = cfg80211_vendor_cmd_reply(skb);
	if (unlikely(ret))
		PRINTM(MERROR, "Vendor Command reply failed ret:%d\n", ret);

done:
	if (status != MLAN_STATUS_PENDING)
		kfree(req);

	LEAVE();
	return ret;
}

/**
 * @brief vendor command to end mfg processes
 *
 * @param wiphy    A pointer to wiphy struct
 * @param wdev     A pointer to wireless_dev struct
 * @param data     a pointer to data
 * @param len      data length
 *
 * @return      0: success, otherwise failure
 */
int woal_vndr_mfg_stop(struct wiphy *wiphy, struct wireless_dev *wdev,
			       const void *data, int data_len)
{
	struct net_device *dev = wdev->netdev;
	moal_private *priv = (moal_private *)woal_get_netdev_priv(dev);
	mlan_ioctl_req *req = NULL;
	mlan_vndr_cmd *vndr_cmd = NULL;
	mlan_status status = MLAN_STATUS_SUCCESS;
	struct sk_buff  *skb  = NULL;
	int ret = -ENOSYS;

	ENTER();

	if (!IS_SD9177(priv->phandle->card_type)) {
		ret = -ENODEV;
		goto done;
	}

	req = woal_alloc_mlan_ioctl_req(sizeof(mlan_vndr_cmd));
	if (req == NULL) {
		ret = -ENOMEM;
		goto done;
	}

	vndr_cmd = (mlan_vndr_cmd *)req->pbuf;
	req->req_id = MLAN_IOCTL_VNDR;
	req->action = MLAN_ACT_SET;
	vndr_cmd->sub_command = MLAN_OID_VNDR_MFG_STOP;

	status = woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT);
	if (status != MLAN_STATUS_SUCCESS) {
		ret = -EFAULT;
		goto done;
	}

	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, sizeof(struct nlattr) + sizeof(u32));

	if (unlikely(!skb)) {
		PRINTM(MERROR, "skb alloc failed\n");
		ret = -ENOMEM;
		goto done;
	}

	nla_put_u32(skb, SONA_MFG_ATTR_CMD_RSP, req->status_code);

	ret = cfg80211_vendor_cmd_reply(skb);
	if (unlikely(ret))
		PRINTM(MERROR, "Vendor Command reply failed ret:%d\n", ret);

done:
	if (status != MLAN_STATUS_PENDING)
		kfree(req);

	LEAVE();
	return ret;
}


//This define value is based on longest string length expected, TRIGGER_FRAME_STR_LEN,  and its command length, 'trigger_frame='
#define MAX_RF_TEST_CMD_LEN 14 + 250

/**
 * @brief vendor regulatory helper function to configure basic
 *        regulatory test parameters
 *
 * @param moal_handle   A handle to the moal_handle structure
 * @param channel       The channel to be used for test 
 * @param bw            The channel bandwidth
 * @param pwr           The power to be used for test, in dBm
 * @param rate          The data rate to be used for test 
 *
 * @return      0: success, otherwise failure
 */
static int woal_vndr_reg_cfg_basics(moal_handle *handle, t_u32 channel, t_u32 bw, t_s32 pwr, t_u32 rate)
{
	mlan_status status = MLAN_STATUS_SUCCESS;
	char databuf[MAX_RF_TEST_CMD_LEN];
	uint32_t val = 0;
	int mod = 0;
	int ret = 0;

	ENTER();

	val = (channel > 14)?3:11;
	snprintf(databuf,sizeof(databuf),"%s=%d 0\n","radio_mode",val);
	status = woal_process_rf_test_mode_cmd(handle, MFG_CMD_RADIO_MODE_CFG, databuf, strnlen(databuf, MAX_RF_TEST_CMD_LEN), MLAN_ACT_SET,val);
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		PRINTM(MERROR, "Failed to set MFG_CMD_RADIO_MODE_CFG %d\n", status);
		ret = -EFAULT;
		goto done;
	}

	val = (channel > 14)?1:0;
	status = woal_process_rf_test_mode_cmd(handle, MFG_CMD_RF_BAND_AG, NULL, 0, MLAN_ACT_SET, val);	
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		PRINTM(MERROR, "Failed to set MFG_CMD_RF_BAND_AG %d\n", status);
		ret = -EFAULT;
		goto done;
	}

	status = woal_process_rf_test_mode_cmd(handle, MFG_CMD_TX_ANT, NULL, 0, MLAN_ACT_SET, 1);
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		PRINTM(MERROR, "Failed to set MFG_CMD_TX_ANT %d\n", status);
		ret = -EFAULT;
		goto done;
	}

	status = woal_process_rf_test_mode_cmd(handle, MFG_CMD_RX_ANT, NULL, 0, MLAN_ACT_SET, 1);
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		PRINTM(MERROR, "Failed to set MFG_CMD_RX_ANT %d\n", status);
		ret = -EFAULT;
		goto done;
	}

	val = (bw == REG_ATTR_BW_80)?4:bw;
	status = woal_process_rf_test_mode_cmd(handle, MFG_CMD_RF_CHANNELBW, NULL, 0, MLAN_ACT_SET, val);
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		PRINTM(MERROR, "Failed to set MFG_CMD_RF_CHANNELBW %d\n", status);
		ret = -EFAULT;
		goto done;
	}

	status = woal_process_rf_test_mode_cmd(handle, MFG_CMD_RF_CHAN, NULL, 0, MLAN_ACT_SET, channel);
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		PRINTM(MERROR, "Failed to set MFG_CMD_RF_CHAN %d\n", status);
		ret = -EFAULT;
		goto done;
	}

	//Determine modulation
	if (rate <=3 ) {
		//CCK
		mod = 0;
	}
	else if (rate < 12) {
		//OFDM
		mod = 1;
	}
	else {
		//MCS
		mod = 2;
	}

	snprintf(databuf,sizeof(databuf),"%s=%d %d 0\n", "tx_power", pwr, mod);
	status = woal_process_rf_test_mode_cmd(handle, MFG_CMD_RFPWR, databuf, strnlen(databuf, MAX_RF_TEST_CMD_LEN), MLAN_ACT_SET, 0);
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		PRINTM(MERROR, "Failed to set MFG_CMD_RFPWR %d\n", status);
		ret = -EFAULT;
		goto done;
	}

done:
	LEAVE();
	return ret;
}

/**
 * @brief vendor regulatory helper function to configure continous tx command
 *
 * @param moal_handle  A handle to the moal_handle structure
 * @param enable       True if request is to enable, otherwise disable
 * @param cw           True if request transmit mode is for cw, otherwise pkt 
 * @param rate         The data rate to be used for test 
 *
 * @return      0: success, otherwise failure
 */
static int woal_vndr_reg_cfg_txc(moal_handle *handle, BOOLEAN enable, BOOLEAN cw, t_u32 rate)
{
	mlan_status status = MLAN_STATUS_SUCCESS;
	char databuf[MAX_RF_TEST_CMD_LEN];
	int ret = 0;

	ENTER();

	//Note:  If request is to disable txc, everything beyond TX mode is a don't care.
	snprintf(databuf,sizeof(databuf),"%s=%d %d 0xAAAA 0 3 %d\n","tx_continuous", enable==MTRUE?1:0, cw==MTRUE?1:0, rate);	
	status = woal_process_rf_test_mode_cmd(handle, MFG_CMD_TX_CONT, databuf, strnlen(databuf, MAX_RF_TEST_CMD_LEN), MLAN_ACT_SET, 0);
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		PRINTM(MERROR, "Failed to set MFG_CMD_TX_CONT %d\n", status);
		ret = -EFAULT;
		goto done;
	}

done:
	LEAVE();
	return ret;
}

/**
 * @brief vendor regulatory helper function to configure trigger frame parameters
 *
 * @param moal_handle  A handle to the moal_handle structure
 * @param enable       True if request is to enable transmit, otherwise disable
 * @param hetb         The trigger base type
 * @param bw           The bw to be used for test 
 * @param rate         The data rate to be used for test
 * @param idx          The RU Index
 *
 * @return      0: success, otherwise failure
 */
static int woal_vndr_reg_cfg_trg(moal_handle *handle, BOOLEAN enable, t_u32 hetb, t_u32 bw, t_u32 rate, t_u32 idx)
{
	mlan_status status = MLAN_STATUS_SUCCESS;
	char databuf[MAX_RF_TEST_CMD_LEN];
	int ret = 0;

	ENTER();

	//Note:  If request is to disable, everything beyond the enable is a don't care.
	snprintf(databuf,sizeof(databuf),"%s=%d %d 1 2 5484 0 1000 0 0 %d 1 0 0 0 1 0 1 0 65535 0 511 5 0 %d 1 %d 0 0 90 0 0 0 0\n","trigger_frame",  enable==MTRUE?1:0, hetb, 
			        bw, idx, rate);	
	status = woal_process_rf_test_mode_cmd(handle, MFG_CMD_CONFIG_TRIGGER_FRAME, databuf, strnlen(databuf, MAX_RF_TEST_CMD_LEN), MLAN_ACT_SET, 0);
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		ret = -EFAULT;
		goto done;
	}

done:
	LEAVE();
	return ret;
}

#define RATE_HE_SS1_MCS10 0x210a

/**
 * @brief vendor regulatory helper function to configure TX frame parameters
 *
 * @param moal_handle  A handle to the moal_handle structure
 * @param enable       True if request is to enable transmit, otherwise disable
 * @param bw           The channel bandwidth
 * @param rate         The data rate to be used for test
 *
 * @return      0: success, otherwise failure
 */
static int woal_vndr_reg_cfg_tx_frame(moal_handle *handle, BOOLEAN enable, t_u32 bw, t_u32 rate)
{
	mlan_status status = MLAN_STATUS_SUCCESS;
	char databuf[MAX_RF_TEST_CMD_LEN];
	int ret  = 0;
	int code = 1;

	ENTER();

	if (bw == REG_ATTR_BW_20) {
		if (rate < RATE_HE_SS1_MCS10 ) {
			code = 0;
		}
	}

	//Note:  If request is to disable, everything beyond the enable is a don't care.
	snprintf(databuf,sizeof(databuf),"%s=%d %d 0xabababab 0x256 0 20 0 0 0 %d 0 0 0 -1 -1 -1 -1 -1 -1 -1 -1 de:ad:00:00:be:ef\n", "tx_frame", enable, rate, code);
	status = woal_process_rf_test_mode_cmd(handle, MFG_CMD_TX_FRAME, databuf, strnlen(databuf, MAX_RF_TEST_CMD_LEN), MLAN_ACT_SET, 0);
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		ret = -EFAULT;
		goto done;
	}

done:
	LEAVE();
	return ret;
}

/**
 * @brief vendor regulatory helper function to reset per
 *
 * @param moal_handle  A handle to the moal_handle structure
 * @param sona_rx_data A pointer to structure to return rx counts
 *
 * @return      0: success, otherwise failure
 */
static int woal_vndr_reg_cfg_rx(moal_handle *handle)
{
	mlan_status status = MLAN_STATUS_SUCCESS;
	int ret  = 0;

	ENTER();

	status = woal_process_rf_test_mode_cmd(handle, MFG_CMD_CLR_RX_ERR, NULL, 0, MLAN_ACT_SET, 0);
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		ret = -EFAULT;
		goto done;
	}

done:
	LEAVE();
	return ret;
}

/**
 * @brief vendor command to enable/disable test mode
 *
 * @param wiphy    A pointer to wiphy struct
 * @param wdev     A pointer to wireless_dev struct
 * @param data     a pointer to data
 * @param len      data length
 *
 * @return      0: success, otherwise failure
 */
int woal_vndr_reg_tst(struct wiphy *wiphy, struct wireless_dev *wdev,
			       const void *data, int data_len)
{
	moal_private *priv  = (moal_private *)woal_get_netdev_priv(wdev->netdev);
	moal_handle *handle = priv->phandle;
	struct sk_buff *skb = NULL;
	mlan_status status  = MLAN_STATUS_SUCCESS;
	struct nlattr *tb[ATTR_REG_MAX + 1];
	int ret = -ENOSYS;

	ENTER();

	if (!IS_SD9177(handle->card_type)) {
		ret = -ENODEV;
		goto err;
	}

	ret = nla_parse(tb, ATTR_REG_MAX, data, data_len, NULL,NULL);
	if (ret) {
		PRINTM(MERROR, "nla parse failed\n");
		goto err;
	}

	if (!tb[ATTR_REG_ENABLE]) {
		ret = -EINVAL;
		goto err;
	}

	status = woal_process_rf_test_mode(handle, nla_get_u32(tb[ATTR_REG_ENABLE]));
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		ret = -EFAULT;
		goto err;
	}

	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, sizeof(struct nlattr) + sizeof(u32));
	if (unlikely(!skb)) {
		PRINTM(MERROR, "skb alloc failed\n");
		ret = -ENOMEM;
		goto err;
	}

	nla_put_u32(skb, SONA_REG_ATTR_RSP_CMD, 0);

	ret = cfg80211_vendor_cmd_reply(skb);
	if (unlikely(ret))
		PRINTM(MERROR, "Vendor Command reply failed ret:%d\n", ret);

err:
	LEAVE();
	return ret;
}

/**
 * @brief vendor command to run continous tx test
 *
 * @param wiphy    A pointer to wiphy struct
 * @param wdev     A pointer to wireless_dev struct
 * @param data     a pointer to data
 * @param len      data length
 *
 * @return      0: success, otherwise failure
 */
int woal_vndr_reg_txc(struct wiphy *wiphy, struct wireless_dev *wdev,
			       const void *data, int data_len)
{
	moal_private *priv  = (moal_private *)woal_get_netdev_priv(wdev->netdev);
	moal_handle *handle = priv->phandle;
	struct sk_buff *skb = NULL;
	mlan_status status  = MLAN_STATUS_SUCCESS;
	struct nlattr *tb[ATTR_REG_MAX + 1];
	int ret = -ENOSYS;

	ENTER();

	if (!IS_SD9177(handle->card_type)) {
		ret = -ENODEV;
		goto err;
	}

	ret = nla_parse(tb, ATTR_REG_MAX, data, data_len, NULL,NULL);
	if (ret) {
		PRINTM(MERROR, "nla parse failed\n");
		goto err;
	}

	if ( !tb[ATTR_REG_ENABLE] ) {
		ret = -EINVAL;
		goto err;
	}

	if ( 0 == nla_get_u32(tb[ATTR_REG_ENABLE]) ) {
		//Disable test
		status = woal_vndr_reg_cfg_txc(handle, MFALSE, MFALSE, 0);
		if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
			ret = -EFAULT;
			goto err;
		}
		goto done;
	}

	if ( !(tb[ATTR_REG_CHANNEL] && tb[ATTR_REG_BW] &&
	 	   tb[ATTR_REG_RATE] && tb[ATTR_REG_PWR] ) ) {
		ret = -EINVAL;
		goto err;
	}

	status = woal_vndr_reg_cfg_basics(handle, 
			    nla_get_u32(tb[ATTR_REG_CHANNEL]), 
			    nla_get_u32(tb[ATTR_REG_BW]),
			    nla_get_s32(tb[ATTR_REG_PWR]),
			    nla_get_u32(tb[ATTR_REG_RATE]));
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		ret = -EFAULT;
		goto err;
	}

	status = woal_vndr_reg_cfg_txc(handle, MTRUE, MFALSE, nla_get_u32(tb[ATTR_REG_RATE]));
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		ret = -EFAULT;
		goto err;
	}

done:
	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, sizeof(struct nlattr) + sizeof(u32));
	if (unlikely(!skb)) {
		PRINTM(MERROR, "skb alloc failed\n");
		ret = -ENOMEM;
		goto err;
	}

	nla_put_u32(skb, SONA_REG_ATTR_RSP_CMD, 0);

	ret = cfg80211_vendor_cmd_reply(skb);
	if (unlikely(ret))
		PRINTM(MERROR, "Vendor Command reply failed ret:%d\n", ret);

err:
	LEAVE();
	return ret;
}

/**
 * @brief vendor command to run carrier wave test
 *
 * @param wiphy    A pointer to wiphy struct
 * @param wdev     A pointer to wireless_dev struct
 * @param data     a pointer to data
 * @param len      data length
 *
 * @return      0: success, otherwise failure
 */
int woal_vndr_reg_cw(struct wiphy *wiphy, struct wireless_dev *wdev,
			       const void *data, int data_len)
{
	moal_private *priv  = (moal_private *)woal_get_netdev_priv(wdev->netdev);
	moal_handle *handle = priv->phandle;
	struct sk_buff *skb = NULL;
	mlan_status status  = MLAN_STATUS_SUCCESS;
	struct nlattr *tb[ATTR_REG_MAX + 1];
	int ret = -ENOSYS;

	ENTER();

	if (!IS_SD9177(handle->card_type)) {
		ret = -ENODEV;
		goto err;
	}

	ret = nla_parse(tb, ATTR_REG_MAX, data, data_len, NULL,NULL);
	if (ret) {
		PRINTM(MERROR, "nla parse failed\n");
		goto err;
	}

	if ( !tb[ATTR_REG_ENABLE] ) {
		ret = -EINVAL;
		goto err;
	}

	if ( 0 == nla_get_u32(tb[ATTR_REG_ENABLE]) ) {
		//Disable test
		status = woal_vndr_reg_cfg_txc(handle, MFALSE, MTRUE, 0);
		if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
			ret = -EFAULT;
			goto err;
		}
		goto done;
	}

	if ( !(tb[ATTR_REG_CHANNEL] && tb[ATTR_REG_BW] &&
	 	   tb[ATTR_REG_RATE] && tb[ATTR_REG_PWR]) ) {
		ret = -EINVAL;
		goto err;
	}

	status = woal_vndr_reg_cfg_basics(handle, 
			    nla_get_u32(tb[ATTR_REG_CHANNEL]), 
			    nla_get_u32(tb[ATTR_REG_BW]),
			    nla_get_s32(tb[ATTR_REG_PWR]),
			    nla_get_u32(tb[ATTR_REG_RATE]));
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		ret = -EFAULT;
		goto err;
	}

	status = woal_vndr_reg_cfg_txc(handle, MTRUE, MTRUE, nla_get_u32(tb[ATTR_REG_RATE]));
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		ret = -EFAULT;
		goto err;
	}

done:	
	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, sizeof(struct nlattr) + sizeof(u32));
	if (unlikely(!skb)) {
		PRINTM(MERROR, "skb alloc failed\n");
		ret = -ENOMEM;
		goto err;
	}

	nla_put_u32(skb, SONA_REG_ATTR_RSP_CMD, 0);

	ret = cfg80211_vendor_cmd_reply(skb);
	if (unlikely(ret))
		PRINTM(MERROR, "Vendor Command reply failed ret:%d\n", ret);

err:
	LEAVE();
	return ret;
}

#define HETB_TRG_BASE   1
#define HETB_STANDALONE 2
/**
 * @brief vendor command to run tx ofdma test
 *
 * @param wiphy    A pointer to wiphy struct
 * @param wdev     A pointer to wireless_dev struct
 * @param data     a pointer to data
 * @param len      data length
 *
 * @return      0: success, otherwise failure
 */
int woal_vndr_reg_tx(struct wiphy *wiphy, struct wireless_dev *wdev,
			       const void *data, int data_len)
{
	moal_private *priv  = (moal_private *)woal_get_netdev_priv(wdev->netdev);
	moal_handle *handle = priv->phandle;
	struct sk_buff *skb = NULL;
	mlan_status status  = MLAN_STATUS_SUCCESS;
	struct nlattr *tb[ATTR_REG_MAX + 1];
	int ret = -ENOSYS;

	ENTER();

	if (!IS_SD9177(handle->card_type)) {
		ret = -ENODEV;
		goto err;
	}

	ret = nla_parse(tb, ATTR_REG_MAX, data, data_len, NULL,NULL);
	if (ret) {
		PRINTM(MERROR, "nla parse failed\n");
		goto err;
	}

	if (!tb[ATTR_REG_ENABLE]) {
		ret = -EINVAL;
		goto err;
	}

	if ( 0 == nla_get_u32(tb[ATTR_REG_ENABLE])) {
		//Disable test
		status = woal_vndr_reg_cfg_tx_frame(handle, MFALSE, REG_ATTR_BW_20, 0);
		if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {			
			ret = -EFAULT;
			goto err;
		}
		goto done;
	}

	if ( !(tb[ATTR_REG_CHANNEL] && tb[ATTR_REG_BW] &&
	 	   tb[ATTR_REG_RATE] && tb[ATTR_REG_PWR] && 
		   tb[ATTR_REG_IDX]) ) {
		ret = -EINVAL;
		goto err;
	}

	status = woal_vndr_reg_cfg_basics(handle, 
			    nla_get_u32(tb[ATTR_REG_CHANNEL]), 
			    nla_get_u32(tb[ATTR_REG_BW]),
			    nla_get_s32(tb[ATTR_REG_PWR]),
			    nla_get_u32(tb[ATTR_REG_RATE]));
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		ret = -EFAULT;
		goto err;
	}

	status = woal_vndr_reg_cfg_trg(handle, MTRUE, HETB_STANDALONE, nla_get_u32(tb[ATTR_REG_BW]), nla_get_u32(tb[ATTR_REG_RATE]), nla_get_u32(tb[ATTR_REG_IDX]));
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		ret = -EFAULT;
		goto err;
	}

	status = woal_vndr_reg_cfg_tx_frame(handle, MTRUE, nla_get_u32(tb[ATTR_REG_BW]), nla_get_u32(tb[ATTR_REG_RATE]));
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		ret = -EFAULT;
		goto err;
	}

done:
	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, sizeof(struct nlattr) + sizeof(u32));
	if (unlikely(!skb)) {
		PRINTM(MERROR, "skb alloc failed\n");
		ret = -ENOMEM;
		goto err;
	}

	nla_put_u32(skb, SONA_REG_ATTR_RSP_CMD, 0);

	ret = cfg80211_vendor_cmd_reply(skb);
	if (unlikely(ret))
		PRINTM(MERROR, "Vendor Command reply failed ret:%d\n", ret);

err:
	LEAVE();
	return ret;
}

/**
 * @brief vendor command to run rx test
 *
 * @param wiphy    A pointer to wiphy struct
 * @param wdev     A pointer to wireless_dev struct
 * @param data     a pointer to data
 * @param len      data length
 *
 * @return      0: success, otherwise failure
 */
int woal_vndr_reg_rx(struct wiphy *wiphy, struct wireless_dev *wdev,
			       const void *data, int data_len)
{
	moal_private *priv  = (moal_private *)woal_get_netdev_priv(wdev->netdev);
	moal_handle *handle = priv->phandle;
	struct sk_buff *skb = NULL;
	mlan_status status  = MLAN_STATUS_SUCCESS;
	struct nlattr *tb[ATTR_REG_MAX + 1];
	sona_rx_data vdata;
	int ret = -ENOSYS;

	ENTER();

	memset(&vdata, 0, sizeof(vdata));

	if (!IS_SD9177(handle->card_type)) {
		ret = -ENODEV;
		goto err;
	}

	ret = nla_parse(tb, ATTR_REG_MAX, data, data_len, NULL,NULL);
	if (ret) {
		PRINTM(MERROR, "nla parse failed\n");
		goto err;
	}

	if ( !tb[ATTR_REG_ENABLE] ) {
		ret = -EINVAL;
		goto err;
	}

	if ( 0 == nla_get_u32(tb[ATTR_REG_ENABLE])) {
		//Disable test, return counts
		vdata.total = handle->rf_data->rx_tot_pkt_count++;
		vdata.bcast = handle->rf_data->rx_mcast_bcast_pkt_count++;
		vdata.err   = handle->rf_data->rx_pkt_fcs_err_count++;
		goto done;
	}

	if ( !(tb[ATTR_REG_CHANNEL] && tb[ATTR_REG_BW]) ) {
		ret = -EINVAL;
		goto err;
	}

	status = woal_vndr_reg_cfg_basics(handle, 
			    nla_get_u32(tb[ATTR_REG_CHANNEL]), 
			    nla_get_u32(tb[ATTR_REG_BW]),
				0, 0);
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		ret = -EFAULT;
		goto err;
	}

	status = woal_vndr_reg_cfg_rx(handle);
	if (status != MLAN_STATUS_SUCCESS && status != MLAN_STATUS_PENDING) {
		ret = -EFAULT;
		goto err;
	}

done:
	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, (sizeof(struct nlattr)*2) + sizeof(u32) + sizeof(vdata));
	if (unlikely(!skb)) {
		PRINTM(MERROR, "skb alloc failed\n");
		ret = -ENOMEM;
		goto err;
	}

	nla_put_u32(skb, SONA_REG_ATTR_RSP_CMD, 0);
	nla_put(skb, SONA_REG_ATTR_RSP_DATA, sizeof(vdata), &vdata);

	ret = cfg80211_vendor_cmd_reply(skb);
	if (unlikely(ret))
		PRINTM(MERROR, "Vendor Command reply failed ret:%d\n", ret);

err:
	LEAVE();
	return ret;
}

int woal_vndr_add_card_dpc(moal_handle *handle)
{
	mlan_status ret = MLAN_STATUS_SUCCESS;
	mlan_status status = MLAN_STATUS_SUCCESS;
	moal_private *priv  = NULL;
	mlan_ioctl_req *req = NULL;
	int i;

	if (!IS_SD9177(handle->card_type)) {
		goto done;
	}

	//prod-29240 
	if ((handle->params.disable_phy & VNDR_DISABLE_PHY_ALL) == VNDR_DISABLE_PHY_ALL) {
		PRINTM(MERROR, "Atleast one radio must be enabled.\n");
		ret = MLAN_STATUS_FAILURE;
		goto done;
	}

	if (handle->params.disable_phy & VNDR_DISABLE_PHY_ALL) {
		mlan_ds_radio_cfg *radio_cfg = NULL;
		t_u32 bandcfg = 0;

		if (handle->params.disable_phy & VNDR_DISABLE_PHY_2G) {
			bandcfg = BAND_A | BAND_AN | BAND_AAC | BAND_AAX;
		}
		else if (handle->params.disable_phy & VNDR_DISABLE_PHY_5G) {
			bandcfg = BAND_B | BAND_G | BAND_GN | BAND_GAC| BAND_GAX;
		}

		for (i = 0; i < MIN(handle->priv_num, MLAN_MAX_BSS_NUM); i++) {
			priv = handle->priv[i];

			req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_radio_cfg));
			if (req == NULL) {
				PRINTM(MERROR, "Failed to allocate bandcfg request\n");
				ret = MLAN_STATUS_FAILURE;
				goto done;
			}

			/* Send config_bands and adhoc_start_band values to MLAN */
			radio_cfg = (mlan_ds_radio_cfg *)req->pbuf;
			radio_cfg->sub_command = MLAN_OID_BAND_CFG;
			radio_cfg->param.band_cfg.config_bands = bandcfg;

			req->action = MLAN_ACT_SET;
			req->req_id = MLAN_IOCTL_RADIO_CFG;

			status = woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT);
			if (status != MLAN_STATUS_PENDING)
				kfree(req);

			if (status != MLAN_STATUS_SUCCESS) {
				PRINTM(MERROR, "Failed to set bandcfg.\n");
				ret = MLAN_STATUS_FAILURE;
				goto done;
			}
		}

		PRINTM(MMSG, "SET: bandcfg to 0x%x\n", bandcfg);
	}

	//prod-29036
	if (handle->params.httxcfg_2) {
		uint16_t drv_mode = handle->params.httxcfg_2 >> 16;
		bool bSet = false;

		for (i = 0; i < MIN(handle->priv_num, MLAN_MAX_BSS_NUM); i++) {
			priv = handle->priv[i];
			bSet = false;

			#ifdef STA_SUPPORT
			if ((drv_mode & DRV_MODE_STA) && priv->bss_type == MLAN_BSS_TYPE_STA) {
				bSet = true;
			}
			#endif
			#ifdef UAP_SUPPORT
			if ((drv_mode & DRV_MODE_UAP) && priv->bss_type == MLAN_BSS_TYPE_UAP) {
				bSet = true;
			}
			#endif
			#ifdef WIFIDIRECT_SUPPORT
			if ((drv_mode & DRV_MODE_WIFIDIRECT) && priv->bss_type == MLAN_BSS_TYPE_WIFIDIRECT) {
				bSet = true;
			}
			#endif

			if (bSet) {
				//Set .11n cfg
				mlan_ds_11n_cfg *cfg_11n = NULL;

				req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
				if (req == NULL) {
					PRINTM(MERROR, "Failed to allocate 11n cfg request\n");
					ret = MLAN_STATUS_FAILURE;
					goto done;
				}

				/* Update 11n tx parameters in MLAN */
				cfg_11n = (mlan_ds_11n_cfg *)req->pbuf;
				cfg_11n->sub_command = MLAN_OID_11N_CFG_TX;
				cfg_11n->param.tx_cfg.httxcap = handle->params.httxcfg_2 & 0xFF;
				cfg_11n->param.tx_cfg.misc_cfg = BAND_SELECT_BG;

				PRINTM(MCMND, "SET: 2.4 httxcap:0x%x\n", cfg_11n->param.tx_cfg.httxcap);
				req->req_id = MLAN_IOCTL_11N_CFG;
				req->action = MLAN_ACT_SET;

				status = woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT);
				if (status != MLAN_STATUS_PENDING)
					kfree(req);

				if (status != MLAN_STATUS_SUCCESS) {
					PRINTM(MERROR, "Failed to set 2.4 11n cfg\n");
					ret = MLAN_STATUS_FAILURE;
					goto done;
				}
			}
		}
	}

done:
	return ret;
}