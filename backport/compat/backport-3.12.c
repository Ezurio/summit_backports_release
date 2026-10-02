/*
 * Copyright (c) 2013  Hauke Mehrtens <hauke@hauke-m.de>
 *
 * Backport functionality introduced in Linux 3.12.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/export.h>
#include <linux/hid.h>
#include <linux/bug.h>
#include <linux/math64.h>
#include <linux/skbuff.h>
#include <linux/errqueue.h>
#include <net/sock.h>

/*
 * Allocator for buffer that is going to be passed to hid_output_report()
 */
u8 *hid_alloc_report_buf(struct hid_report *report, gfp_t flags)
{
	/*
	 * 7 extra bytes are necessary to achieve proper functionality
	 * of implement() working on 8 byte chunks
	 */

	int len = ((report->size - 1) >> 3) + 1 + (report->id > 0) + 7;

	return kmalloc(len, flags);
}
EXPORT_SYMBOL_GPL(hid_alloc_report_buf);

#if BITS_PER_LONG == 32
/**
 * div64_u64_rem - unsigned 64bit divide with 64bit divisor and remainder
 * @dividend:	64bit dividend
 * @divisor:	64bit divisor
 * @remainder:  64bit remainder
 *
 * This implementation is a comparable to algorithm used by div64_u64.
 * But this operation, which includes math for calculating the remainder,
 * is kept distinct to avoid slowing down the div64_u64 operation on 32bit
 * systems.
 */
#ifndef backports_div64_u64_rem_add
u64 div64_u64_rem(u64 dividend, u64 divisor, u64 *remainder)
{
	u32 high = divisor >> 32;
	u64 quot;

	if (high == 0) {
		u32 rem32;
		quot = div_u64_rem(dividend, divisor, &rem32);
		*remainder = rem32;
	} else {
		int n = 1 + fls(high);
		quot = div_u64(dividend >> n, divisor >> n);

		if (quot != 0)
			quot--;

		*remainder = dividend - quot * divisor;
		if (*remainder >= divisor) {
			quot++;
			*remainder -= divisor;
		}
	}

	return quot;
}
EXPORT_SYMBOL_GPL(div64_u64_rem);
#endif /* backports_div64_u64_rem_add */
#endif /* BITS_PER_LONG */

int sock_recv_errqueue(struct sock *sk, struct msghdr *msg, int len,
		       int level, int type)
{
	struct sock_exterr_skb *serr;
	struct sk_buff *skb, *skb2;
	int copied, err;

	err = -EAGAIN;
	skb = skb_dequeue(&sk->sk_error_queue);
	if (skb == NULL)
		goto out;

	copied = skb->len;
	if (copied > len) {
		msg->msg_flags |= MSG_TRUNC;
		copied = len;
	}
	err = skb_copy_datagram_iovec(skb, 0, msg->msg_iov, copied);
	if (err)
		goto out_free_skb;

	sock_recv_timestamp(msg, sk, skb);

	serr = SKB_EXT_ERR(skb);
	put_cmsg(msg, level, type, sizeof(serr->ee), &serr->ee);

	msg->msg_flags |= MSG_ERRQUEUE;
	err = copied;

	/* Reset and regenerate socket error */
	spin_lock_bh(&sk->sk_error_queue.lock);
	sk->sk_err = 0;
	if ((skb2 = skb_peek(&sk->sk_error_queue)) != NULL) {
		sk->sk_err = SKB_EXT_ERR(skb2)->ee.ee_errno;
		spin_unlock_bh(&sk->sk_error_queue.lock);
		sk_error_report(sk);
	} else
		spin_unlock_bh(&sk->sk_error_queue.lock);

out_free_skb:
	kfree_skb(skb);
out:
	return err;
}
EXPORT_SYMBOL(sock_recv_errqueue);
