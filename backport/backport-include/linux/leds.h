#ifndef __BACKPORT_LINUX_LEDS_H
#define __BACKPORT_LINUX_LEDS_H
#include_next <linux/leds.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(4,11,0)
#define LED_ON	1
#endif

#ifdef CONFIG_LEDS_TRIGGERS

#if LINUX_VERSION_IS_LESS(2,6,39)
void led_trigger_blink(struct led_trigger *trigger,
				unsigned long delay_on,
				unsigned long delay_off);
#elif LINUX_VERSION_IS_EQU(5,14,0) && RHEL_RELEASE_IS_GEQ(9,3)
/* Do nothing, backported by RHEL */
#elif LINUX_VERSION_IS_LESS(6,5,0)
static inline
void backport_led_trigger_blink(struct led_trigger *trigger,
				unsigned long delay_on,
				unsigned long delay_off)
{
	led_trigger_blink(trigger, &delay_on, &delay_off);
}
#define led_trigger_blink LINUX_BACKPORT(led_trigger_blink)
#endif /* < 2.6.39, < 6.5 */

#if LINUX_VERSION_IS_LESS(3,6,0)
static inline
void led_trigger_blink_oneshot(struct led_trigger *trigger,
			       unsigned long delay_on,
			       unsigned long delay_off,
			       int invert)
{}
#elif LINUX_VERSION_IS_EQU(5,14,0) && RHEL_RELEASE_IS_GEQ(9,3)
/* Do nothing, backported by RHEL */
#elif LINUX_VERSION_IS_LESS(6,5,0)
static inline
void backport_led_trigger_blink_oneshot(struct led_trigger *trigger,
					unsigned long delay_on,
					unsigned long delay_off,
					int invert)
{
	led_trigger_blink_oneshot(trigger, &delay_on, &delay_off, invert);
}
#define led_trigger_blink_oneshot LINUX_BACKPORT(led_trigger_blink_oneshot)
#endif /* < 3.6, < 6.5 */

#if LINUX_VERSION_IS_LESS(4,5,0)
#define devm_led_trigger_register LINUX_BACKPORT(devm_led_trigger_register)
extern int devm_led_trigger_register(struct device *dev,
				     struct led_trigger *trigger);
#endif /* < 4.5 */

#else /* !CONFIG_LEDS_TRIGGERS */

#if LINUX_VERSION_IS_LESS(4,4,0)
static inline
void led_trigger_blink(struct led_trigger *trigger,
		       unsigned long delay_on,
		       unsigned long delay_off)
{}

static inline
void led_trigger_blink_oneshot(struct led_trigger *trigger,
			       unsigned long delay_on,
			       unsigned long delay_off,
			       int invert)
{}
#endif /* < 4.4 */

#endif /* CONFIG_LEDS_TRIGGERS */

#endif /* __BACKPORT_LINUX_LEDS_H */
