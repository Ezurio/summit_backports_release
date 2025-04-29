#include <linux/export.h>
#include <linux/crypto.h>
#include <linux/completion.h>

void crypto_req_done(struct crypto_async_request *req, int err)
{
	struct crypto_wait *wait = req->data;

	if (err == -EINPROGRESS)
		return;

	wait->err = err;
	complete(&wait->completion);
}
EXPORT_SYMBOL_GPL(crypto_req_done);
